// General Include Statements
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// Platform Specific Include Statements
#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
    #define GETCWD _getcwd
#else
    #include <sys/wait.h>
    #include <unistd.h>
    #define GETCWD getcwd
#endif

using namespace std;

// Global Constants
int status = true;

// Function Prototypes
void shell_loop();
string get_directory();
string apply_style(string text, string style);
vector<char> read_line();
vector<string> split_line(vector<char> line);
int execute(vector<string> args);
int execute_command(vector<string> args);
int print_error(string error);

// Built-in Commands Prototypes
int lemshell_cd(vector<string> args);
int lemshell_clear(vector<string> args);
int lemshell_exit(vector<string> args);
int lemshell_help(vector<string> args);

/**
 * A simple shell program that can execute commands
 * The main function of the program will run the shell's loop
 * 
 * This version is currently meant to work specifically on Windows.
 * It will be updated to work on Linux and other Unix-like operating systems in the future.
 * 
 * Compile with:
 * g++ lemshell.cpp -o lemshell
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
    if (GETCWD(cwd, sizeof(cwd)) != NULL) {
        return cwd;
    } else {
        return "";
    }
}

/**
 * Apply a style to the text
 * The style is applied using ANSI escape codes
 * 
 * Current Acceptable styles:
 * "blue" - Blue text
 * "red" - Red text
 * "yellow" - Yellow text
 * 
 * @param text The text to style
 * @param style The style to apply
 * @return The styled text
 */
string apply_style(string text, string style) {
    if (style == "blue") {
        return "\033[34m" + text + "\033[0m";
    } else if (style == "red") {
        return "\033[31m" + text + "\033[0m";
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
        return lemshell_exit(args);
    } else if (args[0] == "help") {
        return lemshell_help(args);
    }

    // Else execute the new process on this thread
    return 1;
}

/**
 * Print an error message to cerr
 * 
 * @param error The error message to print
 * @return The status of the execution
 */
int print_error(string error){
    cerr << apply_style("Error: " + error, "red") << endl;
    return 0;
}

/**
 * Built-in command to change the directory
 * This command should have received two arguments:
 * 0. The cd command itself
 * 1. The directory to change to
 * If the directory is not provided, the command will not execute
 * 
 * @param args The arguments to execute
 * @return The status of the execution
 */
int lemshell_cd(vector<string> args){
    // Check if the directory is provided
    if (args.size() < 2) {
        return print_error("No directory provided");
    }

    // Change the directory
    if (chdir(args[1].c_str()) != 0) {
        return print_error("Directory not found");
    }

    return 1;
}

/**
 * Built-in command to clear the screen
 * 
 * @param args The arguments to execute
 * @return The status of the execution
 */
int lemshell_clear(vector<string> args){
    cout << "\033[2J\033[1;1H"; // Clear the screen
    return 1;
}

/**
 * Built-in command to exit the shell
 * 
 * @param args The arguments to execute
 * @return The status of the execution
 */
int lemshell_exit(vector<string> args){
    status = 0;
    return 1;
}

/**
 * Built-in command to display the help menu
 * 
 * @param args The arguments to execute
 * @return The status of the execution
 */
int lemshell_help(vector<string> args){
    lemshell_clear(args); // Clear the screen to make room for the help menu
    cout << apply_style("LemShell", "yellow") << " - An OS-independent shell made by a Lemon" << endl;
    cout << "This shell is written in C++ and is not yet finished." << endl;
    cout << "The following built-in commands are available:" << endl;
    cout << apply_style("cd", "blue") << " - Change the directory" << endl;
    cout << apply_style("clear", "blue") << " - Clear the screen" << endl;
    cout << apply_style("exit", "blue") << " - Exit the shell" << endl;
    cout << apply_style("help", "blue") << " - Display this help menu" << endl;
    return 1;
}