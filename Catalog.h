// Catalog.h: interface for the CCatalog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CATALOG_H__7CBB6019_B7F7_11D9_B539_FF062122A253__INCLUDED_)
#define AFX_CATALOG_H__7CBB6019_B7F7_11D9_B539_FF062122A253__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
const int _MAX_CHARS=127;
struct Catalog;
enum {ARCHIVE=1, COMPRESSED=2, DIRECTORY=4, ENCRYPTED=8, HIDDEN=16, READONLY=32, SYSTEM=64};
struct EncodedCatalog
{
	FILETIME ftCreationTime;
	//DWORD dwDetails[2];
	//DWORD nChars:15,
	DWORD nFileName:8,
		nDesc:7,
		myIndex:20,//15,
		flag:2;
	DWORD nFileSizeLow:10,
		parent:20,//15,
		dwFileAttributes:7;
public:
	void Write(fstream &file);
	Catalog Read(fstream &file);
	EncodedCatalog();
	~EncodedCatalog();
};
struct ID3
{
	char TID[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[30];
	BYTE genre;
public:
	ID3();
	~ID3();
};

struct Catalog
{
	FILETIME ftCreationTime;//8
	DWORD myIndex;
	DWORD parent;
	//DWORD nextIndex;
	DWORD nFileSizeLow;
	BYTE nFileName;
	BYTE nDesc;
	//WORD nChars;
	BYTE flag;
	BYTE dwFileAttributes;
	//UCHAR level;
public:
	Catalog(EncodedCatalog enc);
	operator EncodedCatalog();
	Catalog();
	~Catalog();
};

struct Files
{
	char cFileName[_MAX_FNAME];//256
	char desc[_MAX_CHARS];
	void Read(fstream &file, BYTE nFileName, BYTE nDesc);
	void Write(fstream &file, BYTE nFileName, BYTE nDesc);
public:
	Files();
	~Files();
};

struct FileData
{
	FILETIME ftCreationTime;//8
	DWORD nFileSizeLow;//4
	WORD dwFileAttributes;//2
	BYTE flag;
	char cFileName[_MAX_FNAME];
	char desc[_MAX_CHARS];
public:
	FileData(Catalog c, Files f);
	FileData();
	~FileData();
};

class CFilesInfo
{
public:
	FileData fd;
	CFilesInfo(FileData f);
	CFilesInfo();
	~CFilesInfo();
};

struct CSizeDesc
{
	double fFileSize;
	CString desc;
};

class CCatalog
{
public:
	CCatalog *sub;//4
	CCatalog *next;//4
	Catalog cat;//354
	Files files;
	static int Verified(fstream &file);
	CCatalog();
	void FreeTree(CCatalog *cat);
	~CCatalog();
};

#endif // !defined(AFX_CATALOG_H__7CBB6019_B7F7_11D9_B539_FF062122A253__INCLUDED_)
