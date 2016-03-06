# ifndef HashMap
# define HashMap

# define KeyType char*
# define ValueType int


# define HASHING_FUNCTION 1 /*Calls stringHash1 when defined as 1 and stringHash2 when defined as 2*/

# define LOAD_FACTOR_THRESHOLD 0.75 
/* Having a small load factor triggers the resize function more often; It was kept small on purpose*/

struct hashLink
{
   KeyType key; /*the key is used to look up a hashLink*/
   ValueType value; /*the value stored with the hashLink, an int in our case*/
   struct hashLink *next;
};

typedef struct hashLink hashLink; /*For quality of life purposes*/

struct hashMap
{
    hashLink ** table; /*array of pointers to hashLinks*/
    int tableSize; /*number of buckets in the table*/
    int count; /*number of hashLinks in the table*/
};

typedef struct hashMap hashMap;/*For quality of life purposes*/

/*the first hashing function you can use*/
int stringHash1(char * str);

/*the second hashing function you can use*/
int stringHash2(char * str);

/*All descriptions for further functions are included in the .c file*/
void initMap (struct hashMap * ht, int tableSize);

struct hashMap *createMap(int tableSize);


void freeMap (struct hashMap * ht);

void deleteMap(struct hashMap *ht);

void insertMap (struct hashMap * ht, KeyType k, ValueType v);

ValueType* atMap (struct hashMap * ht, KeyType k);

int containsKey (struct hashMap * ht, KeyType k);

void removeKey (struct hashMap * ht, KeyType k);

int size (struct hashMap *ht);

int capacity(struct hashMap *ht);

int emptyBuckets(struct hashMap *ht);

float tableLoad(struct hashMap *ht);

void printMap (struct hashMap * ht);

void printValue(ValueType v);


# endif
