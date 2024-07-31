#ifndef CHPL_STB_IMAGE_WRITE_HELPER_H_
#define CHPL_STB_IMAGE_WRITE_HELPER_H_

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_NO_STDIO
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_WRITE_NO_STDIO
#define STB_IMAGE_WRITE_STATIC

#include "stb_image_write.h"

#endif
