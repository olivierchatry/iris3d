#pragma once

// CDialogBarAnim dialog
#define TIMER_ANIMATION 501
class CDialogBarAnim : public CDialogBar
{
  DECLARE_DYNAMIC(CDialogBarAnim)

public:
  CDialogBarAnim(CWnd *pParent = NULL); // standard constructor
  virtual ~CDialogBarAnim();

  // Dialog Data
  enum
  {
    IDD = IDD_DIALOGANIM
  };
  void SetNumAnim(int num_anim);

protected:
  virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
  int _num_anim;
  int _current_anim;
  int _fps;
  int _timer_id;
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
  void Play();
  afx_msg void OnTimer(UINT nIDEvent);
  void Stop();
  int GetCurrentAnim() { return _current_anim; }
};
