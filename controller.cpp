#include "controller.h"
#include "crop.h"
#include "edge_detection.h"
#include "grey_scale.h"
#include "negative.h"
#include "sharpening.h"


Image Controller(Image& image, FileEntry& info) {
    for (const auto& i : info.filters_) {
        if (i == "-gs") {
            GreyScale gs;
            image = gs.Implement(info, image);
        } else if (i == "-neg") {
            Negative neg;
            image = neg.Implement(info, image);
        } else if (i == "-crop") {
            Crop crop;
            image = crop.Implement(info, image);
        } else if (i == "-sharp") {
            Sharpening sharp;
            image = sharp.Implement(info, image);
        } else if (i == "-edge") {
            Edge edge;
            image = edge.Implement(info, image);
        }
    }
    return image;
}
