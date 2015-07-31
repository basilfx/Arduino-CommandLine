# CommandLine v2.1.0
No-nonsense serial command line interpreter for Arduino.

## Installation
* [Download](https://github.com/basilfx/Arduino-CommandLine/archive/master.zip) or clone this repository to `Arduino/libraries/CommandLine`.
* Restart Arduino IDE.
* Check the examples.

## Howto
A `CommandLine` instance wraps a `Serial` instance. The `CommandLine::update` method should be invoked to run the read, print and eval loop.

Commands can be added with `CommandLine::add` by passing a `Command` structure that takes a command and a callback. Alternatively, you can add a command on-the-fly. In that case, it will allocate memory via `malloc`, but you cannot remove or disable a command afterwards.

A command can be a single word only (no spaces). It will be tokenized using `strtok` using spaces. If an input string matches a command (first match), the attached callback will be invoked with a `char *` to the rest of the arguments (should be parsed using `strtok`).

## Configuration
There are several parameters and functions that can be modified, enabled or disabled by defining them.

* `COMMANDLINE_BUFFER` &mdash; number of characters to buffer for one line. Default is 64 characters.
* `COMMANDLINE_COUNT` &mdash; number of commands that can be added or remove (including on-the-fly ones). Default is 8.
* `COMMANDLINE_PRE_POST` &mdash; if defined, enable support for pre and post callback. Disabled by default.

### Note
Due to the limitations (or design choices) of the Arduino IDE and its build process, the above defines cannot be overridden when using the Arduino IDE. There are three options:

* Add `CommandLine.cpp` and `CommandLine.h` to your project and modify `CommandLine.h` as desired. Make sure you remove the library from your `Arduino/libraries/` folder.
* Modify the `*.build.extra_flags` property in `boards.txt` to add compiler defines (e.g. `-DCOMMANDLINE_PRE_POST`).
* Use a Makefile to compile your sketch and add the desired defines (e.g. `CXX_FLAGS=-DCOMMANDLINE_PRE_POST`).

## License
See the `LICENSE` file (MIT license).
