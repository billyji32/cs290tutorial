#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "hashMap.h"



/*the first hashing function you can use*/
int stringHash1(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += str[i];
	return r;
}

/*the second hashing function you can use*/
int stringHash2(char * str)
{
	int i;
	int r = 0;
	for (i = 0; str[i] != '\0'; i++)
		r += (i+1) * str[i]; /*the difference between stringHash1 and stringHash2 is on this line*/
	return r;
}

/* initialize the supplied hashMap struct*/
void _initMap (struct hashMap * ht, int tableSize)
{
	int index;
	if(ht == NULL)
		return;
	ht->table = (hashLink**)malloc(sizeof(hashLink*) * tableSize);
	ht->tableSize = tableSize;
	ht->count = 0;
	for(index = 0; index < tableSize; index++)
		ht->table[index] = NULL;
}

/* allocate memory and initialize a hash map*/
hashMap *createMap(int tableSize) {
	assert(tableSize > 0);
	hashMap *ht;
	ht = malloc(sizeof(hashMap));
	assert(ht != 0);
	_initMap(ht, tableSize);
	return ht;
}

/*
 Free all memory used by the buckets.
 */
void _freeMap (struct hashMap * ht)
{  
	struct hashLink *temp;
	int i;
	for (i = 0;i < ht->tableSize;i++)
		if (ht->table[i])
		{
			while (ht->table[i]) {
				free(ht->table[i]->key);
				temp = ht->table[i];
				ht->table[i] = ht->table[i]->next;
				free(temp);
			}
		}
}

/* Deallocate buckets and the hash map.*/
void deleteMap(hashMap *ht) {
	assert(ht!= 0);
	/* Free all memory used by the buckets */
	_freeMap(ht);
	/* free the hashMap struct */
	free(ht);
}

/*Helper function to determine which hashing function to use*/
int _getIndex(KeyType k) {
	if (HASHING_FUNCTION==1)  
		return stringHash1(k);
	else
		return stringHash2(k);
}

/* 

* Create a new hash table with size==twice current size
* Add each element in the old table to the new table
* Clean up the old table
*/
void _setTableSize(struct hashMap * ht, int newTableSize)
{
	assert(ht);
	int i; 
	struct hashMap *newTable = createMap(newTableSize);/* create a new table to copy over the current table's values*/
	hashLink *cur, *last; /*pointers to iterate through the current table*/

	for (i = 0; i < ht->tableSize; i++)
	{
		if (ht->table[i] != 0) {
			cur = ht->table[i];
			while (cur != 0)
			{
				insertMap(newTable, cur->key, cur->value);
				last = cur;
				cur = cur->next;
				free(last);
			}
		}
	}

	/*copy over values from the new table to the current table*/
	ht->table = newTable->table;
	ht->tableSize = newTable->tableSize;
	/*no need to update ht->count because count stays the same*/
	free(newTable); /* Free up the table */
}

/*
 Inserts a new hashLink into the table. 
 If the key already exists in this table, remove the hashlink and replace it with the new one
 Also will resize the table(tablesize*2) if the load factor reaches a certain point/
 the table runs out of room for new entries
 */
void insertMap (struct hashMap * ht, KeyType k, ValueType v)
{  
	
	/*Hash the key*/
	int hashIndex = _getIndex(k);
	hashIndex = (int)hashIndex%ht->tableSize;
	printf("%i", hashIndex);

	/*Create a newLink and check that it worked*/
	hashLink *newLink = (struct hashLink *)malloc(sizeof(hashLink));
	assert(newLink);
	
	/*If the current count reaches 5 less than the tablesize, then resize the table*/
	if (ht->tableSize - ht->count < 5)
		_setTableSize(ht, ht->tableSize * 2);
	
	/*If the table already contains this key, first remove it*/
	if (containsKey(ht, k))
		removeKey(ht, k);

  /*Set newLink's attributes*/
	newLink->key = k;
	newLink->value = v;
	
	/*Set newLink->next to the current pointer at this hashIndex, or NULL if there is none*/
	newLink->next = ht->table[hashIndex];
	/*Set table[hashIndex] to newLink*/
	ht->table[hashIndex] = newLink;
	
	/*Increase count*/
	ht->count++;

 /*If the load factor is equal to or greater than LOAD_FACTOR_THRESHOLD, resize the table*/
	if ((ht->count / (double)ht->tableSize) >= LOAD_FACTOR_THRESHOLD)
		_setTableSize(ht, (ht->tableSize)*2);

}

/*
 Returns a (void*) value at the key.
 If the key is not found or the key does not have a value, return NULL.
 */
ValueType* atMap (struct hashMap * ht, KeyType k)
{ 
	assert(ht);
	/*Hash the key*/
	int hashIndex=_getIndex(k);
	hashIndex = hashIndex%ht->tableSize;

	struct hashLink *temp;

	/*If table[hashIndex] is currently NULL, return NULL*/
	if (ht->table[hashIndex] == 0)
		return 0;
	
		temp = ht->table[hashIndex];
		/*Loop through the pointers using ->next until either NULL is reached or the key is found*/
		for (;temp && strcmp(temp->key, k) != 0; temp = temp->next);
		return temp ? temp->value : 0; /*returns the value if k is found or 0 if not*/
}

/*
Returns 1 if key is in the hashMap, 0 otherwise
 */
int containsKey(struct hashMap * ht, KeyType k)
{
	assert(ht);
	/*Hash the key*/

	int hashIndex = _getIndex(k);
	hashIndex = hashIndex%ht->tableSize;

	hashLink *temp;

	temp = ht->table[hashIndex];
	/*Loop through the pointers using ->next until either NULL is reached or the key is found*/
	for (;temp && strcmp(temp->key, k) != 0; temp = temp->next);

	return temp ? 1 : 0; /*returns 1 if k is found or 0 if not*/
}

/*
 find the hashlink for the supplied key and remove it, also freeing the memory
 for that hashlink. it is not an error to be unable to find the hashlink, if it
 cannot be found do nothing (or print a message) but do not use an assert which
 will end your program.

 Finds the hashLink for the supplied key and removes it. 
 Free the memory for that hashLink.
 If key not found, program returns without doing anything.
 */
void removeKey(struct hashMap * ht, KeyType k)
{
	struct hashLink *cur;
	struct hashLink *last;
	assert(ht);

	/*Hash the key*/
	int hashIndex = _getIndex(k);
	hashIndex = (int)hashIndex%ht->tableSize;

	if (containsKey(ht, k)) { /*If key is found in the table*/
		cur = ht->table[hashIndex];
		last = ht->table[hashIndex];

		while (cur != 0) 
		{
			if (strcmp(cur->key, k) == 0)  /*If the key has been found at this location*/
			{
				ht->count--; /*From this point on, a hashLink is guaranteed to be removed, so decrement count*/

				if (cur == ht->table[hashIndex]) /*If the hashLink we want to remove is currently in [hashIndex]*/
					ht->table[hashIndex] = cur->next;

				else
					last->next = cur->next; /*Else cur will equal last->next, so setting last->next to cur->next will remove cur from the array*/

				last = cur; 
				cur = cur->next;  /*Increment cur to the next pointer or NULL*/
				free(last);
				last = 0; 
			}
			
			else {  /*Else if the key is not found at this position in the array*/
				last = cur;
				cur = cur->next; /*Increment cur to the next pointer or NULL*/
			}
		}
	}
}


/*
 returns the number of hashLinks in the table
 */
int size (struct hashMap *ht)
{  
	return ht->count;
}

/*
 returns the number of buckets in the table
 */
int capacity(struct hashMap *ht)
{  
	return ht->tableSize;
}

/*
 returns the number of empty buckets in the table, these are buckets which have
 no hashlinks hanging off of them.
 */
int emptyBuckets(struct hashMap *ht)
{  
	return (ht->tableSize) - (ht->count); /*Not correct but also not used in main program*/
}

/*
 returns the ratio of: (number of hashlinks) / (number of buckets)
 
 this value can range anywhere from zero (an empty table) to more then 1, which
 would mean that there are more hashlinks then buckets 
 */
float tableLoad(struct hashMap *ht)
{  
	return ht->count / ht->tableSize;
}

/*Prints the hashMap*/
void printMap (struct hashMap * ht)
{
	int i;
	struct hashLink *temp;	
	
	for(i = 0;i < capacity(ht); i++){
		temp = ht->table[i];
		if (temp != 0) {
			printf("\nBucket Index %d -> ", i);

			while (temp != 0) {
				printf("Key:%s  |  Value: %i" , temp->key, temp->value);
				//printValue(temp->value);
				printf(" -> ");
				temp = temp->next;
			}
		}
	}
}


