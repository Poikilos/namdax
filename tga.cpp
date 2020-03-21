#ifndef TGA_CPP
#define TGA_CPP
#include <ios>
#include <ostream>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <memory>
#include <ios>
#include <iostream>
#include <iomanip>
#include <fstream>
//#include "basefunc.cpp"
//#include "errlog.cpp"
using namespace std;
extern int iLastErr;
extern bool IsLikeMask(char *, char*);
extern ofstream error_txt;
extern void trunc(char*, int);
extern int IntOfString(char*);
extern char sDigit[10][2];
//namespace dxmanse {

//Sequential values:
//	U=uncompressed C=Compressed
//	CM=Colormapped TC=Truecolor BW=greyscale
#define TGATYPE_NOIMAGEDATA		 0
#define TGATYPE_UCM						 1
#define TGATYPE_UTC						 2
#define TGATYPE_UBW						 3
#define TGATYPE_CCM						 9
#define TGATYPE_CTC						 10
#define TGATYPE_CBW						 11
#define TGA_MAPTYPE_256		1
//Bit values:
#define TGADESC_ALPHA8		8
//extern int iLastErr;
//extern ofstream error_txt;

// container structure for targa .TGA file
typedef struct TARGAINFOFOOTER_STRUCT
{
	BYTE *dump;
	DWORD dwSizeofDump;
} TARGAINFOFOOTER, *LPTARGAINFOFOOTER;

typedef struct TARGA_STRUCT
{
	// Begin header fields in order of writing //
	BYTE bySizeofID;
	BYTE byMapType;
	BYTE byTgaType;
	USHORT nMapOrigin;
	USHORT nMapLength;
	BYTE byMapBitDepth;
	USHORT xImageOrigin;
	USHORT yImageOrigin;
	USHORT width;
	USHORT height;
	BYTE byBitDepth;
	BYTE byDescriptor;//(default zero)
	BYTE *byID;//[bySizeofID] -- custom non-terminated string
	BYTE *byColorMap;//[byMapBitDepth*nMapLength] -- the palette
	// End header fields //
	BYTE *buffer;
	TARGAINFOFOOTER footer;
} TARGA, *LPTARGA;
//TGA functions
//Credits:
// -by J a k e	G u s t a f s o n
//////////////////////////////////////////////////////////////////////

LPTARGA TargaNew(int nWidth1, int nHeight1, int bpp, BYTE byType1)
{//See header for byType1 values (TGATYPE_...).
	iLastErr=0;
	LPTARGA lptarga=(LPTARGA)malloc(sizeof(TARGA));
	memset(lptarga, 0, sizeof(TARGA));
	int iByteDepth = bpp/8;//>>3;
	if (lptarga==NULL)
	{
		iLastErr=100;//general error allocating memory
		return(NULL);
	}
	if (byType1>=4 || byType1==0)
	{
		iLastErr=(108);//assume can't create compressed targa or nodata (0) targa.
		return(NULL);
	}
	if (bpp%8 || bpp>32 || bpp==0)
	{
		iLastErr=(109);//odd bit depth
		return(NULL);
	}

	if (nWidth1 * nHeight1 * iByteDepth > 0)
	{
		if (!(lptarga->buffer = (BYTE*)malloc(iByteDepth*(int)nWidth1*(int)nHeight1))) //if (!(lptarga->buffer = (BYTE *)malloc(640*480*iByteDepth)))
		{
			iLastErr=107;
			return(NULL); //return error
		}
		memset(lptarga->buffer,0,int(iByteDepth*nWidth1*nHeight1));
	}
	else
	{
		iLastErr=108;//can't create zero-length image
		return(NULL);
	}

	//Colormap variables fixed later if 8-bit
	lptarga->bySizeofID=32;//Set the ID length for "Created by an Axle Media product" (non-terminated BYTE array)
	lptarga->byMapType=0;
	lptarga->byTgaType=TGATYPE_UTC;//Uncompressed Truecolor, fixed later if 8-bit
	lptarga->nMapOrigin = 0;
	lptarga->nMapLength = 0;
	lptarga->byMapBitDepth=0;
	lptarga->xImageOrigin = 0;
	lptarga->yImageOrigin = 0;
	lptarga->width = nWidth1;
	lptarga->height = nHeight1;
	lptarga->byBitDepth = bpp;
	lptarga->byDescriptor=0; //byDescriptor&8 if alpha, and maybe other obscure options.

	//Set the ID to "Created by an Axle Media product"(assume 32-character non-terminated string)
	BYTE byIDTemp[32]={'C','r','e','a','t','e','d',' ','b','y',' ','a','n',' ','A','x','l','e',' ','M','e','d','i','a',' ','p','r','o','d','u','c','t'};
	lptarga->byID=NULL;
	lptarga->byID=(BYTE*)malloc(32);
	if (lptarga->byID) for (int i=0; i<32; i++) lptarga->byID[i] = byIDTemp[i];
	else
	{
		iLastErr=100;//couldn't allocate
		return(NULL);
	}

	lptarga->byColorMap=NULL;

	if (bpp==32)
	{
		lptarga->byDescriptor = TGADESC_ALPHA8;
	}
	else if (bpp==24)
	{
	}
	else //assume(bpp==8)
	{
		lptarga->byTgaType = byType1;
		if (byType1==TGATYPE_UCM) //Uncompressed ColorMapped
		{
			lptarga->byMapType=TGA_MAPTYPE_256;
			lptarga->nMapLength=256;//assume 256 palette, others are allowed too.
			lptarga->byMapBitDepth=24;//assume palette 24-bit, others are allowed too.
			lptarga->byColorMap=(BYTE*)malloc(lptarga->byMapBitDepth/8 * lptarga->nMapLength);
		}
		else//assume TGATYPE_UBW
		{
			//colormap variables are already reset to zero.
		}
	}
	return(lptarga);
} // end TargaNew

///////////////////////////////////////////////////////////
TARGA** TargaNewSeq(int nWidth1, int nHeight1, int bpp, BYTE byType1, int iFrames)
{
	iLastErr=0;
	TARGA **lplptarga;
	lplptarga=(TARGA **)malloc(iFrames * sizeof(LPTARGA));
	if (lplptarga==NULL)
	{
		iLastErr=100;
	return(NULL);
	}
	memset(lplptarga, 0, iFrames * sizeof(LPTARGA));

	for (int i=0; i<iFrames; i++)
	{
		lplptarga[i]=TargaNew(nWidth1, nHeight1, bpp, byType1);
	if (lplptarga[i]==NULL)
	{
			for (int ix=0; ix<i; ix++)
		{
		free(lplptarga[ix]);
		lplptarga[ix]==NULL;
		}
		free(lplptarga);
		iLastErr=100;
		return(NULL);
	}
	}
	return (lplptarga);
}
///////////////////////////////////////////////////////////

LPTARGA TargaCopy(LPTARGA lptarga1)
{
	iLastErr=0;
	LPTARGA lptargaNew=NULL;
	lptargaNew=(LPTARGA)malloc(sizeof(TARGA));
	memset(lptargaNew,0,sizeof(TARGA));
	if (lptargaNew==NULL)
	{
		iLastErr=100;//couldn't allocate
		return(NULL);
	}
	if (lptarga1->byBitDepth%8 || lptarga1->byBitDepth>32 || lptarga1->byBitDepth==0)
	{
		iLastErr=109;//odd bit depth
		free(lptargaNew);
		lptargaNew=NULL;
		return(NULL);
	}
	if (lptarga1->width * lptarga1->width * lptarga1->byBitDepth <= 0)
	{
		iLastErr=(107);//couldn't allocate targa buffer
		free(lptargaNew);
		lptargaNew=NULL;
		return(NULL);
	}
	int iByteDepth = lptarga1->byBitDepth/8;
	int iErrNow=0;
	int iErr=0;
	lptargaNew->bySizeofID	=lptarga1->bySizeofID;
	lptargaNew->byMapType	=lptarga1->byMapType;
	lptargaNew->byTgaType	=lptarga1->byTgaType;
	lptargaNew->nMapOrigin	=lptarga1->nMapOrigin;
	lptargaNew->nMapLength	=lptarga1->nMapLength;
	lptargaNew->byMapBitDepth=lptarga1->byMapBitDepth;
	lptargaNew->xImageOrigin	=lptarga1->xImageOrigin;
	lptargaNew->yImageOrigin	=lptarga1->yImageOrigin;
	lptargaNew->width		=lptarga1->width;
	lptargaNew->height		=lptarga1->height;
	lptargaNew->byBitDepth	=lptarga1->byBitDepth;
	lptargaNew->byDescriptor	=lptarga1->byDescriptor;

	lptargaNew->buffer=NULL;
	lptargaNew->buffer = (BYTE *)malloc(lptarga1->width * lptarga1->height * iByteDepth);
	if (lptargaNew->buffer)
		memcpy(lptargaNew->buffer, lptarga1->buffer, lptarga1->width * lptarga1->height * iByteDepth);
	else
		iErrNow=110;//general targa memory error
	if (iErrNow) iErr=iErrNow;

	if (lptarga1->footer.dwSizeofDump>0 && lptarga1->footer.dump)
	{
		lptargaNew->footer.dwSizeofDump = lptarga1->footer.dwSizeofDump;
		lptargaNew->footer.dump = (BYTE *)malloc(lptarga1->footer.dwSizeofDump);
		if (lptargaNew->footer.dump)
			memcpy(lptargaNew->footer.dump, lptarga1->footer.dump, lptarga1->footer.dwSizeofDump);
		else
			iErrNow=110;
		if (iErrNow) iErr=iErrNow;
	}
	else
	{
		lptargaNew->footer.dump=NULL;
		lptargaNew->footer.dwSizeofDump=0;
	}

	if (lptarga1->bySizeofID && lptarga1->byID)
	{
		lptargaNew->bySizeofID=lptarga1->bySizeofID;
		lptargaNew->byID=NULL;
		lptargaNew->byID=(BYTE*)malloc(lptarga1->bySizeofID);
		if (lptargaNew->byID)
			memcpy(lptargaNew->byID, lptarga1->byID, int(lptarga1->bySizeofID));
		else
			iErrNow=110;
		if (iErrNow) iErr=iErrNow;
	}
	else
	{
		lptargaNew->bySizeofID=0;
		lptargaNew->byID=NULL;
	}

	if (lptarga1->nMapLength*lptarga1->byMapBitDepth && lptarga1->byColorMap && lptarga1->byMapBitDepth%8==0)
	{
		int iSizeofBuff=int(lptarga1->nMapLength)*int(lptarga1->byMapBitDepth/8);
		lptargaNew->byColorMap=NULL;
		lptargaNew->byColorMap=(BYTE*)malloc(iSizeofBuff);
		if (lptargaNew->byColorMap)
			memcpy(lptargaNew->byColorMap, lptarga1->byColorMap, iSizeofBuff);
		else
			iErrNow=110;
		if (iErrNow) iErr=iErrNow;
		//length variables already copied.
	}
	else
	{
		lptargaNew->nMapLength=0;
		lptargaNew->byMapBitDepth=0;
		lptargaNew->byColorMap=NULL;
	}
	iLastErr=iErr;
	return(lptargaNew);
} //end TargaCopy

///////////////////////////////////////////////////////////

void TargaFill(LPTARGA lptarga, BYTE *byPix, unsigned int uiPixBytes)
{
	iLastErr=0;
	if (lptarga==NULL)
	{
		return;
	}
	//if (lptarga->buffer==NULL)
	//{
	//	return;
	//}
	//if (byBuff==NULL)
	//{
	//	return;
	//}
	DWORD dwByteDepth, dwPixels, dwPlace;
	BYTE *lpbyte;
	lpbyte=lptarga->buffer;
	dwByteDepth=lptarga->byBitDepth/8;
	dwPixels=DWORD(lptarga->width)*DWORD(lptarga->height)*dwByteDepth;
	for (dwPlace=0; dwPlace<dwPixels; dwPlace++)
	{
		memcpy(lpbyte, byPix, uiPixBytes);
		lpbyte+=dwByteDepth;
	}
	return;
}

///////////////////////////////////////////////////////////

void TargaFlip(BYTE *image, int bytes_per_line, int height)
{
	iLastErr=0;
	BYTE *buffer; // used to perform the image processing
	if (!(buffer = (BYTE *)malloc(bytes_per_line*height)))
	{
	iLastErr=1;
		return;
	}
	memcpy(buffer,image,bytes_per_line*height);
	for (int index=0; index < height; index++)
	memcpy(&image[((height-1) - index)*bytes_per_line],
			 &buffer[index*bytes_per_line], bytes_per_line);
	free(buffer);
	return;
}//end TargaFlip

///////////////////////////////////////////////////////////

LPTARGA TargaLoad(char *sFile)
{
	iLastErr=0;
	//Before using this function, open the file and make sure that
	//it ends with the null terminated BYTE string "TRUEVISION-XFILE"
	LPTARGA lptarga=NULL;
	lptarga=(LPTARGA)malloc(sizeof(TARGA));
	if (lptarga==NULL)
	{
		iLastErr=100;//cannot load null targa pointer
		return(NULL);
	}
	lptarga->byColorMap=NULL;
	lptarga->byID=NULL;
	HANDLE hFile;
	hFile = CreateFile(sFile,//CreateFile will ask for CD-ROM or floppy disk if needed.
		GENERIC_READ,	//needed by ReadFile // GENERIC_READ | GENERIC_WRITE, //use bitwise for both
		FILE_SHARE_READ,//FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE //bitwise for multiple.	0 for total lockout until unloaded.	Delete lets other programs delete it.
		NULL,	//no pointer to SECURITY_ATTRIBUTES struct (which NT parses)
		OPEN_EXISTING,	//create/overwrite file //other kinds: OPEN_ALWAYS(i.e. create/append), CREATE_NEW, OPEN_EXISTING, TRUNCATE_EXISTING(erases data but fails if doesn't exist already)
		FILE_ATTRIBUTE_NORMAL //not normal: -_TEMP for cached, or -_HIDDEN, -_COMPRESSED, _READONLY etc.
		|FILE_FLAG_SEQUENTIAL_SCAN,//optimize caching for sequential read. //others: FILE_FLAG_WRITE_THROUGH forces immediate flush to disk, FILE_FLAG_DELETE_ON_CLOSE, etc.
		NULL);	//(NT only - other OS won't open with non-NULL parameter) handle to template file, for copying attributes from another file 
	if (hFile==NULL)
	{
		error_txt<<"TargaLoad: Error opening file"<<endl;
		CloseHandle(hFile);
		iLastErr=313;
		return(NULL);//file not found
	}
	int bTest=0;
	DWORD dwNumBytesRead=0;
	DWORD dwTotalHeaderRead=0;

	lptarga->bySizeofID=0;
	bTest = ReadFile(hFile,&lptarga->bySizeofID,1,&dwNumBytesRead, NULL);
	if (dwNumBytesRead != 1)// || lptarga->bySizeofID<0)
	{
		CloseHandle(hFile);
		iLastErr=313;
		return(NULL);//"couldn't load source targa file"
	}
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->byMapType,1,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->byTgaType,1,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->nMapOrigin,2,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->nMapLength,2,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->byMapBitDepth,1,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->xImageOrigin,2,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->yImageOrigin,2,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->width,2,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->height,2,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->byBitDepth,1,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	bTest = ReadFile(hFile,&lptarga->byDescriptor,1,&dwNumBytesRead, NULL);
	dwTotalHeaderRead+=dwNumBytesRead;
	if (lptarga->bySizeofID>0)
	{
		lptarga->byID=NULL;
		lptarga->byID=(BYTE*)malloc(lptarga->bySizeofID);
		if (lptarga->byID)
		{
			bTest = ReadFile(hFile,lptarga->byID,lptarga->bySizeofID,&dwNumBytesRead, NULL);
			dwTotalHeaderRead+=dwNumBytesRead;
		}
		else
		{
			CloseHandle(hFile);
		iLastErr=110;
			return(NULL);
		}
	}
	int iSizeofMap=int(lptarga->nMapLength)*int(lptarga->byMapBitDepth/8);
	if (iSizeofMap && lptarga->byMapBitDepth%8==0)
	{
		lptarga->byColorMap=NULL;
		lptarga->byColorMap=(BYTE*)malloc(iSizeofMap);
		if (lptarga->byColorMap)
		{
			bTest = ReadFile(hFile,lptarga->byColorMap,iSizeofMap,&dwNumBytesRead, NULL);
			dwTotalHeaderRead+=dwNumBytesRead;
		}
		else
		{
			CloseHandle(hFile);
			iLastErr=110;
			return(NULL);
		}

	}
	else	//if zero or less, reset:
	{
		iSizeofMap=0;
		lptarga->nMapLength=0;
		lptarga->byMapBitDepth=0;
	}
	
	if (dwTotalHeaderRead != DWORD(18 + lptarga->bySizeofID + iSizeofMap))
	{
		CloseHandle(hFile);
		iLastErr=313;
		return(NULL);//"couldn't load source lptarga file"
	}
	int iByteDepth = lptarga->byBitDepth/8;
	if (!(lptarga->buffer = (BYTE *)malloc((int)lptarga->width*(int)lptarga->height*iByteDepth)))
	{
		CloseHandle(hFile);
		iLastErr=315;
		return(NULL);
	}
	//Read in the buffer
	bTest = ReadFile(hFile,lptarga->buffer,((int)lptarga->width*(int)lptarga->height*iByteDepth),&dwNumBytesRead, NULL);//_lread(file_handle, alphaLook,256*256*256);
	if (dwNumBytesRead != ((DWORD)lptarga->width*(DWORD)lptarga->height*(DWORD)iByteDepth))
	{
		CloseHandle(hFile);
		iLastErr=313;
		return(NULL);//"couldn't load source targa file"
	}

	//FOOTER:
	BYTE tempFooter[521]; //521 is the theoretical maximum footer size (this could be trouble if I'm wrong)
	lptarga->footer.dwSizeofDump=0;
	lptarga->footer.dump=NULL;
	while (dwNumBytesRead)
	{
		bTest = ReadFile(hFile,&tempFooter[lptarga->footer.dwSizeofDump],1,&dwNumBytesRead, NULL);//byte by byte
		lptarga->footer.dwSizeofDump+=(int)dwNumBytesRead;
	}
	if (lptarga->footer.dwSizeofDump)
	{
		lptarga->footer.dump = (BYTE *)malloc(lptarga->footer.dwSizeofDump);
		memcpy(lptarga->footer.dump, tempFooter, lptarga->footer.dwSizeofDump);
	}
	else
	{
		lptarga->footer.dump = NULL;
	}

	bTest=CloseHandle(hFile);
	if (bTest==0) error_txt<< "TargaLoad:	Error closing file."<<endl;
	TargaFlip(lptarga->buffer, lptarga->width*iByteDepth, lptarga->height);
	return(lptarga);
}//end TargaLoad

///////////////////////////////////////////////////////////
TARGA** TargaLoadSeq(char *sFileFirstFrame, int *lpiReturnFrames)
{//pass *0008.tga to start at frame 9, or pass * to add 0000.tga etc. automatically.
	iLastErr=0;
	TARGA **lplptarga=(TARGA**)malloc(*lpiReturnFrames * sizeof(LPTARGA));
	if (lplptarga==NULL)
	{
		iLastErr=100;
		return(NULL);
	}
	char sExtCaseAware[5]={'.','t','g','a','\0'};//overwritten below only if extension was sent
	memset(lplptarga, 0, *lpiReturnFrames * sizeof(LPTARGA));
	char sBaseName[512];
	strcpy(sBaseName, sFileFirstFrame);
	int iLen=strlen(sBaseName);
	int iStartFrame=0;
	if (IsLikeMask(sBaseName, "*.tga"))
	{
		strcpy(sExtCaseAware, &sBaseName[iLen-4]);
	trunc(sBaseName, iLen-4);//remove extension
		static char sNumber[128];
		strcpy(sNumber, &sBaseName[iLen-8]);//-8 to get numbers (8 since iLen is original length).
		trunc(sBaseName, iLen-8);//now remove the numbers 
		iStartFrame=IntOfString(sNumber);
		if (iStartFrame==-1)
		{
			error_txt << "TargaSeq2Array: couldn't interpret starting frame from given name"<<endl;
			iStartFrame=0;
		}
		iLen-=8;
	}
	char sNameNow[512];
	//static char sDigit[10][2]={"0","1","2","3","4","5","6","7","8","9"};
	int iErrNow=0;
	int iErr=0;
	int iCountFiles=0;//replaces value *lpiReturnFrames1 to reflect actual frames loaded

	for (int index=iStartFrame;index<*lpiReturnFrames;index++) //LoadTarga uses the global index!!!
	{
		int i=index;
		strcpy(sNameNow, sBaseName);
		strcat(sNameNow, sDigit[i/1000]);
		if (i>=1000) i-=int(i/1000)*1000;
		strcat(sNameNow, sDigit[index/100]);
		if (i>=100) i-=int(i/100)*100;
		strcat(sNameNow, sDigit[index/10]);
		strcat(sNameNow, sDigit[index%10]);
		strcat(sNameNow, sExtCaseAware);

		lplptarga[index]=TargaLoad(sNameNow);
		iErrNow=iLastErr;
		if(iErrNow)
		{
			if (iCountFiles>0)
			{
				iErr=0;
				index=*lpiReturnFrames;
				*lpiReturnFrames=iCountFiles;
				break;
			}
			else
			{
				error_txt << "Did not load targa named \"" << sNameNow << "\"" << endl;
				iErr=iErrNow;
				index=*lpiReturnFrames;
				*lpiReturnFrames=0;
				break;
			}
		}
		iCountFiles++;
	}
	iLastErr=iErr;
	return(lplptarga);
}

///////////////////////////////////////////////////////////////

void TargaSave(LPTARGA targa, char *sFile)
{
	iLastErr=0;
	int iByteDepth = targa->byBitDepth/8;
	if (iByteDepth<=0 || targa->byBitDepth%8)
	{
	iLastErr=109;
		return;//odd bit depth error
	}
	char newName[256];
	strcpy(newName, sFile);
	int iErrNow=0;
	int iErr=0; //start with a good return

	TargaFlip(targa->buffer, targa->width*iByteDepth, targa->height);//Flip the targa upside-down for save
	BOOL bTest=FALSE;
	//Open the new or used file to overwrite
	HANDLE hFile=CreateFile(newName,GENERIC_WRITE,FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//Save the file header //old://bTest = WriteFile(hFile,&targa->targaFileHeader,sizeof(targaFileHeader),&dwNumBytesWritten, NULL);
	DWORD dwNumBytesWritten=0;
	DWORD dwTotalHeaderWritten=0;
	bTest = WriteFile(hFile,&targa->bySizeofID,1,&dwNumBytesWritten, NULL);
	if (dwNumBytesWritten != 1)
	{
		CloseHandle(hFile);
	iLastErr=307;
		return;//error saving, corrupt or disk error
	}
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->byMapType,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->byTgaType,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->nMapOrigin,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->nMapLength,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->byMapBitDepth,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->xImageOrigin,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->yImageOrigin,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->width,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->height,2,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->byBitDepth,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	bTest = WriteFile(hFile,&targa->byDescriptor,1,&dwNumBytesWritten, NULL);
	dwTotalHeaderWritten+=dwNumBytesWritten;
	if (targa->bySizeofID>0)
	{
		if (targa->byID)
		{
			bTest = WriteFile(hFile,targa->byID,targa->bySizeofID,&dwNumBytesWritten, NULL);
			dwTotalHeaderWritten+=dwNumBytesWritten;
		}
		else
		{
			targa->bySizeofID=0;
			CloseHandle(hFile);
		iLastErr=307;
			return;//ID size is corrupt
		}
	}
	int iSizeofMap=int(targa->nMapLength)*int(targa->byMapBitDepth/8);
	if (iSizeofMap && targa->byMapBitDepth%8==0)
	{
		if (targa->byColorMap)
		{
			bTest = WriteFile(hFile,targa->byColorMap,iSizeofMap,&dwNumBytesWritten, NULL);
			dwTotalHeaderWritten+=dwNumBytesWritten;
		}
		else
		{
			CloseHandle(hFile);
		iLastErr=307;
			return;//colormap size is corrupt
		}

	}
	else // if zero or less, reset:
	{
		iSizeofMap=0;
		targa->nMapLength=0;
		targa->byMapBitDepth=0;
	}
	if (dwTotalHeaderWritten != DWORD(18 + (int)targa->bySizeofID + iSizeofMap))
	{
		CloseHandle(hFile);
	iLastErr=307;
		return;//"couldn't load source targa file"
	}
	//end header data

	int iSizeofBuff=(int)targa->width * (int)targa->height * iByteDepth;
	bTest = WriteFile(hFile, targa->buffer, iSizeofBuff, &dwNumBytesWritten, NULL);	//Save the image data
	if (dwNumBytesWritten!=DWORD(iSizeofBuff))
		iErrNow=307;
	if (iErrNow) iErr=iErrNow;
	if (targa->footer.dwSizeofDump && targa->footer.dump) bTest = WriteFile(hFile,targa->footer.dump, targa->footer.dwSizeofDump, &dwNumBytesWritten, NULL);//Save the file footer info if any
		bTest= CloseHandle(hFile);//Close the file
	TargaFlip(targa->buffer, targa->width*iByteDepth, targa->height);//Flip the targa back to normal
	iLastErr=iErr;
} // end TargaSave

///////////////////////////////////////////////////////////
//void TargaSaveSeq(TARGA **lplptarga, char *sFile, int *lpiReturnFrames)
//{
//	TargaSave(
//}

///////////////////////////////////////////////////////////

void TargaUnload(LPTARGA &lptarga)
{
     int iErr=0;
	iLastErr=0;
	if (lptarga)
	{
		if (lptarga->buffer)
		{
			free(lptarga->buffer);
		}
		if (lptarga->footer.dump)
		{
			free(lptarga->footer.dump);
		}
		if (lptarga->byColorMap)
		{
			free(lptarga->byColorMap);
	}
		if (lptarga->byID)
		{
			free(lptarga->byID);
	}
		memset(lptarga,0,sizeof(TARGA)); //also sets above pointers to null
	free(lptarga);
	lptarga=NULL;
	}
	else iErr=9;

	iLastErr=iErr;
	return;
}
void TargaUnloadSeq(TARGA ** &lplptarga, int iFrames)
{
	iLastErr=0;
	if (lplptarga==NULL)
	{
		iLastErr=51;
		return;
	}
	int iErr=0;
	for (int i=0; i<iFrames; i++)
	{
		if (lplptarga[i])
	{
			TargaUnload(lplptarga[i]);
		lplptarga[i]==NULL;
	}
		if (iLastErr) iErr=iLastErr;
	}
	free(lplptarga);
	lplptarga=NULL;
	return;
}
//}//end namespace
///////////////////////////////////////////////////////////////

#endif
