// Header File for the Built-in Commands
#pragma once
#include <iostream>
#include <map>
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
    #include <unistd.h>a
    #define GETCWD getcwd
#endif

using namespace std;

// Function declarations for built-in commands
int lemshell_cd(vector<string> args);
int lemshell_clear(vector<string> args);
int lemshell_exit(vector<string> args, int &status);
int lemshell_help(vector<string> args);
int lemshell_history(vector<string> args, map<int, vector<string>> &history, int &hNum);
int lemshell_history_clear(vector<string> args, map<int, vector<string>> &history, int &hNum);

// Helper functions
string get_os();
int print_error(string error);