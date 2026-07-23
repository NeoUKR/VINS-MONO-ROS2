#include "parameters.h"
#include <algorithm>
#include <cctype>
#include <rcutils/logging.h>

double INIT_DEPTH;
double MIN_PARALLAX;
double ACC_N, ACC_W;
double GYR_N, GYR_W;

std::vector<Eigen::Matrix3d> RIC;
std::vector<Eigen::Vector3d> TIC;

Eigen::Vector3d G{0.0, 0.0, 9.8};

double BIAS_ACC_THRESHOLD;
double BIAS_GYR_THRESHOLD;
double SOLVER_TIME;
int NUM_ITERATIONS;
int ESTIMATE_EXTRINSIC;
int ESTIMATE_TD;
int ROLLING_SHUTTER;
std::string EX_CALIB_RESULT_PATH;
std::string VINS_RESULT_PATH;
std::string IMU_TOPIC;
double ROW, COL;
double TD, TR;
std::string LOG_LEVEL = "INFO";
int LOG_PERIOD_MS = 2000;

bool configureLogging(const rclcpp::Node::SharedPtr &n)
{
    LOG_LEVEL = n->declare_parameter<std::string>("logging.level", "INFO");
    LOG_PERIOD_MS = n->declare_parameter<int>("logging.period_ms", 2000);

    std::transform(LOG_LEVEL.begin(), LOG_LEVEL.end(), LOG_LEVEL.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });

    int severity = RCUTILS_LOG_SEVERITY_INFO;
    if (LOG_LEVEL == "DEBUG")
        severity = RCUTILS_LOG_SEVERITY_DEBUG;
    else if (LOG_LEVEL == "INFO")
        severity = RCUTILS_LOG_SEVERITY_INFO;
    else if (LOG_LEVEL == "WARNING" || LOG_LEVEL == "WARN")
        severity = RCUTILS_LOG_SEVERITY_WARN;
    else if (LOG_LEVEL == "ERROR")
        severity = RCUTILS_LOG_SEVERITY_ERROR;
    else if (LOG_LEVEL == "CRITICAL" || LOG_LEVEL == "FATAL")
        severity = RCUTILS_LOG_SEVERITY_FATAL;
    else
    {
        RCLCPP_ERROR(n->get_logger(),
                     "Invalid logging.level '%s'. Expected DEBUG, INFO, WARNING, ERROR or CRITICAL.",
                     LOG_LEVEL.c_str());
        return false;
    }

    if (LOG_PERIOD_MS < 0)
    {
        RCLCPP_ERROR(n->get_logger(), "logging.period_ms must be greater than or equal to zero.");
        return false;
    }

    rcutils_logging_set_default_logger_level(severity);
    if (rcutils_logging_set_logger_level(n->get_logger().get_name(), severity) != RCUTILS_RET_OK)
    {
        RCLCPP_ERROR(n->get_logger(), "Failed to set logger level to %s.", LOG_LEVEL.c_str());
        return false;
    }

    return true;
}

template <typename T>
T readParam(rclcpp::Node::SharedPtr n, std::string name)
{
    T ans;
    std::string default_value = "";
    n->declare_parameter<std::string>(name, default_value);
    if (n->get_parameter(name, ans))
    {
        RCLCPP_INFO_STREAM(n->get_logger(), "Loaded " << name << ": " << ans);
    }
    else
    {
        RCLCPP_ERROR_STREAM(n->get_logger(), "Failed to load " << name);
        rclcpp::shutdown();
    }
    return ans;
}

void readParameters(rclcpp::Node::SharedPtr n)
{
    std::string config_file;
    config_file = readParam<std::string>(n, "config_file");
    cv::FileStorage fsSettings(config_file, cv::FileStorage::READ);
    if(!fsSettings.isOpened())
    {
        RCLCPP_ERROR(n->get_logger(), "ERROR: Wrong path to settings");
    }

    fsSettings["imu_topic"] >> IMU_TOPIC;

    SOLVER_TIME = fsSettings["max_solver_time"];
    NUM_ITERATIONS = fsSettings["max_num_iterations"];
    MIN_PARALLAX = fsSettings["keyframe_parallax"];
    MIN_PARALLAX = MIN_PARALLAX / FOCAL_LENGTH;

    std::string OUTPUT_PATH;
    fsSettings["output_path"] >> OUTPUT_PATH;
    VINS_RESULT_PATH = OUTPUT_PATH + "/vins_result_no_loop.csv";
    RCLCPP_INFO(n->get_logger(), "result path %s", VINS_RESULT_PATH.c_str());

    // create folder if not exists
    FileSystemHelper::createDirectoryIfNotExists(OUTPUT_PATH.c_str());

    std::ofstream fout(VINS_RESULT_PATH, std::ios::out);
    fout.close();

    ACC_N = fsSettings["acc_n"];
    ACC_W = fsSettings["acc_w"];
    GYR_N = fsSettings["gyr_n"];
    GYR_W = fsSettings["gyr_w"];
    G.z() = fsSettings["g_norm"];
    ROW = fsSettings["image_height"];
    COL = fsSettings["image_width"];
    RCLCPP_INFO(n->get_logger(), "ROW: %f COL: %f ", ROW, COL);

    ESTIMATE_EXTRINSIC = fsSettings["estimate_extrinsic"];
    if (ESTIMATE_EXTRINSIC == 2)
    {
        RCLCPP_WARN(n->get_logger(), "have no prior about extrinsic param, calibrate extrinsic param");
        RIC.push_back(Eigen::Matrix3d::Identity());
        TIC.push_back(Eigen::Vector3d::Zero());
        EX_CALIB_RESULT_PATH = OUTPUT_PATH + "/extrinsic_parameter.csv";
    }
    else 
    {
        if ( ESTIMATE_EXTRINSIC == 1)
        {
            RCLCPP_WARN(n->get_logger(), " Optimize extrinsic param around initial guess!");
            EX_CALIB_RESULT_PATH = OUTPUT_PATH + "/extrinsic_parameter.csv";
        }
        if (ESTIMATE_EXTRINSIC == 0)
            RCLCPP_WARN(n->get_logger(), " fix extrinsic param ");

        cv::Mat cv_R, cv_T;
        fsSettings["extrinsicRotation"] >> cv_R;
        fsSettings["extrinsicTranslation"] >> cv_T;
        Eigen::Matrix3d eigen_R;
        Eigen::Vector3d eigen_T;
        cv::cv2eigen(cv_R, eigen_R);
        cv::cv2eigen(cv_T, eigen_T);
        Eigen::Quaterniond Q(eigen_R);
        eigen_R = Q.normalized();
        RIC.push_back(eigen_R);
        TIC.push_back(eigen_T);
        RCLCPP_INFO_STREAM(n->get_logger(), "Extrinsic_R : " << std::endl << RIC[0]);
        RCLCPP_INFO_STREAM(n->get_logger(), "Extrinsic_T : " << std::endl << TIC[0].transpose()); 
    } 

    INIT_DEPTH = 5.0;
    BIAS_ACC_THRESHOLD = 0.1;
    BIAS_GYR_THRESHOLD = 0.1;

    TD = fsSettings["td"];
    ESTIMATE_TD = fsSettings["estimate_td"];
    if (ESTIMATE_TD)
        RCLCPP_INFO_STREAM(n->get_logger(), "Unsynchronized sensors, online estimate time offset, initial td: " << TD);
    else
        RCLCPP_INFO_STREAM(n->get_logger(), "Synchronized sensors, fix time offset: " << TD);

    ROLLING_SHUTTER = fsSettings["rolling_shutter"];
    if (ROLLING_SHUTTER)
    {
        TR = fsSettings["rolling_shutter_tr"];
        RCLCPP_INFO_STREAM(n->get_logger(), "rolling shutter camera, read out time per line: " << TR);
    }
    else
    {
        TR = 0;
    }
    
    fsSettings.release();
}
