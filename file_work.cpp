#include "file_work.h"

FileEntry Parsing(int argc, char* argv[]) {
    if (argc < 3) {
        throw std::invalid_argument(
            "\nWrong number of arguments submitted. \nThis program uses the format: "
            "{program name} {read-file path} {write-file path} + {- filter flags and parameters}. "
            "\nIt can apply greyscale, crop, negative, sharp, and edge-detection filters to BMP images.");
    }
    FileEntry user_args;
    user_args.program_name_ = argv[0];
    user_args.file_in_ = argv[1];
    user_args.file_out_ = argv[2];
    for (auto i = 3; i < argc;) {
        if (argv[i][0] == '-') {
            std::string curr_filter = argv[i];
            if (user_args.REALISED_FILTERS.find(curr_filter) == user_args.REALISED_FILTERS.end()) {
                throw std::invalid_argument("Invalid filter flag: \"" + std::string(curr_filter) +
                                            "\" not realised in this program.");
            }
            user_args.filters_.push_back(curr_filter);
            ++i;
            while (i < argc && argv[i][0] != '-') {
                user_args.filter_attributes_[curr_filter].push_back(argv[i]);
                ++i;
            }
        } else {
            throw std::invalid_argument(
                "These are not the droids you are looking for. Use a filter flag starting with \"–\".");
        }
    }
    return user_args;
}

std::string HexAsciiConverter(char* const array_ptr, size_t start, size_t end) {
    std::string result;
    std::stringstream ss;
    for (size_t i = start; i < end; ++i) {
        ss << array_ptr[i];
    }
    ss >> std::hex >> result;
    return result;
}

size_t EndianCharIntConverter(const char* object, size_t position) {
    size_t result = static_cast<uint8_t>(object[position + 1]);
    result <<= 8;
    result += static_cast<size_t>(object[position]);
    return result;
}

Image LoadFile(const std::string& file_name) {
    std::ifstream file(file_name, std::ios::binary);
    Image our_image;
    file.read(our_image.header_, 54);
    if (!file) {
        throw std::invalid_argument("Cannot read image file.");
    }
    if (HexAsciiConverter(our_image.header_, 0, 2) != "BM") {
        throw std::invalid_argument("File does not contain BMP signature in header.");
    }

    our_image.width_ = EndianCharIntConverter(our_image.header_, 18);
    our_image.height_ = EndianCharIntConverter(our_image.header_, 22);
    if (!our_image.width_ || !our_image.height_) {
        throw std::invalid_argument("Image size could not be processed.");
    }
    our_image.data_offset_ = EndianCharIntConverter(our_image.header_, 10);
    if (our_image.data_offset_ != 54) {
        throw std::invalid_argument("Image header offset does not accord with BMP format.");
    }
    our_image.bits_ppx_ = EndianCharIntConverter(our_image.header_, 28);
    if (our_image.bits_ppx_ != 24) {
        throw std::invalid_argument("Image must be 24-bit BMP to work in this program.");
    }
    our_image.bytes_ppx_ = our_image.bits_ppx_ / 8;
    our_image.canvas_ = std::vector<std::vector<PIXEL>>(our_image.height_, std::vector<PIXEL>(our_image.width_));
    our_image.real_size_ = our_image.bytes_ppx_ * our_image.width_;
    our_image.padding_ = (4 - our_image.real_size_ % 4) % 4;
    size_t start = our_image.data_offset_;
    size_t end = start + our_image.real_size_;
    for (size_t i = 0; i < our_image.height_; ++i) {
        file.seekg(start);
        for (size_t j = start; j < end;) {
            file.read(reinterpret_cast<char*>(&our_image.canvas_[i][(j - start) / 3].b), 1);
            file.read(reinterpret_cast<char*>(&our_image.canvas_[i][(j - start) / 3].g), 1);
            file.read(reinterpret_cast<char*>(&our_image.canvas_[i][(j - start) / 3].r), 1);
            j += 3;
        }
        start = end + our_image.padding_;
        end = start + our_image.real_size_;
    }
    return our_image;
}

void WriteByte(std::ostream& out, uint8_t value) {  // Write bytes to binary output stream
    out.write(reinterpret_cast<char*>(&value), 1);
}

void WriteInt(std::ostream& out, uint32_t value) {  // Write int to binary output stream
    for (size_t i = 0; i < BYTES_PER_INT; ++i) {  // Loop for all bytes in i variable
        WriteByte(out, (value >> (i * BITS_PER_BYTE)) & BYTE_MASK); // Consecutively highlighting all bytes in number
    }
}

void WriteZeros(std::ostream& out, size_t count) {  // Write to output stream the number (count) of bytes equal to 0
    for (size_t i = 0; i < count; ++i) {
        WriteByte(out, 0);
    }
}

void SaveFile(const std::string& file_name, const Image& image) {
    std::ofstream out(file_name, std::ios::binary);
    // Пишем header:
    WriteByte(out, BMP_SIGNATURE_BYTE_1);
    WriteByte(out, BMP_SIGNATURE_BYTE_2);
    auto file_size = HEADER_SIZE + image.height_ * (image.width_ * 3 + image.padding_);
    WriteInt(out, file_size);
    WriteZeros(out, 4);
    WriteInt(out, HEADER_SIZE);
    /////
    WriteInt(out, INFO_HEADER_SIZE);
    WriteInt(out, image.width_);
    WriteInt(out, image.height_);
    // planes = 01; 1 0
    WriteByte(out, PLANES);
    WriteZeros(out, PLANES);
    // bit_per_pixel = [0, 24]
    WriteByte(out, BITS_PPX);
    WriteZeros(out, PLANES);
    WriteZeros(out, BITS_PPX);
    // Пишем картинку:
    for (size_t i = 0; i < image.height_; ++i) {
        for (size_t j = 0; j < image.width_; ++j) {
            WriteByte(out, image.canvas_[i][j].b);
            WriteByte(out, image.canvas_[i][j].g);
            WriteByte(out, image.canvas_[i][j].r);
        }
        WriteZeros(out, image.padding_);
    }
}
