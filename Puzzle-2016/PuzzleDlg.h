#pragma once
#include "afxwin.h"
class CPuzzleDlg : public CDialogEx
{
public:
	CPuzzleDlg(CWnd* pParent = NULL);
	~CPuzzleDlg();
	enum { IDD = IDD_PUZZLE_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	HICON m_hIcon;
	CMenu m_wMenu;
	CImage pic;
	HBITMAP hbmn;
	int q,w,x,y,level,v;
	int **ArrCheck;
	//int ArrCheck[16][2];
	int Arr2[64][4];
	int ArrF[64][4];
	int complete;
	bool IsLaunchedOpenPic;
	bool IsLaunchedMix;
	bool IsArrCheck;
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OpenPic();
	void LevelButton();
	void Four();
	void Five();
	void Six();
	void Seven();
	void Eight();
	void RecordsButton();
	void MixButton();
	void Restore();
	void Mix(CDC *memDC, CDC *pDC,HBITMAP hbmn);
	bool Check(int a, int b);
	int RandomC();
	int RandomD();	
	int timer;
	virtual void UpdateAllViews(CView* pSender, LPARAM lHint = 0L, CObject* pHint = NULL ){};
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonUp(UINT, CPoint);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};