#include <iostream>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>

std::string getPublicIP() {
    std::string result = "localhost";
    FILE* fp = popen("curl -s ifconfig.me", "r");
    if (fp) {
        char buffer[128];
        if (fgets(buffer, sizeof(buffer), fp) != nullptr) {
            result = std::string(buffer);
            result.erase(result.find_last_not_of("\n") + 1);
        }
        pclose(fp);
    }
    return result;
}

int main() {
    int port = 8087;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    listen(server_fd, 3);

    std::string publicIP = getPublicIP();

    std::cout << "\n🚀 Application started!\n";
    std::cout << "👉 You can access the application at: http://" 
              << publicIP << ":" << port << "\n\n";

    while (true) {
        socklen_t addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        std::string response =
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
            "<html>"
            "<body style='margin:0; background-color:#0a2540; display:flex; justify-content:center; align-items:center; height:100vh;'>"
            "<h1 style='color:white; font-family:Arial;'>"
            "Hello from C++ Makefile App (Port 8087)!"
            "</h1>"
            "</body>"
            "</html>";

        send(new_socket, response.c_str(), response.size(), 0);
        close(new_socket);
    }

    return 0;
}
