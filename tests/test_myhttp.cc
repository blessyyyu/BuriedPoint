#include <cstdlib>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "src/third_party/boost/asio/connect.hpp"
#include "src/third_party/boost/asio/ip/tcp.hpp"
#include "src/third_party/boost/beast/core.hpp"
#include "src/third_party/boost/beast/http.hpp"
#include "src/third_party/boost/beast/version.hpp"
#include "src/third_party/boost/asio.hpp"

TEST(beast_learn, http_get)
{
    try
    {
        // 向httporg.bin发送get请求
        // 声明boost::asio I/O 对象
        boost::asio::io_context io_context;

        // 声明一个TCP套接字连接对象
        boost::asio::ip::tcp::resolver resolver(io_context);
        boost::beast::tcp_stream stream(io_context);

        // 解析服务器地址和端口
        auto const results = resolver.resolve("httpbin.org", "80");

        // 连接到服务器
        stream.connect(results);

        // 设置HTTP GET请求
        boost::beast::http::request<boost::beast::http::string_body> req{
            boost::beast::http::verb::get, "/get", 11};
        req.set(boost::beast::http::field::host, "httpbin.org");
        req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // 发送HTTP请求
        boost::beast::http::write(stream, req);

        // 接收HTTP响应
        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> res;
        boost::beast::http::read(stream, buffer, res);

        // 输出响应结果
        std::cout << res << std::endl;

        // 关闭连接
        boost::beast::error_code ec;
        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        std::cout << "getchar() to check the statistic." << std::endl;
        getchar();
    }
    catch (std::exception &e)
    {
        std::cerr << "Error " << e.what() << std::endl;
        getchar();
    }
}