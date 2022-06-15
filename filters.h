#pragma once
#include "file_work.h"

class BaseFilter {  // Abstract class for filter
    virtual bool ParamChecker(FileEntry& user_args) = 0;
    virtual Image Implement(FileEntry& user_args, Image& original) = 0;
};

const int MAXIMUM = 255;
const int MINIMUM = 0;
