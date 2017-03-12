#pragma once
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include "fstream"

struct Path;
void get_comand(void);
void start_up(Path & all_pathes);
std::vector <std::string> parsing (std::string inputLine);
void add_spaces(std::string input_line, size_t& pos1, size_t pos2);
std::string give_me_color(int);
