#include "CommandLine.h"

// Keep track of the count, for the count command.
int count = 0;

// CommandLine instance.
CommandLine commandLine(Serial, "> ");

// Commands are simple structures that can be.
Command command = Command("count", &handleCount);
Command addCount = Command("add", &handleAdd);
Command removeCount = Command("remove", &handleRemove);

/**
 * Setup serial port and add commands.
 */
void setup()
{
  Serial.begin(9600);

  // Pre-defined commands
  commandLine.add(command);
  commandLine.add(addCount);
  commandLine.add(removeCount);

  // On-the-fly commands -- instance is allocated dynamically
  commandLine.add("help", handleHelp);
}

/**
 * Read-eval-print-loop.
 */
void loop()
{
  commandLine.update();
}

/**
 * Handle the count command. The command has one additional argument that can be the integer to set the count to.
 *
 * @param tokens The rest of the input command.
 */
void handleCount(char* tokens)
{
  char* token = strtok(NULL, " ");

  if (token != NULL) {
    count = atoi(token);
  } else {
    count++;
  }

  Serial.println(count);
}

/**
 * Add the count command.
 *
 * @param tokens The rest of the input command.
 */
void handleAdd(char* tokens)
{
  commandLine.add(command);
  Serial.println("Command 'count' added.");
}

/**
 * Remove the count command.
 *
 * @param tokens The rest of the input command.
 */
void handleRemove(char* tokens)
{
  commandLine.remove(command);
  Serial.println("Command 'count' removed.");
}

/**
 * Print some help.
 *
 * @param tokens The rest of the input command.
 */
void handleHelp(char* tokens)
{
  Serial.println("Use the commands 'help', 'count', 'add' or 'remove'.");
}
