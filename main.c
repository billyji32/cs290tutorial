#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hashMap.h"

/*
 Takes a FILE pointer and returns you a string which is
 the next word in the file. 
 Words are defined (by this function) to be
 characters or numbers seperated by periods, spaces, or newlines.
 
 when there are no more words in the input file this function will return NULL.
 */
char* getWord(FILE *file);

/*Simple function to determine which hashing funtion to use*/
int getIndex(KeyType k);




int main (int argc, const char * argv[]) {
	const char* filename;
	struct hashMap *hashTable;	
	int tableSize = 10;
	clock_t timer;
	FILE *fileptr;	
    /*
     Optional command line argument usage for filename
    if(argc == 2)
    //    filename = argv[1];
    //else*/
        filename = "input2.txt"; /*specify your input text file here*/
    
  printf("opening file: %s\n", filename);
    
	timer = clock(); /*Used to calculate efficiency*/
	hashTable = createMap(tableSize);	   /*Create a new hashMap*/
	fileptr = fopen(filename, "r");/*Open the file*/
	assert(fileptr != NULL);/*Check that the file opened properly*/
	int value;/*Used to receive the value at the key*/


	for (char* word = getWord(fileptr); word != NULL; word = getWord(fileptr)) /*While the file hasn't reached the end*/
	{			
			if (containsKey(hashTable, word) == 1) { 
			/*If the key is already in the hash table, get the current value at that key and increment it by 1. 
			Then reinsert that key with the new value*/
				value = (int)atMap(hashTable, word);
				value++;
				insertMap(hashTable, word, value);					
			}

			else
				insertMap(hashTable, word, 1); /*else insert the key with a value of 1*/
			}


		fclose(fileptr);/*close the file*/
		
		printMap(hashTable);/*Print the keys and values in the hashMap*/


	timer = clock() - timer;
	/*Print statements for testing purposes*/
	printf("\nconcordance ran in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
	printf("Table emptyBuckets = %d\n", emptyBuckets(hashTable));
    printf("Table count = %d\n", size(hashTable));
	printf("Table capacity = %d\n", capacity(hashTable));
	printf("Table load = %f\n", tableLoad(hashTable));
	

	/*Test the removeKey function*/
	printf("Deleting keys\n");
	removeKey(hashTable, "and");
	removeKey(hashTable, "me");
	removeKey(hashTable, "the");

	printf("Table emptyBuckets = %d\n", emptyBuckets(hashTable));
	printf("Table count = %d\n", size(hashTable));
	printf("Table capacity = %d\n", capacity(hashTable));
	printf("Table load = %f\n", tableLoad(hashTable));
	printMap(hashTable);
		
		/*Delete the hashMap*/
	deleteMap(hashTable);
	printf("\nDeleted the table\n");   
	return 0;
}

void printValue(ValueType v) {
	printf("Value:%d",v);
}

char* getWord(FILE *file)
{
	int length = 0;
	int maxLength = 16;
	char character;
    
	char* word = malloc(sizeof(char) * maxLength);
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
		   character == 39) /*or is an apostrophy*/
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

int getIndex(KeyType k) {
	if (HASHING_FUNCTION == 1)
		return stringHash1(k);
	else
		return stringHash2(k);
}