#include <gtest/gtest.h>
#include <cstdlib>

int main(int argc, char** argv)
{
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}