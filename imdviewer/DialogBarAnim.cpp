// DialogBarAnim.cpp : implementation file
//

#include "stdafx.h"
#include "ImdViewer.h"
#include "ImdViewerDoc.h"
#include "DialogBarBoneList.h"
#include "DialogBarMeshInfo.h"
#include "DialogBarBoneTree.h"
#include "DialogBarAnim.h"
#include "DialogBarConfig.h"


// CDialogBarAnim dialog

IMPLEMENT_DYNAMIC(CDialogBarAnim, CDialogBar)
CDialogBarAnim::CDialogBarAnim(CWnd* pParent /*=NULL*/)
	: CDialogBar(), _current_anim(0), _num_anim(0)
{
}

CDialogBarAnim::~CDialogBarAnim()
{
}

void	CDialogBarAnim::SetNumAnim(int num_anim)
{
	CSliderCtrl	*pSliderCtrl = (CSliderCtrl	*) GetDlgItem(IDC_SLIDERANIM);
	ASSERT (pSliderCtrl != 0);
	_num_anim = num_anim;
	_current_anim = 0;
	pSliderCtrl->SetRange(0, _num_anim - 1, TRUE);
	pSliderCtrl->SetPos(_current_anim);
	CButton	*pButtonStop = (CButton *) GetDlgItem(ID_ANIM_STOP);
	CButton	*pButtonPlay = (CButton *) GetDlgItem(ID_ANIM_PLAY);
	pButtonStop->EnableWindow(TRUE);
	pButtonPlay->EnableWindow(TRUE);
}

void CDialogBarAnim::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogBarAnim, CDialogBar)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogBarAnim message handlers

void CDialogBarAnim::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl	*pSliderCtrl = (CSliderCtrl	*) GetDlgItem(IDC_SLIDERANIM);
	ASSERT (pSliderCtrl != 0);
	_current_anim = pSliderCtrl->GetPos();
	CString	oStrFrame;
	oStrFrame.Format("Frame %d/%d", _current_anim, _num_anim - 1);
	CStatic		*pStaticFrame = (CStatic *) GetDlgItem(IDC_STATICFRAME);
	pStaticFrame->SetWindowText(oStrFrame);
	CDialogBar::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDialogBarAnim::Play()
{
	CButton	*pButtonStop = (CButton *) GetDlgItem(ID_ANIM_STOP);
	CButton	*pButtonPlay = (CButton *) GetDlgItem(ID_ANIM_PLAY);
	pButtonStop->EnableWindow(TRUE);
	pButtonPlay->EnableWindow(FALSE);
	_timer_id = (int) SetTimer(TIMER_ANIMATION, 1000 / 60, 0);
}

void CDialogBarAnim::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == _timer_id)
	{
		_current_anim ++;
		if (_current_anim >= _num_anim )
			_current_anim = 0;
		CSliderCtrl	*pSliderCtrl = (CSliderCtrl	*) GetDlgItem(IDC_SLIDERANIM);
		pSliderCtrl->SetPos(_current_anim);
		CString	oStrFrame;
		oStrFrame.Format("Frame %d/%d", _current_anim, _num_anim - 1);
		CStatic		*pStaticFrame = (CStatic *) GetDlgItem(IDC_STATICFRAME);
		pStaticFrame->SetWindowText(oStrFrame);
	}
	CDialogBar::OnTimer(nIDEvent);
}

void CDialogBarAnim::Stop()
{
	CButton	*pButtonStop = (CButton *) GetDlgItem(ID_ANIM_STOP);
	CButton	*pButtonPlay = (CButton *) GetDlgItem(ID_ANIM_PLAY);
	pButtonStop->EnableWindow(FALSE);
	pButtonPlay->EnableWindow(TRUE);
	KillTimer(_timer_id);
}
