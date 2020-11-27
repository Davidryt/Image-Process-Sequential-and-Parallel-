//
// Created by davidryt on 11/23/20.
//

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
    //LETS READ THE WHOLE FILE
    void read(const char *fname) {
        //first of all we need to setup the file reader
        std::ifstream cursor{ fname, std::ios_base::binary };
        if (cursor) { //If file was reader is working correctly
            cursor.read((char*)&file_header, sizeof(file_head)); //We read as many bytes as the file header has ans store it in our structure
            if(file_header.file_type != 0x4D42) { //Constrint 1: Type is correct
                throw std::runtime_error("Error! Unrecognized file format."); //ERROR
            }
            cursor.read((char*)&bmp_info_header, sizeof(bmp_info_header)); //Looking good, lets continue reading the file, bmp header now and store

            if(bmp_info_header.img_point == 24) { //Constraint 2 = 24 point size
                std::cout << "File has a  " << bmp_info_header.img_point << " points, GOOD\n"; //DEBUG, TO DELETE
            } else {
                std::cerr << "Error! The file has  " << bmp_info_header.img_point << " points instead of 24\n";
                throw std::runtime_error("Error! error file format .");  //ERROR
            }

            // There is smth else after this that is not data (i think reserved),so we search for the end of file header (offset)

            //Object of type ios_base::seekdir. It may take any of the following constant values:
            //value	offset is relative to...
            //ios_base::beg	beginning of the stream
            //ios_base::cur	current position in the stream
            //ios_base::end	end of the stream

            cursor.seekg(file_header.file_offset, cursor.beg);


            //Checking file sizes, necessary? idk

                bmp_info_header.size = sizeof(img_header);
                file_header.file_offset = sizeof(file_head) + sizeof(img_header);
            //END OF idk


            if (bmp_info_header.img_height < 0) { //negative values, not today
                throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");
            }
            //Everyone does this on the web, dunno why is 8 yet
            data.resize(bmp_info_header.img_width * bmp_info_header.img_height * bmp_info_header.img_point / 8);
            //we read the data etc and fill the size
            cursor.read((char*)data.data(), data.size());
            file_header.file_size += static_cast<uint32_t>(data.size());


        }
        else {
            throw std::runtime_error("Unable to open the input image file.");
        }
    }


    void write(const char *fname) {  // Lest go write the image go brrrr
        std::ofstream of{ fname, std::ios_base::binary };  //we create the output file
        if (of) {  //creation was ok then
            if (bmp_info_header.img_point == 24) {  // BACK TO CONSTRAINT 2
                if (bmp_info_header.img_width % 4 == 0) {  // CAN BE SKKPED NECESSARY???
                    of.write((const char*)&file_header, sizeof(file_head));   //Write the file head
                    of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));    //Write the bmp header
                    of.write((const char*)data.data(), data.size());    //    Write the data
                }
            }
            else {
                throw std::runtime_error("The program can treat only 24 point files bro");
            }
        }
        else {
            throw std::runtime_error("Unable to open the output image file.");
        }
    }



private:
    uint32_t row_stride{ 0 };


};