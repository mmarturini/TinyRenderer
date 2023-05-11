#include <gtest/gtest.h>

// TEST(TestSuiteName, TestName) {}
// both names must be valid c++ identifiers and should not contain _
TEST(HelloTest, BasicAssertion) { 
	// Expect two strings not to be equal
	EXPECT_STRNE("hello", "world");
	// Expect equality
	EXPECT_EQ(7 * 6, 42);
}