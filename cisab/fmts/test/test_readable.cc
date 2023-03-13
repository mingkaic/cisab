
#ifndef DISABLE_FMTS_READABLE_TEST

#include <array>
#include <list>
#include <vector>
#include <unordered_map>

#include "gtest/gtest.h"

#include "gmock/gmock.h"

#include "cisab/fmts/format.h"


using ::testing::Return;
using namespace ::cisab::fmts;
using namespace ::cisab::types;


struct MockIterable
{
	MOCK_CONST_METHOD0(begin, const char*(void));

	MOCK_CONST_METHOD0(end, const char*(void));
};


TEST(READABLE, GenericHumanize)
{
	Humanize<size_t> genericunsigned(14);
	Humanize<int> genericint(-15);
	Humanize<std::string> genericstr("message");
	Humanize<double> genericdec(16.001);

	EXPECT_STREQ("14", genericunsigned.to_string().c_str());
	EXPECT_STREQ("-15", genericint.to_string().c_str());
	EXPECT_STREQ("message", genericstr.to_string().c_str());
	EXPECT_STREQ("16.001", genericdec.to_string().c_str());
}


TEST(READABLE, GenericHumanizeStream)
{
	std::stringstream ustream;
	ustream << Humanize<size_t>(14);

	std::stringstream istream;
	istream << Humanize<int>(-15);

	std::stringstream sstream;
	sstream << Humanize<std::string>("message");

	std::stringstream dstream;
	dstream << Humanize<double>(16.001);

	EXPECT_STREQ("14", ustream.str().c_str());
	EXPECT_STREQ("-15", istream.str().c_str());
	EXPECT_STREQ("message", sstream.str().c_str());
	EXPECT_STREQ("16.001", dstream.str().c_str());
}


TEST(READABLE, Iterables)
{
	std::vector<int> emptyv;
	std::vector<int> iv = {14, 15, 16};
	StringsT sv = {
		ExactString("what's\\up\\mybro"),
		ExactString("nothing\\much\\fam"),
		ExactString("\\hella\\lit")
	};
	std::array<uint8_t,4> ua = {9, 0, 3, 36};
	std::list<int8_t> il = {-5, -2, 13, 61};

	Humanize<std::vector<int>> emptyvec(emptyv);
	Humanize<std::vector<int>> ivec(iv);
	Humanize<StringsT> svec(sv);
	Humanize<std::array<uint8_t,4>> uarr(ua);
	Humanize<std::list<int8_t>> ilist(il);

	EXPECT_STREQ("[]", emptyvec.to_string().c_str());
	EXPECT_STREQ("[14\\15\\16]", ivec.to_string().c_str());
	EXPECT_STREQ("[what's\\\\up\\\\mybro\\nothing\\\\much\\\\"
		"fam\\\\\\hella\\\\lit]", svec.to_string().c_str());
	EXPECT_STREQ("[9\\0\\3\\36]", uarr.to_string().c_str());
	EXPECT_STREQ("[-5\\-2\\13\\61]", ilist.to_string().c_str());
}


TEST(READABLE, BoundedArray)
{
	int iv[3] = {14, 15, 16};
	std::string sv[3] = {
		ExactString("what's\\up\\mybro"),
		ExactString("nothing\\much\\fam"),
		ExactString("\\hella\\lit")
	};
	uint8_t ua[4] = {9, 0, 3, 36};
	int8_t il[4] = {-5, -2, 13, 61};

	Humanize<int[3]> ivec(iv);
	Humanize<std::string[3]> svec(sv);
	Humanize<uint8_t[4]> uarr(ua);
	Humanize<int8_t[4]> ilist(il);

	EXPECT_STREQ("[14\\15\\16]", ivec.to_string().c_str());
	EXPECT_STREQ("[what's\\\\up\\\\mybro\\nothing\\\\much\\\\"
		"fam\\\\\\hella\\\\lit]", svec.to_string().c_str());
	EXPECT_STREQ("[9\\0\\3\\36]", uarr.to_string().c_str());
	EXPECT_STREQ("[-5\\-2\\13\\61]", ilist.to_string().c_str());
}


TEST(READABLE, CustomIterables)
{
	MockIterable iterable;

	const char message[13] = "hello world!";
	EXPECT_CALL(iterable, begin()).WillRepeatedly(Return(message));
	EXPECT_CALL(iterable, end()).WillRepeatedly(Return(message + 12));
	Humanize<MockIterable> hit(iterable);

	EXPECT_STREQ("[h\\e\\l\\l\\o\\ \\w\\o\\r\\l\\d\\!]",
		hit.to_string().c_str());
}
TEST(READABLE, SmallInts)
{
	Humanize<uint8_t> usmol(15);
	Humanize<int8_t> smol(-4);

	EXPECT_STREQ("15", usmol.to_string().c_str());
	EXPECT_STREQ("-4", smol.to_string().c_str());
}


TEST(READABLE, Pairs)
{
	Humanize<std::pair<std::string,int>> si({"hell", 69});
	Humanize<std::pair<uint8_t,float>> uf({14, 20.31});

	EXPECT_STREQ("hell:69", si.to_string().c_str());
	EXPECT_STREQ("14:20.31", uf.to_string().c_str());
}


TEST(READABLE, Maps)
{
	std::map<std::string,double> m = {
		{"azz", 554.2},
		{"jas", 332.7},
		{"per", 12.9},
	};
	Humanize<std::map<std::string,double>> hm(m);

	EXPECT_STREQ("[azz:554.2\\jas:332.7\\per:12.9]", hm.to_string().c_str());
}


#endif // DISABLE_FMTS_READABLE_TEST
