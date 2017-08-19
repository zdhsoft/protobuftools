//---------------------------------------------------------------------------

#pragma hdrstop

#include "protoitem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
namespace zdh{
	int ProtoReader::m_ID = 0;
	void ProtoItem::DecodeSubList() {
		ProtoReader r;
		SubList.clear();
		r.Read(m_Data, *this);
	}
	void ProtoItem::ToStringByLengthLimit(UTF8String & s, XByteBuffer & buff, int paramEIT) {
		switch(paramEIT) {
		case EIT_DATA:
			{
				//ProtoReader r;
				//SubList.clear();
				//r.Read(buff, *this);
                s = "";
				//
			}
			break;
		case EIT_BINARY:
			{
				int iCnt = buff.getLength();
				s.printf("len:%d >", iCnt);
				for(int i = 0; i < iCnt; i++) {
					s += IntToHex(buff[i],2);
					s += " ";
				}
			}
			break;
		default:
			{
				std::string sss((const char *)buff.getData(), buff.getLength());
				s.printf("len:%d,<%s>", buff.getLength(), sss.c_str());
			}
		};

	}
}
