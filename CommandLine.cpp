#include "Commandline.h"



/**
 *
 */
CommandLine::CommandLine() {
    index = 0;
    length = 0;
}

void CommandLine::begin(char* _token) {
    enabled = true;
    token = _token;

    // Print commandline token
    COMMANDLINE_INPUT.print(token);
}

void CommandLine::stop() {
    enabled = false;

    // End current line
    COMMANDLINE_INPUT.println("");
}

/**
 *
 */
void CommandLine::loop() {
    if (enabled && COMMANDLINE_INPUT.available()) {
        char input = COMMANDLINE_INPUT.read();

        switch (input) {
            case KEYCODE_ENTER:
                // Write newline
                COMMANDLINE_INPUT.println("");

                // Parse command
                if (length > 0) {
                    // For safety, make sure a NULL terminator is present
                    buffer[length + 1] = '\0';

                    // Split command name
                    char* split = strtok(buffer, " ");
                    uint8_t size = strlen(split);
                    uint8_t success = 0;

                    for (int i = 0; i < index; i++) {
                        if (strncmp(split, commands[i], length) == 0) {
                            callbacks[i](&buffer[size - 1]);
                            success = 1;
                            break;
                        }
                    }
                }

                // Reset
                length = 0;

                // Write new input
                COMMANDLINE_INPUT.print(token);

                break;
            case KEYCODE_BACKSPACE:
            case KEYCODE_DELETE:
                if (length > 0) {
                    // Move pointer
                    length--;

                    // Clear last char
                    COMMANDLINE_INPUT.write(KEYCODE_BACKSPACE);
                    COMMANDLINE_INPUT.write(KEYCODE_SPACE);
                    COMMANDLINE_INPUT.write(KEYCODE_BACKSPACE);
                }

                break;
            default:
                if (input > 31 && input < 127) {
                    if (length < COMMANDLINE_BUFFER) {
                        // Store input
                        buffer[length] = input;

                        // Move pointer
                        length++;

                        // Repeat char
                        COMMANDLINE_INPUT.write(input);
                    }
                }

                break;
        }
    }
}

/**
 *
 */
bool CommandLine::add(char* command, int (*callback)(char*)) {
    if (index < COMMANDLINE_COUNT) {
        // Store command internally
        commands[index] = command;
        callbacks[index] = callback;

        // Increment index
        index++;

        // Done
        return true;
    } else {
        return false;
    }
}