#include "CommandLine.h"

CommandLine::CommandLine(Stream& _serial, char* _token): serial(_serial), token(_token)
{
    index = 0;
    length = 0;
}

bool CommandLine::update()
{
    bool success = false;

    if (this->serial.available()) {
        char input = this->serial.read();

        switch (input) {
            case KEYCODE_ENTER:
                // Write newline
                this->serial.println("");

                // Parse command
                if (length > 0) {
                    // Split command name
                    char* split = strtok(buffer, " ");
                    uint8_t tokenLength = strlen(split);

                    for (int i = 0; i < index; i++) {
                        if (strncmp(split, commands[i]->command, tokenLength) == 0) {
                            if (strlen(commands[i]->command) == tokenLength) {
                                commands[i]->callback(&buffer[tokenLength]);
                                success = true;
                                break;
                            }
                        }
                    }
                }

                // Reset
                length = 0;

                // Write new input
                this->serial.print(token);

                break;
            case KEYCODE_BACKSPACE:
            case KEYCODE_DELETE:
                if (length > 0) {
                    // Move pointer
                    length--;

                    // Clear last char on screen
                    this->serial.write(KEYCODE_BACKSPACE);
                    this->serial.write(KEYCODE_SPACE);
                    this->serial.write(KEYCODE_BACKSPACE);
                }

                break;
            default:
                if (input > 31 && input < 127) {
                    if (length < COMMANDLINE_BUFFER) {
                        // Store input, append NULL char after input for safety reasons.
                        buffer[length] = input;
                        buffer[length + 1] = '\0';

                        // Move pointer
                        length++;

                        // Repeat char
                        this->serial.write(input);
                    }
                }

                break;
        }
    }

    // Done
    return success;
}

bool CommandLine::add(Command& command)
{
    if (index < COMMANDLINE_COUNT) {
        // Store command internally
        this->commands[index] = &command;

        // Increment index
        index++;

        // Done
        return true;
    }

    // No space left
    return false;
}

bool CommandLine::add(char* command, void (*callback)(char*))
{
    Command* cmd = (Command*) malloc(sizeof(Command));

    cmd->command = command;
    cmd->callback = callback;

    this->add(*cmd);
}

bool CommandLine::remove(Command& command)
{
    for (int i = 0; i < index; i++) {
        if (this->commands[i] == &command) {
            // Move other commands one index to the left
            for (int j = i + 1; j < index; j++) {
                this->commands[j - 1] = this->commands[j];
            }

            // Make some room
            index--;

            // Done
            return true;
        }
    }

    // Command not found
    return false;
}

