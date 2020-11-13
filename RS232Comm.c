/* RS232Comm.cpp - Implementation for the RS232 communications module
 * By: Michael A. Galle
 *
 */
#include <Windows.h>    // Includes the functions for serial communication via RS232
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RS232Comm.h"

#define EX_FATAL 1

// Initializes the port and sets the communication parameters
void initPort(HANDLE* hCom, wchar_t* COMPORT, int nComRate, int nComBits, COMMTIMEOUTS timeout) {
	createPortFile(hCom, COMPORT);						// Initializes hCom to point to PORT#
	purgePort(hCom);									// Purges the COM port
	SetComParms(hCom, nComRate, nComBits, timeout);		// Uses the DCB structure to set up the COM port
	purgePort(hCom);
}

// Purge any outstanding requests on the serial port (initialize)
void purgePort(HANDLE* hCom) {
	PurgeComm(*hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
}
//PURGE_RXABORT -> Terminates all outstanding overlapped read operations and returns immediately, even if the read operations haven't finished
//PURGE_RXCLEAR -> Clears the input buffer
//PURGE_TXABORT -> Terminates all outstanding write operations and returns immediately, even if not finished
//PURGE_TXCLEAR -> Clears output buffer
//if successful returns a non-zero

// Output/Input messages to/from ports 
//WriteFileFunction
	//Writes data to COMPORT
void outputToPort(HANDLE* hCom, LPCVOID buf, DWORD szBuf) {
	int i=0;
	DWORD NumberofBytesTransmitted;
	LPDWORD lpErrors=0;
	LPCOMSTAT lpStat=0; 

	i = WriteFile(
		*hCom,										// Write handle pointing to COM port
		buf,										// Buffer size
														//Pointer to buffer containing the data to be written to file
														//buffer must remain valid and cannot be used until write operation is complete
		szBuf,										// Size of buffer
		&NumberofBytesTransmitted,					// Written number of bytes
														//pointer to the variable (szBuf) that receives the number of bytes written
		NULL										//pointer to an overlapped structure -> not needed thus NULL
	);
	// Handle the timeout error
	if (i == 0) {
		printf("\nWrite Error: 0x%x\n", GetLastError());
		ClearCommError(hCom, lpErrors, lpStat);		// Clears the device error flag to enable additional input and output operations. Retrieves information ofthe communications error.	
	}
	else
		printf("\nSuccessful transmission, there were %ld bytes transmitted\n", NumberofBytesTransmitted);
}

//ReadFileFunction
	//Reads data from COMPORTs file. Reads occur at the position specified by the file pointer
DWORD inputFromPort(HANDLE* hCom, LPVOID buf, DWORD szBuf) {
	int i = 0;
	DWORD NumberofBytesRead;						//pointer to the buffer that receives the data read from the file
														//buffer must remain valid for the duration of the read operation. DO NOT USE until read operation is complete
	LPDWORD lpErrors = 0;							//pointer to the variable (lpErrors) that receives the number of bytes read when using hCom
	LPCOMSTAT lpStat = 0;

	i = ReadFile(
		*hCom,										// Read handle pointing to COM port
		buf,										// Buffer size
		szBuf,  									// Size of buffer - Maximum number of bytes to read
		&NumberofBytesRead,
		NULL										//pointer to an overlapped structure -> not needed thus NULL
	);
	// Handle the timeout error
	if (i == 0) {
		printf("\nRead Error: 0x%x\n", GetLastError());
		ClearCommError(hCom, lpErrors, lpStat);		// Clears the device error flag to enable additional input and output operations. Retrieves information of the communications error.
	}
	else
		printf("\nSuccessful reception!, There were %ld bytes read\n", NumberofBytesRead);

	return(NumberofBytesRead);
}



// Sub functions called by above functions
/**************************************************************************************/
// Set the hCom HANDLE to point to a COM port, initialize for reading and writing, open the port and set securities
void createPortFile(HANDLE* hCom, wchar_t* COMPORT) {
	// Call the CreateFile() function 
	*hCom = CreateFile(								//hCom -> handle
		COMPORT,									// COM port number  --> If COM# is larger than 9 then use the following syntax--> "\\\\.\\COM10"
														//Creates a file, COMPORT, that'll return a handle which can be used to access the file
		GENERIC_READ | GENERIC_WRITE,				// Open for read and write
		NULL,										// No sharing allowed
														//the file cannot be shared/opened again until the handle to the file has been closed
		NULL,										// No security
														//the handle cannot be inherited by any child processors that the application may create and the file obtains a default security descriptor
		OPEN_EXISTING,								// Opens the existing com port
														//will only open the file if it exists. If the file does not exist the function fails and the last error code is set
		FILE_ATTRIBUTE_NORMAL,						// Do not set any file attributes --> Use synchronous operation
														//no attributes have been set for the file (only valid if used alone)
		NULL										// No template
														//the template file, supplies file attributes and extended attributes for the file that has been create. Not being used for this file
	);
	
	if (*hCom == INVALID_HANDLE_VALUE) {			//if function fails the return value is INVALID_HANDLE_VALUE
		printf("\nFatal Error 0x%x: Unable to open\n", GetLastError());
	}
	else {
		printf("\nCOM is now open\n");				//if function suceeds -> return value is an open handle to the file and opened for read/write
	}
}

//Function reinitializes all hardware and control settings, does NOT empty output/input queues
static int SetComParms(HANDLE* hCom, int nComRate, int nComBits, COMMTIMEOUTS timeout) {
	DCB dcb;										// Windows DCB [device control block]												
	// Clear DCB to start out clean, then get current settings
	memset(&dcb, 0, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);
	if (!GetCommState(*hCom, &dcb))					//GetCommState -> returns the current control configuration
														//hCom -> handle
														//&dcb -> pointer to DCB structure that contains the configuration information
		return(0);

	// Set our own parameters from Globals
	dcb.BaudRate = nComRate;						// Baud (bit) rate
	dcb.ByteSize = (BYTE)nComBits;					// Number of bits(8)
	dcb.Parity = 0;									// No parity	
	dcb.StopBits = ONESTOPBIT;						// One stop bit
	if (!SetCommState(*hCom, &dcb))					//Configures according to the specs in the DCB structure
		return(0);

	// Set communication timeouts (SEE COMMTIMEOUTS structure in MSDN) - want a fairly long timeout
	//for all read and write operations
	memset((void *)&timeout, 0, sizeof(timeout));
	timeout.ReadIntervalTimeout = 500;				// Maximum time allowed to elapse before arrival of next byte in milliseconds. If the interval between the arrival of any two bytes exceeds this amount, the ReadFile operation is completed and buffered data is returned
	timeout.ReadTotalTimeoutMultiplier = 1;			// The multiplier used to calculate the total time-out period for read operations in milliseconds. For each read operation this value is multiplied by the requested number of bytes to be read
	timeout.ReadTotalTimeoutConstant = 5000;		// A constant added to the calculation of the total time-out period. This constant is added to the resulting product of the ReadTotalTimeoutMultiplier and the number of bytes (above).
	SetCommTimeouts(*hCom, &timeout);				//hCom a handle to the communications device (Create file returns this handle)
	return(1);										//if function succeeds return value is non-zero, in this case it is a 1
}

