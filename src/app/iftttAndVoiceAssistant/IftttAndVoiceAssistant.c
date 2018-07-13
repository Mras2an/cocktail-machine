#include <lwip/sockets.h>
#include "System.h"
#include "cJSON.h"
#include "Wifi.h"
#include "LedRGBHandling.h"
#include "Cocktail.h"
#include "QueueCocktail.h"

#define PORT_NUMBER    4551

static const char HeadPostHttp[] = "HTTP/1.1 204 No Content\r\n\r\n";
static const char HeadPostHttpErr[] = "HTTP/1.1 400 Bad Request\r\n\r\n";

static void IftttAndVoiceAssistant_checkData(int sock);

/******************************************************************************
 * FunctionName : static void SocketAp_checkData(int sock)
 * Parameters   : int
 * Returns      : void
 *******************************************************************************/
static void IftttAndVoiceAssistant_checkData(int sock)
{
    int total = 256;
    char * tmp = OsMalloc(total);
    memset(tmp, '\0', total);
    read(sock, tmp, total);
    BarDebug_info("%s\n", tmp);
    cJSON * _root = cJSON_Parse(tmp);

    if(_root != NULL) {
        cJSON * _name = cJSON_GetObjectItem(_root, "name");

        if(_name != NULL) {
            int numCocktail = Cocktail_isCocktailExiste(_name->valuestring);

            if(numCocktail != 255) {
                write(sock, HeadPostHttp, strlen(HeadPostHttp));
                QueueCocktail_received(numCocktail);
            } else {
                write(sock, HeadPostHttpErr, strlen(HeadPostHttpErr));
            }
        }
    }

    cJSON_Delete(_root);
    OsFree(tmp);
}

/******************************************************************************
 * FunctionName : static void IftttAndVoiceAssistant_task()
 * Parameters   : void
 * Returns      : void
 *******************************************************************************/
static void IftttAndVoiceAssistant_task()
{
    struct sockaddr_in clientAddress;
    struct sockaddr_in serverAddress;
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(sock < 0) {
        BarDebug_err("socket: %d %s", sock, strerror(errno));
        goto END;
    } else {
        BarDebug_info("socket: %d", sock);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT_NUMBER);
    int rc = bind(sock, (struct sockaddr *) &serverAddress,
                  sizeof(serverAddress));

    if(rc < 0) {
        BarDebug_err("bind: %d %s", rc, strerror(errno));
        goto END;
    } else {
        BarDebug_info("Bind ok\n");
    }

    rc = listen(sock, 5);

    if(rc < 0) {
        BarDebug_err("listen: %d %s", rc, strerror(errno));
        goto END;
    }

    while(1) {
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSock = accept(sock, (struct sockaddr *) &clientAddress,
                                &clientAddressLength);

        if(clientSock < 0) {
            BarDebug_err("accept: %d %s", clientSock, strerror(errno));
            goto END;
        } else {
            BarDebug_info("Accept cli\n");
        }

        int total = 1024;
        char *data = OsMalloc(total);
        memset(data, '\0', total);

        while(1) {
            ssize_t sizeRead = read(clientSock, data, total);

            if(sizeRead < 0) {
                BarDebug_err("recv: %d %s", sizeRead, strerror(errno));
                goto END;
            }

            if(sizeRead == 0) {
                BarDebug_info("Exit\n");
                break;
            }

            BarDebug_info("%s\n", data);
            char * pch = NULL;
            pch = strstr((const char *) data, "cocktail");

            if(pch != NULL) {
                IftttAndVoiceAssistant_checkData(clientSock);
            }

            memset(data, '\0', total);
        }

        OsFree(data);
        close(clientSock);
    }

END:
    System_cpuReset();
    vTaskDelete(NULL);
}

/******************************************************************************
 * FunctionName : void IftttAndVoiceAssistant_init(void)
 * Parameters   : void
 * Returns      : void
 *******************************************************************************/
void IftttAndVoiceAssistant_init(void)
{
    OsTaskCreate(IftttAndVoiceAssistant_task, "IftttAndVoiceAssistant_task", 5096, NULL, 1, NULL);
}
