#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

// Working with the command line:
class FileEntry {  // Class for storing command line arguments
public:
    std::string program_name_;
    std::string file_in_;
    std::string file_out_;
    std::vector<std::string> filters_;
    std::map<std::string, std::vector<std::string>> filter_attributes_;
    std::set<std::string> REALISED_FILTERS = {"-gs", "-crop", "-neg", "-sharp", "-edge"};
};

FileEntry Parsing(int argc, char* argv[]);

// Working with pixels:
class PIXEL {  // Colour class:
public:
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

// Work with the image
class Image {  // Class for working with the image, its header, and parameters
public:
    char header_[54];
    // Header description:
    size_t width_ = 0;     // 18, 22
    size_t height_ = 0;    // 22, 26
    size_t bits_ppx_ = 0;  // 28, 30
    uint16_t data_offset_{};
    // Our supplemental variables:
    int bytes_ppx_{};
    int real_size_{};
    int padding_{};
    std::vector<std::vector<PIXEL>> canvas_{};
};

// Working with the file:
Image LoadFile(const std::string& file_name);
void SaveFile(const std::string& file_name, const Image& image);

const size_t BMP_SIGNATURE_BYTE_1 = 0x42;
const size_t BMP_SIGNATURE_BYTE_2 = 0x4D;
const size_t HEADER_SIZE = 54;
const size_t INFO_HEADER_SIZE = 40;

const size_t BITS_PER_BYTE = 8;  // Number of bits per byte
const size_t BYTES_PER_INT = 4;  // Number of bytes in an `int` type variable
const size_t BYTE_MASK = 0xFF;   // Mask for selecting last byte

const size_t PLANES = 1;
const size_t BITS_PPX = 24;
