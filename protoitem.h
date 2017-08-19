// ---------------------------------------------------------------------------

#ifndef protoitemH
#define protoitemH

#include <fstream>
#include <vector>
#include <set>
#include <vcl.h>

namespace zdh {
	typedef unsigned char XByte;
	typedef __int64 XInt64;
	typedef unsigned __int64 XUInt64;
	typedef double  XDouble;
	typedef __int32 XInt32;
	typedef unsigned __int32 XUInt32;


	// ---------------------------------------------------------------------------
	enum EnumWireType {
		EWT_Varint = 0,
		EWT_Fix64 = 1,
		EWT_LengthLimit = 2,
		EWT_StartGroup = 3,
		EWT_EndGroup = 4,
		EWT_Fix32 = 5,
	};


	//具体的数据类型
	enum EnumItemType {
		EIT_UNKNOW = 0,
		EIT_INT32  = 1,
		EIT_INT64  = 2,
		EIT_UINT32 = 3,
		EIT_UINT64 = 4,
		EIT_STRING = 5,
		EIT_DATA   = 6,
		EIT_BINARY = 7,
		EIT_DOUBLE = 8,
		EIT_FLOAT  = 9,
		EIT_HEX64  = 10,
		EIT_HEX32  = 11,
	};

	inline void GetWireTypeItemTypeSet(int paramWireType, std::set<int> & paramSet) {
		paramSet.clear();
		switch(paramWireType) {
		case EWT_Varint:
			paramSet.insert(EIT_INT32);
			paramSet.insert(EIT_INT64);
			paramSet.insert(EIT_UINT32);
			paramSet.insert(EIT_UINT64);
			paramSet.insert(EIT_HEX64);
			paramSet.insert(EIT_HEX32);
			break;
		case EWT_Fix64:
			paramSet.insert(EIT_INT64);
			paramSet.insert(EIT_UINT64);
			paramSet.insert(EIT_HEX64);
			paramSet.insert(EIT_DOUBLE);
			break;
		case EWT_LengthLimit:
			paramSet.insert(EIT_STRING);
			paramSet.insert(EIT_DATA);
			paramSet.insert(EIT_BINARY);
			break;
		case EWT_StartGroup:
			break;
		case EWT_EndGroup:
			break;
		case EWT_Fix32:
			paramSet.insert(EIT_INT32);
			paramSet.insert(EIT_UINT32);
			paramSet.insert(EIT_HEX32);
			paramSet.insert(EIT_FLOAT);
			break;
		}
	}


	class XByteBuffer {
	public:
		XByteBuffer() : m_Length(0), m_DataSize(0), m_Data(nullptr) {
		}

		~XByteBuffer() {
			ToEmpty();
		}

		void ToEmpty() {
			if (m_Data != nullptr) {
				delete[]m_Data;
			}
			m_Length = 0;
			m_DataSize = 0;
			m_Data = nullptr;
		}

		void expandCapacity(int paramMini) {
			if (m_DataSize >= paramMini)
				return;
			if (paramMini <= 0)
				return;
			XByte * p = new XByte[paramMini];
			if (p == nullptr) {
				throw "out of memory!";
			}
			if (m_Length > 0) {
				copyData(m_Data, p, m_Length);
			}
			if (m_Data != nullptr) {
				delete[]m_Data;
			}
			m_Data = p;
			m_DataSize = paramMini;
		}

		void ensureCapacity(int paramCapacity, bool paramRemainData = true) {
			if (!paramRemainData)
				m_Length = 0;
			expandCapacity(paramCapacity);
		}

		void setData(const XByte * paramData, int paramSize, int paramIndex = 0)
		{
			if (paramData == nullptr)
				return;
			ensureCapacity(paramSize, false);
			if (paramSize <= 0)
				return;
			m_Length = paramSize;
			copyData(paramData + paramIndex, m_Data, paramSize);
		}

		void setData(const XByteBuffer & paramBuffer, int paramCopySize,
			int paramStartIndex = 0) {
			if (this == &paramBuffer) {
				XByteBuffer tmp;
				tmp.setData(*this, paramStartIndex, paramCopySize);
				swap(*this, tmp);
			}
			else {
				setData(paramBuffer.m_Data, paramCopySize, paramStartIndex);
			}
		}

		void setData(const XByteBuffer & paramBuffer) {
			setData(paramBuffer, paramBuffer.m_Length, 0);
		}

		void append(const XByte * paramData, int paramSize, int paramIndex = 0) {
			if(paramData == nullptr || paramSize <= 0 || paramIndex < 0) return;
			ensureCapacity(m_Length + paramSize);
			copyData(paramData + paramIndex, m_Data + m_Length,paramSize);
			m_Length += paramSize;
        }

		int getLength() const {
			return m_Length;
		}

		int setLength(int paramLength) {
			ensureCapacity(paramLength);
			if (paramLength < 0)
				m_Length = 0;
			else
				m_Length = paramLength;
			return m_Length;
		}

		int getDataSize() const {
			return m_DataSize;
		}

		void Clear() {
            m_Length = 0;
        }

		const XByte * getData() const {
			return m_Data;
		}

		XByte & operator[](int paramIndex) {
			return m_Data[paramIndex];
		}

		XByte operator[](int paramIndex) const {
			return m_Data[paramIndex];
		}

	private:
		void copyData(const XByte * paramSrc, XByte * paramDest,
			int paramCopySize) {
			for (int i = 0; i < paramCopySize; i++) {
				*paramDest++ = *paramSrc++;
			}
		}

		void swap(XByteBuffer & param1, XByteBuffer & param2) {
			int t1 = param1.m_Length;
			int t2 = param1.m_DataSize;
			XByte * d = param1.m_Data;

			param1.m_Length = param2.m_Length;
			param1.m_DataSize = param2.m_DataSize;
			param1.m_Data = param2.m_Data;

			param2.m_Length = t1;
			param2.m_DataSize = t2;
			param2.m_Data = d;
		}

	private:
		int m_Length;
		int m_DataSize;
		XByte * m_Data;
	};




	class XUtils {
		public:
		static int LoadFile(std::string paramFileName, XByteBuffer & paramData) {
			std::ifstream f;
			f.open(paramFileName, f.binary);
			f.seekg (0, f.end);
			int length = f.tellg();
			f.seekg (0, f.beg);
			paramData.ensureCapacity(length, false);
			XByte tmp[128];
			while(!f.eof()){
				f.read((char *)tmp, 128);
				int iReadCount = f.gcount();
				if(iReadCount > 0) paramData.append(tmp, iReadCount);
				if(iReadCount < 128) break;
			}
			f.close();
            return 0;
		}
		static void CopyData(const XByte * paramSrc, XByte * paramDest, int paramCopySize, bool paramFlag = true){
			if(paramCopySize <= 1) return;
			if(paramFlag) {
				for(int i = 0; i < paramCopySize; i++) {
                    *paramDest ++ = * paramSrc++;
                }
			}
			else {
				paramSrc += (paramCopySize-1);
				for(int i = 0;i < paramCopySize; i++) {
*                    paramDest++ = * paramSrc--;
                }
            }
		}
	};


	union XFix64 {
		XInt64 Value64;
		double ValueDouble;
	};

	union XFix32 {
		XInt32 Value32;
		float ValueFloat;
	};

	class ProtoItem {
	public:
		ProtoItem() {
            Reset();
		}
		ProtoItem(const ProtoItem & item) {
			m_FieldNumber 	= item.m_FieldNumber;
			m_WireType 		= item.m_WireType;
			m_SIntValue 	= item.m_SIntValue;
			m_StringValue 	= item.m_StringValue;
			m_Fix64.Value64 = item.m_Fix64.Value64;
			m_Fix32.Value32 = item.m_Fix32.Value32;
			m_EIT 			= item.m_EIT;
            m_ID            = item.m_ID;
			m_Data.setData(item.m_Data);
            SubList = item.SubList;
		}

		void Reset() {
			m_EIT 			= EIT_UNKNOW;
			m_FieldNumber 	= 0;
			m_WireType 		= 0;
			m_SIntValue 	= 0;
			m_StringValue 	= 0;
			m_Fix64.Value64 = 0;
			m_Fix32.Value32 = 0;
			m_ID            = 0;
			m_Data.Clear();
            SubList.clear();
		}

		ProtoItem * GetItemByID(int paramID) {
			for(auto iter = SubList.begin(); iter != SubList.end(); ++iter) {
				if(iter->m_ID == paramID) {
					return &(*iter);
				}
				ProtoItem * pFind = iter->GetItemByID(paramID);
				if(pFind != nullptr) {
					return pFind;
				}
			}
			return nullptr;
		}
		UTF8String ToString() {
			UTF8String s;
			switch(m_WireType) {
			case EWT_Varint:
				ToStringByVarint(s, m_SIntValue, m_EIT);
				break;
			case EWT_Fix64:
				ToStringByFix64(s,m_Fix64,m_EIT);
				break;
			case EWT_LengthLimit:
				ToStringByLengthLimit(s, m_Data, m_EIT);
				break;
			case EWT_StartGroup:
				break;
			case EWT_EndGroup:
				break;
			case EWT_Fix32:
				ToStringByFix32(s,m_Fix32,m_EIT);
				break;
			}
			return IntToStr(m_FieldNumber) + u"," + IntToStr(m_WireType) + u"]" + s;
		}
	public:
		void DecodeSubList();
	private:
		void ToStringByLengthLimit(UTF8String & s, XByteBuffer & buff, int paramEIT);
		void ToStringByVarint(UTF8String & s, XInt64 v, int paramEIT) {
			switch(paramEIT) {
			case EIT_INT32:
			case EIT_INT64:
				s = IntToStr(v);
				break;
			case EIT_UINT32:
			case EIT_UINT64:
				s = UIntToStr(XUInt64(v));
				break;
			case EIT_HEX32:
				s = u"0x"+IntToHex(XUInt64(XUInt32(v)), 8);
				break;
			case EIT_HEX64:
				s = u"0x"+IntToHex(XUInt64(v), 16);
				break;
			default:
                s = IntToStr(m_SIntValue) + u":" + UIntToStr(XUInt64(m_SIntValue))+u":"+IntToHex(m_SIntValue,16);
			}
		}

		void ToStringByFix64(UTF8String & s, const XFix64 & v, int paramEIT) {
			switch(paramEIT) {
			case EIT_INT64:
				s = IntToStr(v.Value64);
				break;
			case EIT_UINT64:
				s = UIntToStr(XUInt64(v.Value64));
				break;
			case EIT_HEX64:
				s = u"0x"+IntToHex(XUInt64(v.Value64), 16);
				break;
			case EIT_DOUBLE:
				s = FloatToStr(v.ValueDouble);
				break;
			default:
				s = IntToStr(v.Value64) + u":" + FloatToStr(v.ValueDouble);
			}
		}
		void ToStringByFix32(UTF8String & s, const XFix32 & v, int paramEIT) {
			switch(paramEIT) {
			case EIT_INT32:
				s = IntToStr(v.Value32);
				break;
			case EIT_UINT32:
				s = UIntToStr(XUInt32(v.Value32));
				break;
			case EIT_HEX32:
				s = u"0x"+IntToHex(XUInt64(XUInt32(v.Value32)), 8);
				break;
			case EIT_FLOAT:
				s = FloatToStr(v.ValueFloat);
				break;
			default:
				s = IntToStr(v.Value32) + u":" + FloatToStr(v.ValueFloat);
			}
		}


	public:
		int m_ID = 0;
        int m_EIT = EIT_UNKNOW;
		int m_FieldNumber = 0;
		int m_WireType = 0;

		XInt64    m_SIntValue;
		UTF8String m_StringValue;
		XFix64    m_Fix64;
		XFix32    m_Fix32;
		XByteBuffer m_Data;
		std::vector<ProtoItem> SubList;
	};

	class XBufferReader {
		public:
			XBufferReader() {
			}
			bool isEof() const {
				if(m_Buff == nullptr || m_Pos >= m_Buff->getLength()) return true;
				else return false;
			}
			void SetBuff(XByteBuffer * paramBuffer) {
				m_Buff = paramBuffer;
			}

			void SetPos(int paramPos) {
				m_Pos = paramPos;
			}

			bool ReadByte(XByte & paramValue) {
				if(isEof()) return false;
				paramValue = (*m_Buff)[m_Pos++];
				return true;
			}

			bool ReadData(XByteBuffer & paramData, int iCnt) {
				if(isEof()) return false;
				int iLastPos = m_Pos + iCnt;
				if(iLastPos > m_Buff->getLength()) return false;
				paramData.Clear();
				paramData.setData(*m_Buff, iCnt, m_Pos);
                m_Pos = iLastPos;
				return true;
            }

			bool ReadFix64(XFix64 paramValue) {
				if(isEof()) return false;
				int iLastPos = m_Pos + 8;
				if(iLastPos > m_Buff->getLength()) return false;
				XUtils::CopyData(m_Buff->getData() + m_Pos, (XByte *)&paramValue.Value64, 8);
				m_Pos += 8;
				return true;
			}

			bool ReadFix32(XFix32 paramValue) {
				if(isEof()) return false;
				int iLastPos = m_Pos + 4;
				if(iLastPos > m_Buff->getLength()) return false;
				XUtils::CopyData(m_Buff->getData() + m_Pos, (XByte *)&paramValue.Value32, 4);
                m_Pos += 4;
				return true;
			}

			void PosInc() {
                m_Pos++;
			}

			void PosDec() {
                m_Pos--;
            }

		private:
			XByteBuffer *m_Buff = nullptr;
			int m_Pos = 0;
	};

	class ProtoReader {
	public:
		static int m_ID;
	public:

		ProtoReader() {
		}
		void Read(XByteBuffer & paramBuffer,  ProtoItem & paramItem) {
			//paramItem.Reset();
			ProtoItem stItem;
			XBufferReader r;
			r.SetBuff(&paramBuffer);
			r.SetPos(0);
			XByte btFlag = 0;
			int iFieldNum = 0;
			int iWireType = 0;
			XInt64 iTmp = 0;
			XInt64 nTag = 0;
			bool bRun = true;
			while(bRun) {

				//if(!r.ReadByte(btFlag)) break;
				if(!ReadVarint(r, nTag)) break;
				stItem.Reset();
				iWireType = (int)(nTag & 0x7);
				iFieldNum = int((nTag >> 3) & 0x1f);
				stItem.m_FieldNumber = iFieldNum;
				stItem.m_WireType = iWireType;
				switch(iWireType) {
				case EWT_Varint:
					if(!ReadVarint(r, stItem.m_SIntValue)) {
						bRun = false;
					}
					break;
				case EWT_Fix64:
					if(!r.ReadFix64(stItem.m_Fix64)) {
						bRun = false;
					}
					break;

				case EWT_LengthLimit:
					if(!ReadVarint(r, iTmp)) {
						bRun = false;
					}
					if(iTmp > 0) {
						if(!r.ReadData(stItem.m_Data, (int)iTmp)) {
							bRun = false;
						}
					}
					break;

				case EWT_StartGroup:
					break;

				case EWT_EndGroup:
					break;

				case EWT_Fix32:
					if(!r.ReadFix32(stItem.m_Fix32)) {
						bRun = false;
					}
					break;
				}
				if(bRun) {
					stItem.m_ID = m_ID++;
					paramItem.SubList.push_back(stItem);
				}
			}
		}
	private:
	   bool ReadVarint(XBufferReader & r, XInt64 & v) {
			XInt64 nTmp;
			XByte bt;
			v = 0;
			for(int i = 0; i < 16; i++) {
				if(!r.ReadByte(bt)) return false;
				nTmp = bt;
				v = v | (nTmp & 0x7f) << (7*i);
				if((bt & 0x80) == 0) {
					break;
				}
			}
            return true;
	   }
	public:
		int m_Pos = 0;
		int m_FieldNumber = 1;
	};
}
#endif
