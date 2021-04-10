#pragma once

// CAnimToolBar

class CAnimToolBar : public CToolBar
{
  DECLARE_DYNAMIC(CAnimToolBar)

public:
  CAnimToolBar();
  virtual ~CAnimToolBar();
  CSliderCtrl m_slider;
  CStatic m_information;
  CFont m_font;

protected:
  DECLARE_MESSAGE_MAP()
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
};
