#include "connection_with_server.h"
#include "additional.h"

extern volatile bool run;

int connect_to_the_server(struct Arguments serverInfo, int *sockfd)
{
    struct sockaddr_in server_addr;
    int ret = 0;

    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        syslog(LOG_USER | LOG_INFO, "Socket creation failed: %d", *sockfd);
        return *sockfd;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(serverInfo.port);
    server_addr.sin_addr.s_addr = inet_addr(serverInfo.ip);

    if (ret = inet_pton(AF_INET, serverInfo.ip, &(server_addr.sin_addr)) <= 0){
        syslog(LOG_USER | LOG_INFO, "Wrong IP address");
        return ret;
    }

    if (ret = connect(*sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        syslog(LOG_USER | LOG_INFO, "Connection with server failed: %d", ret);
        return ret;
    }

    // Selecting timeout for connection
    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(*sockfd, &write_fds);

    struct timeval timeout;
    timeout.tv_sec = 5; 
    timeout.tv_usec = 0;

    int selectRet;
    if ((selectRet = select(*sockfd + 1, NULL, &write_fds, NULL, &timeout)) == -1) {
        syslog(LOG_USER | LOG_INFO, "Select error: %m");
        return selectRet;
    } else if (selectRet == 0) {
        syslog(LOG_USER | LOG_INFO, "Connection timeout");
        return 110;
    }

    if (!FD_ISSET(*sockfd, &write_fds)) {
        syslog(LOG_USER | LOG_INFO, "Connection was not established");
        return 111;
    }

    return ret;
}

int response_from_the_server(int sockfd, char command[MAX_COMMAND_LEN], char *buffer) 
{
    int ret = 0;
    memset(buffer, 0, BUFFER_SIZE);

    if (ret = send(sockfd, command, strlen(command), 0) < 0) {
        syslog(LOG_USER | LOG_INFO, "Send command failed: %d", ret);
        return ret;
    }

    while (run) {
        ssize_t bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_received == -1) {
            syslog(LOG_USER | LOG_INFO, "Receive failed");
            ret = -1;
            break;
        } else if (bytes_received == 0)
            break;

        buffer[bytes_received] = '\0';

        if (strstr(buffer, "END") != NULL)
            break;

        memset(buffer, 0, BUFFER_SIZE);
    }

    return ret;
}
