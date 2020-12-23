//
// Created by rober on 1/12/20.
//

#include "bmp.h"
#include <dirent.h>
#include <cstring>

void BMP::read(const char *fname) {
    //first of all we need to setup the file reader
    std::ifstream cursor{fname, std::ios_base::binary};
    if (cursor) { //If file was reader is working correctly
        cursor.read((char *) &file_header,
                    sizeof(file_head)); //We read as many bytes as the file header has ans store it in our structure
        if (file_header.file_type != 0x4D42) { //Constrint 1: Type is correct
            throw std::runtime_error("Error! Unrecognized file format."); //ERROR
        }
        cursor.read((char *) &bmp_info_header,
                    sizeof(bmp_info_header)); //Looking good, lets continue reading the file, bmp header now and store

        if (bmp_info_header.img_point == 24) { //Constraint 2 = 24 point size
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
            throw std::runtime_error(
                    "The program can treat only BMP images with the origin in the bottom left corner!");
        }
        //Everyone does this on the web, dunno why is 8 yet
        data.resize(bmp_info_header.img_width * bmp_info_header.img_height * bmp_info_header.img_point / 8);
        //we read the data etc and fill the size
        cursor.read((char *) data.data(), data.size());
        file_header.file_size += static_cast<uint32_t>(data.size());
    } else {
        throw std::runtime_error("Unable to open the input image file.");
    }
}


void BMP::write(const char *fname) {  // Lest go write the image go brrrr
    std::ofstream of{fname, std::ios_base::binary};  //we create the output file

    if (of) {  //creation was ok then
        if (bmp_info_header.img_point == 24) {  // BACK TO CONSTRAINT 2
            if (bmp_info_header.img_width % 4 == 0) {  // CAN BE SKKPED NECESSARY???
                of.write((const char *) &file_header, sizeof(file_head));   //Write the file head
                of.write((const char *) &bmp_info_header, sizeof(bmp_info_header));    //Write the bmp header
                of.write((const char *) data.data(), data.size());    //    Write the data
            }

        } else {
            throw std::runtime_error("The program can treat only 24 point files bro");
        }
    } else {
        throw std::runtime_error("Unable to open the output image file.");
    }
}


int BMP::get_pixel() {
    return 0;
}


int BMP::set_pixel() {
    return 0;
}

void BMP::gaussBlur() {

    getRGB();

    int m[5][5]{{1, 4,  7,  4,  1},
                {4, 16, 26, 16, 4},
                {7, 26, 41, 26, 7},
                {4, 16, 26, 16, 4},
                {1, 4,  7,  4,  1}};
    int w{273};

    auto blue1 = blue;
    auto green1 = green;
    auto red1 = red;

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            int new_blue{0};
            int new_green{0};
            int new_red{0};

            for (int s = -2; s <= 2; s++) {
                for (int t = -2; t <= 2; t++) {
                    if ((i + s) >= 0 and (j + t) >= 0 and (i + s) < getHeight() and (j + t) < getWidth()) {
                        new_blue += m[s + 2][t + 2] * blue[i + s][j + t];
                        new_green += m[s + 2][t + 2] * green[i + s][j + t];
                        new_red += m[s + 2][t + 2] * red[i + s][j + t];
                    }
                }
            }

            blue1[i][j] = new_blue / w;
            green1[i][j] = new_green / w;
            red1[i][j] = new_red / w;
        }
    }

    blue = blue1;
    green = green1;
    red = red1;

    setRGB();
}

void BMP::getRGB() {
    blue.resize(getHeight());
    red.resize(getHeight());
    green.resize(getHeight());

    for (int i = 0; i < getHeight(); i++) {
        blue[i].resize(getWidth());
        red[i].resize(getWidth());
        green[i].resize(getWidth());

        for (int j = 0; j < getWidth(); j++) {
            blue[i][j] = (int) data[(3 * (i * getWidth() + j))];
            green[i][j] = (int) data[(3 * (i * getWidth() + j) + 1)];
            red[i][j] = (int) data[(3 * (i * getWidth() + j) + 2)];
        }
    }
}

void BMP::setRGB() {
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            data[(3 * (i * getWidth() + j))] = blue[i][j];
            data[(3 * (i * getWidth() + j) + 1)] = green[i][j];
            data[(3 * (i * getWidth() + j) + 2)] = red[i][j];
        }
    }
}

void BMP::sobelOperator() {

    getRGB();

    int mx[3][3]{{1,  2,  1},
                 {0,  0,  0},
                 {-1, -2, -1}};


    int my[3][3]{{-1, 0, 1},
                 {-2, 0, 2},
                 {-1, 0, 1}};

    int w{8};

    auto blue1 = blue;
    auto green1 = green;
    auto red1 = red;

    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            int blue_x{0};
            int green_x{0};
            int red_x{0};
            int blue_y{0};
            int green_y{0};
            int red_y{0};

            for (int s = -1; s <= 1; s++) {
                for (int t = -1; t <= 1; t++) {
                    if ((i + s) >= 0 and (j + t) >= 0 and (i + s) < getHeight() and (j + t) < getWidth()) {
                        blue_x += mx[s + 1][t + 1] * blue[i + s][j + t];
                        green_x += mx[s + 1][t + 1] * green[i + s][j + t];
                        red_x += mx[s + 1][t + 1] * red[i + s][j + t];
                        blue_y += my[s + 1][t + 1] * blue[i + s][j + t];
                        green_y += my[s + 1][t + 1] * green[i + s][j + t];
                        red_y += my[s + 1][t + 1] * red[i + s][j + t];
                    }
                }
            }

            blue1[i][j] = (abs(blue_x) + abs(blue_y)) / w;
            green1[i][j] = (abs(green_x) + abs(green_y)) / w;
            red1[i][j] = (abs(red_x) + abs(red_y)) / w;
        }
    }

    blue = blue1;
    green = green1;
    red = red1;

    setRGB();
}