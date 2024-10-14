## Intro (WIP)
This is a simple and lightweight logger with minimal functions such as:
- choose log files directory
- set log file name (the resulting log file name would be: `<file_name>yyyy-mm-dd.log`)
- set if start/stop should be printed on logger creation/destruction
- set if log entries should be written to stdout apart from file

## Build

1. `mkdir build && cd build`
2. `cmake ..` (by default, the debug build type is used)

### target minilogger_demo
`minilogger_demo` target demonstrates logger usage example by creating a log file with chosen number of entries and measuring write time in nanoseconds.

`minilogger_demo <log_file_path> <log_entries_number>`, see help for more info. 
- `make minilogger_demo`

The built demo is saved as `build/<build_type>/minilogger_demo`

### target minilogger
To build the mini-logger *library*:

- `make minilogger`
- `make install`

After, you can include mini_logger.h, link against libminilogger and enjoy super light logging.

## TODO
- add support for multiple log levels (e.g., error (only errors), 
 info (info, errors), debug (all), etc)
- support log rotation
- support one stream mode (write either to file, or to other stream, or to all of them)

