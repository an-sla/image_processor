#pragma once
#include "file_work.h"
#include "filters.h"

// Negative filter:
class Negative : public BaseFilter {
public:
    bool ParamChecker(FileEntry& user_args) override;
    Image Implement(FileEntry& user_args, Image& original) override;
};
