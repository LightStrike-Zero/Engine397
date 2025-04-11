// 2025 Semester 1: Murdoch University
// ICT397 Advanced Game Engine Design and Programming
// Created by Buko in March 2025.
// Assignment 1: Game Engine
// FileHandler Class Header: Manages file handling as required

/**
 * @file DierctionalLight.h
 * @brief FileHandler Class Header: Manages file handling as required
 * This file defines the FileHandler class used to read text files into strings.
 * It is used for tasks like loading help manuals or external configuration data.
 * @author Buko
 * @date March 2025
 */

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

//-------------------------------- Libraries ----------------------------------
#include <string>
#include <fstream>
#include <sstream>

//----------------------------- File Handler Class ----------------------------
/**
 * @class FileHandler
 * @brief Static utility class for handling file reading operations.
 *
 * The FileHandler class provides a single static method for reading the contents of a
 * text file into a string. This is particularly useful for displaying text-based assets
 * such as help manuals or other configuration data within the game engine.
 */
class FileHandler
{
public:

    /**
     * @brief Reads the entire contents of a text file and returns it as a string.
     * @param path The relative or absolute path to the file.
     * @return A std::string containing the full contents of the file.
     */
    static std::string readTextFile(const std::string& path);
};



#endif //FILEHANDLER_H
