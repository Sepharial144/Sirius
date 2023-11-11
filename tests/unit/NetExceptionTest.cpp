#include <gtest/gtest.h>
#include <cstring>

namespace net
{
	TEST(NetApiTest, TranslatePort) {
		EXPECT_EQ(3000, 3000);
		EXPECT_EQ("65536", "65535");
	}
} // namespace net
