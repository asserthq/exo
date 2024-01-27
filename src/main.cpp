#include <iostream>
#include <string>
#include <algorithm>
#include <exception>
#include <random>

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>   
#include <fmt/format.h>

using namespace boost::asio;

int main()
{
    auto ctx = io_context{};
    const auto ep = ip::tcp::endpoint(ip::tcp::v4(), 80);
    auto acceptor = ip::tcp::acceptor(ctx, ep);
    fmt::println("Server running...");

    auto buf = streambuf{};

    try 
    {
        while(true) 
        {
            auto sock = ip::tcp::socket(ctx);
            
            fmt::println("Waiting for connection...");
            acceptor.accept(sock);
            fmt::println("TCP connection established");
            
            const auto bytes_count = read_until(sock, buf, '\n');
            auto str = std::string(std::istreambuf_iterator<char>(&buf), {});
            fmt::println("Recieved {} bytes: {}", bytes_count, str);

            boost::trim(str);
            auto words = std::vector<std::string>{};
            boost::split(words, str, boost::is_space());
         
            auto action = 0;
            while(action == 0)
            {
                try 
                {
                    fmt::println("Askin' for action...");
                    write(sock, buffer("Sort(1) or shuffle(2)? "));
                    read_until(sock, buf, '\n');
                    auto actionStr = std::string(std::istreambuf_iterator<char>(&buf), {});
                    boost::trim(actionStr);
                    action = std::stoi(actionStr);
                    if(action != 1 && action != 2) 
                    {
                        action = 0;
                        throw std::invalid_argument(fmt::format("Invalid action variant: {}", actionStr));
                    }
                
                }
                catch(std::logic_error& iaEx) 
                {
                    fmt::println("{}", iaEx.what());
                    write(sock, buffer(fmt::format("{}\n", iaEx.what())));
                }
            }
            
            if(action == 1) 
            {
                fmt::println("Action 1: sorting...");
                std::ranges::sort(words, std::ranges::lexicographical_compare);
            }
            else if(action == 2)
            {
                fmt::println("Action 2: shuffling...");
                auto rnd = std::random_device{};
                auto randEngine = std::default_random_engine(rnd());
                std::ranges::shuffle(words, randEngine);
            }
            
            str = boost::join(words, " "); 
            
            fmt::println("Sending answer: {}", str);
            write(sock, buffer(str));

            fmt::println("TCP connection closed");
        }
    }
    catch(std::exception& ex)
    {
        fmt::println("{}\n", ex.what());
    }

    fmt::println("Server stopped");
    return 0;
}
