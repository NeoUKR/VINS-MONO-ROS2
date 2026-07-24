#!/usr/bin/env bash

# Automatically prepare ROS 2 for non-interactive shells, including
# `docker compose exec vins bash -c "ros2 ..."`.
if [[ -f "/opt/ros/${ROS_DISTRO:-jazzy}/setup.bash" ]]; then
  source "/opt/ros/${ROS_DISTRO:-jazzy}/setup.bash"
fi

if [[ -f "${VINS_WS:-/workspace}/install/setup.bash" ]]; then
  source "${VINS_WS:-/workspace}/install/setup.bash"
fi
