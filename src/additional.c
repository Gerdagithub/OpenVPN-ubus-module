#include "additional.h"
#include "ubus.h"

volatile bool run = true;

extern char clients[MAX_LENGTH_OF_CLIENT_NAME][MAX_AMOUNT_OF_CLIENTS];
extern int amountOfClients;

void sig_handler(int signal)
{
    run = false;
    uloop_end();
}

int get_common_name_of_a_client(char buffer[BUFFER_SIZE], char fromNextLine[150])
{
    char line[150];
    char temp[150];

    if (!fromNextLine || !buffer){
        syslog(LOG_USER | LOG_ERR, "Could not find needed line");
        return -1;
    }

    size_t length = fromNextLine - buffer;
    memset(line, '\0', sizeof(line));
    strncpy(line, buffer, length);

    if (line[strlen(line) - 1] == 13)
        line[strlen(line) - 1] = '\0';
    
    if (!strcmp(line, "ROUTING TABLE")){
        return 2;
    }

    char *token = strtok(line, ",");
    if (!token){
        syslog(LOG_USER | LOG_ERR, "Could not get client common name");
        return -1;
    }

    strcpy(clients[amountOfClients++], token);
    
    return 0;
}

int get_clients_common_names(char *response) 

{
    char cutFrom[78], buffer[BUFFER_SIZE], temp[BUFFER_SIZE];
    int ret = 0;

    strcpy(buffer, response);
    strcpy(cutFrom, "Common Name,Real Address,Bytes Received,Bytes Sent,Connected Since");

    if (strstr(buffer, cutFrom) == NULL){
        return -1;
    }

    strcpy(temp, strstr(buffer, cutFrom) + strlen(cutFrom)); 
    strncpy(buffer, temp, BUFFER_SIZE);

    while (ret != 2 && run){
        char *fromNextLine = strchr(buffer, '\n');

        if (fromNextLine) {
            ret = get_common_name_of_a_client(buffer, fromNextLine);
            if (ret == 2)
                break;

            strcpy(temp, fromNextLine + 1);
            strcpy(buffer, temp);
        }
    }

    return ret == 2? 0 : ret;
}