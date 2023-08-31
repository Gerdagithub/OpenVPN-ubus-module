#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "connection_with_server.h"

void sig_handler(int signal);
int get_common_name_of_a_client(char buffer[BUFFER_SIZE], char fromNextLine[150]);
int get_clients_common_names(char *response);
#endif