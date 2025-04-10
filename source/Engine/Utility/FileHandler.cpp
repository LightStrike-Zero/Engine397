// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// FileHandler Class Header: Manages file handling as required

//-------------------------------- Libraries ----------------------------------
#include "FileHandler.h"
#include <iostream>

//-----------------------------------------------------------------------------
std::string FileHandler::readTextFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] Failed to open file: " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
