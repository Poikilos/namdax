#ifndef BASEFUNC_CPP
#define BASEFUNC_CPP
#include <cmath>
#include <fstream>
#include <iostream>
//#include <windows.h>
#define STEMPLEN 2048
#define SNUMLEN 128
char sNum[SNUMLEN];
char sTemp[STEMPLEN];
int iLastErr=0; //each call of every function must reset this to track errors.
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
#define BIT1		1
#define BIT2		2
#define BIT3		4
#define BIT4		8
#define BIT5		16
#define BIT6		32
#define BIT7		64
#define BIT8		128
#define BIT9		256
#define BIT10	 512
#define BIT11	 1024
#define BIT12	 2048
#define BIT13	 4096
#define BIT14	 8192
#define BIT15	 16384
#define BIT16	 32768
#define BIT17	 65536
#define BIT18	 131072
#define BIT19	 262144
#define BIT20	 524288
#define BIT21	 1048576
#define BIT22	 2097152
#define BIT23	 4194304
#define BIT24	 8388608
#define BIT25	 16777216
#define BIT26	 33554432
#define BIT27	 67108864
#define BIT28	 134217728
#define BIT29	 268435456
#define BIT30	 536870912
#define BIT31	 1073741824
#define BIT32	 2147483648

DWORD dwBit[33]={0,1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648};
char cDigit[10]={'0','1','2','3','4','5','6','7','8','9'};
char sDigit[10][2]={"0","1","2","3","4","5","6","7","8","9"};
char slash[2]={'\\','\0'};
#define BYROUNDF(f1) (BYTE(f1+.5f))
#define BYROUNDLD(ld) (BYTE(ld + .5L))
#define IROUNDF(f1) (int(f1+.5f))
#define IROUNDD(d) (int(d+.5))
#define IROUNDLD(ld) (int(ld+.5L))
#define BYTEOFHUE(fH360) ( (fH360<360.0f)? BYROUNDF(float(fH360*256.0f/360.0f)) : BYROUNDF(float((fH360-360.0f)*256.0f/360.0f)) ) //assumes fH360<720 //division using 256 allows 256 levels -- 255<360 is okay since 0=0
#define BYTEOFSL(fSatOrLum) ( BYTE( float(fSatOrLum*255.0f)+.5f ) )
#define LOWNIBBLEBITS	 15
#define HIGHNIBBLEBITS	240
#define LOWNIBBLE(by) (by&LOWNIBBLEBITS)
#define HIGHNIBBLE(by) (by>>4)
#define SETLOWNIBBLE(by,by1) ((by&HIGHNIBBLEBITS) & by1) //assumes by1<16
#define SETHIGHNIBBLE(by,by1) ((by*LOWNIBBLEBITS) & (by1<<4)) //assumes by1<16
#define F360_TIMES_256 (92160.0f)
#define F256_DIV_360 (0.711111111111111111111111111111111f)
#define LD256_DIV_360 (0.711111111111111111111111111111111L)
#define BYTEOF360LD(a) (BYTE(( (long double)(a) * 256.0L/360.0L	)+.5L)) //#define LDBYTEOF360(a) (BYTE(( (long double)(a) * LD256_DIV_360 )+.5L))
#define BYTEOF360F(fA) (BYTE((	(fA) * F256_DIV_360	)+.5f) ) //.5f is to change truncation to rounding
#define ROFXY(x,y) ( sqrt( x * x + y * y ) )
#define FMOD(f1,f2) ((f1>f2) ? ( f1 - float(int(f1/f2))*f2) : 0 )
#define DMOD(f1,f2) ((f1>f2) ? ( f1 - double(int(f1/f2))*f2) : 0 )
#define LD180_DIV_PI						57.2957795130823208767981548141052L
#define D180_DIV_PI						 57.2957795130823208767981548141052
#define ONEEIGHTY_DIV_PI				57.2957795130823208767981548141052 // 180/PI
#define DTHETAOFXY(x,y) ( (y!=0 || x!=0) ? (atan2(y,x) * D180_DIV_PI) : 0 )
#define LDTHETAOFXY(x,y) ( (y!=0 || x!=0) ? (atan2(y,x) * LD180_DIV_PI) : 0 )

//debugging lines

int iLogNow=0;
//end debugging

typedef struct IRECT_STRUCT
{
	int top;
	int left;
	int bottom;
	int right;
} IRECT, *LPIRECT;

typedef struct IPOINT_STRUCT
{
	int x;
	int y;
} IPOINT, *LPIPOINT;

typedef struct PIXEL32BGRA_STRUCT
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
}PIXEL32BGRA, *LPPIXEL32BGRA;

typedef struct FPIXELHSL_STRUCT
{
				float h; //degrees
				float s; //percent
				float l; //percent
}FPIXELHSL, *LPFPIXELHSL;

typedef struct PIXELHSL_STRUCT
{
				BYTE h;
				BYTE s;
				BYTE l;
}BYPIXELHSL, *LPBYPIXELHSL;

typedef struct PIXEL24BGR_STRUCT
{
	BYTE b;
	BYTE g;
	BYTE r;
}PIXEL24BGR, *LPPIXEL24BGR;
using namespace std;
inline bool stringerrors(char *string, DWORD dwMaxBuff)
{
				if (string==NULL)
				{
								//error_txt<<"STRING ERROR: "<<"string is NULL!!!"<<endl;
								return(true);
				}
				if (strlen(string)<1 || strlen(string)>=dwMaxBuff)
				{
								//error_txt<<"STRING ERROR: "<<strlen(string)<<" is strlen(string)! (MaxBuff="<<dwMaxBuff<<") string="<<string<<endl;
								return(true);
				}
				return(false);
}

inline void FPOLAROFRECT(float &r,float &theta, float x, float y)
{
				r=(float)ROFXY(x,y);
				float x1=x, y1=y;
				theta=(float)DTHETAOFXY(x1,y1);
}

inline void DPOLAROFRECT(double &r,double &theta, double x, double y)
{
				r=ROFXY(x,y);
				double x1=x, y1=y;
				theta=DTHETAOFXY(x1,y1);
}


//BYTEOF360 intentionally divides by 256 to allow storage of 256 degree variations!!!

//distance of an angle from zero assuming 0 to 360:
//#define FMINANGLE(f) (((f)>180.0f)?(360.0f-(f)):(f))
//difference between 2 angles assuming 0 to 360:
//#define FANGLEDIFF(f1,f2) ( fabs(FMINANGLE(f1)-FMINANGLE(f2)) )
inline float FANGLEDIFF(float f1, float f2) //returns 0 to 180
{
	while (f1>360.0f) f1-=360.0f;
	while (f1<-360.0f) f1+=360.0f;
	while (f2>360.0f) f2-=360.0f;
	while (f2<-360.0f) f2+=360.0f;
	register float fDiff=fabs(f1-f2);
	if (fDiff>180.0f)fDiff=(360.0f-fDiff);
	return(fDiff);
}



inline long double LDANGLE360(long double d1)
{
				while (d1>360.0L) d1-=360.0L;
				while (d1<-360.0L) d1+=360.0L;
				if (d1<0) d1=360.0L+d1;
				return(d1);
}

inline long double LDANGLEDIFF(long double d1, long double d2) //returns 0 to 180
{
				while (d1>360.0L) d1-=360.0L;
				while (d1<-360.0L) d1+=360.0L;
				while (d2>360.0L) d2-=360.0L;
				while (d2<-360.0L) d2+=360.0L;
 	register long double dDiff=abs(d1-d2);
				if (dDiff>180.0L)dDiff=(360.0L-dDiff);
	return(dDiff);
}

void trunc(char *s, int i)
{
	s[i]='\0';
}

void Upperstring(char *s)
{
				static int iAt;
				iAt=strlen(s)-1;
				while (iAt>=0)
				{
								if (islower(s[iAt])) s[iAt]=toupper(s[iAt]);
								iAt--;
				}
				return;
}
void Lowerstring(char *s)
{
				static int iAt;
				iAt=strlen(s)-1;
				while (iAt>=0)
				{
								if (isupper(s[iAt])) s[iAt]=tolower(s[iAt]);
								iAt--;
				}
				return;
}
void DetectSlashType(char *sFullPath) //always run this before using directory string
{
				if (slash[0]!='/' && sFullPath[0]=='/') slash[0]='/';
				return;
}
void ValidateSlash(char *sDir) //detects slash type and adds slash if needed
{
				DetectSlashType(sDir);
				if (sDir[ strlen(sDir)-1 ] != slash[0])
				{
								if (strlen(sDir)+1<MAX_PATH) strcat(sDir,slash);
				}
				return;
}
bool IsLikeMask(char *sFullPath1, char *sMask1)
{//limitations: gets confused if mask has 2 asterisks directly in a row
				static char sDir[MAX_PATH], sMask[MAX_PATH];
				strcpy(sDir,sFullPath1);//necessary to recopy because of Lowerstring, AND for removal of slash
				strcpy(sMask,sMask1);
				Lowerstring(sDir);
				Lowerstring(sMask);
				DetectSlashType(sDir);
				int iDirLen=strlen(sDir);
				int iDirPlace=iDirLen-1;
				if (sDir[iDirPlace]==slash[0]) sDir[iDirPlace]='\0';
				int iMaskLen=strlen(sMask);
				if ((sDir[iDirLen-1]!=sMask[iMaskLen-1]) && (sMask[iMaskLen-1]!='*')) return(false);
				while (iDirPlace>=0 && sMask[iDirPlace]!=slash[0]) iDirPlace--;
				int iToMatch=0;//characters to find (not the asterisks)
				for (int i=0; i<iMaskLen; i++)
				{
								if (sMask[i]!='*') iToMatch++;
				}
				//error_txt<<"iToMatch="<<iToMatch<<endl;
				int iMatches=0;
				int iMaskPlace;
				for (iMaskPlace=0; iMaskPlace<iMaskLen && iDirPlace<iDirLen; iMaskPlace++)
				{
								if (sMask[iMaskPlace]==sDir[iDirPlace])
								{
												iDirPlace++;
												iMatches++;
								}
								else if (sMask[iMaskPlace]=='*')
								{
												if (iMaskPlace+1<iMaskLen)
												{
																iMaskPlace++;
																while (iDirPlace<iDirLen && sMask[iMaskPlace]!=sDir[iDirPlace])
																{
																				iDirPlace++;
																}
																if (iDirPlace<iDirLen && sMask[iMaskPlace]==sDir[iDirPlace])
																{
																				iDirPlace++;
																				iMatches++;
																}

												}
								}
				}
				//error_txt<<"iMatches="<<iMatches<<endl<<endl;
				return((iMatches==iToMatch)?true:false);
} // end IsLikeMask

void strcatfromint(char *string, int i)
{
				if (i==0)
				{
								strcat(string, "0");
								return;
				}
				else if (i<0)//if negative
				{
								strcat(string, "-");
								i=abs(i);
				}
	char sDigit[10][2] = {"0","1","2","3","4","5","6","7","8","9"};
	int iExpOfTen = 0;
	int iTemp = i;
	while (iTemp>9)
	{
		iTemp/=10;
		iExpOfTen++;
	}
	iTemp=i;
	int iDivisor;
	while (iExpOfTen>=0)
	{
		iDivisor=int(pow((long double)10, (long double)iExpOfTen));
								if (iDivisor)
								{
						strcat(string, sDigit[iTemp/iDivisor]);
						iTemp-=iDivisor*int(iTemp/iDivisor);
								}
								else
								{
												//error_txt<<"ERROR: strcatfromint()'s iDivisor="<<iDivisor<<endl;
								}
		iExpOfTen--;
	}
	return;
}

void strcpyfromint(char *string, int i)
{
				string[0]='\0';
				strcatfromint(string, i);
}

void strcatfromdouble(char *sNum, register double d1, int iMaxDecimals)
{
				int iExp10Max=-iMaxDecimals;

				if (d1==0.0)
				{
								strcat(sNum,"0.0");
								return;
				}
				//sNum[0]='\0';
				if (d1<0.0)//if negative
				{
								strcat(sNum, "-");
								d1=fabs(d1);//set abs
				}
				register double d=d1;//d is the destructible version
				int iExp10=15;//DMAXEXP;
				while (d<pow((long double)10,(long double)iExp10)) iExp10--;
				////error_txt<<"float iExp10's = ";
				if (iExp10<0) iExp10=0; //make sure leading zeros get copied
		const static double DMINEXP=-15;
				while ( (d>0 && iExp10>=DMINEXP && iExp10>iExp10Max) || iExp10>=-1 )
				{
								if ( (DWORD)( d/pow((long double)10,(long double)iExp10) ) < 10 )
								{
												//error_txt<<"["<<DWORD( d/pow(10,iExp10) )<<"](e="<<iExp10<<")"<<flush;
												strcat(sNum, sDigit[(DWORD)( d/pow((long double)10,(long double)iExp10) )]);
												if (iExp10==0) strcat(sNum, ".");
								}
								else
								{
												//error_txt<<"ERROR: strcatfromdouble()'s DWORD( d/pow(10,iExp10) ) isn't < 10! =("<<(DWORD)( d/pow((long double)10,(long double)iExp10) )<<")"<<endl;
								}
								d-=double((DWORD)( d/pow((long double)10,(long double)iExp10) ))*pow((long double)10,(long double)iExp10);
								iExp10--;
				}

				////error_txt<<endl<<"strcatfromdouble(): "<<sNum<<endl<<" FROM d1="<<d1<<endl<<endl;
				return;
}
//overload it without Max Decimals, so set Max Decimals to 10:
void strcatfromdouble(char *sNum, register double d1)
{
				strcatfromdouble(sNum, d1, 10);//-(DMINEXP));
}

void strcpyfromdouble(char *sNum, register double d1, int iMaxDecimals)
{
				sNum[0]='\0';//effectively, strcpy
				strcatfromdouble(sNum, d1, iMaxDecimals);
}
//overload it without Max Decimals, so set MaxDecimals to 10:
void strcpyfromdouble(char *sNum, register double d1)
{
				sNum[0]='\0';//effectively, strcpy
				strcatfromdouble(sNum, d1, 10);//-(DMINEXP));
}

int IntOfChar(char *cDigitX)
{
				static int iErrLevel=0;
				for (register int i=0; i<10; i++)
				{
								if (*cDigitX==cDigit[i]) return(i);
				}
				//return before now usually
				//if (iErrLevel<10)error_txt<<"IntOfChar error: "<<*cDigitX<<" is not an integer!";//error if didn't return yet;
				//else if (iErrLevel<11)error_txt<<"LAST WARNING -- IntOfChar error: "<<*cDigitX<<" is not an integer!";//error if didn't return yet;
				iErrLevel++;
				return(0);
}
int IntOfString(char *sNum)
{
				register DWORD dwStrlen=strlen(sNum);
				register DWORD dw=0;//loop var
				register DWORD dwIsNeg=0;
				if (sNum[0]=='-') dwIsNeg=1;
				register int iExp10=strlen(sNum)-1-int(dwIsNeg);
				register int iReturn=0;
				for (dw=dwIsNeg; dw<dwStrlen; dw++)
				{
								if (sNum[dw]!='.')
								{
												iReturn+=powl((long double)10,(long double)iExp10)*(long double)IntOfChar(&sNum[dw]);//math.h: long powl(10,int)
												iExp10--;
								}
				}
				if (dwIsNeg==1) iReturn*=-1;
				return(iReturn);
}//end IntOfString
double DoubleOfString(char *sNum)
{
				register DWORD dwStrlen=strlen(sNum);
				register DWORD dw=0;//loop var
				register int iExp10=-1;
				register DWORD dwIsNeg=0;
				if (sNum[0]=='-')
				{
								dwIsNeg=1;
				}
				for (dw=dwIsNeg; dw<dwStrlen && sNum[dw]!='.'; dw++)
				{
								iExp10++;
				}
				register double dReturn=0;
				for (dw=dwIsNeg; dw<dwStrlen; dw++)
				{
								if (sNum[dw]!='.')
								{
												dReturn+=pow((long double)10,(long double)iExp10)*(double)IntOfChar(&sNum[dw]);//math.h: double pow(10,int)
												iExp10--;
								}
				}
				if (dwIsNeg==1) dReturn*=-1;
				return(dReturn);
}

float FloatOfString(char *sNum)
{
				register DWORD dwStrlen=strlen(sNum);
				register DWORD dw=0;//loop var
				register int iExp10=-1;
				register DWORD dwIsNeg=0;
				if (sNum[0]=='-')
				{
								dwIsNeg=1;
				}
				for (dw=dwIsNeg; dw<dwStrlen && sNum[dw]!='.'; dw++)
				{
								iExp10++;
				}
				register float fReturn=0;
				for (dw=dwIsNeg; dw<dwStrlen; dw++)
				{
								if (sNum[dw]!='.')
								{
												fReturn+=powl(10,iExp10)*(long double)IntOfChar(&sNum[dw]);//math.h: long double powl(10,int)
												iExp10--;
								}
				}
				if (dwIsNeg==1) fReturn*=-1;
				return(fReturn);
}

bool IsExt(char *sFull1, char *sExt1)
{
	static char sFull[MAX_PATH];
	static char sExt[MAX_PATH];
	strcpy(sFull, sFull1);
	strcpy(sExt, sExt1);
	Upperstring(sFull);
	Upperstring(sExt);

	int iFullLen=strlen(sFull);
	int iExtLen=strlen(sExt);
	bool bTrue=true;
	if (sFull[iFullLen-iExtLen-1]!='.') bTrue=false;
	else
	{
		int iExtNow=0;
		for (int iFullNow=iFullLen-iExtLen; iFullNow<iFullLen; iFullNow++, iExtNow++)
		{
			if (sFull[iFullNow]!=sExt[iExtNow])
			{
				bTrue=false;
				break;
			}
		}
	}
	return(bTrue);
}

#endif
