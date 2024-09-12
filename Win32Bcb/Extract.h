//----------------------------------------------------------------------------
#ifndef ExtractH
#define ExtractH
#include "EkImgBtn.hpp"
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include "EkFadeImgBase.hpp"

//----------------------------------------------------------------------------
class TExtractDlg : public TForm
{
__published:
	TImage *BGImg;
	TEkImgBtn *BtnStop;
	TEkImgBtn *BtnStopAll;
	TLabel *LblCaption;
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BGImgMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall BtnStopClick(TObject *Sender);
	void __fastcall BtnStopAllClick(TObject *Sender);
private:
public:
	virtual __fastcall TExtractDlg(TComponent* AOwner);
	//bool CancelAll;
};
//----------------------------------------------------------------------------
extern PACKAGE TExtractDlg *ExtractDlg;
//----------------------------------------------------------------------------
#endif    
