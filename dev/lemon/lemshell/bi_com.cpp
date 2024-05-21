//Implementation File for the Built-in Commands
#include "bi_com.h"
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include "styler.h"

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
 * @param status The status of the shell
 * @return The status of the execution
 */
int lemshell_exit(vector<string> args, int &status){
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
    cout << "The current operating system is: " << get_os() << endl;
    cout << "The following built-in commands are available:" << endl;
    cout << apply_style("cd", "blue") << " - Change the directory" << endl;
    cout << apply_style("clear", "blue") << " - Clear the screen" << endl;
    cout << apply_style("exit", "blue") << " - Exit the shell" << endl;
    cout << apply_style("help", "blue") << " - Display this help menu" << endl;
    cout << apply_style("history", "blue") << " - Display the history of commands" << endl;
    cout << apply_style("history -c(lear)", "blue") << " - Clear the history of commands" << endl;
    return 1;
}

/**
 * Built-in command to display the history of commands
 * Since all keys are integers, the history is ordered by the order of execution
 * We can just decrement the history number until we get to 0
 * 
 * The history command can also be used to clear the history by passing the -c (or -clear) flag
 * 
 * @param args The arguments to execute
 * @param history The history of commands
 * @param hNum The number of commands executed
 * @return The status of the execution
  */
int lemshell_history(vector<string> args, map<int, vector<string>> &history, int &hNum){
    // Check if the history is empty
    if (history.empty()) {
        return print_error("No history available");
    }

    // Check if the clear flag is provided
    if (args.size() > 1 && (args[1] == "-c" || args[1] == "-clear")) {
        return lemshell_history_clear(args, history, hNum);
    }

    // Display the history
    cout << apply_style("History", "green") << endl;
    for (int i = hNum - 1; i >= 0; i--) {
        cout << apply_style(to_string(i), "blue") << ": ";
        for (string arg : history[i]) {
            cout << arg << " ";
        }
        cout << endl;
    }

    return 1;
}

/**
 * Built-in command to clear the history of commands (and reset the history number)
 * 
 * @param args The arguments to execute
 * @param history The history of commands
 * @param hNum The number of commands executed
 * @return The status of the execution
  */
int lemshell_history_clear(vector<string> args, map<int, vector<string>> &history, int &hNum){
    history.clear();
    hNum = 0;
    return 1;
}

/**
 * Get the operating system of the user
 * 
 * @return The operating system of the user
*/
string get_os(){
    #ifdef _WIN32
        return "Windows";
    #else
        return "Unix";
    #endif
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