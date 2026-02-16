#include <gtest/gtest.h>

TEST(SampleTest, BasicAssertions){
    EXPECT_EQ(2+2,4);
    EXPECT_STRNE("Hello", "World");
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}