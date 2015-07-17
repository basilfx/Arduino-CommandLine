// NOTE: Read the README.md file first!

#include "CommandLine.h"

// Keep track of number of bytes processed.
int count = 0;

// CommandLine instance.
CommandLine commandLine(Serial, "> ");

/**
 * Setup serial port and add commands.
 */
void setup()
{
  Serial.begin(9600);

  // Attach pre and post command handlers.
  commandLine.attachPre(handlePre);
  commandLine.attachPost(handlePost);

  // Add commands.
  commandLine.add("count", handleCount);
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
  Serial.print("Number of input bytes processed: ");
  Serial.println(count);
}

/**
 * Handle pre-command callback.
 *
 * @param tokens The the input command.
 */
void handlePre(char* tokens)
{
  count = count + strlen(tokens);
}

/**
 * Remove the count command.
 *
 * @param tokens The the input command.
 * @param success True if some command was executed.
 */
void handlePost(char* tokens, bool success)
{
  if (!success)
  {
    Serial.println("Unknown command. Type 'help' for help.");
  }
}

/**
 * Print some help.
 *
 * @param tokens The rest of the input command.
 */
void handleHelp(char* tokens)
{
  Serial.println("Use the commands 'help' or 'count'.");
}
