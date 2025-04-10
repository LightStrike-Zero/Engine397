// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// FileHandler Class Header: Manages file handling as required

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

//-------------------------------- Libraries ----------------------------------
#include <string>
#include <fstream>
#include <sstream>

//----------------------------- File Handler Class ----------------------------
class FileHandler
{
public:
    static std::string readTextFile(const std::string& path);
};



#endif //FILEHANDLER_H
