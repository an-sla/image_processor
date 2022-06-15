#pragma once
#include "file_work.h"
#include "filters.h"

// Greyscale filter:
class GreyScale : public BaseFilter {
private:
    const double blue_ = 0.299;
    const double red_ = 0.114;
    const double green_ = 0.587;

public:
    bool ParamChecker(FileEntry& user_args) override;
    Image Implement(FileEntry& user_args, Image& original) override;
};
