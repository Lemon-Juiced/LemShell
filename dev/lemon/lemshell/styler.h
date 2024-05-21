// Header file for the Styler
#pragma once

#include <iostream>
#include <string>

using namespace std;

/**
 * Apply a style to the text
 * The style is applied using ANSI escape codes
 * 
 * Current Acceptable styles:
 * "blue" - Blue text
 * "green" - Green text
 * "red" - Red text
 * "yellow" - Yellow text
 * 
 * @param text The text to style
 * @param style The style to apply
 * @return The styled text
 */
string apply_style(string text, string style);
