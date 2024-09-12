//---------------------------------------------------------------------------

#ifndef VideoH
#define VideoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#define WM_GRAPHNOTIFY  WM_APP + 1
//---------------------------------------------------------------------------
class TVideoWnd : public TForm
{
__published:	// IDE-managed Components
	void __fastcall FormClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TVideoWnd(TComponent* Owner);
	//void HITTEST(TWMNCHitTest &Message);
   //	MESSAGE OnUserMsg (TWMMouse &Message);

//	BEGIN_MESSAGE_MAP
//		MESSAGE_HANDLER(WM_NCHITTEST, TWMNCHitTest, HITTEST);
//		//ON_MESSAGE(WM_GRAPHNOTIFY, OnUserMsg);
//	   //	MESSAGE_HANDLER(WM_GRAPHNOTIFY, TWMMouse, OnUserMsg);
//		//MESSAGE_HANDLER(WM_GRAPHNOTIFY, OnUserMsg)
//	END_MESSAGE_MAP(TForm);
};
//---------------------------------------------------------------------------
extern PACKAGE TVideoWnd *VideoWnd;
//---------------------------------------------------------------------------
#endif
