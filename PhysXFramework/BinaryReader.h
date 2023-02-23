#pragma once
#include "Logger.h"
// RAII class
class BinaryReader
{
public:
	BinaryReader(const std::wstring& binaryFile);
	BinaryReader(char* s, UINT32 size);
	~BinaryReader() = default;
	BinaryReader(const BinaryReader& yRef) = delete;
	BinaryReader& operator=(const BinaryReader& yRef) = delete;

	template<typename T>
	T Read()
	{
		if (!m_Exists)
			Logger::GetInstance()->LogError(L"BinaryReader doesn't exist!\nUnable to read binary data...");

		T value;
		m_Reader.read((char*)&value, sizeof(T));
		return value;
	}

	std::wstring ReadString();
	std::wstring ReadLongString();
	std::wstring ReadNullString();

	int GetBufferPosition();
	bool SetBufferPosition(int pos);
	bool MoveBufferPosition(int move);
	bool Exists() const { return m_Exists; }

private:
	bool m_Exists;
	std::ifstream m_Reader;
};
