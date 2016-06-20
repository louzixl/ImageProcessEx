; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLowPassedDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ImageProcessEx.h"
LastPage=0

ClassCount=10
Class1=CImageProcessExApp
Class2=CImageProcessExDoc
Class3=CImageProcessExView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_DIALOG_LINEARTRANS
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG_HISTOEQUIVALIZE
Class5=CChildFrame
Class6=CAboutDlg
Class7=CRowToBmpDlg
Resource4=IDR_IMAGEPTYPE
Class8=CLinearTransDlg
Resource5=IDD_DIALOG_RAWTOBMP
Class9=CHistoEquivalize
Resource6=IDD_ABOUTBOX
Class10=CLowPassedDlg
Resource7=IDD_DIALOG_LOWPASS

[CLS:CImageProcessExApp]
Type=0
HeaderFile=ImageProcessEx.h
ImplementationFile=ImageProcessEx.cpp
Filter=N

[CLS:CImageProcessExDoc]
Type=0
HeaderFile=ImageProcessExDoc.h
ImplementationFile=ImageProcessExDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC

[CLS:CImageProcessExView]
Type=0
HeaderFile=ImageProcessExView.h
ImplementationFile=ImageProcessExView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=ID_LOWPASS


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=ImageProcessEx.cpp
ImplementationFile=ImageProcessEx.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_IMAGEPTYPE]
Type=1
Class=CImageProcessExView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_RAWTOBMP
Command18=ID_LINEARENHENCE
Command19=ID_HISTOEQUIVALIZE
Command20=ID_LOWPASS
Command21=ID_WINDOW_NEW
Command22=ID_WINDOW_CASCADE
Command23=ID_WINDOW_TILE_HORZ
Command24=ID_WINDOW_ARRANGE
Command25=ID_APP_ABOUT
CommandCount=25

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DIALOG_RAWTOBMP]
Type=1
Class=CRowToBmpDlg
ControlCount=20
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_RAWFILE,static,1342308352
Control4=IDC_EDIT_RAWCIN,edit,1350631552
Control5=IDC_BUTTON_RAWVIEW,button,1342242816
Control6=IDC_STATIC_BMPFILE,static,1342308352
Control7=IDC_EDIT_BMPCIN,edit,1350631552
Control8=IDC_BUTTON_BMPVIEW,button,1342242816
Control9=IDC_STATIC_RAWPARAM,button,1342177287
Control10=IDC_STATIC_WIDTH,static,1342308352
Control11=IDC_EDIT_WIDTHCIN,edit,1350631552
Control12=IDC_STATIC_HEIGHT,static,1342308352
Control13=IDC_EDIT_HEIGHTCIN,edit,1350631552
Control14=IDC_STATIC_GRAYORBGR,button,1342177287
Control15=IDC_STATIC_FORMAT,button,1342177287
Control16=IDC_RADIO_GRAY,button,1342308361
Control17=IDC_RADIO_BGR,button,1342177289
Control18=IDC_RADIO_BSQ,button,1342308361
Control19=IDC_RADIO_BIP,button,1342177289
Control20=IDC_RADIO_BIL,button,1342177289

[CLS:CRowToBmpDlg]
Type=0
HeaderFile=RowToBmpDlg.h
ImplementationFile=RowToBmpDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CRowToBmpDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_LINEARTRANS]
Type=1
Class=CLinearTransDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_LINEARINPUT,static,1342308352
Control4=IDC_EDIT_LINEARINPUT,edit,1350631552
Control5=IDC_BUTTON_LINEARINPUT,button,1342242816
Control6=IDC_STATIC_LINEAROUTPUT,static,1342308352
Control7=IDC_EDIT_LINEAROUTPUT,edit,1350631552
Control8=IDC_BUTTON_LINEAROUTPUT,button,1342242816

[CLS:CLinearTransDlg]
Type=0
HeaderFile=LinearTransDlg.h
ImplementationFile=LinearTransDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CLinearTransDlg
VirtualFilter=dWC

[DLG:IDD_DIALOG_HISTOEQUIVALIZE]
Type=1
Class=CHistoEquivalize
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_HISTOIN,static,1342308352
Control4=IDC_EDIT_HISTOIN,edit,1350631552
Control5=IDC_BUTTON_HISTOINPUT,button,1342242816
Control6=IDC_STATIC_HISTOOUT,static,1342308352
Control7=IDC_EDIT_HISTOOUT,edit,1350631552
Control8=IDC_BUTTON_HISTOOUTPUT,button,1342242816

[CLS:CHistoEquivalize]
Type=0
HeaderFile=HistoEquivalize.h
ImplementationFile=HistoEquivalize.cpp
BaseClass=CDialog
Filter=D
LastObject=CHistoEquivalize
VirtualFilter=dWC

[DLG:IDD_DIALOG_LOWPASS]
Type=1
Class=CLowPassedDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_LOWPASSIN,static,1342308352
Control4=IDC_EDIT_LOWPASSIN,edit,1350631552
Control5=IDC_BUTTON_LOWPASSIN,button,1342242816
Control6=IDC_STATIC_LOWPASSOUT,static,1342308352
Control7=IDC_EDIT_LOWPASSOUT,edit,1350631552
Control8=IDC_BUTTON_LOWPASSOUT,button,1342242816

[CLS:CLowPassedDlg]
Type=0
HeaderFile=LowPassedDlg.h
ImplementationFile=LowPassedDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CLowPassedDlg
VirtualFilter=dWC

