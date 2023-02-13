#include <gtest/gtest.h>

void TestGroup00();
void TestGroup10();
void TestGroup20();
void TestGroup30();
void TestGroup40();
void TestGroup50();
void TestGroup60();
void TestGroup70();

//TEST(TestCaseName0, TestName0) {
//  EXPECT_EQ(1, 1);
//  EXPECT_TRUE(true);
//}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

//    TestGroup00();
//    TestGroup10();
//    TestGroup20();
//    TestGroup30();
//    TestGroup40();
//    TestGroup50();
//    TestGroup60();
//	  TestGroup70();

    int rv = RUN_ALL_TESTS();

    return rv;
}
