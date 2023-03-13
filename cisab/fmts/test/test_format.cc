
#ifndef DISABLE_FMTS_TEST

#include <array>
#include <list>
#include <vector>
#include <unordered_map>

#include "gtest/gtest.h"

#include "cisab/fmts/format.h"


using namespace ::cisab::fmts;


TEST(FORMAT, ExactString)
{
	EXPECT_STREQ("abcd\\\\efgh\\\\ijkl\\\\", ExactString("abcd\\efgh\\ijkl\\").to_string().c_str());
	EXPECT_STREQ("\\\\abcd\\\\efgh\\\\ijkl", ExactString("\\abcd\\efgh\\ijkl").to_string().c_str());
}


TEST(FORMAT, ExactStringStreamed)
{
	std::stringstream ss;
	ss << ExactString("abcd\\efgh\\ijkl\\");
	EXPECT_STREQ("abcd\\\\efgh\\\\ijkl\\\\", ss.str().c_str());
}


TEST(FMTS, Join)
{
	EXPECT_STREQ("hello world !",
		join(" ", std::vector<std::string>{"hello", "world", "!"}).c_str());
}


TEST(FMTS, Sprintf)
{
	const char* str = "string";
	const char* oth = "other";
	std::string s = sprintf("%% %s %d %.3f %% %%%% %s %d %.1f %% %%%%",
		str, 123, 5.689, oth, 77, 0.4, 12);
	EXPECT_STREQ("% string 123 5.689 % %% other 77 0.4 % %%", s.c_str());
}


TEST(FMTS, LeftTrim)
{
	std::string content = "    abce  dfgi hjk lm  ";
	ltrim(content);
	EXPECT_STREQ("abce  dfgi hjk lm  ", content.c_str());
}


TEST(FMTS, RightTrim)
{
	std::string content = "    abce  dfgi hjk lm  ";
	rtrim(content);
	EXPECT_STREQ("    abce  dfgi hjk lm", content.c_str());
}


TEST(FMTS, Trim)
{
	std::string content = "    abce  dfgi hjk lm  ";
	trim(content);
	EXPECT_STREQ("abce  dfgi hjk lm", content.c_str());
}

TEST(FMTS, LeftStrip)
{
	std::string content = "_*_(abce  dfgi hjk lm)*";
	lstrip(content, std::unordered_set<char>{'_', '*', '('});
	EXPECT_STREQ("abce  dfgi hjk lm)*", content.c_str());
}


TEST(FMTS, RightStrip)
{
	std::string content = "_*_(abce  dfgi hjk lm)*";
	rstrip(content, std::unordered_set<char>{'_', '*', ')'});
	EXPECT_STREQ("_*_(abce  dfgi hjk lm", content.c_str());
}


TEST(FMTS, Strip)
{
	std::string content = "_*_(abce  dfgi hjk lm)*";
	strip(content, std::unordered_set<char>{'_', '*', '(', ')'});
	EXPECT_STREQ("abce  dfgi hjk lm", content.c_str());
}


TEST(FMTS, Split)
{
	std::string content = ", ,abce, df,gi, hjk, lm, ";
	auto arrs = split(content, ", ");
	// expect ["", ",abce", "df,gi", "hjk", "lm", ""]
	ASSERT_EQ(6, arrs.size());
	EXPECT_STREQ("", arrs[0].c_str());
	EXPECT_STREQ(",abce", arrs[1].c_str());
	EXPECT_STREQ("df,gi", arrs[2].c_str());
	EXPECT_STREQ("hjk", arrs[3].c_str());
	EXPECT_STREQ("lm", arrs[4].c_str());
	EXPECT_STREQ("", arrs[5].c_str());
}


#endif // DISABLE_FMTS_TEST
