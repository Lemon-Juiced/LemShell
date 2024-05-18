#include <iostream>
#include <string>
#include <unistd.h> // This is for <code>getcwd</code> in <code>get_directory()</code>
#include <vector>
#include "command.h"

using namespace std;

// Global Constants
Command commandCaller;
int status = true;

// Function Prototypes
void shell_loop();
string get_directory();
string apply_style(string text, string style);
vector<char> read_line();
vector<string> split_line(vector<char> line);
int execute(vector<string> args);

/**
 * A simple shell program that can execute commands
 * The main function of the program will run the shell's loop
 * 
 * Compile with:
 * g++ lemshell.cpp -o lemshell command.cpp
 * 
 * @param argc The number of arguments
 * @param argv The arguments
 */
int main(int argc, char **argv) {
    // Run the shell loop
    shell_loop();

    // Exit the program
    return EXIT_SUCCESS;
}

/**
 * The main loop of the shell
 * This function will run the shell and wait for user input
 * Additionally, it will execute the commands when prompted
 */
void shell_loop() {
    std::vector<string> history;
    vector<char> line;
    vector<string> args;

    do {
        string prompt = apply_style("LemShell: " + get_directory() + "> ", "yellow");
        cout << prompt;
        line = read_line();
        args = split_line(line);
        status = execute(args);
    } while (status);
}

/**
 * Get the current directory
 * 
 * @return The current directory
 */
string get_directory() {
    char cwd[1024]; // Buffer to store the current directory
    getcwd(cwd, sizeof(cwd));
    return cwd;
}

/**
 * Apply a style to the text
 * The style is applied using ANSI escape codes
 * 
 * Current Acceptable styles:
 * "normal"- Normal text
 * "yellow" - Yellow text
 * 
 * @param text The text to style
 * @param style The style to apply
 * @return The styled text
 */
string apply_style(string text, string style) {
    if (style == "normal") {
        return "\033[0m" + text + "\033[0m";
    } else if (style == "yellow") {
        return "\033[33m" + text + "\033[0m";
    } else {
        return text; // Return the text as is
    }
}

/**
 * Read a line from the user
 * 
 * @return The line read from the user
 */
vector<char> read_line() {
    // Reads a string of characters and then converts it to a vector
    string line;
    getline(cin, line);
    return vector<char>(line.begin(), line.end());
}

/**
 * Split the line into arguments
 * 
 * @param line The line to split
 * @return The arguments
 */
vector<string> split_line(vector<char> line) {
    vector<string> args;
    string arg = "";
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
            args.push_back(arg);
            arg = "";
        } else {
            arg += line[i];
        }
    }
    if (!arg.empty()) {
        args.push_back(arg);
    }
    return args;
}

/**
 * Execute the arguments
 * 
 * To do: make function work
 * 
 * @param args The arguments to execute
 * @return The status of the execution
 */
int execute(vector<string> args) {
    // Check if the command is a built-in command
    if (args[0] == "exit") {
        status = false; // This sets the status to false, which will exit the shell
        return 0;
    } else if (args[0] == "clear") {
        commandCaller.clear();
    } else {
        cout << "Command not found: " << args[0] << endl;
    }

    return 1;
}