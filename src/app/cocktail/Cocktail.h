#ifndef COCKTAIL_H_
#define COCKTAIL_H_

#define MAX_NAME_SIZE 20
#define MAX_NOTE_SIZE 80
#define MAX_NOTE_SIZE_FOR_BOTTLE 10
#define MAX_COCKTAIL 30
#define MAX_BOTTLE   10
#define MAX_INGREDIENT 8

void Cocktail_init(void);

void Cocktail_createHtmlCodeForBottles(char * data);

void Cocktail_createHtmlCodeForCocktails(char * data);

int Cocktail_isCocktailExiste(char * data);

int Cocktail_getDispoIngredients(int * qBottle, int * qPosition, int * qMeasure, char * qNote, int numCocktail);

#endif /* COCKTAIL_H_ */
