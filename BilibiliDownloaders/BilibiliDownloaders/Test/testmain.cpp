#pragma execution_character_set("utf-8")

#include "MainWindow/MainWindow.h"
#include <QtWidgets/QApplication>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include <ObjBase.h>

#include "Test/SimpleNetwork.h"
#include "loger/Loger.h"
#include "Protocol.h"
#include "Aria2Net/AriaServer/AriaServer.h"
#include "Aria2Net/AriaClient/AriaClient.h"
#include "FFmpeg/FFmpegHelper.h"
#include "Aria2Net/Protocol/Protocol.h"

// #define Test

#include <nlohmann/json.hpp>
#include <cstdio>
#include <chrono>

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"  // support for loading levels from the environment variable
#include "spdlog/fmt/ostr.h" // support for user defined types
#include "spdlog/sinks/stdout_color_sinks.h"
// or #include "spdlog/sinks/stdout_sinks.h" if no colors needed.
void stdout_logger_example()
{
    // Create color multi threaded logger.
    auto console = spdlog::stdout_color_mt("console");
    // or for stderr:
    // auto console = spdlog::stderr_color_mt("error-logger");
}

#include "spdlog/sinks/basic_file_sink.h"
void basic_example()
{
    // Create basic file logger (not rotated).
    auto my_logger = spdlog::basic_logger_mt("file_logger", "logs/basic-log.txt", true);
}

#include "spdlog/sinks/rotating_file_sink.h"
void rotating_example()
{
    // Create a file rotating logger with 5mb size max and 3 rotated files.
    auto rotating_logger = spdlog::rotating_logger_mt("some_logger_name", "logs/rotating.txt", 1048576 * 5, 3);
}

#include "spdlog/sinks/daily_file_sink.h"
void daily_example()
{
    // Create a daily logger - a new file is created every day on 2:30am.
    auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
}

#include "spdlog/cfg/env.h"
void load_levels_example()
{
    // Set the log level to "info" and mylogger to "trace":
    // SPDLOG_LEVEL=info,mylogger=trace && ./example
    spdlog::cfg::load_env_levels();
    // or from command line:
    // ./example SPDLOG_LEVEL=info,mylogger=trace
    // #include "spdlog/cfg/argv.h" // for loading levels from argv
    // spdlog::cfg::load_argv_levels(args, argv);
}

#include "spdlog/async.h"
void async_example()
{
    // Default thread pool settings can be modified *before* creating the async logger:
    // spdlog::init_thread_pool(32768, 1); // queue with max 32k items 1 backing thread.
    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");
    // alternatively:
    // auto async_file = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>("async_file_logger", "logs/async_log.txt");

    for (int i = 1; i < 101; ++i)
    {
        async_file->info("Async message #{}", i);
    }
}

// Log binary data as hex.
// Many types of std::container<char> types can be used.
// Iterator ranges are supported too.
// Format flags:
// {:X} - print in uppercase.
// {:s} - don't separate each byte with space.
// {:p} - don't print the position on each line start.
// {:n} - don't split the output to lines.

#include "spdlog/fmt/bin_to_hex.h"
void binary_example()
{
    std::vector<char> buf(80);
    for (int i = 0; i < 80; i++)
    {
        buf.push_back(static_cast<char>(i & 0xff));
    }
    spdlog::info("Binary example: {}", spdlog::to_hex(buf));
    spdlog::info("Another binary example:{:n}", spdlog::to_hex(std::begin(buf), std::begin(buf) + 10));
    // more examples:
    // logger->info("uppercase: {:X}", spdlog::to_hex(buf));
    // logger->info("uppercase, no delimiters: {:Xs}", spdlog::to_hex(buf));
    // logger->info("uppercase, no delimiters, no position info: {:Xsp}", spdlog::to_hex(buf));
    // logger->info("hexdump style: {:a}", spdlog::to_hex(buf));
    // logger->info("hexdump style, 20 chars per line {:a}", spdlog::to_hex(buf, 20));
}

// Log a vector of numbers
#ifndef SPDLOG_USE_STD_FORMAT
#    include "spdlog/fmt/ranges.h"
void vector_example()
{
    std::vector<int> vec = { 1, 2, 3 };
    spdlog::info("Vector example: {}", vec);
}

#else
void vector_example() {}
#endif

// ! DSPDLOG_USE_STD_FORMAT

// Compile time log levels.
// define SPDLOG_ACTIVE_LEVEL to required level (e.g. SPDLOG_LEVEL_TRACE)
void trace_example()
{
    // trace from default logger
    SPDLOG_TRACE("Some trace message.. {} ,{}", 1, 3.23);
    // debug from default logger
    SPDLOG_DEBUG("Some debug message.. {} ,{}", 1, 3.23);

    // trace from logger object
    auto logger = spdlog::get("file_logger");
    SPDLOG_LOGGER_TRACE(logger, "another trace message");
}

// stopwatch example
#include "spdlog/stopwatch.h"
#include <thread>
void stopwatch_example()
{
    spdlog::stopwatch sw;
    std::this_thread::sleep_for(std::chrono::milliseconds(123));
    spdlog::info("Stopwatch: {} seconds", sw);
}

//#include "spdlog/sinks/udp_sink.h"
//void udp_example()
//{
//    spdlog::sinks::udp_sink_config cfg("127.0.0.1", 11091);
//    auto my_logger = spdlog::udp_logger_mt("udplog", cfg);
//    my_logger->set_level(spdlog::level::debug);
//    my_logger->info("hello world");
//}

// A logger with multiple sinks (stdout and file) - each with a different format and log level.
void multi_sink_example()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::warn);
    console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/multisink.txt", true);
    file_sink->set_level(spdlog::level::trace);

    spdlog::logger logger("multi_sink", { console_sink, file_sink });
    logger.set_level(spdlog::level::debug);
    logger.warn("this should appear in both console and file");
    logger.info("this message should not appear in the console, only in the file");
}

// User defined types logging
struct my_type
{
    int i = 0;
    explicit my_type(int i)
        : i(i) {};
};

namespace fmt_lib = spdlog::fmt_lib;
template<>
struct fmt_lib::formatter<my_type> : fmt_lib::formatter<std::string>
{
    auto format(my_type my, format_context& ctx) -> decltype(ctx.out())
    {
        return fmt_lib::format_to(ctx.out(), "[my_type i={}]", my.i);
    }
};

void user_defined_example()
{
    spdlog::info("user defined type: {}", my_type(14));
}

// Custom error handler. Will be triggered on log failure.
void err_handler_example()
{
    // can be set globally or per logger(logger->set_error_handler(..))
    spdlog::set_error_handler([](const std::string& msg) { printf("*** Custom log error handler: %s ***\n", msg.c_str()); });
}



// Log patterns can contain custom flags.
// this will add custom flag '%*' which will be bound to a <my_formatter_flag> instance
#include "spdlog/pattern_formatter.h"
class my_formatter_flag : public spdlog::custom_flag_formatter
{
public:
    void format(const spdlog::details::log_msg&, const std::tm&, spdlog::memory_buf_t& dest) override
    {
        std::string some_txt = "custom-flag";
        dest.append(some_txt.data(), some_txt.data() + some_txt.size());
    }

    std::unique_ptr<custom_flag_formatter> clone() const override
    {
        return spdlog::details::make_unique<my_formatter_flag>();
    }
};

void custom_flags_example()
{

    using spdlog::details::make_unique; // for pre c++14
    auto formatter = make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<my_formatter_flag>('*').set_pattern("[%n] [%*] [%^%l%$] %v");
    // set the new formatter using spdlog::set_formatter(formatter) or logger->set_formatter(formatter)
    // spdlog::set_formatter(std::move(formatter));
}

void file_events_example()
{
    // pass the spdlog::file_event_handlers to file sinks for open/close log file notifications
    spdlog::file_event_handlers handlers;
    handlers.before_open = [](spdlog::filename_t filename) { spdlog::info("Before opening {}", filename); };
    handlers.after_open = [](spdlog::filename_t filename, std::FILE* fstream) {
        spdlog::info("After opening {}", filename);
        fputs("After opening\n", fstream);
    };
    handlers.before_close = [](spdlog::filename_t filename, std::FILE* fstream) {
        spdlog::info("Before closing {}", filename);
        fputs("Before closing\n", fstream);
    };
    handlers.after_close = [](spdlog::filename_t filename) { spdlog::info("After closing {}", filename); };
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/events-sample.txt", true, handlers);
    spdlog::logger my_logger("some_logger", file_sink);
    my_logger.info("Some log line");
}

void replace_default_logger_example()
{
    // store the old logger so we don't break other examples.
    auto old_logger = spdlog::default_logger();

    auto new_logger = spdlog::basic_logger_mt("new_default_logger", "logs/new-default-log.txt", true);
    spdlog::set_default_logger(new_logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::debug("This message should not be displayed!");
    spdlog::set_level(spdlog::level::trace);
    spdlog::debug("This message should be displayed..");

    spdlog::set_default_logger(old_logger);
}

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavformat/avformat.h> 
#include <libavcodec/avcodec.h> 
#include <libswscale/swscale.h> 

#ifdef __cplusplus
}
#endif

int test_ffmpeg()
{
    void* ofmt_opaque = NULL;
    const AVOutputFormat* ofmt = NULL;
    while ((ofmt = av_muxer_iterate(&ofmt_opaque)))
    {
        printf("short name:%-30s  long name:%-50s  \n", ofmt->name, ofmt->long_name);
    }

    return 0;
}



std::string GenerateGuid()
{
    GUID guid;
    CoCreateGuid(&guid);
    char cBuffer[64] = { 0 };
    sprintf_s(cBuffer, sizeof(cBuffer),
        "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
        guid.Data1, guid.Data2,
        guid.Data3, guid.Data4[0],
        guid.Data4[1], guid.Data4[2],
        guid.Data4[3], guid.Data4[4],
        guid.Data4[5], guid.Data4[6],
        guid.Data4[7]);

    return std::string(cBuffer);
}

#ifdef Test
int main(int argc, char *argv[])
{


    // Log levels can be loaded from argv/env using "SPDLOG_LEVEL"
    load_levels_example();

    spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:>8} aligned, {:<8} aligned", "right", "left");

    // Runtime log levels
    spdlog::set_level(spdlog::level::info); // Set global log level to info
    spdlog::debug("This message should not be displayed!");
    spdlog::set_level(spdlog::level::trace); // Set specific logger's log level
    spdlog::debug("This message should be displayed..");

    // Customize msg format for all loggers
    spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
    spdlog::info("This an info message with custom format");
    spdlog::set_pattern("%+"); // back to default format
    spdlog::set_level(spdlog::level::info);

    // Backtrace support
    // Loggers can store in a ring buffer all messages (including debug/trace) for later inspection.
    // When needed, call dump_backtrace() to see what happened:
    spdlog::enable_backtrace(10); // create ring buffer with capacity of 10  messages
    for (int i = 0; i < 100; i++)
    {
        spdlog::debug("Backtrace message {}", i); // not logged..
    }
    // e.g. if some error happened:
    spdlog::dump_backtrace(); // log them now!

    try
    {
        stdout_logger_example();
        basic_example();
        rotating_example();
        daily_example();
        async_example();
        binary_example();
        vector_example();
        multi_sink_example();
        user_defined_example();
        err_handler_example();
        trace_example();
        stopwatch_example();
        //udp_example();
        custom_flags_example();
        file_events_example();
        replace_default_logger_example();

        // Flush all *registered* loggers using a worker thread every 3 seconds.
        // note: registered loggers *must* be thread safe for this to work correctly!
        spdlog::flush_every(std::chrono::seconds(3));

        // Apply some function on all registered loggers
        spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info("End of example."); });

        // Release all spdlog resources, and drop all loggers in the registry.
        // This is optional (only mandatory if using windows + async log).
        spdlog::shutdown();
    }

    // Exceptions will only be thrown upon failed logger or sink construction (not during logging).
    catch (const spdlog::spdlog_ex& ex)
    {
        std::printf("Log initialization failed: %s\n", ex.what());
        return 1;
    }

    nlohmann::json j;

    test_ffmpeg();

   
    QApplication a(argc, argv);
    aria2net::AriaServer AriaServer;
    SimpleNetwork simpleNetwork;
    aria2net::AriaClient ariaClient;
    

    nlohmann::json json;
    json["fav_id"] = "99184721";
    json["biz_type"] = "2";
    //json["cookie"] = "buvid3=B3B4EAE9-9253-F3D9-8C60-C246E56B920589724infoc; _uuid=734A72C8-16E1-D6B10-6AC8-10EF8910F15C3989392infoc; blackside_state=1; rpdid=0zbfAHJovL|5BgcxYFU|3eP|3w1MSWvJ; video_page_version=v_old_home; LIVE_BUVID=AUTO9916384917126731; sid=bk3ft85m; fingerprint=e1aee79dc22cd0b3f0668009d49d02d4; buvid_fp_plain=B3B4EAE9-9253-F3D9-8C60-C246E56B920589724infoc; DedeUserID=287031869; DedeUserID__ckMd5=e26fb0593e97d8fc; SESSDATA=a44128a9%2C1654764587%2C94d03*c1; bili_jct=271f101a4d37853f339b84c6202904e2; i-wanna-go-back=-1; b_ut=5; buvid4=9C93EF9A-7F4E-035A-203A-90FE5082E3C519443-022020421-9TO+hWhxQHMnx8ShDLZrEbchd/A4l8nP2exKxyNZ3er/tlneEFjSDg%3D%3D; buvid_fp=0e2e0d8ea52d21fd63fd81fc0ee84b05; CURRENT_BLACKGAP=0; bp_video_offset_287031869=641915508515406000; PVID=1; nostalgia_conf=-1; CURRENT_FNVAL=4048; CURRENT_QUALITY=16; innersign=0; b_lsid=DC94FA64_17FD0FBC70E";

    // ariaClient.Request(QString("http://api.vc.bilibili.com/user_plus/v1/Fav/add"), json.dump());

    aria2net::AriaSendOption ariaSendOption;
    ariaSendOption.SetDir(QApplication::applicationFilePath().toStdString());
    ariaSendOption.SetOut("qq.exe");

    aria2net::AriaSendData sendDate;
    

    std::list<std::string> params;
    params.emplace_back("token:downkyi");
    params.emplace_back("[\"http://dl_dir.qq.com/qqfile/qq/QQ2011/QQ2011.exe\"]");
    params.emplace_back(ariaSendOption.toString());
    std::string id = GenerateGuid();
    sendDate.SetId(GenerateGuid());
    sendDate.SetMethod("aria2.addUri");
    sendDate.SetJsonrpc("2.0");
    sendDate.SetParams(params);

    ariaClient.Request(sendDate.toString());

    const std::string audeo = "test.mp3";
    const std::string video = "test.mp4";
    const std::string destionVideo = "result.mp4";
    FFmpegHelper::MegerVideo(audeo, video, destionVideo);
    
    MainWindow w;
    w.show();
    return a.exec();
}

#endif // Test


