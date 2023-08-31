#ifndef CONNECTION_WITH_SERVER_H
#define CONNECTION_WITH_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <stdbool.h>
#include <sys/time.h>

#include "argp_for_openvpn.h"

#define MAX_COMMAND_LEN 100
#define BUFFER_SIZE 1024

int connect_to_the_server(struct Arguments serverInfo, int *sockfd);
int response_from_the_server(int sockfd, char command[MAX_COMMAND_LEN], char *buffer);

#endif