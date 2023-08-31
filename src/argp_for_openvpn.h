#ifndef ARGP_FOR_OPENVPN_H
#define ARGP_FOR_OPENVPN_H

#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <stdbool.h>

static struct argp_option options[] = {
    {"serverIpAddress",   'a', "SERVER_IP_ADDRESS", 0,
    "Server's IP address" },

    {"port",    'p', "PORT", 0,
    "Port" },

    {"becomeDaemon", 'D', NULL, 0, 
    "Become daemon program" },

    { 0 }
};

struct Arguments
{
    char ip[40];
    int port;
    bool becomeDaemon;
};

error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = { options, parse_opt, 0, 0 };

#endif