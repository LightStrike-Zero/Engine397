/**
* @file ImageData.h
 * @brief Defines the RawImageData structure for handling raw image data.
 *
 * The RawImageData structure is used to represent raw image data, including
 * both compressed and uncompressed formats. It provides constructors for
 * initializing the data with relevant metadata such as dimensions, size, and format.
 *
 * @details
 * - Supports both compressed and uncompressed image data.
 * - Includes metadata like width, height, data size, and format.
 * - Uses default values for compressed images (width and height set to -1).
 *
 * @date Created: 12/04/2025
 */

#ifndef IMAGEDATA_H
#define IMAGEDATA_H
#include <string>
#include <utility>

/**
 * @struct RawImageData
 * @brief Represents raw image data with associated metadata.
 *
 * The RawImageData structure is designed to store raw image data along with
 * its metadata, such as dimensions, size, compression status, and format for transmission.
 */
struct RawImageData {
    unsigned char* data; ///< Pointer to the raw image data.
    size_t dataSize;     ///< Size of the raw image data in bytes.
    int width;           ///< Width of the image (-1 if compressed).
    int height;          ///< Height of the image (-1 if compressed).
    bool isCompressed;   ///< Flag indicating whether the image is compressed.
    std::string format;  ///< Format of the image (e.g., "PNG", "JPEG").

    /**
     * @brief Constructor for compressed image data.
     *
     * Initializes the RawImageData structure for compressed images, where
     * width and height are set to -1 by default.
     *
     * @param data Pointer to the raw image data.
     * @param dataSize Size of the raw image data in bytes.
     * @param format Format of the image (default is an empty string).
     */
    RawImageData(unsigned char* data, const size_t dataSize, std::string format = "")
        : data(data), dataSize(dataSize), width(-1), height(-1), 
          isCompressed(true), format(std::move(format)) {}

    /**
     * @brief Constructor for uncompressed image data.
     *
     * Initializes the RawImageData structure for uncompressed images, where
     * width and height are explicitly provided.
     *
     * @param data Pointer to the raw image data.
     * @param width Width of the image.
     * @param height Height of the image.
     * @param dataSize Size of the raw image data in bytes.
     * @param format Format of the image (default is an empty string).
     */
    RawImageData(unsigned char* data, const int width, const int height, const size_t dataSize,
                 std::string  format = "")
        : data(data), dataSize(dataSize), width(width), height(height), 
          isCompressed(false), format(std::move(format)) {}

    /**
     * @brief Default destructor.
     *
     * Cleans up the RawImageData structure.
     */
    ~RawImageData() {
        delete[] data;
    };
};

#endif //IMAGEDATA_H
