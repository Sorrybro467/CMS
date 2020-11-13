#include <stdio.h>
#include <stdlib.h>
#include <time.h>			// Needed for srand
#include <Windows.h>
#include "message.h"
#include "sound.h"
#include <string.h>
#include "RS232Comm.h"

int Settings(int SampleRate, int RecordTime, int BitRate, int COMPort, int RxID, int TxID, int Compression) {

	char c, save1, save2;
	int save3;
	do {
		printf("would you like to edit Audio? (A)\n ");
		printf("would you like to edit Communications settigns? (C)\n ");
		printf("would you like to exit? (E)\n ");
		scanf_s("%c", &save1, 1);
		while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input

		switch (save1) {
		case 'A':
			printf("Would you like to change Recording time?(T)\n ");
			printf("would you like to change Sample rate?(R)\n ");
			printf("would you like to exit? (E)\n ");
			scanf_s("%c", &save2, 1);
			while ((c = getchar()) != '\n' && c != EOF) {}
			switch (save2) {
			case 'T':
				printf("How long would you like to record?(enter a number of seconds)\n ");
				scanf_s("%d", &save3, 5);
				RecordTime = save3;
				break;

			case 'R':
				printf("what would you like to change the SampleRate too?(enter a number)\n ");
				scanf_s("%d", &save3, 5);
				SampleRate = save3;
				break;
			case 'E':
				break;
			}
			break;
		case 'C':
			printf("Would you like to change Bitrate?(B)\n ");
			printf("Would you like to change COM PORT?(C)\n ");
			printf("Would you like to change RxID?(R)\n ");
			printf("Would you like to change TxID?(T)\n ");
			printf("Would you like to apply Compression?(P)\n ");

			scanf_s("%c", &save2, 1);
			while ((c = getchar()) != '\n' && c != EOF) {}
			switch (save2) {
			case 'B':
				printf("What would you like to change the Bitrate to?(enter a number)\n ");
				scanf_s("%d", &save3, 5);
				BitRate = save3;
				break;

			case 'C':
				printf("What would you like to change the COM PORT to?(enter a number)\n ");
				scanf_s("%d", &save3, 5);
				COMPort = save3;
				break;

			case 'R':
				printf("What would you like to change the RxID to?(enter a number)\n ");
				scanf_s("%d", &save3, 5);
				RxID = save3;
				break;

			case 'T':
				printf("What would you like to change the TxID to?(enter a number)\n ");
				scanf_s("%d", &save3, 5);
				TxID = save3;
				break;

			case 'P':
				printf("What would you like to change the Compression to?(enter a number)\n ");
				scanf_s("%d", &save3, 5);
				Compression = save3;
				break;
			}
			break;
		case 'E':
			break;
		}

	} while (save1 != 'E');
}

