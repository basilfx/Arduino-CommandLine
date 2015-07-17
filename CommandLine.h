#ifndef __COMMANDLINE_H__
#define __COMMANDLINE_H__

#include <inttypes.h>
#include <string.h>

#include "Arduino.h"

// Number of commands to support.
#ifndef COMMANDLINE_COUNT
    #define COMMANDLINE_COUNT 8
#endif

// Maximum number of characters in input buffer.
#ifndef COMMANDLINE_BUFFER
    #define COMMANDLINE_BUFFER 64
#endif

// Add support for pre and post command execution. It's disabled by default.
// #define COMMANDLINE_PRE_POST

// Keycode defines.
#define KEYCODE_BACKSPACE 8
#define KEYCODE_TAB 9
#define KEYCODE_ENTER 13
#define KEYCODE_SPACE 32
#define KEYCODE_UP 65
#define KEYCODE_DOWN 66
#define KEYCODE_DELETE 127

/**
 * Command instance
 */
struct Command
{
    char* command;
    void (*callback)(char*);

    /**
     * Construct a new command
     *
     * @param command Name of the command
     * @param callback Function pointer
     */
    Command(char* _command, void (*_callback)(char*)): command(_command), callback(_callback) {}
};

/**
 * CommandLine instance.
 */
class CommandLine
{

public:

    /**
     * Construct a new CommandLine instance
     *
     * @param serial Serial stream to wrap
     * @param token Command line token to indicate new line (e.g. "> ")
     */
    CommandLine(Stream& serial, char* token);

    /**
     * Read the serial stream and evaluate commands.
     *
     * @return True if a command was evaluated and executed, false otherwise.
     */
    bool update(void);

    /**
     * Add a new command.
     *
     * @param Comand instance to add.
     * @return True on success, false otherwise.
     */
    bool add(Command& command);

    /**
     * Add a new command, dynamically.
     *
     * @param command Name of the command.
     * @param callback Function pointer.
     * @return True on success, false otherwise.
     */
    bool add(char* command, void (*callback)(char*));

    /**
     * Remove a command instance
     *
     * @param Comand instance to remove.
     * @return True on success, false otherwise.
     */
    bool remove(Command& command);

#ifdef COMMANDLINE_PRE_POST
    /**
     * Attach pre-command execution handler. This callback is invoked
     * before each non-empty input command. Set to NULL to clear.
     *
     * @param callback Function pointer callback. Parameter is input the
     *                 string.
     */
    void attachPre(void (*callback)(char*));

    /**
     * Attach post-command execution handler. This callback is invoked
     * after each non-empty input command. Set to NULL to clear.
     *
     * @param callback Function pointer callback. First parameter is the
     *                 input string, second parameter indicates success.
     */
    void attachPost(void (*callback)(char*, bool));
#endif

private:
    Stream& serial;

    uint8_t index;
    Command* commands[COMMANDLINE_COUNT];

    uint8_t length;
    char buffer[COMMANDLINE_BUFFER + 1];

    char* token;

#ifdef COMMANDLINE_PRE_POST
    void (*preCallback)(char*);
    void (*postCallback)(char*, bool);
#endif
};

#endif
