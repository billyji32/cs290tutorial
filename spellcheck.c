/*A spellcheck program that loads a dictionary into a hashMap and then takes user input and checks if that words is in the hashTable*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "hashMap.h"


char* getWord(FILE *file);
/*
 Takes a FILE pointer and returns you a string which is
 the next word in the file. 
 Words are defined (by this function) to be
 characters or numbers seperated by periods, spaces, or newlines.
 
 when there are no more words in the input file this function will return NULL.
 */

void loadDictionary(FILE* file, struct hashMap* ht);
/*
 Load the contents of file into hashmap ht
 */


int main (int argc, const char * argv[]) {
  /*Used for testing time/efficiency*/
  clock_t timer;
  timer = clock();

	/*Initiate a new hashMap*/
  struct hashMap* hashTable;
  int tableSize = 1000;
  hashTable=createMap(tableSize);

  /*Open the file for reading*/
  const char* filename="dictionary.txt"; /*Input filename here or use provided dictionary file*/
	FILE* dictionary;
  dictionary = fopen(filename, "r");

	/*Add all words in the provided dictionary file to the hashTable*/
  loadDictionary(dictionary,hashTable);

  timer = clock() - timer;
	printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
  
  char* word = (char*)malloc(256*sizeof(char));/*Used to hold each word in the file*/
  int quit=0;

/*While loop that checks user input against the hashTable, enter 'quit' to exit*/
  while(!quit){
    printf("Enter a word for spell checking, or 'quit' to exit the program ");
    scanf("%s",word);
	if (containsKey(hashTable, word) == 1)
		printf("The word '%s' exists in our dictionary \n", word);
	else
		printf("The word '%s' does not exist in our dictionary, please check your spelling and try again \n", word);
  
      if(strcmp(word,"quit")==0)
      quit=!quit;
  }
  free(word);
     
  return 0;
}

/*Simply inserts each word into the hashTable with a value of 1(Value is unnecessary for this program's purposes*/
void loadDictionary(FILE* file, struct hashMap* ht)
{
	for (char* word = getWord(file); word != NULL; word = getWord(file))
	{
		insertMap(ht, word, 1);
	}
}

char* getWord(FILE *file)
{
	int length = 0;
	int maxLength = 16;
	char character;
    
	char* word = (char*)malloc(sizeof(char) * maxLength);
	assert(word != NULL);
    
	while( (character = fgetc(file)) != EOF)
	{
		if((length+1) > maxLength)
		{
			maxLength *= 2;
			word = (char*)realloc(word, maxLength);
		}
		if((character >= '0' && character <= '9') || /*is a number*/
		   (character >= 'A' && character <= 'Z') || /*or an uppercase letter*/
		   (character >= 'a' && character <= 'z') || /*or a lowercase letter*/
		   (character == 39)) /*or is an apostrophy*/
		{
			word[length] = character;
			length++;
		}
		else if(length > 0)
			break;
	}
    
	if(length == 0)
	{
		free(word);
		return NULL;
	}
	word[length] = '\0';
	return word;
}
