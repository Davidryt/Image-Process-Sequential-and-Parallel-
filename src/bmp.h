//
// Created by davidryt on 11/23/20.
//

// Header guard start
#ifndef BMP_H
#define BMP_H

#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

#pragma pack(push, 1)   //I HAVE NO IDEA, BUT DELETED AND NOTHING WILL WORK
struct file_head {
    uint16_t file_type{ 0x4D42 };          // File type always BM which is 0x4D42 (stored as hex uint16_t in little endian)
    uint32_t file_size{ 0 };               // Size of the file (in bytes)
    uint16_t file_res1{ 0 };               // Reserved, always 0
    uint16_t file_res2{ 0 };               // Reserved, always 0
    uint32_t file_offset{ 0 };             // Start position of pixel data (bytes from the beginning of the file)
};

struct img_header {
    uint32_t size{ 0 };                      // Size of this header (in bytes)
    int32_t img_width{ 0 };                      // width of bitmap in pixels
    int32_t img_height{ 0 };                     // width of bitmap in pixels
    //       (if positive, bottom-up, with origin in lower left corner)
    //       (if negative, top-down, with origin in upper left corner)
    uint16_t img_planes{ 1 };                    // No. of planes for the target device, this is always 1
    uint16_t img_point{ 0 };                 // No. of bits per pixel
    uint32_t img_comp{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t size_image{ 0 };                // 0 - for uncompressed images
    int32_t img_horiz{ 0 };
    int32_t img_vert{ 0 };
    uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};

#pragma pack(pop)  //CAN BE DELETED BUT MEMORY FK

struct BMP {
    file_head file_header;
    img_header bmp_info_header;
    std::vector<uint8_t> data;

    BMP(const char *fname) {
        read(fname);
    }

    void read(const char *fname);
    void write(const char *fname);

    int get_pixel();
    int set_pixel();

private:
    uint32_t row_stride{ 0 };
};

// Head guard end
#endif