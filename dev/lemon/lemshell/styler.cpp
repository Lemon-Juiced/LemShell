// Implementation file for the Styler
#include "styler.h"
#include <iostream>
#include <string>

using namespace std;

string apply_style(string text, string style) {
    if (style == "blue") {
        return "\033[34m" + text + "\033[0m";
    } else if (style == "green") {
        return "\033[32m" + text + "\033[0m";
    } else if (style == "red") {
        return "\033[31m" + text + "\033[0m";
    } else if (style == "yellow") {
        return "\033[33m" + text + "\033[0m";
    } else {
        return text; // Return the text as is
    }
}