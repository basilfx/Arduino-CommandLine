# CommandLine v2.0
No-nonsense serial command line interpreter for Arduino.

## Installation
* [Download](https://github.com/basilfx/Arduino-CommandLine/archive/master.zip) or clone this repository to `Arduino/libraries/CommandLine`
* Restart Arduino IDE
* Check the examples

## Howto
The `CommandLine` class wraps a `Serial` instance. The `CommandLine::update` method should be invoked to run the read,
print and eval loop.

Commands can be added with `CommandLine::add` by passing a `Command` structure that takes a command and a callback.
Alternatively, you can add a command on-the-fly. In that case, it will allocate memory on-the-fly, but you cannot
remove or disable a command afterwards.

## Configuration
By default, the number of commands supported is 8 and the maximum line buffer is 64 characters. These limmits can be
changed by defining `COMMANDLINE_BUFFER` and `COMMANDLINE_COUNT` before `CommandLine.h` is included.

## License
See the `LICENSE` file (MIT license).