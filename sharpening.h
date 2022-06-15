#pragma once
#include "file_work.h"
#include "filters.h"

// Sharpening filter:
class Sharpening : public BaseFilter {
private:
    const int other_pix_ = -1;
    const int main_pix_ = 5;

public:
    bool ParamChecker(FileEntry& user_args) override;
    Image Implement(FileEntry& user_args, Image& original) override;
};
