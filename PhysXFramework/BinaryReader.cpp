#include "stdafx.h"
#include "BinaryReader.h"
using namespace std;

BinaryReader::BinaryReader(const std::wstring& binaryFile)
	: m_Exists(false)
	, m_Reader{ ifstream(binaryFile.c_str(), ios::in | ios::binary) }
{
	if (m_Reader.is_open()) m_Exists = true;
	else
	{
		wstringstream ss;
		ss << L"BinaryReader::Open -> Failed to open the file!\nBinaryFile: " << binaryFile;
		Logger::GetInstance()->LogWarning(ss.str());
	}
}
BinaryReader::BinaryReader(char* s, UINT32 size)
	: m_Exists(false)
	, m_Reader{ ifstream(s, ios::in | ios::binary) }
{
	if (m_Reader.is_open()) m_Exists = true;
	else
	{
		wstringstream ss;
		ss << L"BinaryReader::Open -> Failed to open the file!\nBinaryFile: " << s;
		Logger::GetInstance()->LogWarning(ss.str());
	}
}

wstring BinaryReader::ReadLongString()
{
	if (!m_Exists)
	{
		Logger::GetInstance()->LogError(L"BinaryReader doesn't exist!\nUnable to read binary data...");
		return L"";
	}

	auto stringLength = Read<UINT>();

	wstringstream ss;
	for (UINT i{ 0 }; i < stringLength; ++i)
	{
		ss << Read<wchar_t>();
	}

	return ss.str();
}

wstring BinaryReader::ReadNullString()
{
	if (!m_Exists)
	{
		Logger::GetInstance()->LogError(L"BinaryReader doesn't exist!\nUnable to read binary data...");
		return L"";
	}

	string buff;
	getline(m_Reader, buff, '\0');

	return wstring(buff.begin(), buff.end());
}

wstring BinaryReader::ReadString()
{
	if (!m_Exists)
	{
		Logger::GetInstance()->LogError(L"BinaryReader doesn't exist!\nUnable to read binary data...");
		return L"";
	}
	int stringLength = (int)Read<char>();

	wstringstream ss;
	for (int i{ 0 }; i < stringLength; ++i)
	{
		ss << Read<char>();
	}

	return ss.str();
}

int BinaryReader::GetBufferPosition()
{
	if (m_Exists)
	{
		return static_cast<int>(m_Reader.tellg());
	}

	Logger::GetInstance()->LogWarning(L"BinaryReader::GetBufferPosition> m_pReader doesn't exist");
	return -1;
}

bool BinaryReader::SetBufferPosition(int pos)
{
	if (m_Exists)
	{
		m_Reader.seekg(pos);
		return true;
	}

	Logger::GetInstance()->LogWarning(L"BinaryReader::SetBufferPosition> m_pReader doesn't exist");
	return false;
}

bool BinaryReader::MoveBufferPosition(int move)
{
	auto currPos = GetBufferPosition();
	if (currPos > 0)
	{
		return SetBufferPosition(currPos + move);
	}
	return false;
}
