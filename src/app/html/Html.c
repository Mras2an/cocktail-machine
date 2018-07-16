#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "tcpip_adapter.h"
#include "lwip/err.h"
#include "string.h"
#include "cJSON.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/api.h"
#include "Cocktail.h"
#include "Html.h"
#include "Bsp.h"
#include "BspConfig.h"
#include "QueueCocktail.h"
#include <stdio.h>
#include "Debug.h"
#include "FileSystem.h"
#include "System.h"
#include "MotorHandling.h"
#include "Board.h"

#define MY_IP "192.168.1.51/"
#define delay(ms) (vTaskDelay(ms/portTICK_RATE_MS))

const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static char info[] = "<p>Free heap: %d Ko,</p><p>Cpu: 240Mhz,</p><p>(Project: 1117 Version 1.0)</p>";
char http_index_hml[20000];

char * json_unformatted;
const int CONNECTED_BIT = BIT0;

static void html_indexBegin(char * data);

static void html_tabBegin(char * data);

static void html_tabColumnBegin(char * data, char *name);
static void html_tabColumnMiddle(char * data, char *name);
static void html_tabColumnEnd(char * data, char *name);
static void html_tabEnd(char * data);
static void html_indexEnd(char * data);
static void html_indexTitle(char * data);

//static void html_tabColumnBeginWithBp(char * data, char *name);
//static void html_addButton(char * data, char * page, char * buttonName);
//static void html_addImage(char * data, char * url);


static void html_indexBegin(char * data)
{
    const static char http_begin[] = "<!DOCTYPE html>"
                                     "<html>\n"
                                     "<head>\n"
                                     "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
                                     /*CSS*/
                                     "<style>\n"
                                     "h1 {\n"
                                     "color: white;\n"
                                     "text-align: center;\n"
                                     "}\n"
                                     "p {\n"
                                     "font-family: verdana;\n"
                                     "font-size: 20px;\n"
                                     "}\n"
                                     "body {\n"
                                     "background-color: lightblue;\n"
                                     "}\n"
                                     "table\n"
                                     "{\n"
                                     "border-collapse: collapse;\n"
                                     "}\n"
                                     "td\n"
                                     "{\n"
                                     "border: 1px solid black;\n"
                                     "}\n"
                                     ".button {\n"
                                     " width:85px;\n"
                                     " height:85px;\n"
                                     " background:#fafafa;\n"
                                     " box-shadow:2px 2px 8px #aaa;\n"
                                     " font:bold 13px Arial;\n"
                                     " border-radius:50%;\n"
                                     " color:#555;\n"
                                     "}\n"
                                     "</style>\n";
    /* end css */
    memcpy(data, http_begin, strlen(http_begin));
}

/******************************************************************************
 * FunctionName : static void html_indexTitle(char * data)
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
static void html_indexTitle(char * data)
{
    strcat(data,
           "<title>Connected bar</title><link href=\"https://www.bodipure.com/wp-content/uploads/2016/09/B-favicon.png\" rel=\"icon\" type=\"image/x-icon\" /></head><body><h1>Welcome, on Quentin's bar!</h1><hr color=\"white\"><p>Please select your shooter or cocktail:</p>");
}

/******************************************************************************
 * FunctionName : static void html_indexReturn(char * data)
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
//static void html_indexReturn(char * data)
//{
//    strcat(data, "<title>Connected bar</title></head><body><h1>Nice!</h1><hr color=\"white\"><p>Click here to return on cocktails page.</p>");
//}

/******************************************************************************
 * FunctionName : static void html_tabBegin(char * data)
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
static void html_tabBegin(char * data)
{
    strcat(data, "<center><table>");
}

/******************************************************************************
 * FunctionName : static void html_tabColumnBeginWithBp(char * data, char *name)
 * Parameters   : char *
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
//static void html_tabColumnBeginWithBp(char * data, char *name)
//{
//  strcat(data, "<tr><td>");
//  html_addButtonCss(data, "lqt", name);
//  strcat(data, "</td>");
//}

/******************************************************************************
 * FunctionName : static void html_tabColumnBegin(char * data, char *name)
 * Parameters   : char *
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
static void html_tabColumnBegin(char * data, char *name)
{
    strcat(data, "<tr><td>");
    strcat(data, name);
    strcat(data, "</td>");
}

/******************************************************************************
 * FunctionName : static void html_tabColumnMiddle(char * data, char *name)
 * Parameters   : char *
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
static void html_tabColumnMiddle(char * data, char *name)
{
    strcat(data, "<td>");
    strcat(data, name);
    strcat(data, "</td>");
}

/******************************************************************************
 * FunctionName : static void html_tabColumnEnd(char * data, char *name)
 * Parameters   : char *
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
static void html_tabColumnEnd(char * data, char *name)
{
    strcat(data, "<td>");
    strcat(data, name);
    strcat(data, "</td></tr>");
}

/******************************************************************************
 * FunctionName : static void html_tabEnd(char * data)
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
static void html_tabEnd(char * data)
{
    strcat(data, "</table></center>");
}

/******************************************************************************
 * FunctionName : void html_addButtonCss(char * data, char * page, char * buttonName)
 * Parameters   : char *
 * Parameters   : char *
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
void html_addButtonCss(char * data, char * page, char * buttonName)
{
    /* bp with css */
    strcat(data, "<input type=\"button\" class=\"button\" value=\"");
    strcat(data, buttonName);
    strcat(data, "\" onclick=\"window.location.href='http://"MY_IP);
    strcat(data, page);
    strcat(data, "'\" >\n");
}

/******************************************************************************
 * FunctionName : static void html_addButton(char * data, char * page, char * buttonName)
 * Parameters   : char *
 * Parameters   : char *
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
//static void html_addButton(char * data, char * page, char * buttonName)
//{
//  /* bp without css */
//  strcat(data, "<form>\n<input type=\"button\" value=\"");
//  strcat(data, buttonName);
//  strcat(data, "\" onclick=\"window.location.href='http://"MY_IP);
//  strcat(data, page);
//  strcat(data, "'\" />\n</form>\n");
//}

/******************************************************************************
 * FunctionName : static void html_addImage(char * data, char * url)
 * Parameters   : char *
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
//static void html_addImage(char * data, char * url)
//{
//  strcat(data, "<img src=\"");
//  strcat(data, url);
//  strcat(data, "\">");
//}

/******************************************************************************
 * FunctionName : static void html_indexEnd(char * data)
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
static void html_indexEnd(char * data)
{
    const static char http_end[] = "</body>\n"
                                   "</html>\n";
    strcat(data, http_end);
}

/******************************************************************************
 * FunctionName : static void http_server_netconn_serve(struct netconn *conn)
 * Parameters   : netconn *
 * Returns      : none
 *******************************************************************************/
static void http_server_netconn_serve(struct netconn *conn)
{
    struct netbuf *inbuf;
    char *buf;
    u16_t buflen;
    err_t err;
    err = netconn_recv(conn, &inbuf);

    if(err == ERR_OK) {
        netbuf_data(inbuf, (void**) &buf, &buflen);

        if(buflen >= 5 &&
           buf[0] == 'G' &&
           buf[1] == 'E' &&
           buf[2] == 'T' &&
           buf[3] == ' ' &&
           buf[4] == '/') {
            netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
            netconn_write(conn, http_index_hml, sizeof(http_index_hml) - 1, NETCONN_NOCOPY);
            int numCocktail = Cocktail_isCocktailExiste(&buf[5]);

            if(numCocktail != 255) {
                BarDebug_info("Free Heap = %d o\n", OsGetFreeHeapSize());
                QueueCocktail_received(numCocktail);
            }

            if(strncmp(&buf[5], "Update", strlen("Update")) == 0) {
                if(Fs_write("Update", "Update", "Update") != ESP_OK) {
                    BarDebug_err("Error to update\n");
                } else {
                    System_cpuReset();
                }
            }

            if(strncmp(&buf[5], "AxeX+1", strlen("AxeX+1")) == 0) {
                BarDebug_info("Move AxeX+1\n");
                MotorHandling_setPositionOnX(1);
            }

            if(strncmp(&buf[5], "Pump+2", strlen("Pump+2")) == 0) {
                BarDebug_info("Clean Pump+2\n");
                MotorHandling_getAMeasureOnPump(2, MOTOR_PUMP_3);
                MotorHandling_getAMeasureOnPump(2, MOTOR_PUMP_2);
                MotorHandling_getAMeasureOnPump(2, MOTOR_PUMP_1);
            }
        }
    }

    netconn_close(conn);
    netbuf_delete(inbuf);
}

/******************************************************************************
 * FunctionName : static void http_server(void *pvParameters)
 * Parameters   : void *
 * Returns      : none
 *******************************************************************************/
static void http_server(void *pvParameters)
{
    struct netconn *conn, *newconn;
    err_t err;
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 80);
    netconn_listen(conn);

    do {
        err = netconn_accept(conn, &newconn);

        if(err == ERR_OK) {
            http_server_netconn_serve(newconn);
            netconn_delete(newconn);
        }
    } while(err == ERR_OK);

    netconn_close(conn);
    netconn_delete(conn);
}

/******************************************************************************
 * FunctionName : void Html_init(void)
 * Parameters   : void
 * Returns      : none
 *******************************************************************************/
void Html_init(void)
{
    memset(http_index_hml, '\0', 20000);
    html_indexBegin(http_index_hml);
    html_indexTitle(http_index_hml);
    html_tabBegin(http_index_hml);
    html_tabColumnBegin(http_index_hml, "Cocktail");
    html_tabColumnMiddle(http_index_hml, "Ingredient(s) à ajouter");
    html_tabColumnEnd(http_index_hml, "Ingredient(s) Fourni(s)");
    Cocktail_createHtmlCodeForCocktails(http_index_hml);
    html_tabEnd(http_index_hml);
    char data[strlen(info) + 4];
    memset(data, '\0', strlen(info) + 4);
    sprintf(data, info, (OsGetFreeHeapSize()/1024));
    strcat(http_index_hml, data);
    html_addButtonCss(http_index_hml, "Update", "Update");
    html_addButtonCss(http_index_hml, "AxeX+1", "AxeX+1");
    html_addButtonCss(http_index_hml, "Pump+2", "Pump+2");
    html_indexEnd(http_index_hml);
    xTaskCreate(&http_server, "http_server", 9216, NULL, 5, NULL);
}
