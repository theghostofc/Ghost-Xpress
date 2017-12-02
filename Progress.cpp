// Progress.cpp : implementation file
//

#include "stdafx.h"
#include "ghost xpress.h"
#include "Progress.h"
#include <math.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgress dialog


CProgress::CProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgress)
	DDX_Control(pDX, IDC_PROGRESS_CAT, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgress, CDialog)
	//{{AFX_MSG_MAP(CProgress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgress message handlers

CSizeDesc CProgress::CreateCatalog(CCatalog *cat, char path[_MAX_PATH])//CString path)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	CSizeDesc sizeDesc;
	double fFileSize=0.0;
	//CString desc;
	DWORD index=0;
	static DWORD level;
	strcat(path,"\\*");
	if(cat==NULL)
		return CSizeDesc();//CString();
	hFind=FindFirstFile(path, &fd);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		abort=(index)?true:false;
		return CSizeDesc();//CString();
	}
	CCatalog *temp=NULL;
	CCatalog *next=NULL;
	int flag=0;
	int len=strlen(path);	
	do
	{
		temp=NULL;
		path[len-1]=0;
		if(fd.cFileName[0] != _T('.'))
		{
			index++;
			strcat(path,fd.cFileName);
			m_Progress.StepIt();
			temp=new CCatalog;
			GetDlgItem(IDC_STATIC_CDDRIVE)->SetWindowText(temp->files.cFileName);
			if(cat->sub==NULL)
			{
				cat->sub=temp;
				next=temp;
			}
			else
			{
				next->next=temp;
				next=temp;
			}
			if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
			{
				level++;
				sizeDesc=CreateCatalog(temp, path);
				strncpy(temp->files.desc, sizeDesc.desc, _MAX_CHARS-1);
				temp->cat.nDesc=strlen(temp->files.desc);
				fFileSize+=sizeDesc.fFileSize;
				path[len-1]=0;
				level--;
			}
			else if(flag!=2 && !strnicmp(GetFileName(fd.cFileName, 1),"TxT",3) && !strnicmp(fd.cFileName,"Serial",5))
			{
				flag=2;
				sizeDesc.desc=path;
				fFileSize+=double((fd.nFileSizeHigh*(MAXDWORD+1))+fd.nFileSizeLow);
			}
			else if(!flag && !strnicmp(GetFileName(fd.cFileName, 1),"TxT",3) && !strnicmp(fd.cFileName,"ReadMe",6))
			{
				flag=1;
				sizeDesc.desc=path;
				fFileSize+=double((fd.nFileSizeHigh*(MAXDWORD+1))+fd.nFileSizeLow);
			}
			else
				fFileSize+=double((fd.nFileSizeHigh*(MAXDWORD+1))+fd.nFileSizeLow);
			Convert(temp, fd, sizeDesc);
			temp->cat.nFileName=strlen(temp->files.cFileName);
			if(!GetFileName(path,1).CompareNoCase("MP3"))
				strncpy(temp->files.desc,GetMP3Data(path),_MAX_CHARS-1);
			temp->cat.nDesc=strlen(temp->files.desc);
			if(abort)
				return CSizeDesc();//CString();
		}
	}while(FindNextFile(hFind,&fd));
	//Convert(temp, fd);
	Sort(cat);
	sizeDesc.desc=GetDesc(sizeDesc.desc);
	sizeDesc.desc.TrimLeft();
	sizeDesc.desc.TrimRight();
	sizeDesc.desc=sizeDesc.desc.Left(_MAX_CHARS-1);
	sizeDesc.fFileSize=fFileSize;
	return sizeDesc;//desc.Left(_MAX_CHARS-1);
}

void CProgress::Finalize(CCatalog *cat, DWORD parent, BYTE init)
{
	static DWORD index=0;
	index=(init)?0:index;
	if(cat==NULL)
		return;
	CCatalog *next=cat->sub;
	while(next!=NULL)
	{
		m_Progress.StepIt();
		GetDlgItem(IDC_STATIC_CDDRIVE)->SetWindowText(next->files.cFileName);
		next->cat.myIndex=++index;
		if((next->cat.dwFileAttributes & DIRECTORY)==DIRECTORY)
			Finalize(next, index);
		next->cat.parent=parent;
		next=next->next;
	}
}

CString CProgress::GetDesc(CString filePath)
{
	fstream file;
	char data[_MAX_CHARS]={0};
	file.open(filePath,ios::in|ios::binary|ios::nocreate);
	if(!file)
		return CString();
	file.read(data,sizeof(char)*_MAX_CHARS-1);
	file.close();
	return data;
}

DWORD CProgress::Create(CCatalog *cat)
{
	CCatalog *next=cat->sub;
	EncodedCatalog enc;
	DWORD count=0;
	if(next==NULL)
		return 0;
	while(next!=NULL)
	{
		m_Progress.StepIt();
		GetDlgItem(IDC_STATIC_CDDRIVE)->SetWindowText(next->files.cFileName);
		enc=next->cat;
		enc.Write(file);
		next->files.Write(file,next->cat.nFileName,next->cat.nDesc);
		if(file.bad() || abort)
		{
			abort=true;
			return 0;
		}
		if(abort)
			return 0;
		count++;
		if((next->cat.dwFileAttributes & DIRECTORY)==DIRECTORY)
			count+=Create(next);
		next=next->next;
	}
	return count;
}

UINT CProgress::SaveCatalog(LPVOID pParam)
{
	CProgress *pCsd=(CProgress*)pParam;
	char temp[_MAX_PATH]="C:";
	CCatalog *cat=new CCatalog;
	temp[0]=pCsd->m_Drive;
	strcpy(cat->files.cFileName,pCsd->m_Volume);
	cat->cat.dwFileAttributes=DIRECTORY;
	CSizeDesc sizeDesc=pCsd->CreateCatalog(cat,temp);
	strncpy(cat->files.desc,sizeDesc.desc,_MAX_CHARS-1);
	if(pCsd->abort)
	{
		pCsd->MessageBox("Error Creating Catalog\nPlease Insert the Media again and Retry","Error",MB_OK|MB_ICONSTOP);
		pCsd->SendMessage(WM_COMMAND,IDCANCEL);
		AfxEndThread(0);
		return 0;
	}
	pCsd->Finalize(cat,0,1);
	cat->cat.myIndex=0;
	cat->cat.parent=0xFFFFF;//MAXDWORD;
	cat->cat.nFileSizeLow=0;
	cat->cat.dwFileAttributes=DIRECTORY;
	memset((void*)&cat->cat.ftCreationTime,0,sizeof(cat->cat.ftCreationTime));
	cat->cat.nFileName=strlen(cat->files.cFileName);
	cat->cat.nDesc=strlen(sizeDesc.desc);
	//pCsd->MessageBox(pCsd->ShowTree(cat));
	CString path=".\\Catalog\\";
	path+=pCsd->m_Volume;
	path+=".gxc";
	pCsd->file.open(path,ios::out|ios::binary);
	if(!pCsd->file)
	{
		pCsd->MessageBox("Error Creating Catalog","Error",MB_OK|MB_ICONSTOP);
		pCsd->file.close();
		remove(path);
		pCsd->SendMessage(WM_COMMAND,IDCANCEL);
		AfxEndThread(0);
		return 0;
	}
	pCsd->file.write("Gx2",sizeof(char)*3);
	EncodedCatalog enc=cat->cat;
	enc.Write(pCsd->file);
	cat->files.Write(pCsd->file, cat->cat.nFileName, cat->cat.nDesc);
	DWORD count=pCsd->Create(cat);
	cat->FreeTree(cat);
	delete cat;
	pCsd->m_Progress.SetPos(100);
	pCsd->file.close();
	pCsd->bClose=true;
	if(pCsd->abort)
	{
		pCsd->MessageBox("Error Finalizing Catalog","Error",MB_OK|MB_ICONSTOP);
		remove(path);
	}
	else
		pCsd->MessageBox("Catalog Created","Done",MB_OK|MB_ICONINFORMATION);
 	pCsd->SendMessage(WM_COMMAND,IDCANCEL);
	AfxEndThread(0);
	return 0;
}

void CProgress::Sort(CCatalog *cat)
{
	if(cat->sub==NULL || 
		cat->sub->next==NULL)
		return;
	CCatalog *next=cat->sub;
	cat->sub=NULL;
	CCatalog *nDirs=NULL, *nFiles=NULL;
	CCatalog *nD=NULL;
	CCatalog *n=NULL, *temp=NULL;
	do
	{
		temp=next;
		next=next->next;
		temp->next=NULL;
		if((temp->cat.dwFileAttributes & DIRECTORY)==DIRECTORY)
		{
			n=nDirs;
			nD=nDirs;
			if(nDirs==NULL)
				nDirs=temp;
			else if(nDirs->next==NULL)
			{
				if(stricmp(temp->files.cFileName, nD->files.cFileName)<=0)
					nD->next=temp;
				else
				{
					temp->next=nDirs;
					nDirs=temp;
				}
			}
			else
			{
				int count=0;
				while(nD!=NULL && stricmp(temp->files.cFileName,nD->files.cFileName)<=0)
				{
					n=nD;
					nD=nD->next;
					count++;
				}
				if(count)
				{
					temp->next=nD;
					n->next=temp;
				}
				else
				{
					temp->next=nDirs;
					nDirs=temp;
				}
			}
		}
		else
		{
			n=nFiles;
			nD=nFiles;
			if(nFiles==NULL)
				nFiles=temp;
			else if(nFiles->next==NULL)
			{
				if(stricmp(GetFileName(temp->files.cFileName,1), GetFileName(nD->files.cFileName,1))<0)
					nD->next=temp;
				else if(stricmp(GetFileName(temp->files.cFileName,1), GetFileName(nD->files.cFileName,1))>0)
				{
					temp->next=nFiles;
					nFiles=temp;
				}
				else if(stricmp(temp->files.cFileName, nD->files.cFileName)<=0)
					nD->next=temp;
				else
				{
					temp->next=nFiles;
					nFiles=temp;
				}
			}
			else
			{
				while(nD!=NULL && stricmp(GetFileName(temp->files.cFileName,1),GetFileName(nD->files.cFileName,1))<0)
				{
					n=nD;
					nD=nD->next;
				}
				int count=0;
				while(nD!=NULL && stricmp(temp->files.cFileName,nD->files.cFileName)<=0)
				{
					n=nD;
					nD=nD->next;
					count++;
				}
				if(count)
				{
					temp->next=nD;
					n->next=temp;
				}
				else
				{
					if(n->files.cFileName==nD->files.cFileName)
					{
						temp->next=nFiles;
						nFiles=temp;
					}
					else
					{
						temp->next=nD;
						n->next=temp;
					}
				}
			}
		}
	}while(next!=NULL);
	if(nFiles!=NULL)
	{
		cat->sub=nFiles;
		n=nFiles;
		while(n->next!=NULL)
			n=n->next;
		if(nDirs!=NULL)
			n->next=nDirs;
	}
	else
		cat->sub=nDirs;
}

CString CProgress::GetFileName(CString pathName, int style)
{
	TCHAR ch=_T('\\');
	if(style)
		ch=_T('.');
	int pos=pathName.ReverseFind(ch);
	return pathName.Right(pathName.GetLength()-(pos+1));
}

BOOL CProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	abort=false;
	m_Progress.SetRange(0,100);
	char buff[_MAX_PATH]={0};
	getcwd(buff,_MAX_PATH);
	strcat(buff,"\\Catalog");
	mkdir(buff);
	bClose=false;
	m_pthread=AfxBeginThread(SaveCatalog,(LPVOID)this,THREAD_PRIORITY_HIGHEST);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgress::Convert(CCatalog *f, const WIN32_FIND_DATA fd, const CSizeDesc sizeDesc)
{
	strcpy(f->files.cFileName,strupr((char*)fd.cFileName));
	f->cat.dwFileAttributes=GetAttribs(fd.dwFileAttributes);
	f->cat.ftCreationTime=fd.ftCreationTime;
	double fileSize=((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
		?sizeDesc.fFileSize
		:double((fd.nFileSizeHigh*(MAXDWORD+1))+fd.nFileSizeLow);
	while(fileSize>1023.0)
	{
		fileSize/=1024.0;
		f->cat.flag++;
	}
	f->cat.nFileSizeLow=(DWORD)ceil(fileSize);
}

BYTE CProgress::GetAttribs(DWORD attribs)
{
	DWORD dwAttribs[]={FILE_ATTRIBUTE_ARCHIVE, FILE_ATTRIBUTE_COMPRESSED,
		FILE_ATTRIBUTE_DIRECTORY, FILE_ATTRIBUTE_ENCRYPTED, FILE_ATTRIBUTE_HIDDEN,
		FILE_ATTRIBUTE_READONLY, FILE_ATTRIBUTE_SYSTEM};
	BYTE bAttr=0;
	BYTE bAttribs[7]={ARCHIVE, COMPRESSED, DIRECTORY, ENCRYPTED, HIDDEN, READONLY, SYSTEM};
	for(int i=0;i<7;i++)
		bAttr |= ((attribs & dwAttribs[i])==dwAttribs[i])?bAttribs[i]:0;
	return bAttr;
}

CString CProgress::ShowTree(CCatalog *cat)
{
	CString s, t;
	static int count=0;
	if(cat==NULL)
		return s;
	CCatalog *next=cat;
	while(next!=NULL)
	{
		if((next->cat.dwFileAttributes & DIRECTORY)==DIRECTORY)
		{
			s+="\n";
			for(int i=0;i<count;i++)
				s+="\t";
			t.Format("%d. %d. ", next->cat.parent, next->cat.myIndex);
			s+=t;
			s+=next->files.cFileName;
			count++;
			s+=ShowTree(next->sub);
			count--;
		}
		next=next->next;
	}
	return s;
}

CString CProgress::GetMP3Data(CString strPathName)
{
	fstream file;
	ID3 id3;
	CString data;
	file.open(strPathName,ios::in|ios::binary|ios::nocreate);
	if(!file)
		return CString();
	file.seekg(-1*(_MAX_CHARS+1),ios::end);
	file.read((char*)&id3,sizeof(ID3));
	file.close();
	if(!strnicmp(id3.TID, "TAG", 3))
	{
		data=id3.artist;
		if(strlen(id3.title))
		{
			data+=" - ";
			data+=id3.title;
		}
		if(strlen(id3.album))
		{	
			data+=" - ";
			data+=id3.album;
		}
	}
	return data;
}

void CProgress::OnCancel() 
{
	if(!bClose)
	{
		SuspendThread(m_pthread->m_hThread);
		if(MessageBox("This will Stop the Current Catalog. Are you sure ?", "Are you sure ?", MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDYES)
		{
			TerminateThread(m_pthread->m_hThread, 0);
			CDialog::OnCancel();
			SendMessage(WM_COMMAND,ID_VIEW_REFRESH);
			bClose=true;
		}
		else
			ResumeThread(m_pthread->m_hThread);
		return;
	}
	CDialog::OnCancel();
	SendMessage(WM_COMMAND,ID_VIEW_REFRESH);
}
