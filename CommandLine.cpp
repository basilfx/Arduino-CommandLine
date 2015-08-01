#include "CommandLine.h"

CommandLine::CommandLine(Stream& _serial, char* _token): serial(_serial), token(_token)
{
    input.index = 0;
    commands.index = 0;

    #if COMMANDLINE_HISTORY
        history.index = 0;
    #endif
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
                if (this->input.index > 0) {
                    // Add to history if it is not the same as the previous item added.
                    #if COMMANDLINE_HISTORY > 0
                        if (this->history.index == 0 || strncmp(this->history.items[0], this->input.buffer, COMMANDLINE_BUFFER) != 0) {
                            if (this->history.index < COMMANDLINE_HISTORY) {
                                this->history.index++;
                            }

                            // Dequeue first item added by shifting each item one index up.
                            for (int i = this->history.index - 1; i > 0; i--) {
                                strncpy(this->history.items[i], this->history.items[i - 1], COMMANDLINE_BUFFER);
                            }

                            // Current buffer is inserted at position zero.
                            strncpy(this->history.items[0], this->input.buffer, COMMANDLINE_BUFFER);
                        }
                    #endif

                    // Split command name
                    char* split = strtok(this->input.buffer, " ");
                    uint8_t length = strnlen(split, COMMANDLINE_BUFFER);

                    // Handle post command callback.
                    #ifdef COMMANDLINE_PRE_POST
                        if (this->preCallback != NULL) {
                            this->preCallback(this->input.buffer);
                        }
                    #endif

                    // Find the first matching command and invoke callback.
                    for (int i = 0; i < this->commands.index; i++) {
                        if (strncmp(split, this->commands.items[i]->command, length) == 0) {
                            if (strlen(this->commands.items[i]->command) == length) {
                                this->commands.items[i]->callback(&this->input.buffer[this->input.index]);
                                success = true;
                                break;
                            }
                        }
                    }

                    // Handle post command callback.
                    #ifdef COMMANDLINE_PRE_POST
                        if (this->postCallback != NULL) {
                            this->postCallback(this->input.buffer, success);
                        }
                    #endif
                }

                // Reset the byte buffer index.
                this->input.index = 0;

                // Point to last history item added. By setting it to the history length, the up/down
                // handler will treat the first keypress as a special one.
                #if COMMANDLINE_HISTORY > 0
                    this->history.current = this->history.index;
                #endif

                // Write a new input token.
                this->serial.print(this->token);

                break;
            case KEYCODE_BACKSPACE:
            case KEYCODE_DELETE:
                if (this->input.index > 0) {
                    // Reduce byte buffer index.
                    this->input.index--;

                    // Clear last char on screen.
                    this->serial.write(KEYCODE_BACKSPACE);
                    this->serial.write(KEYCODE_SPACE);
                    this->serial.write(KEYCODE_BACKSPACE);
                }

                break;
            #if COMMANDLINE_HISTORY > 0
                case KEYCODE_UP:
                    if (this->history.index > 0) {
                        // Decide on item to show. If current is equal to the index, show the newest item first.
                        if (this->history.current == this->history.index) {
                            this->history.current = 0;
                        } else {
                            this->history.current = (this->history.current + 1) % this->history.index;
                        }

                        // Restore from history.
                        this->restore();
                    }

                    break;
                case KEYCODE_DOWN:
                    if (this->history.index > 0) {
                        // Decide on item to show. If current is equal to the index, show the oldest item first.
                        if (this->history.current == this->history.index) {
                            this->history.current = this->history.index - 1;
                        } else {
                            this->history.current = (this->history.current == 0 ? this->history.index : this->history.current) - 1;
                        }

                        // Restore from history.
                        this->restore();
                    }

                    break;
            #endif
            default:
                if (input > 31 && input < 127) {
                    if (this->input.index < COMMANDLINE_BUFFER) {
                        // Store input, append NULL char after input for safety reasons.
                        this->input.buffer[this->input.index] = input;
                        this->input.buffer[this->input.index + 1] = '\0';

                        // Move pointer
                        this->input.index++;

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
    // Check if command was already added.
    for (int i = 0; i < COMMANDLINE_COUNT; i++) {
        if (this->commands.items[i] == &command) {
            // Command was already added
            return true;
        }
    }

    // Add it to the list.
    if (this->commands.index < COMMANDLINE_COUNT) {
        this->commands.items[this->commands.index] = &command;
        this->commands.index++;

        // Command added
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
    for (int i = 0; i < this->commands.index; i++) {
        if (this->commands.items[i] == &command) {
            // Move other commands one index to the left
            for (int j = i + 1; j < this->commands.index; j++) {
                this->commands.items[j - 1] = this->commands.items[j];
            }

            this->commands.index--;

            // Done
            return true;
        }
    }

    // Command not found
    return false;
}

#if COMMANDLINE_HISTORY > 0
    void CommandLine::restore()
    {
        // Copy history to buffer
        strncpy(this->input.buffer, this->history.items[this->history.current], COMMANDLINE_BUFFER);
        this->input.index = strnlen(this->input.buffer, COMMANDLINE_BUFFER);

        // Show new line
        this->serial.write("\33[2K\r");
        this->serial.print(this->token);
        this->serial.print(this->input.buffer);
    }
#endif

#ifdef COMMANDLINE_PRE_POST
    void CommandLine::attachPre(void (*callback)(char*))
    {
        this->preCallback = callback;
    }

    void CommandLine::attachPost(void (*callback)(char*, bool))
    {
        this->postCallback = callback;
    }
#endif
