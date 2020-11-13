/* Implementation: Functions for File IO - Getting random messages from a file
*  By: Michael Galle
* 
* Edited by: Selman Bursal, Alex Taylor, Robyn Porter
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>			// Dynamic memory allocation for arrays that store quote location and length
#include "message.h"
#include <string.h>

#define MAXCHAR 250

FILE* fp;
char str[MAXCHAR];

char newq[] = "%%\n";
//char filename = "FortuneCookies.txt";

//part 2

static link pHead, pTail;

// Function returns number of quotes in the file (only need to run once)
int fnumQuotes() {
	int howmany = 0; 
	

	fp = fopen("FortuneCookies.txt", "r");
	if (fp == NULL) {
		printf("Could not open file %s", "FortuneCookies.txt");
		return 1;
	}
	while (fgets(str, MAXCHAR, fp) != NULL) {

		if (strcmp(str, newq) == 0) { //strcmp compares strings and if all char in strings = 0 then they are the exact same
			howmany++;
		}
	}
	fclose(fp);
    return howmany; 
}


// Function gets a random number between min and max (max is the number of quotes in the file)
int frandNum(int min, int max) {
	return rand() % max + min; //sets range between min and max
}



// Function returns an array that indicates the start of every quote in the file (number of characters from the start of the file) 
long int* fquoteIndices(int numQuotes) {
	long int* indexofq = (long int*)malloc(sizeof(long int)*numQuotes);
	
	int i = 0; //index
	long int numofchar = 0;

	fp = fopen("FortuneCookies.txt", "r");
	if (fp == NULL) {
		printf("Could not open file %s", "FortuneCookies.txt");
		return NULL;
	}
	
	while (fgets(str, MAXCHAR, fp) != NULL) {
		
		numofchar += strlen(str);

		if (strcmp(str, newq) == 0) { //strcmp compares strings and if all char in strings = 0 then they are the exact same
			//printf("Quote %d is %d chars away from the start \n",i, numofchar);
			
			indexofq[i] = numofchar;
			i++;
		}
		//printf("%d, %d \n",i, numofchar);
	}
	indexofq[i] = numofchar; // the closing postion of the last qoute
	fclose(fp);
	return indexofq;
	
}

// Function returns the smaller of the actual quote length or MAX_QUOTE_LENGTH
int* fquoteLength(int numQuotes, long int* quoteIndices) {
	long int* quotelen = (long int*)malloc(sizeof(long int) * numQuotes);
	int i = 0;		//index 
	
	while (i < numQuotes) {
		quotelen[i] = (quoteIndices[i + 1] - quoteIndices[i])-4; //taking away 3 for the %%\n and 1 for the \n after author's name 
		if (quotelen[i] <= MAX_QUOTE_LENGTH) {
			//printf("the quote lenght is smaller then %d, the actual lenght is: %d \n", MAX_QUOTE_LENGTH, quotelen);
		}
		else if (quotelen > MAX_QUOTE_LENGTH) {
			//printf("the quote lenght is larger then %d, the  lenght has been change to %d, the actaul length WAS: %d \n", MAX_QUOTE_LENGTH, MAX_QUOTE_LENGTH, quotelen);
			quotelen[i] = MAX_QUOTE_LENGTH;
		}
		i++;
	}
	return quotelen;
} 

// Function that gets q random quote from the FortuneCookies file 
int GetMessageFromFile(char* buff, int iLen, int randNum, int numQuotes, long int* quoteIndices, int* quoteLengths) {
	int howmanycharsin=0;
	int i = 0;
	
	fp = fopen("FortuneCookies.txt", "r");
	if (fp == NULL) {
		printf("Could not open file %s", "FortuneCookies.txt");
		return NULL;
	}


	while (fgets(str, MAXCHAR, fp) != NULL) {
		if (howmanycharsin < quoteIndices[randNum]) {
		howmanycharsin += strlen(str);
		} 
		else if (howmanycharsin < (quoteIndices[randNum+1]-4)) {
			for (int j = 0; j < strlen(str);j++) {
				if (i <= quoteLengths[randNum]) {		// if reached the length of the quote or max length allowed (140 characters)
					buff[i] = str[j];			//write the line to the message in the struct
					i++;
				} else {
					buff[i] = '\0';
					//printf("\n\nmessage grabbed: \n");
					//printf("%s", buff);
					fclose(fp);
					return 0;
				}
			}
		}
	}
	fclose(fp);
	return 1; 
}


// part 2

void InitQueue(void) {						// no nodes, everything links to NULL
	pHead = pTail = NULL;
}

int isQueueEmpty(void) {				
	return(pHead == NULL);
}

void AddToQueue(link pn) {			// add node to the end of the queue
	if (pHead == NULL) {					// if nothing in queue
		pHead = pTail = pn;
	}
	else {									// else, queue already has a node(s)
		pTail->pNext = pn;
		pTail = pn;
	}
	pn->pNext = NULL;
}

link DeQueue(void) {				// remove head from queue
	link pTemp;					
	
	if (pHead == NULL) return(NULL);
	
	pTemp = pHead;
	pHead = pHead->pNext;
	printf("\n %s", pTemp->Data.message);

	return (pTemp);
}
