/* main.cpp - Main file for testing the File IO functions that get random messages from a file
*  By: Michael Galle
*
*  Edited by: Selman Bursal, Alex Taylor, Robyn Porter
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>			// Needed for srand
#include <Windows.h>
#include "message.h"
#include "sound.h"
#include <string.h>
#include "RS232Comm.h"
#include "Cases.h"


// Declare constants, variables and communication parameters
const int BUFSIZE = 140;							// Buffer size
//setting up two virtual ports
wchar_t COMPORT_Rx[] = L"COM7";						// COM port used for Rx (use L"COM6" for transmit program)
wchar_t COMPORT_Tx[] = L"COM6";						// COM port used for Rx (use L"COM6" for transmit program)

// Communication variables and parameters
HANDLE hComRx;										// Pointer to the selected COM port (Receiver)
HANDLE hComTx;										// Pointer to the selected COM port (Transmitter)
int nComRate = 9600;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;								// A commtimeout struct variable




int main(int argc, char* argv[]) {
	int numQuotes;					// Number of quotes in the file
	long int* quoteIndices;			// Array of quote locations in the file (index correspondes to quote number)
	int* quoteLengths;				// Array of quote lengths (index correspondes to quote number)
	char testBuff[MAX_QUOTE_LENGTH]; // Buffer to write the message to
	int result;						 // result = 0 if successfully get a message

	int num = 3;
	int count = 1;

	link p, q;

	extern short iBigBuf[];												// buffer
	extern long  lBigBufSize;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));		// buffer used for reading recorded sound from file

	char save;
	char replay;
	char c;																// used to flush extra input
	FILE* f;



	do {
		printf("What function would you like to use? \n r to record and play an audio message \n t to send a message \n c to close program\n > ");
		scanf_s("%c", &save, 1);
		while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
		
		switch (save) {
		case 'r':
			caseR(iBigBuf, lBigBufSize, save,c, iBigBufNew); //replay,f,

			break;
			
		// assignment 1 code to get message and add to queue
		case 't':
			numQuotes = fnumQuotes();									// Number of quotes

			quoteIndices = fquoteIndices(numQuotes);					// Index locations of the quotes

			quoteLengths = fquoteLength(numQuotes, quoteIndices);		// Length of each quote (up to MAX_QUOTE_LENGTH) - cut off after (140 chars)

			// Get the random message from the file
			srand(time(NULL));					// Seed the random number generator

			InitQueue();

			printf("there are %d quotes.\n", numQuotes);

			for (int i = 0; i < num; i++) {
				p = (link)malloc(sizeof(Node));
				GetMessageFromFile(p->Data.message, MAX_QUOTE_LENGTH, frandNum(1, numQuotes), numQuotes, quoteIndices, quoteLengths);
				AddToQueue(p);
			}

			count = 1;

			while (!isQueueEmpty()) {
				printf("\n\nmessage %d: \n", count);
				q = DeQueue();
				count++;
			}

			printf("\n\n");
			break;
		case 'c':
			
			return 0;
		} 
	} while (save != 'c');



	return(0);
}
