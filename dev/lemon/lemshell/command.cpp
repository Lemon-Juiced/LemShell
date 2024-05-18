#include "command.h"
#include <iostream>

using namespace std;

Command::Command() {}

Command::~Command() {}

/**
 * Clear the screen
 */
void Command::clear() {
    cout << "\033[2J\033[1;1H"; // Clear the screen
}
