//---------------------------------------------------------------------------

#ifndef mainFrmH
#define mainFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include "protoitem.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TImageList *ImageList1;
	TTreeView *TreeView1;
	TSplitter *Splitter1;
	TMemo *Memo1;
	TOpenDialog *OpenDialog1;
	TImageList *ImageList2;
	TPopupMenu *PopupMenu1;
	TActionList *ActionList1;
	TAction *ActionSetInt32;
	TAction *ActionSetInt64;
	TAction *ActionUint32;
	TAction *ActionUint64;
	TAction *ActionString;
	TMenuItem *Int321;
	TMenuItem *Int641;
	TMenuItem *N1;
	TMenuItem *Uint321;
	TMenuItem *UInt641;
	TAction *ActionData;
	TAction *ActionBinary;
	TAction *ActionDouble;
	TAction *ActionFloat;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *double1;
	TMenuItem *Float1;
	TAction *ActionHex64;
	TAction *ActionHex32;
	TMenuItem *N321;
	TMenuItem *N641;
	void __fastcall ToolButton1Click(TObject *Sender);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall ActionSetInt32Execute(TObject *Sender);
private:	// User declarations
	zdh::XByteBuffer m_Buff;
	zdh::ProtoItem m_Item;
	int m_ID = 0;
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
