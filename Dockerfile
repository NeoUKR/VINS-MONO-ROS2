ARG ROS_DISTRO=jazzy
FROM ros:${ROS_DISTRO}-ros-base AS dependencies

ARG ROS_DISTRO
ENV ROS_DISTRO=${ROS_DISTRO} \
    DEBIAN_FRONTEND=noninteractive \
    VINS_WS=/workspace

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

RUN apt-get update && apt-get install -y --no-install-recommends \
        libboost-filesystem-dev \
        libboost-program-options-dev \
        libboost-system-dev \
        libceres-dev \
        libeigen3-dev \
        libopencv-dev \
        python3-colcon-common-extensions \
        python3-rosdep \
        ros-${ROS_DISTRO}-cv-bridge \
        ros-${ROS_DISTRO}-image-transport \
        ros-${ROS_DISTRO}-message-filters \
        ros-${ROS_DISTRO}-tf2-ros \
    && rm -rf /var/lib/apt/lists/*

WORKDIR ${VINS_WS}

FROM dependencies AS build

COPY . ${VINS_WS}
COPY docker/entrypoint.sh /usr/local/bin/vins-entrypoint
RUN chmod +x /usr/local/bin/vins-entrypoint \
    && source /opt/ros/${ROS_DISTRO}/setup.bash \
    && colcon build --merge-install --executor sequential

ENTRYPOINT ["/usr/local/bin/vins-entrypoint"]
CMD ["shell"]
