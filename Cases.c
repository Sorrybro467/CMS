#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>			// Dynamic memory allocation for arrays that store quote location and length
#include "message.h"
#include <string.h>



void Cases() {
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
		// initialize playback and recording
		InitializePlayback();
		InitializeRecording();

		// start recording
		RecordBuffer(iBigBuf, lBigBufSize);
		CloseRecording();

		// playback recording 
		printf("\nPlaying recording from buffer\n");
		PlayBuffer(iBigBuf, lBigBufSize);
		ClosePlayback();

		// save audio recording  
		printf("Would you like to save your audio recording? (y/n): ");
		scanf_s("%c", &save, 1);
		while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
		if ((save == 'y') || (save == 'Y')) {
			/* Open input file */
			fopen_s(&f, "C:\\myfiles\\recording.dat", "wb");
			if (!f) {
				printf("unable to open %s\n", "C:\\myfiles\\recording.dat");
				return 0;
			}
			printf("Writing to sound file ...\n");
			fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
			fclose(f);
		}

		// replay audio recording from file -- read and store in buffer, then use playback() to play it
		printf("Would you like to replay the saved audio recording from the file? (y/n): ");
		scanf_s("%c", &replay, 1);
		while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
		if ((replay == 'y') || (replay == 'Y')) {
			/* Open input file */
			fopen_s(&f, "C:\\myfiles\\recording.dat", "rb");
			if (!f) {
				printf("unable to open %s\n", "C:\\myfiles\\recording.dat");
				return 0;
			}
			printf("Reading from sound file ...\n");
			fread(iBigBufNew, sizeof(short), lBigBufSize, f);				// Record to new buffer iBigBufNew
			fclose(f);
		}
		InitializePlayback();
		printf("\nPlaying recording from saved file ...\n");
		PlayBuffer(iBigBufNew, lBigBufSize);
		ClosePlayback();

		printf("\n");


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

}