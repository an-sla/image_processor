#include "controller.h"
#include "file_work.h"


int main(int argc, char* argv[]) {
    FileEntry user_args = Parsing(argc, argv);
    Image image = LoadFile(user_args.file_in_);
    image = Controller(image, user_args);
    SaveFile(user_args.file_out_, image);
    return 0;
}
