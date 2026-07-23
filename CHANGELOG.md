# Release history

All notable changes to VINS-MONO-ROS2 are documented in this file.

The product version uses four numeric components: `MAJOR.MINOR.FEATURE.PATCH`.
Git tags use the corresponding `vMAJOR_MINOR_FEATURE_PATCH` format, with
two-digit zero-padded components after the major version.

## [v1_00_01_05] - 2026-07-23

### Changed

- Removed sensor timestamps from periodic status, state transitions, waiting
  output, and the successful initialization report. The ROS log prefix remains
  the single displayed time source.
- Shortened periodic INFO field names and numeric precision so initialization
  and tracking telemetry fit on one terminal line.

### Documentation

- Added Ukrainian user and logging guides covering build, configuration,
  startup, runtime states, initialization output, diagnostics, version
  reporting, log levels, color output, and INFO throttling.

## [v1_00_01_04] - 2026-07-23

### Fixed

- Waiting output is emitted immediately and periodically reports whether IMU
  and camera feature data are missing or being received.
- Periodic initialization and tracking telemetry is sent through the ROS INFO
  logger so it is displayed immediately by `ros2 launch`.
- Current tracking position, orientation, velocity, feature count, time offset,
  and processing time are visible at every configured reporting interval.
- Successful initialization is printed as a complete multi-line block,
  including pose, velocity, biases, gravity, timing, camera count, extrinsic
  mode, and per-camera extrinsic parameters.

## [v1_00_01_03] - 2026-07-23

### Added

- Periodically updated waiting status with current time, message counters, and
  the latest IMU and feature timestamps.
- Forced terminal color support for warning and error severity output.

### Changed

- Estimator launch now uses an emulated TTY so live status updates and colors
  are rendered immediately.
- Ceres progress output is suppressed at the INFO level, including during
  initial SFM.
- Low-level initialization and marginalization details are available only at
  the DEBUG level.

## [v1_00_01_02] - 2026-07-23

### Changed

- Periodic initialization and tracking telemetry now updates a single terminal
  status line instead of appending one line per reporting interval.
- Initialization diagnostics are consolidated into the live status line.

## [v1_00_01_01] - 2026-07-23

### Added

- Estimator version banner at process startup.
- `--version` and `-V` command-line options for querying the installed
  estimator version.

### Changed

- All ROS package manifests now report the ROS-compatible version `1.0.1`.
- Product versioning now uses the four-component `1.0.1.1` format.

## [v1_00_01_00] - 2026-07-23

### Added

- Configurable estimator logging levels and throttling period.
- Timestamped runtime telemetry for initialization and tracking states.
- Explicit state-transition and initialization-summary messages.
- Dedicated feature-tracker launch file with RViz visualization.

### Changed

- Ceres iteration logging is now enabled only at the `DEBUG` level.
- Feature tracking and RViz can now be launched separately.
- Critical estimator conditions are reported at the `ERROR` level.

## [v1_00_00_00] - 2026-07-23

Initial release of the VINS-MONO ROS 2 port.

### Added

- ROS 2 packages for camera models, feature tracking, state estimation,
  pose-graph optimization, benchmarking, the AR demo, and shared configuration.
- Launch files and configurations for EuRoC and other supported datasets.
- RViz configurations and sample benchmark data.
- Initial project documentation and GPLv3 licensing information.

[v1_00_01_05]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v1_00_01_05
[v1_00_01_04]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v1_00_01_04
[v1_00_01_03]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v1_00_01_03
[v1_00_01_02]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v1_00_01_02
[v1_00_01_01]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v1_00_01_01
[v1_00_01_00]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v1_00_01_00
[v1_00_00_00]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v1_00_00_00
