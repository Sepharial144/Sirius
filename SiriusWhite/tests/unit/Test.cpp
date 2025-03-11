#include <gtest/gtest.h>

#include <string>

class SuperTest : public ::testing::Test
{
public:
	void SetUp()
	{}

	void TearDown()
	{}

};

TEST_F(SuperTest, supertest)
{
	const std::string testString{ "Super test" };
	EXPECT_STREQ(testString.c_str(), testString.c_str());
	EXPECT_EQ(1, 1);
}