#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>

using namespace boost::asio;

int main()
{
    auto ctx = io_context{};
    auto ep = ip::tcp::endpoint(ip::tcp::v4(), 80);
    auto acceptor = ip::tcp::acceptor(ctx, ep);
    fmt::println("Server running");

    try 
    {
        auto sock = ip::tcp::socket(ctx);
        acceptor.accept(sock);
        fmt::println("Connection established");
        
        auto buf = streambuf{};
        auto should_stop = false;
        while(!should_stop) 
        {
            auto bytes_count = read_until(sock, buf, '\n');
            auto str = std::string(std::istreambuf_iterator<char>(&buf), {});
            fmt::println("Recieved {} bytes: {}", bytes_count, str);

            boost::to_lower(str);
            boost::trim(str);
            should_stop = (str == "stop");
            
            str.append("::exo\n");
            fmt::println("Sending answer: {}", str);
            write(sock, buffer(str));
        }
        sock.close();
        fmt::println("Connection closed");
    }
    catch(std::exception& ex)
    {
        fmt::println("{}", ex.what());
    }

    fmt::println("Server stopped");

    return 0;
}
