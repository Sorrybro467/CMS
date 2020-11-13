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


char c, save;

int SampleRate=6, RecordTime=5;
int BitRate=9600, COMPort=1;
int RxID=100, TxID=100, Compression=0;

int main(int argc, char* argv[]) {

	// will be having a text file to read from to know settings from past uses but for now having set them up for every boot 
	//but also setting up default settings 

	printf("the SampleRate is: %d KHz, the RecordTime is: %d secs\n", SampleRate, RecordTime);
	printf("the Bitrate is: %d, the Comports are: %d\n", BitRate, COMPort);
	printf("the RxID is: 0x%d, the TxID is: 0x%d, the Compression is: %d\n", RxID, TxID, Compression);

	printf("would you like to run audio/random messges (r) or rs232 (c) or change settings?(s)\n");
	scanf_s("%c", &save, 1);


	while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input

	if (save == 'r') {
		Messages();
	}
	if (save=='c') {
		mainrs232();
	}
	if (save=='s') {
	Settings(SampleRate, RecordTime, BitRate, COMPort,RxID, TxID, Compression);
	}

	return(0);
}
