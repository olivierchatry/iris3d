// OC : creating file, adding about proc
// OC : adding configuration proc
#include <stdafx.h>
extern HINSTANCE hInstance;

/*
 *	Usefull function to add node to treeview
 */
HTREEITEM AddNode(HWND hwnd, int dlg_item, CHAR *str, HTREEITEM parent)
{
  TV_INSERTSTRUCT tvinsert;

  tvinsert.hParent = parent;
  if (parent)
    tvinsert.hInsertAfter = TVI_LAST;
  tvinsert.item.mask = TVIF_TEXT;
  tvinsert.item.pszText = str;
  return (HTREEITEM)SendDlgItemMessage(hwnd, dlg_item, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
}

/*
 *	Callback procedure for about box
 */
INT_PTR CALLBACK AboutBoxDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_INITDIALOG:
    CenterWindow(hWnd, GetParent(hWnd));
    break;
  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case IDOK:
      EndDialog(hWnd, 1);
      break;
    }
    break;
  default:
    return FALSE;
  }
  return TRUE;
}

void EnableDialogObjectAnimationItem(HWND hWnd, bool animation_enabled)
{
  EnableWindow(GetDlgItem(hWnd, IDC_SPIN_BEGIN), animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_SPIN_END), animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_EDIT_BEGIN), animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_EDIT_END), animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_SPIN_STATIC), !animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_EDIT_STATIC), !animation_enabled);
}

void EnableDialogBonetAnimationItem(HWND hWnd, bool animation_enabled)
{
  EnableWindow(GetDlgItem(hWnd, IDC_SPIN_BONE_BEGIN), animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_SPIN_BONE_END), animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_EDIT_BONE_BEGIN), animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_EDIT_BONE_END), animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_EDIT_BONE_FILE), animation_enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_BUTTON_BONE_FILE), animation_enabled);
}

void EnableDialogObjectItem(HWND hWnd, bool enabled)
{
  EnableWindow(GetDlgItem(hWnd, IDC_CHECKONLYSELECTED), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_CHECKSAMPLEANIMATION), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_CHECKSKIN), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_CHECKONLYSELECTED), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_SPIN_BEGIN), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_SPIN_END), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_EDIT_BEGIN), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_EDIT_END), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_RADIOSAMPLEMATRIX), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_RADIOSAMPLEVERTEX), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_SPIN_STATIC), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_EDIT_STATIC), enabled);
  EnableWindow(GetDlgItem(hWnd, IDC_CHECK_STICHSTRIP), enabled);
  if (enabled)
    EnableDialogObjectAnimationItem(hWnd, IsDlgButtonChecked(hWnd, IDC_CHECKSAMPLEANIMATION) == BST_CHECKED);
}

/*
 *	Callback procedure for configuration
 */
INT_PTR CALLBACK ExportDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  ISpinnerControl *spin;
  Interval animRange;
  ImdExp *exp = (ImdExp *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
  switch (msg)
  {
  case WM_INITDIALOG:
    exp = (ImdExp *)lParam;
    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)lParam);
    CenterWindow(hWnd, GetParent(hWnd));
    animRange = exp->GetInterface()->GetAnimRange();
    // Setup the spinner controls for the controller key sample rate
    spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_BEGIN));
    spin->LinkToEdit(GetDlgItem(hWnd, IDC_EDIT_BEGIN), EDITTYPE_INT);
    spin->SetLimits(animRange.Start() / GetTicksPerFrame(), animRange.End() / GetTicksPerFrame(), TRUE);
    spin->SetScale(1.0f);
    spin->SetValue(animRange.Start() / GetTicksPerFrame(), FALSE);
    ReleaseISpinner(spin);

    // Setup the spinner controls for the mesh definition sample rate
    spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_END));
    spin->LinkToEdit(GetDlgItem(hWnd, IDC_EDIT_END), EDITTYPE_INT);
    spin->SetLimits(animRange.Start() / GetTicksPerFrame(), animRange.End() / GetTicksPerFrame(), TRUE);
    spin->SetScale(1.0f);
    spin->SetValue(animRange.End() / GetTicksPerFrame(), FALSE);
    ReleaseISpinner(spin);

    spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_BONE_BEGIN));
    spin->LinkToEdit(GetDlgItem(hWnd, IDC_EDIT_BONE_BEGIN), EDITTYPE_INT);
    spin->SetLimits(animRange.Start() / GetTicksPerFrame(), animRange.End() / GetTicksPerFrame(), TRUE);
    spin->SetScale(1.0f);
    spin->SetValue(animRange.Start() / GetTicksPerFrame(), FALSE);
    ReleaseISpinner(spin);

    spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_BONE_END));
    spin->LinkToEdit(GetDlgItem(hWnd, IDC_EDIT_BONE_END), EDITTYPE_INT);
    spin->SetLimits(animRange.Start() / GetTicksPerFrame(), animRange.End() / GetTicksPerFrame(), TRUE);
    spin->SetScale(1.0f);
    spin->SetValue(animRange.End() / GetTicksPerFrame(), FALSE);
    ReleaseISpinner(spin);

    // ...
    spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_STATIC));
    spin->LinkToEdit(GetDlgItem(hWnd, IDC_EDIT_STATIC), EDITTYPE_INT);
    spin->SetLimits(animRange.Start() / GetTicksPerFrame(), animRange.End() / GetTicksPerFrame(), TRUE);
    spin->SetScale(1.0f);
    spin->SetValue(animRange.Start() / GetTicksPerFrame(), FALSE);
    ReleaseISpinner(spin);
    {
      HIMAGELIST himagelist = ImageList_Create(16, 16, ILC_COLOR16, 1, 10);
      HBITMAP hbitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAPTREE)); // load the picture from the resource
      ImageList_Add(himagelist, hbitmap, NULL);
      DeleteObject(hbitmap);
      SendDlgItemMessage(hWnd, IDC_TREESCENE, TVM_SETIMAGELIST, 0, (LPARAM)himagelist);
      exp->PopulateTreeView(hWnd);
    }
    CheckDlgButton(hWnd, IDC_CHECKSKIN, exp->_plugin_config._export_skin ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_CHECKBONES, exp->_plugin_config._export_bones ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_CHECKSAMPLEANIMATION, exp->_plugin_config._sample_animation ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_CHECKONLYSELECTED, exp->_plugin_config._only_selected ? BST_CHECKED : BST_UNCHECKED);
    SetDlgItemText(hWnd, IDC_EDIT_BONE_FILE, exp->_bone_file_name.c_str());
    CheckDlgButton(hWnd, IDC_CHECK3DVIEWER, exp->_plugin_config._3d_window ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_CHECKLOGWINDOW, exp->_plugin_config._log_window ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_CHECKDOSLOGWINDOW, exp->_plugin_config._dos_log_window ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_RADIOSAMPLEMATRIX, exp->_plugin_config._sample_matrix ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_RADIOSAMPLEVERTEX, exp->_plugin_config._sample_matrix ? BST_UNCHECKED : BST_CHECKED);
    CheckDlgButton(hWnd, IDC_CHECKEXPORTOBJECTDATA, exp->_plugin_config._export_object ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_CHECK_STICHSTRIP, exp->_plugin_config._export_skin ? BST_CHECKED : BST_UNCHECKED);
    EnableDialogObjectItem(hWnd, exp->_plugin_config._export_object);
    EnableDialogBonetAnimationItem(hWnd, exp->_plugin_config._export_bones);
    break;
  case CC_SPINNER_CHANGE:
    spin = (ISpinnerControl *)lParam;
    break;

  case WM_COMMAND:
    switch (LOWORD(wParam))
    {
    case IDOK:
      exp->_plugin_config._export_skin = IsDlgButtonChecked(hWnd, IDC_CHECKSKIN) == BST_CHECKED;
      exp->_plugin_config._export_bones = IsDlgButtonChecked(hWnd, IDC_CHECKBONES) == BST_CHECKED;
      exp->_plugin_config._sample_animation = IsDlgButtonChecked(hWnd, IDC_CHECKSAMPLEANIMATION) == BST_CHECKED;
      exp->_plugin_config._3d_window = IsDlgButtonChecked(hWnd, IDC_CHECK3DVIEWER) == BST_CHECKED;
      exp->_plugin_config._log_window = IsDlgButtonChecked(hWnd, IDC_CHECKLOGWINDOW) == BST_CHECKED;
      exp->_plugin_config._dos_log_window = IsDlgButtonChecked(hWnd, IDC_CHECKDOSLOGWINDOW) == BST_CHECKED;
      exp->_plugin_config._sample_matrix = IsDlgButtonChecked(hWnd, IDC_RADIOSAMPLEMATRIX) == BST_CHECKED;
      exp->_plugin_config._export_object = IsDlgButtonChecked(hWnd, IDC_CHECKEXPORTOBJECTDATA) == BST_CHECKED;
      exp->_plugin_config._stich_strip = IsDlgButtonChecked(hWnd, IDC_CHECK_STICHSTRIP) == BST_CHECKED;
      if (!exp->_plugin_config._sample_animation)
      {
        spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_STATIC));
        exp->SetBeginFrame(spin->GetIVal());
        exp->SetEndFrame(spin->GetIVal());
        ReleaseISpinner(spin);
      }
      else
      {
        spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_BEGIN));
        exp->SetBeginFrame(spin->GetIVal());
        ReleaseISpinner(spin);
        spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_END));
        exp->SetEndFrame(spin->GetIVal());
        ReleaseISpinner(spin);
      }
      spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_BONE_BEGIN));
      exp->_plugin_config._begin_bone_frame = (spin->GetIVal());
      ReleaseISpinner(spin);
      spin = GetISpinner(GetDlgItem(hWnd, IDC_SPIN_BONE_END));
      exp->_plugin_config._end_bone_frame = (spin->GetIVal());
      ReleaseISpinner(spin);

      {
        TCHAR bone_file[MAX_PATH] = {0};
        GetDlgItemText(hWnd, IDC_EDIT_BONE_FILE, bone_file, MAX_PATH - 1);
        exp->_bone_file_name = bone_file;
      }
      EndDialog(hWnd, 1);
      break;
    case IDCANCEL:
      EndDialog(hWnd, 0);
      break;
    case IDC_CHECKSAMPLEANIMATION:
      EnableDialogObjectAnimationItem(hWnd, IsDlgButtonChecked(hWnd, IDC_CHECKSAMPLEANIMATION) == BST_CHECKED);
      break;
    case IDC_CHECKEXPORTOBJECTDATA:
      EnableDialogObjectItem(hWnd, IsDlgButtonChecked(hWnd, IDC_CHECKEXPORTOBJECTDATA) == BST_CHECKED);
      break;
    case IDC_CHECKBONES:
      EnableDialogBonetAnimationItem(hWnd, IsDlgButtonChecked(hWnd, IDC_CHECKBONES) == BST_CHECKED);
      break;
    }
    break;
  default:
    return FALSE;
  }
  return TRUE;
}
