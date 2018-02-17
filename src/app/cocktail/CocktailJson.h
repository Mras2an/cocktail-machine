#ifndef COCKTAIL_JSON_H_
#define COCKTAIL_JSON_H_
//char bb[] = "{\"bottles\":[{\"bottle\":{\"name\":\"eau\",\"note\":\"0%/vol\",\"position\":0}},{\"bottle\":{\"name\":\"whisky\",\"note\":\"50%/vol\",\"position\":1}},{\"bottle\":{\"name\":\"tequila\",\"note\":\"50%/vol\",\"position\":2}},{\"bottle\":{\"name\":\"pastis\",\"note\":\"50%/vol\",\"position\":3}},{\"bottle\":{\"name\":\"sirop de sucre\",\"note\":\"50%/vol\",\"position\":4}},{\"bottle\":{\"name\":\"get 27\",\"note\":\"0%/vol\",\"position\":5}},{\"bottle\":{\"name\":\"curacao bleu\",\"note\":\"20%/vol\",\"position\":6}},{\"bottle\":{\"name\":\"cointreau\",\"note\":\"25%/vol\",\"position\":7}},{\"bottle\":{\"name\":\"jus de citron\",\"note\":\"0%/vol\",\"position\":9}},{\"bottle\":{\"name\":\"rhum blanc\",\"note\":\"50%/vol\",\"position\":8}}]}";
char bb[] =
  "{\"bottles\":[{\"bottle\":{\"name\":\"eau\",\"note\":\"1\",\"position\":0}},"
  "{\"bottle\":{\"name\":\"jus de citron\",\"note\":\"2\",\"position\":0}},"
  "{\"bottle\":{\"name\":\"whisky\",\"note\":\"50%/vol\",\"position\":1}},"
  //"{\"bottle\":{\"name\":\"tequila\",\"note\":\"50%/vol\",\"position\":2}},"
  "{\"bottle\":{\"name\":\"rhum blanc\",\"note\":\"50%/vol\",\"position\":2}},"
  "{\"bottle\":{\"name\":\"pastis\",\"note\":\"50%/vol\",\"position\":3}},"
  //"{\"bottle\":{\"name\":\"sirop de sucre\",\"note\":\"50%/vol\",\"position\":4}},"
  //"{\"bottle\":{\"name\":\"get 27\",\"note\":\"0%/vol\",\"position\":4}},"
  "{\"bottle\":{\"name\":\"tequila\",\"note\":\"0%/vol\",\"position\":4}},"
  "{\"bottle\":{\"name\":\"curacao bleu\",\"note\":\"20%/vol\",\"position\":5}},"
  "{\"bottle\":{\"name\":\"cointreau\",\"note\":\"25%/vol\",\"position\":6}},"
  "{\"bottle\":{\"name\":\"get 27\",\"note\":\"50%/vol\",\"position\":7}}]}";
  //"{\"bottle\":{\"name\":\"rhum blanc\",\"note\":\"50%/vol\",\"position\":7}}]}";

char cc[] =
  "{\"cocktails\":[{\"cocktail\":{\"name\":\"Mojito\",\"ingredients\":[{\"ingredient\":{\"name\":\"rhum blanc\",\"measure\":2}},{\"ingredient\":{\"name\":\"menthe verte\",\"measure\":1}},{\"ingredient\":{\"name\":\"jus de citron\",\"measure\":1}},{\"ingredient\":{\"name\":\"sucre de canne\",\"measure\":1}},{\"ingredient\":{\"name\":\"eau gazeuse\",\"measure\":2}},{\"ingredient\":{\"name\":\"glace pilee\",\"measure\":1}}]}},"
  "{\"cocktail\":{\"name\":\"PetitJaune\",\"ingredients\":[{\"ingredient\":{\"name\":\"pastis\",\"measure\":2}},{\"ingredient\":{\"name\":\"eau\",\"measure\":5}}]}},"
  "{\"cocktail\":{\"name\":\"BlueSea\",\"ingredients\":[{\"ingredient\":{\"name\":\"limonade\",\"measure\":3}},{\"ingredient\":{\"name\":\"jus de citron\",\"measure\":2}},{\"ingredient\":{\"name\":\"rhum blanc\",\"measure\":2}},{\"ingredient\":{\"name\":\"curacao bleu\",\"measure\":2}},{\"ingredient\":{\"name\":\"cointreau\",\"measure\":1}},{\"ingredient\":{\"name\":\"tequila\",\"measure\":1}}]}},"
  "{\"cocktail\":{\"name\":\"TiPunch\",\"ingredients\":[{\"ingredient\":{\"name\":\"rhum blanc\",\"measure\":2}},{\"ingredient\":{\"name\":\"sirop de sucre\",\"measure\":1}},{\"ingredient\":{\"name\":\"jus de citron\",\"measure\":1}}]}},"
  "{\"cocktail\":{\"name\":\"CubaLibre\",\"ingredients\":[{\"ingredient\":{\"name\":\"rhum blanc\",\"measure\":2}},{\"ingredient\":{\"name\":\"coca\",\"measure\":5}}]}},"
  "{\"cocktail\":{\"name\":\"BlueHawaiian\",\"ingredients\":[{\"ingredient\":{\"name\":\"rhum blanc\",\"measure\":2}},{\"ingredient\":{\"name\":\"curacao bleu\",\"measure\":1}},{\"ingredient\":{\"name\":\"jus d'ananas\",\"measure\":3}},{\"ingredient\":{\"name\":\"coco\",\"measure\":1}}]}},"
  "{\"cocktail\":{\"name\":\"JagerBomb\",\"ingredients\":[{\"ingredient\":{\"name\":\"jagermeister\",\"measure\":1}},{\"ingredient\":{\"name\":\"redbull\",\"measure\":4}}]}},"
  "{\"cocktail\":{\"name\":\"Cascade\",\"ingredients\":[{\"ingredient\":{\"name\":\"jus d'orange\",\"measure\":2}},{\"ingredient\":{\"name\":\"curacao bleu\",\"measure\":1}},{\"ingredient\":{\"name\":\"get 27\",\"measure\":1}},{\"ingredient\":{\"name\":\"rhum blanc\",\"measure\":2}},{\"ingredient\":{\"name\":\"curacao bleu\",\"measure\":1}}]}},"
  //"{\"cocktail\":{\"name\":\"PetitJaune\",\"noteF\":\"pastis, eau.\",\"noteA\":\"\",\"ingredients\":[{\"ingredient\":{\"name\":\"pastis\",\"measure\":2}},{\"ingredient\":{\"name\":\"eau\",\"measure\":5}}]}},"
  "{\"cocktail\":{\"name\":\"Daiquiri\",\"ingredients\":[{\"ingredient\":{\"name\":\"rhum blanc\",\"measure\":2}},{\"ingredient\":{\"name\":\"sirop de sucre\",\"measure\":1}},{\"ingredient\":{\"name\":\"jus de citron\",\"measure\":2}}]}},"
  //"{\"cocktail\":{\"name\":\"Perroget\",\"ingredients\":[{\"ingredient\":{\"name\":\"pastis\",\"measure\":1}},{\"ingredient\":{\"name\":\"get 27\",\"measure\":1}},{\"ingredient\":{\"name\":\"eau\",\"measure\":2}}]}},"
  //"{\"cocktail\":{\"name\":\"Goudron\",\"ingredients\":[{\"ingredient\":{\"name\":\"pastis\",\"measure\":1}},{\"ingredient\":{\"name\":\"coca\",\"measure\":2}}]}},"
  //"{\"cocktail\":{\"name\":\"Tomate\",\"noteF\":\"pastis, eau, sirop de grenadine.\",\"noteA\":\"\",\"ingredients\":[{\"ingredient\":{\"name\":\"pastis\",\"measure\":1}},{\"ingredient\":{\"name\":\"sirop de grenadine\",\"measure\":1}},{\"ingredient\":{\"name\":\"eau\",\"measure\":2}}]}},"
  "{\"cocktail\":{\"name\":\"Tomate\",\"ingredients\":[{\"ingredient\":{\"name\":\"pastis\",\"measure\":1}},{\"ingredient\":{\"name\":\"sirop de grenadine\",\"measure\":1}},{\"ingredient\":{\"name\":\"eau\",\"measure\":2}}]}},"
  "{\"cocktail\":{\"name\":\"Barbarie\",\"ingredients\":[{\"ingredient\":{\"name\":\"pastis\",\"measure\":1}},{\"ingredient\":{\"name\":\"curacao bleu\",\"measure\":1}},{\"ingredient\":{\"name\":\"eau\",\"measure\":5}}]}},"
  "{\"cocktail\":{\"name\":\"OldFashioned\",\"ingredients\":[{\"ingredient\":{\"name\":\"sirop de sucre\",\"measure\":1}},{\"ingredient\":{\"name\":\"eau gazeuse\",\"measure\":1}},{\"ingredient\":{\"name\":\"whisky\",\"measure\":2}},{\"ingredient\":{\"name\":\"jus d'orange\",\"measure\":2}}]}},"
  "{\"cocktail\":{\"name\":\"Ccc\",\"ingredients\":[{\"ingredient\":{\"name\":\"whisky\",\"measure\":2}},{\"ingredient\":{\"name\":\"jus de citron\",\"measure\":1}},{\"ingredient\":{\"name\":\"coca\",\"measure\":4}}]}},"
  "{\"cocktail\":{\"name\":\"WhiskyCoca\",\"ingredients\":[{\"ingredient\":{\"name\":\"whisky\",\"measure\":2}},{\"ingredient\":{\"name\":\"caca\",\"measure\":4}}]}},"
  "{\"cocktail\":{\"name\":\"Maudite\",\"ingredients\":[{\"ingredient\":{\"name\":\"whisky\",\"measure\":2}},{\"ingredient\":{\"name\":\"get 27\",\"measure\":1}}]}},"
  "{\"cocktail\":{\"name\":\"Fourmi\",\"ingredients\":[{\"ingredient\":{\"name\":\"whisky\",\"measure\":2}},{\"ingredient\":{\"name\":\"curacao bleu\",\"measure\":1}},{\"ingredient\":{\"name\":\"coca\",\"measure\":4}}]}},"
  "{\"cocktail\":{\"name\":\"Millionnaire\",\"ingredients\":[{\"ingredient\":{\"name\":\"whisky\",\"measure\":2}},{\"ingredient\":{\"name\":\"cointreau\",\"measure\":1}}]}},"
  "{\"cocktail\":{\"name\":\"JuraMidge\",\"ingredients\":[{\"ingredient\":{\"name\":\"whisky\",\"measure\":2}},{\"ingredient\":{\"name\":\"jus de citron\",\"measure\":1}}]}},"
  "{\"cocktail\":{\"name\":\"PinaColada\",\"ingredients\":[{\"ingredient\":{\"name\":\"rhum blanc\",\"measure\":2}},{\"ingredient\":{\"name\":\"jus d'ananas\",\"measure\":4}},{\"ingredient\":{\"name\":\"lait de coco\",\"measure\":1}}]}}]}";

#endif /* COCKTAIL_JSON_H_ */
