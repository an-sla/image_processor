#pragma once
#include "file_work.h"
#include "filters.h"

// Greyscale filter:
class Crop : public BaseFilter {
private:
    unsigned long width_;
    unsigned long height_;

public:
    bool ParamChecker(FileEntry& user_args) override;
    Image Implement(FileEntry& user_args, Image& original) override;
};
