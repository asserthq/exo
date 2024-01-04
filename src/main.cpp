#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>

#include <boost/asio.hpp>
#include <fmt/format.h>

using namespace boost::asio;

int main()
{
    auto ctx = io_context{};

    while(true)
    {
        try 
        {
            auto ep = ip::tcp::endpoint(ip::tcp::v4(), 88);
            auto acceptor = ip::tcp::acceptor(ctx, ep);
            auto sock = ip::tcp::socket(ctx);
            acceptor.accept(sock);
            
            auto buf = streambuf{};
            auto bytes_count = read_until(sock, buf, '\n');

            auto str = std::string(std::istreambuf_iterator<char>(&buf), {});
            fmt::println("Reflected {} bytes: {}", bytes_count - 1, str);
            
            write(sock, buf);
            sock.close();

            //auto str_lower = 
            //    str 
            //    | std::views::take(bytes_count - 1) 
            //    | std::views::transform([](auto&& s) { return std::tolower(s); });

            if(/*std::ranges::equal("stop", str_lower) ||*/ "stop\n" == str)
            {
                break;
            }
        }
        catch(std::exception& ex)
        {
            fmt::println("{}", ex.what());
        }
    }

    return 0;
}
