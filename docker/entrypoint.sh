#!/usr/bin/env bash
set -e

cd "${VINS_WS:-/workspace}"
source "/opt/ros/${ROS_DISTRO}/setup.bash"

build_workspace() {
  colcon build --merge-install --executor sequential "$@"
}

source_workspace() {
  if [[ -f install/setup.bash ]]; then
    source install/setup.bash
  fi
}

case "${1:-shell}" in
  build)
    shift
    build_workspace "$@"
    ;;
  test)
    shift
    build_workspace
    source_workspace
    colcon test --merge-install "$@"
    colcon test-result --verbose
    ;;
  version)
    shift
    source_workspace
    ros2 run vins_estimator vins_estimator --version "$@"
    ;;
  shell)
    shift
    source_workspace
    exec bash "$@"
    ;;
  *)
    source_workspace
    exec "$@"
    ;;
esac
