// General Include Statements
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include "bi_com.h"
#include "styler.h"

// Platform Specific Include Statements
#ifdef _WIN32
    #include <direct.h> // Used for _getcwd in get_directory()
    #include <windows.h> // Used for system in execute_command()
    #define GETCWD _getcwd // Macro to get the current directory
#else
    #include <sys/wait.h> // Used for system in execute_command()
    #include <unistd.h> // Used for getcwd in get_directory()
    #define GETCWD getcwd // Macro to get the current directory
#endif

using namespace std;

// Global Constants
int status = true;
int hNum = 0;
map<int, vector<string>> history; // The int passed in here should be an hNum

// Function Prototypes
void shell_loop();
string get_directory();
string apply_style(string text, string style);
vector<char> read_line();
vector<string> split_line(vector<char> line);
int execute(vector<string> args);
int execute_command(vector<string> args);

/**
 * A simple shell program that can execute commands
 * The main function of the program will run the shell's loop
 * 
 * This version is currently meant to work specifically on Windows.
 * It will be updated to work on Linux and other Unix-like operating systems in the future.
 * 
 * This can be compiled using the following command: make
 * This can be run with the following command: ./lemshell (Unix) or .\lemshell.exe (Windows)
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
    if (GETCWD(cwd, sizeof(cwd)) != NULL) {
        return cwd;
    } else {
        return "";
    }
}

/**
 * Read a line from the user
 * The user also has additional input options here, for example:
 * The user can use the up and down arrow keys to scroll through the history.
 * The user can press the tab key to autocomplete commands.
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
 * Split the line into strings of commands
 * 
 * @param line The line to split
 * @return The vector of strings of commands
 */
vector<string> split_line(vector<char> line) {
    vector<string> args;
    string arg = "";
    for (char c : line) {
        if (c == ' ') {
            args.push_back(arg);
            arg = "";
        } else {
            arg += c;
        }
    }
    // Before pushing the last argument, check if it is empty
    if (!arg.empty()) args.push_back(arg);

    return args;
}

/**
 * Execute the arguments by creating a new process for each command.
 * 
 * @param args The arguments to execute
 * @return The status of the execution (1 for success, 0 for failure)
 */
int execute(vector<string> args) {
    // Check if the command is empty
    if (args.empty()) {
        return 1;
    }

    // Create a thread to execute the command
    thread p_thread(execute_command, args);

    // Wait for the process to finish
    p_thread.join();

    // Add the command to the history
    history[hNum] = args;

    // Increment the history number
    hNum++;

    // Return
    return status;
}

/**
 * Execute the command on its own thread
 * 
 * @param args The arguments to execute
 * @return The status of the execution
 */
int execute_command(vector<string> args){
    // Check if the command is a built-in command
    if (args[0] == "cd") {
        return lemshell_cd(args);
    } else if (args[0] == "clear") {
        return lemshell_clear(args);
    } else if (args[0] == "exit") {
        return lemshell_exit(args, status);
    } else if (args[0] == "help") {
        return lemshell_help(args);
    } else if (args[0] == "history") {
        return lemshell_history(args, history, hNum);
    }

    // Otherwise, we can execute this as a system command by converting the vector back to a string
    string command = "";
    for (string arg : args) {
        command += arg + " ";
    }
    system(command.c_str());

    // Else execute the new process on this thread
    return 1;
}