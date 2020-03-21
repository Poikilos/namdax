#ifndef ERRLOG_CPP
#define ERRLOG_CPP
#include <fstream>
#include <iostream>
#include <ostream>
#include <cstdio>
#include <windows.h>
#include <ctime>
//#include "basefunc.cpp"

using namespace std;
//namespace dxmanse {
void strcatfromsiverrno(char *, int);
void strcpyfromsiverrno(char *, int);
void strcatfromdatetime(char *, char *, char *);
void strcpyfromdatetime(char *, char *, char *);

//GLOBALS
#ifdef RBE_DEBUG
	#define bDebugMode true
#else
	#define bDebugMode false
#endif

ofstream error_txt("debug.txt");
FILETIME fileTime1;
ULARGE_INTEGER uLargeIntegerTime1;
BYTE byEndl[2]={0x0D,0x0A};// 13=cr, 10=lf (see winuser.h vk_return)
char sEndl[3]={0x0D,0x0A,'\0'};
int* iErrorLevel=NULL,
	iErrors=NULL,
	iErrNow=NULL,
	iErrorNow=NULL,
	iErr=NULL,
	sErrorNow=NULL,
	sErrorLog=NULL,
	sTitlePrev=NULL;//why these here? So compiler gives errors if hardcoded error logs are defined elsewhere

//#include "BASEFUNC.CPP"
//Defines errlog, a class ErrEng object.
//ErrEng::Add
//-It is a replacement function for MessageBox.
//-It takes the exact parameters of MessageBox, and uses the title as a heading.
//-It's overload can instead take a SIV Error Number and title (i.e. a function name)

#define MAXLOG 32768
#define MAXTITLE 256

//ASSUMES 2048:
#ifndef STEMPLEN
#define STEMPLEN 2048
#endif
extern char sTemp[STEMPLEN];
extern void strcatfromint(char *string, int i);
//CLASS
class ErrEng
{
private:
	int iErrors;
	int iErrorLevel;
	char sErrorsNow[MAXLOG]; //local errors
	char sErrorLog[MAXLOG]; //all errors
	char sTitlePrev[MAXTITLE];
	char sDateStarted[STEMPLEN];//date of program start (of constructor call)
	HWND hWnd;
	void WriteLog();
public:
	ErrEng();
	~ErrEng();//dumps logfile if iErrorLevel
	void Dump();//Call after MAJOR SECTIONS
	void Add(HWND,char*,char*,DWORD);//call after every RBE function
	void Add(int,char*,char*);//alternative call after every RBE function
	void SetMessageDest(HWND);
};
void ErrEng::WriteLog()
{
	/*
	int iChar=0;
	int iDest=0;
	static char sLogTemp[MAXLOG];
	int iLen=strlen(sErrorLog);
	while (sErrorLog[iChar]!='\0')
	{
		sLogTemp[iDest]=sErrorLog[iChar];
		if (sErrorLog[iChar]==sEndl[0] && sErrorLog[iChar+1]!=sEndl[1])
		{//if no linefeed, add one
			iDest++;
			sLogTemp[iDest]=sEndl[1];
		}

		iDest++;
	}
	strcpy(sLogTemp, sErrorLog);
	*/

	ofstream error_log;
	if (bDebugMode==false)
	{
		strcpy(sTemp, sDateStarted);
		strcat(sTemp, " Error Log.txt");
		error_log.open(sTemp);
	}

	if (bDebugMode) error_txt<<sErrorLog;
	else error_log<<sErrorLog;
	
	/*
	while(sErrorLog[iChar]!='\0')
	{
		if (sErrorLog[iChar]!=sEndl[0] && sErrorLog[iChar]!=sEndl[1]) sTemp[iDest]=sErrorLog[iChar];
		if (sErrorLog[iChar]==sEndl[0])
		{
			iDest++;
			sTemp[iDest]='\0';
			if (bDebugMode) error_txt<<sTemp<<sEndl;
			else error_log<<sTemp<<sEndl;
			iDest=0;
		}
		else
		{
			iDest++;
		}
		iChar++;
	}
	*/
		
	if (bDebugMode==false) error_log.close();
	memset(sErrorLog,0,sizeof(char) * MAXLOG);
	memset(sErrorsNow,0,sizeof(char) * MAXLOG);
	
	return;
}
ErrEng::ErrEng()
{
	iErrors=0;
	iErrorLevel=0;
	memset(sErrorsNow,0,sizeof(char) * MAXLOG);
	memset(sErrorLog,0,sizeof(char) * MAXLOG);
	memset(sTitlePrev,0,sizeof(char) * MAXTITLE);
	hWnd=NULL;
	memset(sDateStarted, 0, STEMPLEN*sizeof(char));
	strcatfromdatetime(sDateStarted, "-", ".");
}
ErrEng::~ErrEng()
{
	if (iErrorLevel)
	{
		Dump();
	}
	if (iErrors)
	{
	WriteLog(); //opens and closes the local error log
	}
	error_txt.close(); //closes the global debug log
}
void ErrEng::Dump(void) //dumps current strings to log string.
{
	if (iErrorLevel)
	{
		if (hWnd && (bDebugMode==false)) MessageBox(hWnd, sErrorsNow,"Error Log will save on exit",MB_OK | MB_ICONEXCLAMATION);
		iErrors+=iErrorLevel;
		strcat(sErrorLog, sErrorsNow);
	}
	iErrorLevel=0;
	memset(sErrorsNow,0,sizeof(char) * MAXLOG);
	return;
}
void ErrEng::Add(HWND hWnd1,char *sContent,char *sTitleOrNullForPrevious,DWORD dw)
{
	if (hWnd1!=NULL) hWnd=hWnd1;
	if ( sTitleOrNullForPrevious!=NULL
		&& strcmp(sTitleOrNullForPrevious, sTitlePrev)!=0 )
	{
		strcat(sErrorsNow, sEndl);
		strcat(sErrorsNow, sTitleOrNullForPrevious);
	}
	
	strcat(sErrorsNow, sEndl);
	strcat(sErrorsNow, "-");
	strcat(sErrorsNow, sContent);
	iErrorLevel++;
	if (sTitleOrNullForPrevious!=NULL) strcpy(sTitlePrev, sTitleOrNullForPrevious);
	if (bDebugMode)
	{
		Dump(); 
		WriteLog();
	}
	return;
}
void ErrEng::Add(int iSivErr, char* sContent, char *sTitleOrNullForPrevious)
{
	if (iSivErr || bDebugMode)
	{
		if (sTitleOrNullForPrevious!=NULL)
		{
			if (strcmp(sTitleOrNullForPrevious, sTitlePrev)!=0)
			{
				strcat(sErrorsNow, sEndl);
				strcat(sErrorsNow, sTitleOrNullForPrevious);
			}
		}
		strcat(sErrorsNow, sEndl);
	if (sContent) strcat(sErrorsNow, "-");
	if (sContent) strcat(sErrorsNow, sContent);
		strcat(sErrorsNow, "...");
		strcatfromsiverrno(sErrorsNow, iSivErr);
		iErrorLevel++;
		if (sTitleOrNullForPrevious!=NULL) strcpy(sTitlePrev, sTitleOrNullForPrevious);
	}
	if (bDebugMode)
	{
		Dump();
		WriteLog();
	}
	return;
}
void ErrEng::SetMessageDest(HWND hwndOrNULL)
{
	hWnd=hwndOrNULL;
}


ErrEng errlog;

////////////////////////////// FUNCTIONS ////////////////////

void strcatfromsiverrno(char *sErr, int iNum)//[])
{
	//0-99 - general
	//100's - Memory
	//200's - DirectX
	//300's - File
	//400's - Data (also used when checking data at file load time)
	//500's - DirectX, 550-599 are fatal
	if (sErr==NULL)// sErr=(char*)malloc(255*sizeof(char));
	{
		errlog.Add(NULL,"sErr==NULL!","strcatfromsiverrno",0);
	return;//debug
	}
	if (iNum)
	{
		strcat(sErr, "Fail, error #");
		strcatfromint(sErr, iNum);
		strcat(sErr, " ");
	}
	if (iNum==0) strcat(sErr,"Success.");
	else if (iNum==1) strcat(sErr,"General Siv error");
	else if (iNum==2) strcat(sErr,"Dimensions are too big (greater than 1048576 pixels) for SIV");
	else if (iNum==3) strcat(sErr,"Too many blocks!	May need to increase compression or decrease image size.");
	else if (iNum==4) strcat(sErr,"Image dimensions must both be divisible by 4 for this SIV method.");
	else if (iNum==5) strcat(sErr,"User cancelled compression.");
	else if (iNum==6) strcat(sErr,"Fast copy cannot transfer dissimilar bitdepths");
	else if (iNum==7) strcat(sErr,"Null siv pointer.");
	else if (iNum==8) strcat(sErr,"SIV_LOADED flag is off.");
	else if (iNum==9) strcat(sErr,"Null targa pointer.");
	else if (iNum==10) strcat(sErr,"Function not implemented.");
	else if (iNum==11) strcat(sErr,"Siv header is null!");
	else if (iNum==12) strcat(sErr,"Wrong function chosen for this SIV's method");
	else if (iNum==13) strcat(sErr,"Null sivcaps pointer");

	else if (iNum==20) strcat(sErr,"This version of the decompressor cannot load this type of Siv, or the Siv header is corrupt.");

	else if (iNum==40) strcat(sErr,"This version of the compressor cannot encode this type of pixel");
	else if (iNum==41) strcat(sErr,"This version of the compressor cannot encode use this set of Siv options.");
	else if (iNum==42) strcat(sErr,"This version of the compressor cannot encode this Siv method.");
	else if (iNum==43) strcat(sErr,"To many pattern instances.	Image must be too large.");
	else if (iNum==50) strcat(sErr,"Siv is already unloaded.");
	else if (iNum==51) strcat(sErr,"Targa is already unloaded.");

	else if (iNum==60) strcat(sErr,"alphablender is full");

	//Memory errors
	else if (iNum==100) strcat(sErr,"General error allocating memory");
	else if (iNum==101) strcat(sErr,"Couldn't create Siv block pixel buffer");
	else if (iNum==102) strcat(sErr,"Couldn't create block flags buffer");
	else if (iNum==103) strcat(sErr,"Couldn't create Siv map buffer");
	else if (iNum==104) strcat(sErr,"Couldn't create temp block pixel buffer");
	else if (iNum==105) strcat(sErr,"Couldn't create temp map buffer");
	else if (iNum==106) strcat(sErr,"Couldn't allocate Siv header");
	else if (iNum==107) strcat(sErr,"Couldn't allocate targa buffer");
	else if (iNum==108) strcat(sErr,"Couldn't create this type of targa -- try uncompressed non-zero-length image");
	else if (iNum==109) strcat(sErr,"Couldn't create targa with odd bit depth -- try 8/16/24/32");
	else if (iNum==110) strcat(sErr,"General error allocating Targa memory");
	else if (iNum==111) strcat(sErr,"Intermediate targa bitdepth error");
	//memory read errors:
	else if (iNum==150) strcat(sErr,"General typesafe error (bad data)");
	else if (iNum==151) strcat(sErr,"Uncompress buffer is NULL");
	//Siv DirectX pattern surface - see 500's for regular directX surface
	else if (iNum==200) strcat(sErr,"General error in Siv DirectX surface pattern buffer");
	//File
		//Save
	else if (iNum==300) strcat(sErr,"General error saving Siv file");
	else if (iNum==301) strcat(sErr,"Error saving Siv header");
	else if (iNum==302) strcat(sErr,"Error saving Siv integer buffer");
	else if (iNum==303) strcat(sErr,"Error saving Siv map buffer");
	else if (iNum==304) strcat(sErr,"Error saving Siv block pixel buffer");
	else if (iNum==305) strcat(sErr,"Error saving Siv pattern buffer");
	else if (iNum==306) strcat(sErr,"Error saving Siv because SivType (color depth etc.) is not supported");
	else if (iNum==307) strcat(sErr,"Error saving Targa, corrupt data or disk error");
		//Load
	else if (iNum==310) strcat(sErr,"General error loading Siv file");
	else if (iNum==311) strcat(sErr,"File not found");
	else if (iNum==312) strcat(sErr,"Not all data from Siv file was used, check header and integer data");
	else if (iNum==313) strcat(sErr,"Siv couldn't load source targa file");
	else if (iNum==314) strcat(sErr,"Siv couldn't load source targa into non-null buffer");
	else if (iNum==315) strcat(sErr,"Siv couldn't load source targa because it couldn't allocate buffer memory");
	else if (iNum==316) strcat(sErr,"Siv couldn't load source targa into null pointer");
	else if (iNum==317) strcat(sErr,"Siv file has unknown bit depth or siv type");
	else if (iNum==318) strcat(sErr,"Siv file is too big and probably corrupt!");
	else if (iNum==319) strcat(sErr,"Loading source targa file to null pointer is impossible");
	//Data
		//General
	else if (iNum==400) strcat(sErr,"General Siv data error (Siv appears to have bad data)");
	else if (iNum==401) strcat(sErr,"Siv header appears to be bad");
	else if (iNum==402) strcat(sErr,"Siv file is missing or incomplete (read less than size of header)");
	else if (iNum==403) strcat(sErr,"Siv header's specified bit depth is not usable");
	else if (iNum==404) strcat(sErr,"Siv header's bit depth is a negative number");
	else if (iNum==405) strcat(sErr,"Siv header says Siv is not loaded!");
	else if (iNum==406) strcat(sErr,"Siv cannot draw source targa to surface.");
	else if (iNum==450) strcat(sErr,"General TargaFX error");
	else if (iNum==451) strcat(sErr,"Couldn't interpret compressed targa");
		//integer buffer
	else if (iNum==410) strcat(sErr,"Data in Siv integer buffer is bad");
	else if (iNum==411) strcat(sErr,"Siv Integer buffer doesn't exist");
	else if (iNum==412) strcat(sErr,"Data in Siv integer buffer is incomplete");
	else if (iNum==413) strcat(sErr,"Data in Siv integer buffer is beyond normal range");
	else if (iNum==414) strcat(sErr,"Data in Siv integer buffer is bad since negative");
	else if (iNum==415) strcat(sErr,"Data in Siv integer buffer is bad since map size is zero");
	else if (iNum==416) strcat(sErr,"Data in Siv integer buffer is bad since zero");
	else if (iNum==417) strcat(sErr,"Values in Siv integer buffer are incongruous");
		//map buffer
	else if (iNum==420) strcat(sErr,"Data in Siv map is bad");
	else if (iNum==421) strcat(sErr,"Siv map doesn't exist");
	else if (iNum==422) strcat(sErr,"Data in Siv map is incomplete");
	else if (iNum==423) strcat(sErr,"Data in Siv map is beyond range");
	else if (iNum==424) strcat(sErr,"Data in Siv map is negative");
	else if (iNum==425) strcat(sErr,"Siv map contains a misplaced zero");
		//block buffer
	else if (iNum==430) strcat(sErr,"Data in Siv block pixel buffer is bad");
	else if (iNum==431) strcat(sErr,"Siv block pixel buffer doesn't exist");
	else if (iNum==432) strcat(sErr,"Data in Siv block pixel buffer is incomplete");
	else if (iNum==433) strcat(sErr,"Siv block pixel buffer isn't correct size");
	else if (iNum==434) strcat(sErr,"Siv block pixel buffer contains a zero block");
	else if (iNum==435) strcat(sErr,"Siv block pixel buffer contains multiple zero blocks");
		//animation
	else if (iNum==440) strcat(sErr,"Siv animation data error");
	else if (iNum==441) strcat(sErr,"(User canceled the animation/Siv)");
	//DirectX
	else if (iNum==500) strcat(sErr,"General Siv DirectX Error");
	else if (iNum==502) strcat(sErr,"Siv couldn't release DirectDraw Lock");
	else if (iNum==503) strcat(sErr,"Siv couldn't set DirectX cooperative level");
	else if (iNum==504) strcat(sErr,"Siv couldn't set DirectX display mode");
	else if (iNum==505) strcat(sErr,"Siv couldn't lock DirectX surface");
	else if (iNum==506) strcat(sErr,"Siv couldn't unlock DirectX surface");
		//directX fatal errors
	else if (iNum==550) strcat(sErr,"Serious Siv DirectX Error!");
	else if (iNum==551) strcat(sErr,"Siv couldn't create DirectX surface (low on memory?)!");
	else if (iNum==552) strcat(sErr,"Siv couldn't find directX 7 or higher on your system!");
	else if (iNum==553) strcat(sErr,"Siv couldn't attach backbuffer!");
	else //Unknown
		strcat(sErr,"No Description");
	return;
}//end strcatfromsiverrno


void strcpyfromsiverrno(char *s, int i)
{
	if (s==NULL)// sErr=(char*)malloc(255*sizeof(char));
	{
		errlog.Add(NULL,"s==NULL!","strcatfromsiverrno",0);
	}
	else
	{
		s[0]='\0';	
		strcatfromsiverrno(s, i);
	}
	return;
}//end strcpyfromsiverrno


void strcatfromdatetime(char *s, char *sDateDelimiter, char *sTimeDelimiter)
{//i.e. strcatfromdate(s,&st,"/",":")
	bool bFatal=false;
	if (s==NULL)
	{
	errlog.Add(NULL,"s==NULL!","strcatfromdatetime",0);
		bFatal=true;
	}
	if (sDateDelimiter==NULL)
	{
	errlog.Add(NULL,"sDateDelimiter==NULL!","strcatfromdatetime",0);
	bFatal=true;
	}
	if (sTimeDelimiter==NULL)
	{
	errlog.Add(NULL,"sTimeDelimiter==NULL!","strcatfromdatetime",0);
	bFatal=true;
	}
	if (bFatal) return;

	SYSTEMTIME systemTime1;
	GetLocalTime( &systemTime1 );
	strcatfromint(s, systemTime1.wYear);
	strcat(s, sDateDelimiter);
	if (systemTime1.wMonth<10) strcat(s, "0");
	strcatfromint(s, systemTime1.wMonth);
	strcat(s, sDateDelimiter);
	if (systemTime1.wDay<10) strcat(s, "0");
	strcatfromint(s, systemTime1.wDay);
	strcat(s, " ");
	if (systemTime1.wHour<10) strcat(s, "0");
	strcatfromint(s, systemTime1.wHour);
	strcat(s, sTimeDelimiter);
	if (systemTime1.wMinute<10) strcat(s, "0");
	strcatfromint(s, systemTime1.wMinute);
	strcat(s, sTimeDelimiter);
	if (systemTime1.wSecond<10) strcat(s, "0");
	strcatfromint(s, systemTime1.wSecond);
	return;
}//end strcatfromdatetime


void strcpyfromdatetime(char *s, char *sDateDelimiter, char *sTimeDelimiter)
{
	s[0]='\0';
	strcatfromdatetime(s, sDateDelimiter, sTimeDelimiter);
	return;
}//end strcpyfromdatetime
//}//end namespace
#endif
