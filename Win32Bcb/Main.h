//---------------------------------------------------------------------------

#ifndef MainH
#define MainH

#include "EkFadeImgBase.hpp"
#include "EkImgProgressBar.hpp"
#include "EkImgTrackBar.hpp"
#include "EkLabel.hpp"
#include "EkImgBtn.hpp"
#include "EkImgCheckBox.hpp"
#include "EkDirBrowse.hpp"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <XPMan.hpp>
#include <List.h>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TImage *ImgIcon;
	TLabel *LblXctEntriesTxt;
	TLabel *LblInfo01;
	TLabel *LblInfo02;
	TLabel *LblInfo03;
	TLabel *LblInfo04;
	TLabel *LblInfo05;
	TLabel *LblInfo06;
	TLabel *LblInfo07;
	TLabel *LblInfo08;
	TLabel *LblInfo09;
	TLabel *LblSizeVal;
	TLabel *LblFilesVal;
	TLabel *LblArchiveVal;
	TLabel *LblAudioVal;
	TLabel *LblMem;
	TImage *ImgDrag;
	TEkImgBtn *BtnTgle;
	TEkImgBtn *BtnSelDir;
	TEkImgBtn *BtnIni;
	TEkImgCheckBox *ChkAutoEditClr;
	TEkImgCheckBox *ChkAutoXpand;
	TEkImgBtn *BtnExt;
	TEkImgCheckBox *ChkList;
	TEkImgCheckBox *ChkSpeed;
	TEkImgCheckBox *ChkTidyName;
	TEkImgCheckBox *ChkXct;
	TLabel *LblFilesTxt;
	TLabel *LblOptions;
	TLabel *LblRenameFiles;
	TLabel *LblFolderName;
	TLabel *LblArchiveTxt;
	TLabel *LblAudioTxt;
	TLabel *LblSizeTxt;
	TEkImgBtn *BtnScan;
	TEkImgBtn *BtnPlay;
	TEkImgBtn *BtnPause;
	TEkImgBtn *BtnStop;
	TEkImgBtn *BtnPrev;
	TEkImgBtn *BtnNext;
	TEkImgProgressBar *EkImgProgressBar;
	TEkImgBtn *BtnFade;
	TEkImgBtn *BtnClose;
	TEkImgBtn *BtnMin;
	TEkImgBtn *BtnGuiWidth;
	TEkImgBtn *BtnGuiHeight;
	TEkImgBtn *BtnClrPlayList;
	TEkImgCheckBox *BtnLoopPlayList;
	TEkImgBtn *BtnOpen;
	TEdit *EditFolName;
	TEdit *EditRenFiles;
	TListView *ListView1;
	TOpenDialog *OpenDialog;
	TPopupMenu *PopupMenu1;
	TMenuItem *ChkAllMenu;
	TMenuItem *ChkAllAudioMenu;
	TMenuItem *ChkNoneMenu;
	TMenuItem *SwapChkdMenu;
	TOpenDialog *OpenDialogFileSwap;
	TImageList *ImageList1;
	TImageList *ImageList2;
	TPopupMenu *PopupMenuMain;
	TMenuItem *PopMenuOnTop;
	TMenuItem *PopMenuClose;
	TTimer *TimrDebug;
	TOpenDialog *OpenDialogScx;
	TXPManifest *XPManifest1;
	TTimer *TimrRefreshRate;
	TEkLabel *LblCaption;
	TLabel *LblFmt;
	TComboBox *ComboFormat;
	TLabel *LblBuf;
	TComboBox *ComboBufSize;
	TComboBox *ComboSmpRate;
	TLabel *LblSmpRate;
	TComboBox *ComboChan;
	TLabel *LblChan;
	TComboBox *ComboBits;
	TLabel *LblBits;
	TLabel *LblXctEntriesVal;
	TImage *ImgMain;
	TImageList *ImgListGui;
	TMenuItem *PopMenuScreenSnap;
	TEkDirBrowse *EkDirBrowse1;
	TEkImgCheckBox *ChkAutoGuiClr;
	TTimer *TimrDblBuff;
	TEkImgCheckBox *ChkAutoScx;
	TEkImgTrackBar *EkszTrackBar;
	TTimer *TimrCursorHide;
	TMenuItem *PopMenuShowHints;
	TMenuItem *PopMenuScrollingText;
	TMenuItem *PopMenuAnimatedProgressBar;
	TMenuItem *PopMenuFadeFx;
	void __fastcall BtnExtClick(TObject *Sender);
	void __fastcall EditFolNameDblClick(TObject *Sender);
	void __fastcall EditRenFilesDblClick(TObject *Sender);
	void __fastcall ChkAllMenuClick(TObject *Sender);
	void __fastcall ChkNoneMenuClick(TObject *Sender);
	void __fastcall EditRenFilesKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EditRenFilesChange(TObject *Sender);
	void __fastcall Show1Click(TObject *Sender);
	void __fastcall Verbose1Click(TObject *Sender);
	void __fastcall SwapChkdMenuClick(TObject *Sender);
	void __fastcall CheckAll2Click(TObject *Sender);
	void __fastcall CheckNone1Click(TObject *Sender);
	void __fastcall BtnScanClick(TObject *Sender);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall PopMenuOnTopClick(TObject *Sender);
	void __fastcall PopMenuCloseClick(TObject *Sender);
	void __fastcall TimrDebugTimer(TObject *Sender);
	void __fastcall ComboSmpRateDblClick(TObject *Sender);
	void __fastcall EditFolNameMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall EditRenFilesMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall TimrRefreshRateTimer(TObject *Sender);
	void __fastcall ComboSmpRateChange(TObject *Sender);
	void __fastcall ChkAllAudioMenuClick(TObject *Sender);
	void __fastcall ListView1Data(TObject *Sender, TListItem *Item);
	void __fastcall ListView1GetImageIndex(TObject *Sender, TListItem *Item);
	void __fastcall ListView1SelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
	void __fastcall ListView1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ListView1ColumnClick(TObject *Sender, TListColumn *Column);
	void __fastcall ShowAllButOffsets1Click(TObject *Sender);
	void __fastcall EditFolNameChange(TObject *Sender);
	void __fastcall BtnOpenClick(TObject *Sender);
	void __fastcall ChkListClick(TObject *Sender);
	void __fastcall ChkXctClick(TObject *Sender);
	void __fastcall ChkTidyNameClick(TObject *Sender);
	void __fastcall ChkAutoEditClrClick(TObject *Sender);
	void __fastcall ChkSpeedClick(TObject *Sender);
	void __fastcall BtnIniClick(TObject *Sender);
	void __fastcall BtnSelDirClick(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, char &Key);
	void __fastcall ChkAutoXpandClick(TObject *Sender);
	void __fastcall ImgDragMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall BtnPlayClick(TObject *Sender);
	void __fastcall BtnStopClick(TObject *Sender);
	void __fastcall BtnNextClick(TObject *Sender);
	void __fastcall BtnPauseClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall BtnFadeClick(TObject *Sender);
	void __fastcall BtnPrevClick(TObject *Sender);
	void __fastcall BtnCloseClick(TObject *Sender);
	void __fastcall BtnMinClick(TObject *Sender);
	void __fastcall BtnGuiWidthClick(TObject *Sender);
	void __fastcall BtnGuiHeightClick(TObject *Sender);
	void __fastcall BtnClrPlayListClick(TObject *Sender);
	void __fastcall BtnLoopPlayListClick(TObject *Sender);
	void __fastcall ListView1DblClick(TObject *Sender);
	void __fastcall BtnTgleClick(TObject *Sender);
	void __fastcall EkszTrackBarMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall EkszTrackBarMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall EkszTrackBarChange(TObject *Sender);
	void __fastcall EkImgProgressBarMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall EkImgProgressBarMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall PopMenuScreenSnapClick(TObject *Sender);
	void __fastcall ChkAutoGuiClrClick(TObject *Sender);
	void __fastcall TimrDblBuffTimer(TObject *Sender);
	void __fastcall ChkAutoScxClick(TObject *Sender);
	void __fastcall ImgDragDblClick(TObject *Sender);
	void __fastcall EkImgProgressBarChange(TObject *Sender);
	void __fastcall ListView1Edited(TObject *Sender, TListItem *Item,
          AnsiString &S);
	void __fastcall PopMenuShowHintsClick(TObject *Sender);
	void __fastcall PopMenuScrollingTextClick(TObject *Sender);
	void __fastcall PopMenuAnimatedProgressBarClick(TObject *Sender);
	void __fastcall PopMenuFadeFxClick(TObject *Sender);

private:

	void __fastcall WmDropFiles(TWMDropFiles& Msg); // Drag & Drop
	void __fastcall OnFormMove(TWMMove& Msg);

	void __fastcall MinimizeForm(TObject* Sender);
	void __fastcall RestoreForm(TObject* Sender);
	void __fastcall DeactivateForm(TObject* Sender);
	void __fastcall ActivateForm(TObject* Sender);

	//void __fastcall ShowHint(System::AnsiString &HintStr, bool&CanShow, THintInfo &HintInfo);
	void __fastcall AppMessage(MSG &msg, bool &handled);
	MESSAGE void __fastcall Erase(TWMEraseBkgnd &Msg);
	//MESSAGE void __fastcall MseMove(TWMMouse &Msg);
	//MESSAGE void __fastcall WMNCHitTest(TWMNCHitTest &Msg);
	//void __fastcall WndProc(TMessage &Msg);

	Graphics::TBitmap* ImgMainBak;

public:

	EkszList List;
	bool Minimized;

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_MOVE, TWMMove, OnFormMove)
		MESSAGE_HANDLER(WM_ERASEBKGND, TWMEraseBkgnd, Erase)
		//MESSAGE_HANDLER(WM_MOUSEMOVE, TWMMouse, MseMove)
		//VCL_MESSAGE_HANDLER( WM_NCHITTEST, TWMNCHitTest, WMNCHitTest )
		MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles) // Drag & Drop
	END_MESSAGE_MAP(TForm)

	__fastcall TForm1(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
