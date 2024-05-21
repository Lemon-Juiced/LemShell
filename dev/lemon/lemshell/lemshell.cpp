// General Include Statements
#include <iostream>
#include <string>
#include <vector>
#include "command.h"

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
 * This version is currently meant to work specifically on Windows.
 * It will be updated to work on Linux and other Unix-like operating systems in the future.
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
 * Execute the arguments by creating a new process for each command.
 * 
 * @param args The arguments to execute
 * @return The status of the execution
 */
int execute(vector<string> args) {
    // This does not work on Windows
    /*
    pid_t pid, wait_pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child Process
        if (execvp(args[0].c_str(), (char *const *)&args[0]) == -1) {
            string error = "Error: Command not found";
            cerr << apply_style(error, "red") << endl;
        }
    } else if (pid < 0) {
        // Error Forking
        string error = "Error: Forking failed";
        cerr << apply_style(error, "red") << endl;
    } else {
        // Parent Process
        do {
            wait_pid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
    */
}