// dllConsoleTest.cpp : Defines the entry point for the console application.
//


#include "stdafx.h" 
#include <windows.h> 
#include <stdio.h> 
#include <string.h>


// Define dll file names
#define	DLL_FILE_NAME		TEXT("SII_LTP9000_API.DLL")	

// Sample commands
#define	HEADER_CMD		{0x12,'q', 0x00}
#define	FOOTER_CMD		{0x12,'q', 0x0F}
#define	SAMPLE_CMD		{0x12,'r'}

// Define string commands for GetSiiPrinterData()
static LPCTSTR TBL_CMD[] =			
{
	TEXT("AUTO_STATUS_BACK"),
	TEXT("FUNCTION_SET_RESP"),
	TEXT("REMAIN_MEMORY_CAP"),
	TEXT("STATUS_DATA_SEND_PAPER"),
	TEXT("STATUS_DATA_SEND_PRESENTER"),
	TEXT("PRINTER_ID_SEND_DEV"),
	TEXT("PRINTER_ID_SEND_TYPE"),
	TEXT("PRINTER_ID_SEND_ROM"),
	TEXT("CG_INFO_RESP"),
	TEXT("GET_RAW_DATA_AUTO_STATUS"),
	TEXT("GET_RAW_DATA_EXEC_RESP"),
	TEXT("GET_RAW_DATA_CMD_RESP"),
	TEXT("")
};
 

/*	Define finction type		*/
typedef DWORD (*tProcOpenPrinter) (LPCTSTR pszName,LPDWORD pdwSessionId);
typedef DWORD (*tProcOpenPrinterData) (DWORD dwSessionId);
typedef DWORD (*tProcSetPrinterData) (DWORD,LPBYTE,DWORD,LPDWORD);
typedef DWORD (*tProcClosePrinter) (DWORD dwSessionId);
typedef DWORD (*tProcSetPrinterAutoStatus) (DWORD dwSessionId,LPDWORD pdwStatus);

BYTE	pHeader[]	= HEADER_CMD;
BYTE	pFooter[]	= FOOTER_CMD;
BYTE	pSampCmd[]	= SAMPLE_CMD;
BYTE	pBuf[]		= "Hello\n";
DWORD	cbWritten	= 0;
DWORD	dwSessionId = 0;	// Session ID for dll's function	
TCHAR pMsg[1024];
TCHAR	lpPrinterName[]= TEXT("default");
 
HINSTANCE hinstLib; 
tProcOpenPrinter ProcOpenPrinter;  
tProcOpenPrinterData ProcOpenPrinterData; 
tProcClosePrinter ProcClosePrinter;
tProcSetPrinterData ProcSetPrinterData; 
tProcSetPrinterAutoStatus ProcSetPrinterAutoStatus; 

int main( void ) 
{ 
   
    BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 


    // Get a handle to the DLL module.
 
    hinstLib = LoadLibrary(DLL_FILE_NAME); 
 
    // If the handle is valid, try to get the function address.
 
    if (hinstLib != NULL) 
    { 
		
		//ProcSetPrinterReset = (MYPROC) GetProcAddress(hinstLib,"SetSiiPrinterReset");
		
		//ProcSetPrinterCallBackStatus = (MYPROC) GetProcAddress(hinstLib,"SetSiiPrinterCallbackStatus");
		//ProcSetPrinterTimeout = (MYPROC) GetProcAddress(hinstLib,"SetSiiPrinterTimeout");
        // If the function address is valid, call the function.
		ProcOpenPrinter = (tProcOpenPrinter) GetProcAddress(hinstLib, "OpenSiiPrinterA");
		ProcOpenPrinterData = (tProcOpenPrinterData) GetProcAddress(hinstLib, "GetSiiPrinterDataW");
		ProcClosePrinter = (tProcClosePrinter) GetProcAddress(hinstLib,"CloseSiiPrinter" );
		ProcSetPrinterData = (tProcSetPrinterData) GetProcAddress(hinstLib,	"SetSiiPrinterData");
		ProcSetPrinterAutoStatus = (tProcSetPrinterAutoStatus) GetProcAddress(hinstLib,	"GetSiiPrinterAutoStatus");

        if ((NULL != ProcOpenPrinter) &&
			(NULL != ProcOpenPrinterData) &&
			(NULL != ProcClosePrinter) &&
			(NULL != ProcSetPrinterData) &&
			(NULL != ProcSetPrinterAutoStatus)
			)
        {
             printf("DLL loaded\n"); 
			fRunTimeLinkSuccess = TRUE;   		
        
			ProcOpenPrinter(lpPrinterName,&dwSessionId); 
			printf("%s printer open",lpPrinterName);
			

		
			ProcSetPrinterData( dwSessionId, pHeader, sizeof(pHeader), &cbWritten );
			ProcSetPrinterData( dwSessionId, pBuf, sizeof(pBuf), &cbWritten );		// Sample Print Data	
			ProcSetPrinterData( dwSessionId, pFooter, sizeof(pFooter), &cbWritten );	// For checking "GET_RAW_DATA_EXEC_RESP"
			ProcSetPrinterData( dwSessionId, pSampCmd, sizeof(pSampCmd), &cbWritten );// For checking "GET_RAW_DATA_CMD_RESP"
			 printf("Printing\n"); 


			ProcClosePrinter(dwSessionId);
		
			printf("Printer closed\n"); 
		
		
		}
        // Free the DLL module.
 
        fFreeResult = FreeLibrary(hinstLib); 
    } 

    // If unable to call the DLL function, use an alternative.
    if (! fRunTimeLinkSuccess) 
        printf("Message printed from executable\n"); 

    return 0;

}



