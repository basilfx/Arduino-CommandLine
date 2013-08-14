#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <inttypes.h>
#include <string.h>

#include "Arduino.h"

#ifndef COMMANDLINE_COUNT
	#define COMMANDLINE_COUNT 8
#endif

#ifndef COMMANDLINE_BUFFER
	#define COMMANDLINE_BUFFER 64
#endif

#ifndef COMMANDLINE_INPUT
	#define COMMANDLINE_INPUT Serial
#endif

#ifndef COMMANDLINE_TOKEN
    #define COMMANDLINE_TOKEN = "> "
#endif

#define KEYCODE_ENTER 13
#define KEYCODE_BACKSPACE 8
#define KEYCODE_DELETE 127
#define KEYCODE_TAB 9
#define KEYCODE_SPACE 32

class CommandLine
{
    public:

    CommandLine();

    void loop(void);

    void begin(char* _token);

    void stop(void);

    bool add(char* command, int (*callback)(char*));

    private:
    uint8_t enabled;

    uint8_t index;
    char* commands[COMMANDLINE_COUNT];
    int (*callbacks[COMMANDLINE_COUNT])(char*);

    uint8_t length;
    char buffer[COMMANDLINE_BUFFER + 1];

    char* token;

};
#endif