#pragma once
class CBaseModelInfo
{
public:
	char pad_0000[0x18]; //0x0000
	uint32_t m_model; //0x0018
	char pad_001C[0x81]; //0x001C
	BYTE m_model_type; //0x009D
	char pad_009D[0x2A2]; //0x009E
	int32_t m_handlingType; //0x0340
}; //Size: 0x0344