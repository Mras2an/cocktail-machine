#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "cJSON.h"
#include "Bsp.h"
#include "BspConfig.h"
#include "Html.h"
#include "CocktailJson.h"
#include "Cocktail.h"
#include "CpuDelay.h"

typedef struct {
    char name[MAX_NAME_SIZE];
    char note[MAX_NOTE_SIZE_FOR_BOTTLE];
    int position;
} sBottle;

typedef struct {
    char name[MAX_NAME_SIZE];
    int measure;
} sIngredient;

typedef struct cocktail {
    char name[MAX_NAME_SIZE];
    sIngredient ingredient[MAX_INGREDIENT];
} sCocktail;

sCocktail cocktail[MAX_COCKTAIL];
sBottle bottle[MAX_BOTTLE];

static void cocktail_task();
static int Cocktail_isBottleExiste(char * data);

/******************************************************************************
 * FunctionName : void Cocktail_init(void)
 * Parameters   : void
 * Returns      : none
 *******************************************************************************/
void Cocktail_init(void)
{
    OsTaskCreate(cocktail_task, "cocktail_task", 56000, NULL, 1, NULL);
}

/******************************************************************************
 * FunctionName : int Cocktail_isCocktailExiste(char * data)
 * Parameters   : char *
 * Returns      : int
 *******************************************************************************/
int Cocktail_isCocktailExiste(char * data)
{
    int i = 0;

    for(i = 0; i < MAX_COCKTAIL; i++) {
        if(cocktail[i].name[0] != '\0') {
            if(strncmp(data, cocktail[i].name, strlen(cocktail[i].name)) == 0) {
                BarDebug_info("---------- %s ----------\n", cocktail[i].name);
                return i;
            }
        }
    }

    return 255;
}

/******************************************************************************
 * FunctionName : void Cocktail_createHtmlCodeForBottles(char * data)
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
void Cocktail_createHtmlCodeForBottles(char * data)
{
    int i;

    for(i = 0; i < MAX_BOTTLE; i++) {
        if(bottle[i].name[0] != '\0') {
            strcat(data, "<tr><td>");
            html_addButtonCss(data, bottle[i].name, bottle[i].name);
            strcat(data, "</td>");
            strcat(data, "<td>");
            strcat(data, "");
            strcat(data, "</td>");
            strcat(data, "<td>");
            strcat(data, bottle[i].note);
            strcat(data, "</td></tr>");
        }
    }
}

/******************************************************************************
 * FunctionName : static int Cocktail_isBottleExiste(char * data)
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
static int Cocktail_isBottleExiste(char * data)
{
    int i;

    for(i = 0; i < MAX_BOTTLE; i++) {
        if(bottle[i].name[0] != '\0') {
            if(strncmp(data, bottle[i].name, strlen(data)) == 0) {
                return 1;
            }
        }
    }

    return 0;
}

/******************************************************************************
 * FunctionName : void Cocktail_createHtmlCodeForCocktails(char * data)
 * Parameters   : char *
 * Returns      : none
 *******************************************************************************/
void Cocktail_createHtmlCodeForCocktails(char * data)
{
    int i, j;

    for(i = 0; i < MAX_COCKTAIL; i++) {
        if(cocktail[i].name[0] != '\0') {
            strcat(data, "<tr><td>");
            html_addButtonCss(data, cocktail[i].name, cocktail[i].name);
            strcat(data, "</td>");
            char iHave[MAX_NOTE_SIZE], iDontHave[MAX_NOTE_SIZE];
            memset(iHave, '\0', MAX_NOTE_SIZE);
            memset(iDontHave, '\0', MAX_NOTE_SIZE);

            for(j = 0; j < MAX_INGREDIENT; j++) {
                if(cocktail[i].ingredient[j].name[0] != '\0') {
                    if(Cocktail_isBottleExiste(cocktail[i].ingredient[j].name)) {
                        strncat(iHave, cocktail[i].ingredient[j].name, strlen(cocktail[i].ingredient[j].name));
                        strncat(iHave, ", ", 2);
                    } else {
                        strncat(iDontHave, cocktail[i].ingredient[j].name, strlen(cocktail[i].ingredient[j].name));
                        strncat(iDontHave, ", ", 2);
                    }
                }
            }

            strcat(data, "<td>");
            strncat(data, iDontHave, strlen(iDontHave));
            strcat(data, "</td>");
            strcat(data, "<td>");
            strncat(data, iHave, strlen(iHave));
            strcat(data, "</td></tr>");
        }
    }
}

/******************************************************************************
 * FunctionName : int Cocktail_getDispoIngredients(int * qBottle, int * qPosition, int * qMeasure, char * qNote, int numCocktail)
 * Parameters   : int *
 * Parameters   : int *
 * Parameters   : int *
 * Parameters   : char *
 * Parameters   : int
 * Returns      : int
 *******************************************************************************/
int Cocktail_getDispoIngredients(int * qBottle, int * qPosition, int * qMeasure, char * qNote, int numCocktail)
{
    int i, j = 0, k;

    for(k = 0; k < MAX_INGREDIENT; k++) {
        if(cocktail[numCocktail].ingredient[k].name[0] != '\0') {
            for(i = 0; i < MAX_BOTTLE; i++) {
                if(bottle[i].name[0] != '\0') {
                    if(strncmp(cocktail[numCocktail].ingredient[k].name,
                               bottle[i].name, strlen(cocktail[numCocktail].ingredient[k].name)) == 0) {
                        BarDebug_info("name: %s\n", cocktail[numCocktail].ingredient[k].name);
                        qBottle[j] = i;
                        qPosition[j] = bottle[i].position;
                        qNote[j] = bottle[i].note[0];
                        qMeasure[j] =
                            cocktail[numCocktail].ingredient[k].measure;
                        j++;
                    }
                }
            }
        }
    }

    return j;
}

/******************************************************************************
 * FunctionName : static void cocktail_task()
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
static void cocktail_task()
{
    int i, j;

    for(i = 0; i < MAX_BOTTLE; i++) {
        memset(bottle[i].name, '\0', MAX_NAME_SIZE);
        memset(bottle[i].note, '\0', MAX_NOTE_SIZE_FOR_BOTTLE);
    }

    for(i = 0; i < MAX_COCKTAIL; i++) {
        memset(cocktail[i].name, '\0', MAX_NAME_SIZE);

        for(j = 0; j < MAX_INGREDIENT; j++) {
            memset(cocktail[i].ingredient[j].name, '\0', MAX_NAME_SIZE);
        }
    }

    cJSON * _root = cJSON_Parse(bb);

    if(_root != NULL) {
        cJSON * _bottles = cJSON_GetObjectItem(_root, "bottles");

        if(_bottles != NULL) {
            int nbBottles = cJSON_GetArraySize(_bottles);

            for(i = 0; i < nbBottles; i++) {
                cJSON * _tab = cJSON_GetArrayItem(_bottles, i);

                if(_tab != NULL) {
                    cJSON * _bottle = cJSON_GetObjectItem(_tab, "bottle");

                    if(_bottle != NULL) {
                        cJSON * _name = cJSON_GetObjectItem(_bottle, "name");

                        if(_name != NULL) {
                            cJSON * _note = cJSON_GetObjectItem(_bottle, "note");

                            if(_note != NULL) {
                                cJSON * _position = cJSON_GetObjectItem(_bottle, "position");

                                if(_position != NULL) {
                                    memcpy(bottle[i].name, _name->valuestring, strlen(_name->valuestring));
                                    memcpy(bottle[i].note, _note->valuestring, strlen(_note->valuestring));
                                    bottle[i].position = (int) _position->valuedouble;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    cJSON_Delete(_root);
    _root = cJSON_Parse(cc);

    if(_root != NULL) {
        cJSON * _cocktails = cJSON_GetObjectItem(_root, "cocktails");

        if(_cocktails != NULL) {
            int nbCocktails = cJSON_GetArraySize(_cocktails);

            for(i = 0; i < nbCocktails; i++) {
                cJSON * _tab = cJSON_GetArrayItem(_cocktails, i);

                if(_tab != NULL) {
                    cJSON * _cocktail = cJSON_GetObjectItem(_tab, "cocktail");

                    if(_cocktail != NULL) {
                        cJSON * _name = cJSON_GetObjectItem(_cocktail, "name");

                        if(_name != NULL) {
                            memcpy(cocktail[i].name, _name->valuestring, strlen(_name->valuestring));
                            cJSON * _ingredients = cJSON_GetObjectItem(_cocktail, "ingredients");

                            if(_ingredients != NULL) {
                                int nbIngredients = cJSON_GetArraySize(_ingredients);

                                for(j = 0; j < nbIngredients; j++) {
                                    cJSON * _tab2 = cJSON_GetArrayItem(_ingredients, j);

                                    if(_tab2 != NULL) {
                                        cJSON * _ingredient = cJSON_GetObjectItem(_tab2, "ingredient");

                                        if(_ingredient != NULL) {
                                            cJSON * _nameIngredient = cJSON_GetObjectItem(_ingredient, "name");

                                            if(_nameIngredient != NULL) {
                                                cJSON * _measureIngredient = cJSON_GetObjectItem(_ingredient, "measure");

                                                if(_measureIngredient != NULL) {
                                                    memcpy(cocktail[i].ingredient[j].name,  _nameIngredient->valuestring, strlen(_nameIngredient->valuestring));
                                                    cocktail[i].ingredient[j].measure = (int) _measureIngredient->valuedouble;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    cJSON_Delete(_root);
    Html_init();

    while(1) {
        CpuDelay_ms(1000);
    }
}
