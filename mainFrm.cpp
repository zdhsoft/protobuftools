//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mainFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}

void BuildTree(TTreeView * pView, TTreeNode * pNode, std::vector<zdh::ProtoItem> & SubList) {
	//auto & SubList = m_Item.SubList;
	for(auto iter = SubList.begin(); iter != SubList.end(); ++iter) {
		TTreeNode * pItem = pView->Items->AddChild(pNode, iter->ToString());
		pItem->ImageIndex = (*iter).m_WireType;
		pItem->SelectedIndex = (*iter).m_WireType;
		pItem->Data = (void *)iter->m_ID;

	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmMain::ToolButton1Click(TObject *Sender)
{
	//
	if(OpenDialog1->Execute()) {
		UTF8String s(OpenDialog1->FileName);
		//zdh::XUtils::LoadFile(s.c_str(), m_Buff);
		zdh::XUtils::LoadFile(s.c_str(), m_Buff);
		zdh::ProtoReader r;
		zdh::ProtoReader::m_ID = 0;
		m_Item.Reset();
		r.Read(m_Buff, m_Item);
		TreeView1->Items->Clear();
		TTreeNode * pRoot = TreeView1->Items->Add(nullptr, "Root");
		BuildTree(TreeView1, pRoot, m_Item.SubList);
	}
//	UTF8String s = "D:\\tmp\\db\\m\\0000012606376909670322508";
//	zdh::XUtils::LoadFile(s.c_str(), m_Buff);
//	zdh::ProtoReader r;
//	zdh::ProtoReader::m_ID = 0;
//	m_Item.Reset();
//	r.Read(m_Buff, m_Item);
//	TreeView1->Items->Clear();
//	TTreeNode * pRoot = TreeView1->Items->Add(nullptr, "Root");
//	BuildTree(TreeView1, pRoot, m_Item.SubList);
//	auto & SubList = m_Item.SubList;
//	for(auto iter = SubList.begin(); iter != SubList.end(); ++iter) {
//		TTreeNode * pItem = TreeView1->Items->AddChild(pRoot, iter->ToString());
//		pItem->ImageIndex = (*iter).m_WireType;
//		pItem->SelectedIndex = (*iter).m_WireType;
//		pItem->Data = (void *)iter->m_ID;
//
//	}
	//ShowMessage("hello");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::PopupMenu1Popup(TObject *Sender)
{
	for(int i = 0; i < ActionList1->ActionCount; i++) {
		ActionList1->Actions[i]->Visible = false;
	}
	//ActionList1->ActionCount
	TTreeNode * p = TreeView1->Selected;
	if(p == nullptr) return;
	p->Selected = true;
	int nID = (int)p->Data;
	auto * pFindItem = m_Item.GetItemByID(nID);
	if(pFindItem == nullptr) return;
	std::set<int> s;
	zdh::GetWireTypeItemTypeSet(pFindItem->m_WireType, s);
	for(int i = 0; i < ActionList1->ActionCount; i++) {
		auto * pAction = ActionList1->Actions[i];
		auto itfind = s.find(pAction->Tag);
		if(itfind != s.end()) {
			pAction->Visible = true;
		}
	}
}
//---------------------------------------------------------------------------

//       void __fastcall TfrmMain::ActionSetInt32Execute(TObject *Sender)
//{
//	//
//	auto * pAction = dynamic_cast<TContainedAction *>(Sender);
//	if(pAction != nullptr) {
//		Memo1->Lines->Add(IntToStr(pAction->Tag));
//	}
//	else {
//		Memo1->Lines->Add("nullptr");
//	}
//
//}
////


void __fastcall TfrmMain::ActionSetInt32Execute(TObject *Sender)
{
	auto * pAction = dynamic_cast<TContainedAction *>(Sender);
	if(pAction != nullptr) {
		//Memo1->Lines->Add(IntToStr(pAction->Tag));
		TTreeNode * p = TreeView1->Selected;
		if(p == nullptr) return;
		int nID = (int)p->Data;
		auto * pFindItem = m_Item.GetItemByID(nID);
		if(pFindItem == nullptr) return;
		pFindItem->m_EIT = pAction->Tag;
		if(pAction->Tag == zdh::EIT_DATA) {
			pFindItem->DecodeSubList();
			for(int i = 0; i < p->Count; i++) {
                p->Item[i]->Delete();
			}
			BuildTree(TreeView1, p, pFindItem->SubList);
		}
		p->Text = pFindItem->ToString();
	}
	else {
		//Memo1->Lines->Add("nullptr");
	}
}
//---------------------------------------------------------------------------

