//
// Created by rober on 2/12/20.
//

#include <bmp.h>
#include "gtest/gtest.h"
#include <iostream>

namespace {
    TEST(bmpIo, testRead) {
        std::cout << get_current_dir_name();
        EXPECT_NO_THROW({
            char* temp = strcat(get_current_dir_name(), "/copy.bmp");
            BMP bmp {temp};
            bmp.write(strcat(get_current_dir_name(), "/copy2.bmp"));
        });
    }
}