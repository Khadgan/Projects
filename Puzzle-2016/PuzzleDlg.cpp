#include "stdafx.h"
#include "resource.h"
#include "afxdialogex.h"
#include "Puzzle.h"
#include "PuzzleDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CPuzzleDlg::CPuzzleDlg(CWnd* pParent)
	: CDialogEx(CPuzzleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CPuzzleDlg::~CPuzzleDlg(){}
void CPuzzleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CPuzzleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_OPENPIC,OpenPic)
	ON_COMMAND(ID_MIX,MixButton)
	ON_COMMAND(ID_RESTORE,Restore)
	ON_COMMAND(ID_LEVEL,LevelButton)
	ON_COMMAND(ID_RECORDS,RecordsButton)
	ON_COMMAND(ID_4X4,Four)
	ON_COMMAND(ID_5X5,Five)
	ON_COMMAND(ID_6X6,Six)
	ON_COMMAND(ID_7X7,Seven)
	ON_COMMAND(ID_8X8,Eight)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()
BOOL CPuzzleDlg::OnInitDialog()
{
	CWnd* m_hWnd = AfxGetApp()->GetMainWnd();
	m_hWnd->MoveWindow(0,0,810,650);	
	CDialogEx::OnInitDialog();
	srand(time(0));
	IsLaunchedOpenPic = false;
	IsLaunchedMix = false;
	IsArrCheck = true;
	level = 0;
	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		
	return TRUE; 
}

int CPuzzleDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_wMenu.LoadMenu(IDR_MENU);
	SetMenu(&m_wMenu);
	return 0;	
}

int CPuzzleDlg::RandomC()
{	
	int e=0,g=level;
	/*if((rand()%4+1)==1)
		e=0;
	else if((rand()%3+1)==1)
			e=800-(x*3);
		else	if((rand()%2+1)==1)
				e=800-(x*2);
			else
				e=800-x;
	return e;*/

	for(int i=0; i<level; i++)
	{
		if((rand()%g+1)==1)
			return e;
		else
		{
			g--;
			e=e+x;
		}
	}
}
int CPuzzleDlg::RandomD()
{	
	int f=0,h=level;
	/*if((rand()%4+1)==1)
		f=0;
	else	if((rand()%3+1)==1)
			f=600-(y*3);
		else	if((rand()%2+1)==1)
				f=600-(y*2);
			else
				f=600-y;
	return f;*/

	for(int i=0; i<level; i++)
	{
		if((rand()%h+1)==1)
			return f;
		else
		{
			h--;
			f=f+y;
		}
	}
}
bool CPuzzleDlg::Check(int a, int b)
{
	if (IsArrCheck)
	{
		v=level*level;
		ArrCheck = new int *[v];
		for (int i = 0; i < v; i++) 
		{
			ArrCheck[i] = new int [2];
		}
		IsArrCheck = false;
	}

	if (IsLaunchedMix)
	{
		IsLaunchedMix = false;
		int p;
		for (int i = 0; i<level*level;i++)
		{
			ArrCheck[i][0]=-1;
			ArrCheck[i][1]=-1;
		}
		for (int i=0; i<level*level; i++)
		{
			p=0;
			do
			{
				if(ArrCheck[i][0]==a && ArrCheck[i][1]==b)
				{
					p++;
					return false;
				}
				else
				{	
					if(ArrCheck[i][0]==(-1) && ArrCheck[i][1]==(-1))		
					{
						ArrCheck[i][0]=a;
						ArrCheck[i][1]=b;
						return true;	
					}
					else
					{
						break;
					}
				}
			} while(p!=1);
			if(p>=1)
			{
				break;
			}
		}
	}
	else
	{
		int r;
		for (int i=0; i<level*level; i++)
		{
			r=0;
			do
			{
				if(ArrCheck[i][0]==a && ArrCheck[i][1]==b)
				{
					r++;
					return false;
				}
				else
				{	
					if(ArrCheck[i][0]==(-1) && ArrCheck[i][1]==(-1))		
					{
						ArrCheck[i][0]=a;
						ArrCheck[i][1]=b;
						return true;
					}
					else
					{
						break;
					}
				}
			} while(r!=1);
			if(r>=1)
			{
				break;
			}
		}
	}
}
void CPuzzleDlg::Mix(CDC *memDC, CDC *pDC,HBITMAP hbmn)
{
	int n,m,z,t=0,w,v;
	if (level==4 || 5 || 8)
	{
		w=800;
		v=600;
	}
	if (level==6)
	{
		w=798;
		v=600;
	}
	if (level==7)
	{
		w=798;
		v=595;
	}	
	for (int i = 0; i < w+1-x;i+=x)
	{
		for (int j = 0; j < v+1-y;j+=y)
		{			
			do
			{	
				z=0;			
				n=RandomC();
				m=RandomD();
				if (CPuzzleDlg::Check(n,m)==true)
				{
					z++;
					if(n==w-x && m==v-y)
					{
						//pDC->StretchBlt(i,j,x,y,memDC,n,m,pic.GetWidth()/level,pic.GetHeight()/level,WHITENESS);
						pDC->BitBlt(i,j,x,y,memDC,n,m,WHITENESS);
					}
					else
					{
						//pDC->StretchBlt(i,j,x,y,memDC,n,m,pic.GetWidth()/level,pic.GetHeight()/level,SRCCOPY);
						pDC->BitBlt(i,j,x,y,memDC,n,m,SRCCOPY);
					}
				}
			} while(z!=1);
			Arr2[t][0]=i;
			Arr2[t][1]=j;
			Arr2[t][2]=n;
			Arr2[t][3]=m;
			if(n==w-x && m==v-y)
			{
				q=t;
			}
			t++;
		}
	}
}
void CPuzzleDlg::OpenPic()
{	
	CFileDialog fileDialog(TRUE,NULL,NULL, OFN_HIDEREADONLY,_T("Файлы картинок (*.png; *.bmp; *.jpeg; *.jpg)|*.png; *.bmp; *.jpeg; *.jpg|"));
	int result = fileDialog.DoModal();
	if (result == IDOK)
	{	
		IsLaunchedMix = false;
		pic.Destroy();
		pic.Load(fileDialog.GetPathName());
		//hbmn = (HBITMAP)LoadImage(NULL,fileDialog.GetPathName(),IMAGE_BITMAP,NULL,NULL,LR_LOADFROMFILE);
		hbmn = pic;
		IsLaunchedOpenPic = true;
		Invalidate(0);
	}
}

void CPuzzleDlg::LevelButton()
{
	
}

void CPuzzleDlg::Four()
{
	level=4;
	x=200;
	y=150;
	MixButton();
}

void CPuzzleDlg::Five()
{
	level=5;
	x=160;
	y=120;
	MixButton();
}

void CPuzzleDlg::Six()
{
	level=6;
	x=133;
	y=100;
	MixButton();
}

void CPuzzleDlg::Seven()
{
	level=7;
	x=114;
	y=85;
	MixButton();
}

void CPuzzleDlg::Eight()
{
	level=8;
	x=100;
	y=75;
	MixButton();
}

void CPuzzleDlg::MixButton()
{
	if(IsLaunchedOpenPic)
	{
		if (level == 0)
			Four();
		IsLaunchedMix = true;
		if (IsArrCheck == false)
		{
			for (int i = 0; i < v; i++)
			{
				delete []ArrCheck[i];
			}
			delete []ArrCheck;
			IsArrCheck = true;
		}
		timer = SetTimer(1,1000,NULL);
		Invalidate(0);
	}
	else
		MessageBox(_T("Сперва выберите картинку."));
}
void CPuzzleDlg::Restore()
{
	if(IsLaunchedOpenPic)
	{
		IsLaunchedMix = false;
		Invalidate(0);
	}
	else
		MessageBox(_T("Сперва выберите картинку."));
}

void CPuzzleDlg::RecordsButton()
{
	
}

void CPuzzleDlg::OnLButtonUp(UINT, CPoint)
{
	if(IsLaunchedOpenPic && IsLaunchedMix)
	{
		POINT pt;
		int temp;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		CClientDC *pdc = new CClientDC(this);
		CRect r_CL;
		GetClientRect(&r_CL);
		CDC memDC;
		if (!memDC.CreateCompatibleDC(pdc))
			return;
		CBitmap* oldBmp;
		CBitmap memBmp;
		if (!memBmp.CreateCompatibleBitmap(pdc, r_CL.Width(), r_CL.Height()))
			return;
		oldBmp = memDC.SelectObject(&memBmp);
		memDC.SelectObject(hbmn);
		complete=0;
		for (int i=0;i<level*level;i++)
		{
			if((pt.x >= Arr2[i][0]) && (pt.x <= Arr2[i][0]+x) && (pt.y >= Arr2[i][1]) && (pt.y <= Arr2[i][1]+y))
			{
				if(((Arr2[i][0] == Arr2[q][0]) && (abs(Arr2[i][1]-Arr2[q][1])==y)) || ((Arr2[i][1] == Arr2[q][1]) && (abs(Arr2[i][0]-Arr2[q][0])==x)))
				{
					temp = Arr2[i][0];
					Arr2[i][0] = Arr2[q][0];
					Arr2[q][0] = temp;
					temp = Arr2[i][1];
					Arr2[i][1] = Arr2[q][1];
					Arr2[q][1] = temp;
					//pdc->StretchBlt(Arr2[i][0],Arr2[i][1],x,y,&memDC,Arr2[i][2],Arr2[i][3],pic.GetWidth()/level,pic.GetHeight()/level,SRCCOPY);
					pdc->BitBlt(Arr2[i][0],Arr2[i][1],x,y,&memDC,Arr2[i][2],Arr2[i][3],SRCCOPY);
					//pdc->StretchBlt(Arr2[q][0],Arr2[q][1],x,y,&memDC,Arr2[q][2],Arr2[q][3],pic.GetWidth()/level,pic.GetHeight()/level,WHITENESS);
					pdc->BitBlt(Arr2[q][0],Arr2[q][1],x,y,&memDC,Arr2[q][2],Arr2[q][3],WHITENESS);
					for (int i=0;i<level*level;i++)
					{
						if((Arr2[i][0]==Arr2[i][2]) && (Arr2[i][1]==Arr2[i][3]))
						{
							complete++;
						}
					}
					if(complete==level*level)
					{
						MessageBox(_T("Картинка собрана!!!\n"));
						IsLaunchedMix=false;
						Invalidate(0);
					}
				}
			}
		}
		memDC.SelectObject(oldBmp);
		memBmp.DeleteObject();
		memDC.DeleteDC();
		delete pdc;
	}
}
void CPuzzleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CClientDC *pdc = new CClientDC(this);
		CRect temp;
		GetClientRect(temp);
		CRect r_CL(0, 0, temp.right, temp.bottom);		
		CDC memDC;
		if (!memDC.CreateCompatibleDC(pdc))
		return;
		CBitmap* oldBmp;
		CBitmap memBmp;
		if (!memBmp.CreateCompatibleBitmap(pdc, r_CL.Width(), r_CL.Height()))
		return;
		oldBmp = memDC.SelectObject(&memBmp);
		if(IsLaunchedOpenPic)
		{
			memDC.SelectObject(hbmn);		
			if(IsLaunchedMix)
			{	
				pdc->BitBlt(r_CL.left, r_CL.top, r_CL.Width(), r_CL.Height(), &memDC, 0, 0, WHITENESS);
				Mix(&memDC,pdc,hbmn);
				IsLaunchedMix = true;
			}
			else
				//pdc->StretchBlt(r_CL.left, r_CL.top, r_CL.Width(), r_CL.Height(), &memDC, 0, 0, pic.GetWidth(), pic.GetHeight(),SRCCOPY);
				pdc->BitBlt(r_CL.left, r_CL.top, r_CL.Width(), r_CL.Height(), &memDC, 0, 0, SRCCOPY);
		}
		memDC.SelectObject(oldBmp);
		memBmp.DeleteObject();
		memDC.DeleteDC();
		delete pdc;
	}
}
HCURSOR CPuzzleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPuzzleDlg::OnClose()
{
	if (IsArrCheck == false)
	{
		for (int i = 0; i < v; i++)
		{
			delete []ArrCheck[i];
		}
		delete []ArrCheck;
	}
	CDialogEx::OnClose();
}


void CPuzzleDlg::OnTimer(UINT_PTR nIDEvent)
{
	

	CDialogEx::OnTimer(nIDEvent);
}
