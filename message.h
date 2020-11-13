/* Interface: Header file for File IO for getting messages from a file randomly
*  By: Michael Galle
* 
*  Edited by: Selman Bursal, Alex Taylor, Robyn Porter
*/

#pragma once

#define MAX_QUOTE_LENGTH 140

// These functions will produce the inputs to the GetMessageFromFile() function
int frandNum(int min, int max);							// Function gets a random number between min and max (max is the number of quotes in the file)
int fnumQuotes(void);									// Function returns number of quotes in the file (only need to run once)
long int* fquoteIndices(int numQuotes);					// Function returns an array that indicates the start of every quote in the file (number of characters from the start of the file) 
int* fquoteLength(int numQuotes, long int* quoteIndices); // Function returns the smaller of the actual quote length or MAX_QUOTE_LENGTH

// Function that gets q random quote from the FortuneCookies file 
int GetMessageFromFile(char* buff, int iLen, int randNum, int numQuotes, long int* quoteIndices, int* quoteLengths);  // buff is a character array (need top pass the node element that is a character array) 

typedef struct node Node;
typedef struct item Item;
typedef Node* link;

//part 2
struct item {
	char message[140];
	short sid;					// 2 bytes
	short rid;				// 2 bytes
	int priority;					// 1 byte
	short msn;						//2 bytes for message sequence number
	char later[25];

};

struct node {
	link pNext;
	Item Data;
};

void InitQueue(void);				// initialises FIFO queue
int isQueueEmpty(void);				// checks to see if the queue is empty
void AddToQueue(link pn);	// adds a quote struct to the tail of the queue	
link DeQueue(void);		// pops the head of the queue off


