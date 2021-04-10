#pragma once

INT_PTR CALLBACK AboutBoxDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ExportDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HTREEITEM AddNode(HWND hwnd, int dlg_item, CHAR *str, HTREEITEM parent);
