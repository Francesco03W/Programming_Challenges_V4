//C libraries
#include <cstring>
#include <time.h>
//c++ libraries
//#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

enum priority {low,medium,high,urgent};

struct note
{
    note(std::string text,priority p) : text(text), p(p) {}; 
    std::string text;
    priority p=low;
    char cdate[26];
};