/*
    Copyright (c) Raja Jamwal, <linux1@zoho.com>

    This file is part of PPTMote.

    PPTMote is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PPTMote is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with PPTMote.  If not, see <http://www.gnu.org/licenses/>.*/

#include "libpptmote.h"
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <windows.h>
#include "resource.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")

HWND hWnd = NULL;

char * key_mapping [] = {"Left mouse button","Right mouse button","Control-break processing","Middle mouse button (three-button mouse)","X1 mouse button ","X2 mouse button ","Undefined ","BACKSPACE key ","TAB key ","Reserved ","CLEAR key ", "--", "--", "ENTER key ","Undefined ","Undefined ","SHIFT key ","CTRL key ","ALT key ","PAUSE key ","CAPS LOCK key ","IME Kana mode,IME Hanguel mode,IME Hangul mode ","Undefined ","IME Junja mode ","IME final mode ","IME Hanja mode,IME Kanji mode ","Undefined ","ESC key ","IME convert ","IME nonconvert ","IME accept ","IME mode change request ","SPACEBAR ","PAGE UP key ","PAGE DOWN key ","END key ","HOME key ","LEFT ARROW key ","UP ARROW key ","RIGHT ARROW key ","DOWN ARROW key ","SELECT key ","PRINT key ","EXECUTE key ","PRINT SCREEN key ","INS key ","DEL key ","HELP key ","0 key","1 key","2 key","3 key","4 key","5 key","6 key ","7 key ","8 key ","9 key ","Undefined ","Undefined ","Undefined ","Undefined ","Undefined ","Undefined ","Undefined ","A key ","B key ","C key ","D key ","E key ","F key ","G key ","H key ","I key ","J key ","K key ","L key ","M key ","N key ","O key ","P key ","Q key ","R key ","S key ","T key ","U key ","V key ","W key ","X key ","Y key ","Z key ","Left Windows key (Natural keyboard) ","Right Windows key (Natural keyboard) ","Applications key (Natural keyboard) ","Reserved ","Computer Sleep key ","Numeric keypad 0 key ","Numeric keypad 1 key ","Numeric keypad 2 key ","Numeric keypad 3 key ","Numeric keypad 4 key ","Numeric keypad 5 key ","Numeric keypad 6 key ","Numeric keypad 7 key ","Numeric keypad 8 key ","Numeric keypad 9 key ","Multiply key ","Add key ","Separator key ","Subtract key ","Decimal key ","Divide key ","F1 key ","F2 key ","F3 key ","F4 key ","F5 key ","F6 key ","F7 key ","F8 key ","F9 key ","F10 key ","F11 key ","F12 key ","F13 key ","F14 key ","F15 key ","F16 key ","F17 key ","F18 key ","F19 key ","F20 key ","F21 key ","F22 key ","F23 key ","F24 key ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","NUM LOCK key ","SCROLL LOCK key ","OEM specific ","OEM specific ","OEM specific ","OEM specific ","OEM specific ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Unassigned ","Left SHIFT key ","Right SHIFT key ","Left CONTROL key ","Right CONTROL key ","Left MENU key ","Right MENU key ","Browser Back key ","Browser Forward key ","Browser Refresh key ","Browser Stop key ","Browser Search key ","Browser Favorites key ","Browser Start and Home key ","Volume Mute key ","Volume Down key ","Volume Up key ","Next Track key ","Previous Track key ","Stop Media key ","Play/Pause Media key ","Start Mail key ","Select Media key ","Start Application 1 key ","Start Application 2 key ","Reserved ","Reserved ","Not-Fixed ","For any country/region, the '+' key ","For any country/region, the ',' key ","For any country/region, the '-' key ","For any country/region, the '.' key ","Not-Fixed ","Not-Fixed ","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved","Reserved11","Reserved","Reserved","Reserved","Reserved","Unassigned","Unassigned","Unassigned","US standard keyboard, the '[{' key","US standard keyboard, the '\|' key","US standard keyboard, the ']}' key","US standard keyboard, the 'single-quote/double-quote' key","Not-Fixed","Reserved","OEM specific","angle bracket key or the backslash key on the RT 102-key keyboard","OEM specific","OEM specific","IME PROCESS key","OEM specific","pass Unicode characters as if they were keystrokes (read VK_PACKET)","Unassigned","OEM specific","OEM specific","OEM specific","OEM specific","OEM specific","OEM specific","OEM specific","OEM specific","OEM specific","OEM specific","OEM specific","OEM specific","OEM specific","Attn key","CrSel key","ExSel key","Erase EOF key","Play key","Zoom key","Reserved","PA1 key","Clear key" };

void send_to_wnd (HWND hWnd ,char * szBuffer)
{
   HWND hEdit = GetDlgItem (hWnd, IDC_EDIT2);
   int ndx = GetWindowTextLength (hEdit);
   SetFocus (hEdit);
   SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
   SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) ((LPSTR) szBuffer));
}


int log (char * msg)
{
    send_to_wnd (hWnd, msg);
    return 0;
}

DWORD WINAPI start_mote (HWND hDlg)
{
    while (1)
    {
        if(connect_show_prompt ()==1)
        {
            key_emulate ();
        }
    }

    return 0;
}
/* Routine	   : AppDlgProc
   Description : Dialog procedure for the Application Dialog	
*/

HANDLE hThread  = NULL;
DWORD hThreadID = NULL;

BOOL CALLBACK AppDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_INITDIALOG:
            hWnd = hwnd;
            log ("PPMote\n");
            log ("Trivial remote control using OBEX protocol response\n");
			log ("https://github.com/linuxexp/\n");
            SetDlgItemText (hwnd, IDC_EDIT1, "pptmote");
            set_callback (log);
            
            for (int i=0; i<(sizeof(key_mapping)/sizeof(key_mapping[0]));i++)
            {
                SendMessage (GetDlgItem(hwnd, IDC_COMBO2), CB_ADDSTRING, 
                             0, (LPARAM) key_mapping[i]);
            }

        return TRUE;

        case WM_CLOSE:
        {
            CoUninitialize();
            TerminateThread (hThread, 0);
            EndDialog(hwnd, 0);
        }
        return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case ID_OK:
                    char dev_name [512];
                    GetDlgItemText (hwnd, IDC_EDIT1, dev_name, 512);
                    set_device (dev_name);
                    init_pptmote ();
                    hThread = CreateThread (NULL,NULL,(LPTHREAD_START_ROUTINE)start_mote,hwnd,0,&hThreadID);
                    EnableWindow (GetDlgItem(hwnd, ID_OK), FALSE);
                    //connect_show_prompt ();
                return TRUE;

                case IDC_COMBO2:
                    if (HIWORD (wParam) == CBN_SELCHANGE)
                    {
                        unsigned int key = SendMessage ((HWND)lParam, CB_GETCURSEL, 0, 0);
                        log ("Changing key ...\n");
                        set_key_code (key);
                    }
                return TRUE;

				case IDC_CHECKBOX1:
                    if (HIWORD (wParam) == BN_CLICKED)
                    {
                        unsigned int key = SendMessage ((HWND)lParam, BM_GETCHECK, 0, 0);
						//BST_CHECKED
						if ( key == BST_CHECKED)
						{
							log ("Will send actual data along with file prompt ...\n");
							set_file_send (TRUE);
						}
						else
						{
							log ("Only file prompt will be shown ...\n");
							set_file_send (FALSE);
						}
                        
                    }
                return TRUE;
            }
        break;

        default:
        return FALSE;
    }
    
    return TRUE;
}

/* Routine	   : WinMain
   Description : Application Start	
*/

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int main()
{
    CoInitializeEx(NULL,NULL);InitCommonControls();
    DialogBoxA(GetModuleHandle(NULL),MAKEINTRESOURCEA(IDD_APP), NULL, AppDlgProc);
    return 0;
}
