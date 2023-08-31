#include "argp_for_openvpn.h"
#include "daemon.h"
#include "connection_with_server.h"
#include "ubus.h"
#include "additional.h"

extern volatile bool run;

int main(int argc, char **argv) 
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    struct Arguments serverInfo = { 0 };
    char *response = NULL;
    int sockfd, ret = 0;

    strcpy(serverInfo.ip, "");
    serverInfo.becomeDaemon = false;

    const char *LOGNAME = "OpenVPN clients";
    openlog(LOGNAME, LOG_PID, LOG_USER); 

    syslog(LOG_USER | LOG_INFO, "OpenVPN ubus module started");

    argp_parse(&argp, argc, argv, 0, 0, &serverInfo);
    if (serverInfo.port == 0)
        serverInfo.port = 7505; // using default port

    if (serverInfo.becomeDaemon)
        ret = become_daemon(0);

    if (ret) {
        syslog(LOG_USER | LOG_INFO, "Failed to daemonize the program");
        goto cleanUp;
    }

    if (ret = connect_to_the_server(serverInfo, &sockfd))
        goto cleanUp;

    response = (char*)malloc(sizeof(char) * BUFFER_SIZE); 
    if (!run || (ret = response_from_the_server(sockfd, "status\n", response)))
        goto cleanUp;

    if (ret = get_clients_common_names(response))
        goto cleanUp;

    ret = ubus_loop();

cleanUp:
    if (response != NULL)
        free(response);

    close(sockfd);

    syslog(LOG_USER | LOG_INFO, "OpenVPN ubus module ended");
    closelog();

    return 0;
}