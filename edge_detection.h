#pragma once
#include "file_work.h"
#include "filters.h"

// Edge_detection filter:
class Edge : public BaseFilter {
private:
    unsigned int threshold_;
    const int main_pix_ = 4;
    const int other_pix_ = -1;

public:
    bool ParamChecker(FileEntry& user_args) override;
    Image Implement(FileEntry& user_args, Image& original) override;
};
