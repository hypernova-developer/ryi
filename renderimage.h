#ifndef RENDERIMAGE_H
#define RENDERIMAGE_H

#include <vector>
#include <raylib.h>

struct RenderImage {
    char* path;
    Texture2D image;

    static std::vector<RenderImage> load_images_from_dir(char*);
};

#endif // RENDERIMAGE_H
