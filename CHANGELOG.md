# Release history

All notable changes to VINS-MONO-ROS2 are documented in this file.

The project uses release identifiers in the `vMAJOR_MINOR_PATCH` format.

## [v01_01_01] - 2026-07-23

### Added

- Estimator version banner at process startup.
- `--version` and `-V` command-line options for querying the installed
  estimator version.

### Changed

- All ROS package manifests now report version `1.1.1`.

## [v01_01_00] - 2026-07-23

### Added

- Configurable estimator logging levels and throttling period.
- Timestamped runtime telemetry for initialization and tracking states.
- Explicit state-transition and initialization-summary messages.
- Dedicated feature-tracker launch file with RViz visualization.

### Changed

- Ceres iteration logging is now enabled only at the `DEBUG` level.
- Feature tracking and RViz can now be launched separately.
- Critical estimator conditions are reported at the `ERROR` level.

## [v01_00_00] - 2026-07-23

Initial release of the VINS-MONO ROS 2 port.

### Added

- ROS 2 packages for camera models, feature tracking, state estimation,
  pose-graph optimization, benchmarking, the AR demo, and shared configuration.
- Launch files and configurations for EuRoC and other supported datasets.
- RViz configurations and sample benchmark data.
- Initial project documentation and GPLv3 licensing information.

[v01_01_01]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v01_01_01
[v01_01_00]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v01_01_00
[v01_00_00]: https://github.com/NeoUKR/VINS-MONO-ROS2/releases/tag/v01_00_00
