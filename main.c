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

int main(int argc, char* argv[]) {
	printf("would you like to test audio/random messges(s) or rs232 (c)\n");
	scanf_s("%c", &save, 1);


	while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input

	if (save == 's') {
		Messages();

	}
	if (save=='c') {
		mainrs232();


	}

	return(0);
}
