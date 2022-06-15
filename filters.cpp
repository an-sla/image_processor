#include <algorithm>
#include <cstdlib>
#include <cmath>

#include "crop.h"
#include "edge_detection.h"
#include "filters.h"
#include "grey_scale.h"
#include "negative.h"
#include "sharpening.h"


bool GreyScale::ParamChecker(FileEntry& user_args) {
    if (user_args.filter_attributes_.empty() ||
        user_args.filter_attributes_.find("-gs") == user_args.filter_attributes_.end()) {
        return true;
    } else {
        throw std::invalid_argument("Greyscale takes no parameters. Try again.");
    }
}

Image GreyScale::Implement(FileEntry& user_args, Image& original) {
    if (ParamChecker(user_args)) {
        Image result = original;
        // Iterating over the canvas to apply effect:
        for (auto& v : result.canvas_) {
            for (auto& p : v) {
                uint8_t grey = blue_ * p.b + green_ * p.g + red_ * p.r;
                p.r = p.b = p.g = grey;
            }
        }
        return result;
    }
    throw std::bad_exception();
}

bool Negative::ParamChecker(FileEntry& user_args) {
    if (user_args.filter_attributes_.empty() ||
        user_args.filter_attributes_.find("-neg") == user_args.filter_attributes_.end()) {
        return true;
    } else {
        throw std::invalid_argument("Negative takes no parameters. Try again.");
    }
}

Image Negative::Implement(FileEntry& user_args, Image& original) {
    if (ParamChecker(user_args)) {
        Image result = original;
        // Iterating over the canvas to apply effect:
        for (auto& v : result.canvas_) {
            for (auto& p : v) {
                p.r = MAXIMUM - p.r;
                p.b = MAXIMUM - p.b;
                p.g = MAXIMUM - p.g;
            }
        }
        return result;
    }
    throw std::bad_exception();
}

bool Crop::ParamChecker(FileEntry& user_args) {
    if (user_args.filter_attributes_.empty() ||
        (user_args.filter_attributes_.find("-crop") == user_args.filter_attributes_.end())) {
        throw std::invalid_argument("Crop takes parameters. Include them and try again.");
    } else if (user_args.filter_attributes_["-crop"].size() != 2) {
        throw std::invalid_argument("Crop takes exactly 2 parameters. Include them and try again.");
    } else {
        height_ = strtol(user_args.filter_attributes_["-crop"][0].c_str(), nullptr, 10);
        width_ = strtol(user_args.filter_attributes_["-crop"][1].c_str(), nullptr, 10);
        if (width_ == 0L || height_ == 0L) {
            throw std::invalid_argument("Crop parameters must be numbers. Try again.");
        } else {
            return true;
        }
    }
}

Image Crop::Implement(FileEntry& user_args, Image& original) {
    if (ParamChecker(user_args)) {
        if (height_ < original.height_) {
            std::reverse(original.canvas_.begin(), original.canvas_.end());
            original.canvas_.resize(height_);
            std::reverse(original.canvas_.begin(), original.canvas_.end());
            original.height_ = static_cast<size_t>(height_);
        }
        if (width_ < original.width_) {
            for (auto& row : original.canvas_) {
                row.resize(width_);
            }
            original.width_ = static_cast<size_t>(width_);
        }
        return original;
    }
    throw std::bad_exception();
}

bool Sharpening::ParamChecker(FileEntry& user_args) {
    if (user_args.filter_attributes_.empty() ||
        user_args.filter_attributes_.find("-sharp") == user_args.filter_attributes_.end()) {
        return true;
    } else {
        throw std::invalid_argument("Sharpening takes no parameters. Try again.");
    }
}

Image Sharpening::Implement(FileEntry& user_args, Image& original) {
    if (ParamChecker(user_args)) {
        Image result = original;
        for (size_t i = 0; i < original.height_; ++i) {
            for (size_t j = 0; j < original.width_; ++j) {
                // Iterating over borders and corners (code varies, cannot be separated into uniform function):
                if (i == 0 && j == 0) {
                    // left top corner
                    result.canvas_[i][j].r =
                        std::clamp(main_pix_ * original.canvas_[i][j].r + other_pix_ * original.canvas_[i + 1][j].r +
                                       other_pix_ * original.canvas_[i][j + 1].r,
                                   MINIMUM, MAXIMUM);
                    result.canvas_[i][j].g =
                        std::clamp(main_pix_ * original.canvas_[i][j].g + other_pix_ * original.canvas_[i + 1][j].g +
                                       other_pix_ * original.canvas_[i][j + 1].g,
                                   MINIMUM, MAXIMUM);
                    result.canvas_[i][j].b =
                        std::clamp(main_pix_ * original.canvas_[i][j].b + other_pix_ * original.canvas_[i + 1][j].b +
                                       other_pix_ * original.canvas_[i][j + 1].b,
                                   MINIMUM, MAXIMUM);
                } else if (i == 0 && j == original.width_ - 1) {
                    // right top corner
                    result.canvas_[i][j].r =
                        std::clamp(other_pix_ * original.canvas_[i][j - 1].r + main_pix_ * original.canvas_[i][j].r +
                                       other_pix_ * original.canvas_[i + 1][j].r,
                                   MINIMUM, MAXIMUM);
                    result.canvas_[i][j].g =
                        std::clamp(other_pix_ * original.canvas_[i][j - 1].g + main_pix_ * original.canvas_[i][j].g +
                                       other_pix_ * original.canvas_[i + 1][j].g,
                                   MINIMUM, MAXIMUM);
                    result.canvas_[i][j].b =
                        std::clamp(other_pix_ * original.canvas_[i][j - 1].b + main_pix_ * original.canvas_[i][j].b +
                                       other_pix_ * original.canvas_[i + 1][j].b,
                                   MINIMUM, MAXIMUM);
                } else if (i == original.height_ - 1 && j == 0) {
                    // left bottom corner
                    result.canvas_[i][j].r =
                        std::clamp(other_pix_ * original.canvas_[i - 1][j].r + main_pix_ * original.canvas_[i][j].r +
                                       other_pix_ * original.canvas_[i][j + 1].r,
                                   MINIMUM, MAXIMUM);
                    result.canvas_[i][j].g =
                        std::clamp(other_pix_ * original.canvas_[i - 1][j].g + main_pix_ * original.canvas_[i][j].g +
                                       other_pix_ * original.canvas_[i][j + 1].g,
                                   MINIMUM, MAXIMUM);
                    result.canvas_[i][j].b =
                        std::clamp(other_pix_ * original.canvas_[i - 1][j].b + main_pix_ * original.canvas_[i][j].b +
                                       other_pix_ * original.canvas_[i][j + 1].b,
                                   MINIMUM, MAXIMUM);

                } else if (i == original.height_ - 1 && j == original.width_ - 1) {
                    // right bottom corner
                    result.canvas_[i][j].r =
                        std::clamp(other_pix_ * original.canvas_[i][j - 1].r +
                                       other_pix_ * original.canvas_[i - 1][j].r + main_pix_ * original.canvas_[i][j].r,
                                   MINIMUM, MAXIMUM);
                    result.canvas_[i][j].g =
                        std::clamp(other_pix_ * original.canvas_[i][j - 1].g +
                                       other_pix_ * original.canvas_[i - 1][j].g + main_pix_ * original.canvas_[i][j].g,
                                   MINIMUM, MAXIMUM);
                    result.canvas_[i][j].b =
                        std::clamp(other_pix_ * original.canvas_[i][j - 1].b +
                                       other_pix_ * original.canvas_[i - 1][j].b + main_pix_ * original.canvas_[i][j].b,
                                   MINIMUM, MAXIMUM);

                } else if (i == 0) {
                    // top edge
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].r + main_pix_ * original.canvas_[i][j].r +
                            other_pix_ * original.canvas_[i + 1][j].r + other_pix_ * original.canvas_[i][j + 1].r,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].g = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].g + main_pix_ * original.canvas_[i][j].g +
                            other_pix_ * original.canvas_[i + 1][j].g + other_pix_ * original.canvas_[i][j + 1].g,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].b = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].b + main_pix_ * original.canvas_[i][j].b +
                            other_pix_ * original.canvas_[i + 1][j].b + other_pix_ * original.canvas_[i][j + 1].b,
                        MINIMUM, MAXIMUM);

                } else if (i == original.height_ - 1) {
                    // bottom edge
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].r + other_pix_ * original.canvas_[i - 1][j].r +
                            main_pix_ * original.canvas_[i][j].r + other_pix_ * original.canvas_[i][j + 1].r,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].g = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].g + other_pix_ * original.canvas_[i - 1][j].g +
                            main_pix_ * original.canvas_[i][j].g + other_pix_ * original.canvas_[i][j + 1].g,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].b = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].b + other_pix_ * original.canvas_[i - 1][j].b +
                            main_pix_ * original.canvas_[i][j].b + other_pix_ * original.canvas_[i][j + 1].b,
                        MINIMUM, MAXIMUM);
                } else if (j == 0) {
                    // left edge
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i - 1][j].r + main_pix_ * original.canvas_[i][j].r +
                            other_pix_ * original.canvas_[i + 1][j].r + other_pix_ * original.canvas_[i][j + 1].r,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].g = std::clamp(
                        other_pix_ * original.canvas_[i - 1][j].g + main_pix_ * original.canvas_[i][j].g +
                            other_pix_ * original.canvas_[i + 1][j].g + other_pix_ * original.canvas_[i][j + 1].g,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].b = std::clamp(
                        other_pix_ * original.canvas_[i - 1][j].b + main_pix_ * original.canvas_[i][j].b +
                            other_pix_ * original.canvas_[i + 1][j].b + other_pix_ * original.canvas_[i][j + 1].b,
                        MINIMUM, MAXIMUM);
                } else if (j == original.width_ - 1) {
                    // right edge
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].r + other_pix_ * original.canvas_[i - 1][j].r +
                            main_pix_ * original.canvas_[i][j].r + other_pix_ * original.canvas_[i + 1][j].r,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].g = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].g + other_pix_ * original.canvas_[i - 1][j].g +
                            main_pix_ * original.canvas_[i][j].g + other_pix_ * original.canvas_[i + 1][j].g,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].b = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].b + other_pix_ * original.canvas_[i - 1][j].b +
                            main_pix_ * original.canvas_[i][j].b + other_pix_ * original.canvas_[i + 1][j].b,
                        MINIMUM, MAXIMUM);
                } else {
                    // all other cases — inside canvas
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].r + other_pix_ * original.canvas_[i - 1][j].r +
                            main_pix_ * original.canvas_[i][j].r + other_pix_ * original.canvas_[i + 1][j].r +
                            other_pix_ * original.canvas_[i][j + 1].r,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].g = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].g + other_pix_ * original.canvas_[i - 1][j].g +
                            main_pix_ * original.canvas_[i][j].g + other_pix_ * original.canvas_[i + 1][j].g +
                            other_pix_ * original.canvas_[i][j + 1].g,
                        MINIMUM, MAXIMUM);
                    result.canvas_[i][j].b = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].b + other_pix_ * original.canvas_[i - 1][j].b +
                            main_pix_ * original.canvas_[i][j].b + other_pix_ * original.canvas_[i + 1][j].b +
                            other_pix_ * original.canvas_[i][j + 1].b,
                        MINIMUM, MAXIMUM);
                }
            }
        }
        return result;
    }
    throw std::bad_exception();
}

bool Edge::ParamChecker(FileEntry& user_args) {
    if (user_args.filter_attributes_.empty() ||
        (user_args.filter_attributes_.find("-edge") == user_args.filter_attributes_.end())) {
        throw std::invalid_argument("Edge takes one parameter. Include it and try again.");
    } else if (user_args.filter_attributes_["-edge"].size() != 1) {
        throw std::invalid_argument("Edge takes exactly 1 parameter. Try again");
    } else {
        char* end_ptr;
        threshold_ = std::lround(strtod(user_args.filter_attributes_["-edge"][0].c_str(), &end_ptr) * 255);
        if (*end_ptr != '\0') {
            throw std::invalid_argument("Edge parameter must be a number. Try again.");
        } else {
            return true;
        }
    }
}

Image Edge::Implement(FileEntry& user_args, Image& original) {
    if (ParamChecker(user_args)) {
        GreyScale gs;
        user_args.filters_.push_back("-gs");
        original = gs.Implement(user_args, original);
        Image result = original;
        for (size_t i = 0; i < original.height_; ++i) {
            for (size_t j = 0; j < original.width_; ++j) {
                if (i == 0 && j == 0) {
                    // left top corner
                    result.canvas_[i][j].r =
                        std::clamp(main_pix_ * original.canvas_[i][j].r + other_pix_ * original.canvas_[i + 1][j].r +
                                       other_pix_ * original.canvas_[i][j + 1].r,
                                   MINIMUM, MAXIMUM);
                    if (result.canvas_[i][j].r > threshold_) {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 255;
                    } else {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 0;
                    }
                } else if (i == 0 && j == original.width_ - 1) {
                    // right top corner
                    result.canvas_[i][j].r =
                        std::clamp(other_pix_ * original.canvas_[i][j - 1].r + main_pix_ * original.canvas_[i][j].r +
                                       other_pix_ * original.canvas_[i + 1][j].r,
                                   MINIMUM, MAXIMUM);
                    if (result.canvas_[i][j].r > threshold_) {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 255;
                    } else {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 0;
                    }
                } else if (i == original.height_ - 1 && j == 0) {
                    // left bottom corner
                    result.canvas_[i][j].r =
                        std::clamp(other_pix_ * original.canvas_[i - 1][j].r + main_pix_ * original.canvas_[i][j].r +
                                       other_pix_ * original.canvas_[i][j + 1].r,
                                   MINIMUM, MAXIMUM);
                    if (result.canvas_[i][j].r > threshold_) {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 255;
                    } else {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 0;
                    }
                } else if (i == original.height_ - 1 && j == original.width_ - 1) {
                    // right bottom corner
                    result.canvas_[i][j].r =
                        std::clamp(other_pix_ * original.canvas_[i][j - 1].r +
                                       other_pix_ * original.canvas_[i - 1][j].r + main_pix_ * original.canvas_[i][j].r,
                                   MINIMUM, MAXIMUM);
                    if (result.canvas_[i][j].r > threshold_) {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 255;
                    } else {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 0;
                    }
                } else if (i == 0) {
                    // top edge
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].r + main_pix_ * original.canvas_[i][j].r +
                            other_pix_ * original.canvas_[i + 1][j].r + other_pix_ * original.canvas_[i][j + 1].r,
                        MINIMUM, MAXIMUM);
                    if (result.canvas_[i][j].r > threshold_) {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 255;
                    } else {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 0;
                    }
                } else if (i == original.height_ - 1) {
                    // bottom edge
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].r + other_pix_ * original.canvas_[i - 1][j].r +
                            main_pix_ * original.canvas_[i][j].r + other_pix_ * original.canvas_[i][j + 1].r,
                        MINIMUM, MAXIMUM);
                    if (result.canvas_[i][j].r > threshold_) {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 255;
                    } else {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 0;
                    }
                } else if (j == 0) {
                    // left edge
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i - 1][j].r + main_pix_ * original.canvas_[i][j].r +
                            other_pix_ * original.canvas_[i + 1][j].r + other_pix_ * original.canvas_[i][j + 1].r,
                        MINIMUM, MAXIMUM);
                    if (result.canvas_[i][j].r > threshold_) {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 255;
                    } else {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 0;
                    }
                } else if (j == original.width_ - 1) {
                    // right edge
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].r + other_pix_ * original.canvas_[i - 1][j].r +
                            main_pix_ * original.canvas_[i][j].r + other_pix_ * original.canvas_[i + 1][j].r,
                        MINIMUM, MAXIMUM);
                    if (result.canvas_[i][j].r > threshold_) {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 255;
                    } else {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 0;
                    }
                } else {
                    result.canvas_[i][j].r = std::clamp(
                        other_pix_ * original.canvas_[i][j - 1].r + other_pix_ * original.canvas_[i - 1][j].r +
                            main_pix_ * original.canvas_[i][j].r + other_pix_ * original.canvas_[i + 1][j].r +
                            other_pix_ * original.canvas_[i][j + 1].r,
                        MINIMUM, MAXIMUM);
                    if (result.canvas_[i][j].r > threshold_) {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 255;
                    } else {
                        result.canvas_[i][j].r = result.canvas_[i][j].g = result.canvas_[i][j].b = 0;
                    }
                }
            }
        }
        user_args.filters_.pop_back();
        return result;
    }
    throw std::bad_exception();
}
