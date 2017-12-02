// Catalog.cpp: implementation of the CCatalog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ghost Xpress.h"
#include "Catalog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCatalog::CCatalog()
{
	memset((void*)this,0,sizeof(*this));
}

CCatalog::~CCatalog()
{

}

void CCatalog::FreeTree(CCatalog *cat)
{
	CCatalog *n;
	if(cat==NULL)
		return;
	if(cat->sub==NULL)
		return;
	CCatalog *next=cat->sub;
	do
	{
		n=next;
		next=next->next;
		n->next=NULL;
		if((n->cat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
			FreeTree(n);
		delete n;
	}while(next!=NULL);
}

int CCatalog::Verified(fstream &file)
{
	char ch[3]={0};
	file.read(ch,sizeof(char)*3);
	if(ch[0]!='G' || ch[1]!='x')
	{
		file.close();
		return 0;
	}
	else if(ch[2]!='2')
	{
		file.close();
		return 0;
	}
	file.seekg(3,ios::beg);
	return 1;
}

Catalog::Catalog()
{
	memset((void*)this,0,sizeof(*this));
}

Catalog::~Catalog()
{
}

CFilesInfo::CFilesInfo()
{
	memset((void*)this,0,sizeof(*this));
}

CFilesInfo::~CFilesInfo()
{

}

CFilesInfo::CFilesInfo(FileData f)//, Files f)
{
	fd=f;
}

void Files::Read(fstream &file, BYTE nFileName, BYTE nDesc)
{
	file.read(cFileName,sizeof(char)*nFileName);
	file.read(desc,sizeof(char)*nDesc);
	cFileName[nFileName]=0;
	desc[nDesc]=0;
}

void Files::Write(fstream &file, BYTE nFileName, BYTE nDesc)
{
	file.write(cFileName,sizeof(char)*nFileName);
	file.write(desc,sizeof(char)*nDesc);
}

Files::Files()
{
	memset((void*)this,0,sizeof(*this));
}

Files::~Files()
{
}

FileData::FileData()
{
	memset((void*)this,0,sizeof(*this));
}

FileData::~FileData()
{
}

FileData::FileData(Catalog c, Files f)
{
	strcpy(cFileName,f.cFileName);
	strcpy(desc,f.desc);
	flag=c.flag;
	dwFileAttributes=c.dwFileAttributes;
	ftCreationTime=c.ftCreationTime;
	nFileSizeLow=c.nFileSizeLow;
}

EncodedCatalog::EncodedCatalog()
{
	memset((void*)this,0,sizeof(*this));
}

EncodedCatalog::~EncodedCatalog()
{
}

Catalog::Catalog(EncodedCatalog enc)
{
	ftCreationTime=enc.ftCreationTime;
	nFileName=enc.nFileName;
	nDesc=enc.nDesc;
	myIndex=enc.myIndex;
	flag=enc.flag;
	nFileSizeLow=enc.nFileSizeLow;
	parent=enc.parent;
	dwFileAttributes=enc.dwFileAttributes;
}
	/*ftCreationTime=enc.ftCreationTime;
	nChars=LOWORD(enc.dwDetails[0] & 0x00007FFF);
	myIndex=LOWORD((enc.dwDetails[0] & 0x3FFF8000)>>15);
	flag=HIBYTE(HIWORD(enc.dwDetails[0] & 0xC0000000))>>6;
	nFileSizeLow=LOWORD(enc.dwDetails[1] & 0x000003FF);
	parent=LOWORD((enc.dwDetails[1] & 0x01FFFC00)>>10);
	dwFileAttributes=(HIBYTE(HIWORD(enc.dwDetails[1] & 0xFE000000))>>1);
	/*nChars=LOWORD(enc.dwDetails[0] & 0x00007FFF);
	nFileSizeLow=LOWORD((enc.dwDetails[0] & 0x07FF8000)>>15);
	myIndex=LOWORD(enc.dwDetails[1] & 0x000003FF);
	myIndex<<=5;
	myIndex|=(HIBYTE(HIWORD(enc.dwDetails[0] & 0xF8000000))>>3);//temp;
	parent=LOWORD((enc.dwDetails[1] & 0x01FFFC00)>>10);
	dwFileAttributes=(HIBYTE(HIWORD(enc.dwDetails[1] & 0xFE000000))>>1);*/

Catalog::operator EncodedCatalog()
{
	EncodedCatalog enc;
	enc.ftCreationTime=ftCreationTime;
	enc.nFileName=nFileName;
	enc.nDesc=nDesc;
	enc.myIndex=myIndex;
	enc.flag=flag;
	enc.nFileSizeLow=nFileSizeLow;
	enc.parent=parent;
	enc.dwFileAttributes=dwFileAttributes;
	//enc.dwDetails[0] = (((flag & 0x0003)<<30) | ((myIndex & 0x7FFF)<<15) | (nChars & 0x7FFF));
	//enc.dwDetails[1] = (((dwFileAttributes & 0x7F)<<25) | ((parent & 0x7FFF)<<10) | (nFileSizeLow & 0x03FF));
	//enc.dwDetails[0] = (((myIndex & 0x001F)<<27) | (nFileSizeLow<<15) | nChars);
	//enc.dwDetails[1] = (((dwFileAttributes & 0x7F)<<25) | (parent<<10) | (myIndex>>5));
	return enc;
}

Catalog EncodedCatalog::Read(fstream &file)
{
	file.read((char*)this,sizeof(EncodedCatalog));
	return Catalog(*this);
}

void EncodedCatalog::Write(fstream &file)
{
	file.write((char*)this,sizeof(EncodedCatalog));
}

ID3::ID3()
{
	memset((void*)this,0,sizeof(*this));
}

ID3::~ID3()
{
}