#include "gtest/gtest.h"

#include "cisab/logs/logger.h"


using namespace cisab::logs;
using namespace cisab::fmts;


const std::string log_level_ret = "103272";

struct TestLogger : public iLogger
{
	static std::string latest_warning_;
	static std::string latest_error_;
	static std::string latest_fatal_;
	static std::string latest_log_msg_;
	static std::string set_log_level_;

	std::string get_log_level (void) const override
	{
		return log_level_ret;
	}

	void set_log_level (const std::string& log_level) override
	{
		set_log_level_ = log_level;
	}

	bool supports_level (size_t msg_level) const override
	{
		return true;
	}

	bool supports_level (const std::string& msg_level) const override
	{
		return true;
	}

	void log (size_t msg_level, const std::string& msg,
		const SrcLocT& location = SrcLocT::current()) override
	{
		switch (msg_level)
		{
			case WARN:
				warn(msg);
				break;
			case ERROR:
				error(msg);
				break;
			case FATAL:
				fatal(msg);
				break;
			default:
				std::stringstream ss;
				ss << msg_level << msg;
				latest_log_msg_ = ss.str();
		}
	}

	void log (const std::string& msg_level, const std::string& msg,
		const SrcLocT& location = SrcLocT::current()) override
	{
		log(enum_log(msg_level), msg);
	}

	void warn (const std::string& msg) const
	{
		latest_warning_ = msg;
	}

	void error (const std::string& msg) const
	{
		latest_error_ = msg;
	}

	void fatal (const std::string& msg) const
	{
		latest_fatal_ = msg;
	}
};


std::string TestLogger::latest_warning_;

std::string TestLogger::latest_error_;

std::string TestLogger::latest_fatal_;

std::string TestLogger::latest_log_msg_;

std::string TestLogger::set_log_level_ = "0";

std::shared_ptr<TestLogger> tlogger = std::make_shared<TestLogger>();


int main (int argc, char** argv)
{
	set_logger(std::static_pointer_cast<iLogger>(tlogger));
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


#ifndef DISABLE_LOGS_TEST


struct LOGS : public ::testing::Test
{
protected:
	void TearDown (void) override
	{
		TestLogger::latest_warning_ = "";
		TestLogger::latest_error_ = "";
		TestLogger::latest_fatal_ = "";
		TestLogger::set_log_level_ = "0";
	}
};


TEST_F(LOGS, Default)
{
	DefLogger log;
	EXPECT_STREQ("info", log.get_log_level().c_str());
	log.set_log_level("debug");
	EXPECT_STREQ("debug", log.get_log_level().c_str());
	log.log(INFO, "log info message");
	log.log(WARN, "log warn message");
	log.log(ERROR, "log error message");
	try
	{
		log.log(FATAL, "log fatal message");
		FAIL() << "log.fatal failed to throw error";
	}
	catch (std::runtime_error& e)
	{
		const char* msg = e.what();
		EXPECT_STREQ("log fatal message", msg);
	}
	catch (...)
	{
		FAIL() << "expected to throw runtime_error";
	}
}


TEST_F(LOGS, Trace)
{
	trace("tracing message");
	const char* cmsg = TestLogger::latest_log_msg_.c_str();
	auto expect = sprintf("%dtracing message", TRACE);
	EXPECT_STREQ(expect.c_str(), cmsg);
}


TEST_F(LOGS, TraceFmt)
{
	tracef("tracing %.2f message %d with format %s", 4.15, 33, "applepie");
	const char* cmsg = TestLogger::latest_log_msg_.c_str();
	auto expect = sprintf("%dtracing 4.15 message 33 with format applepie", TRACE);
	EXPECT_STREQ(expect.c_str(), cmsg);
}


TEST_F(LOGS, Debug)
{
	debug("debugging message");
	const char* cmsg = TestLogger::latest_log_msg_.c_str();
	auto expect = sprintf("%ddebugging message", CISAB_DEBUG);
	EXPECT_STREQ(expect.c_str(), cmsg);
}


TEST_F(LOGS, DebugFmt)
{
	debugf("debugging %.3f message %d with format %s", 0.31, 7, "orange");
	const char* cmsg = TestLogger::latest_log_msg_.c_str();
	auto expect = sprintf("%ddebugging 0.310 message 7 with format orange", CISAB_DEBUG);
	EXPECT_STREQ(expect.c_str(), cmsg);
}


TEST_F(LOGS, Info)
{
	info("infoing message");
	const char* cmsg = TestLogger::latest_log_msg_.c_str();
	auto expect = sprintf("%dinfoing message", INFO);
	EXPECT_STREQ(expect.c_str(), cmsg);
}


TEST_F(LOGS, InfoFmt)
{
	infof("infoing %.4f message %d with format %s", 3.1415967, -1, "plum");
	const char* cmsg = TestLogger::latest_log_msg_.c_str();
	auto expect = sprintf("%dinfoing 3.1416 message -1 with format plum", INFO);
	EXPECT_STREQ(expect.c_str(), cmsg);
}


TEST_F(LOGS, Warn)
{
	warn("warning message");
	const char* cmsg = TestLogger::latest_warning_.c_str();
	EXPECT_STREQ("warning message", cmsg);
}


TEST_F(LOGS, WarnFmt)
{
	warnf("warning %.2f message %d with format %s", 4.15, 33, "applepie");
	const char* cmsg = TestLogger::latest_warning_.c_str();
	EXPECT_STREQ("warning 4.15 message 33 with format applepie", cmsg);
}


TEST_F(LOGS, Error)
{
	error("erroring message");
	const char* emsg = TestLogger::latest_error_.c_str();
	EXPECT_STREQ("erroring message", emsg);
}


TEST_F(LOGS, ErrorFmt)
{
	errorf("erroring %.3f message %d with format %s", 0.31, 7, "orange");
	const char* emsg = TestLogger::latest_error_.c_str();
	EXPECT_STREQ("erroring 0.310 message 7 with format orange", emsg);
}


TEST_F(LOGS, Fatal)
{
	fatal("fatal message");
	const char* fmsg = TestLogger::latest_fatal_.c_str();
	EXPECT_STREQ("fatal message", fmsg);
}


TEST_F(LOGS, FatalFmt)
{
	fatalf("fatal %.4f message %d with format %s", 3.1415967, -1, "plum");
	const char* fmsg = TestLogger::latest_fatal_.c_str();
	EXPECT_STREQ("fatal 3.1416 message -1 with format plum", fmsg);
}


TEST_F(LOGS, GlobalGetSet)
{
	EXPECT_STREQ(log_level_ret.c_str(), get_log_level().c_str());

	set_log_level("1231");
	EXPECT_STREQ("1231", TestLogger::set_log_level_.c_str());
}


TEST(DEFAULT, Logger)
{
	DefLogger logger;
	EXPECT_TRUE(logger.supports_level(INFO));
	EXPECT_TRUE(logger.supports_level("info"));

	logger.log(INFO, "hello");
	logger.log("warn", "world");
	logger.log("error", "oh no");

	try
	{
		logger.log(FATAL, "death");
		FAIL() << "not expecting fatal to succeed";
	}
	catch (std::exception& e)
	{
		EXPECT_STREQ("death", e.what());
	}
}


TEST(DEFAULT, Conversions)
{
	EXPECT_EQ(INFO, enum_log("info"));
	EXPECT_EQ(WARN, enum_log("warn"));
	EXPECT_EQ(ERROR, enum_log("error"));
	EXPECT_EQ(FATAL, enum_log("fatal"));
	EXPECT_EQ(NOT_SET, enum_log("hello"));

	EXPECT_STREQ("info", name_log(INFO).c_str());
	EXPECT_STREQ("warn", name_log(WARN).c_str());
	EXPECT_STREQ("error", name_log(ERROR).c_str());
	EXPECT_STREQ("fatal", name_log(FATAL).c_str());
	EXPECT_STREQ("", name_log(NOT_SET).c_str());
}


#endif // DISABLE_LOGS_TEST
