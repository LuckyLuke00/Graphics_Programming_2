//--------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____|
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____|
//
// Overlord Engine v0.44
// Author: Thomas Goussaert
// http://www.digitalartsandentertainment.com/
//--------------------------------------------------------------------------------------

#pragma once
#include "stdafx.h"
#include <typeinfo>
#include <unordered_map>
#include <sys/types.h>
#include <sys/stat.h>
#include "Logger.h"

using namespace std;

class BaseLoader
{
public:
	BaseLoader() = default;
	virtual ~BaseLoader() = default;

	virtual const type_info& GetType() const = 0;
	virtual void Unload() = 0;
	virtual void SetDevice(ID3D11Device* pDevice) { m_pDevice = pDevice; }

	BaseLoader(const BaseLoader& obj) = delete;
	BaseLoader& operator=(const BaseLoader& obj) = delete;
	BaseLoader(const BaseLoader&& obj) = delete;
	BaseLoader& operator=(const BaseLoader&& obj) = delete;

protected:
	ID3D11Device* m_pDevice{};

private:
};

template <class T>
class ContentLoader : public BaseLoader
{
public:
	ContentLoader()
	{
		++m_LoaderReferences;
	}

	~ContentLoader() override = default;
	ContentLoader(const ContentLoader& obj) = delete;
	ContentLoader& operator=(const ContentLoader& obj) = delete;
	ContentLoader(const ContentLoader&& obj) = delete;
	ContentLoader& operator=(const ContentLoader&& obj) = delete;

	virtual const type_info& GetType() const { return typeid(T); }

	T* GetContent(const wstring& assetFile)
	{
		for (pair<wstring, T*> kvp : m_ContentReferences)
		{
			if (kvp.first.compare(assetFile) == 0)
				return kvp.second;
		}

		//Does File Exists?
		struct _stat buff;
		int result = -1;
		result = _wstat(assetFile.c_str(), &buff);
		if (result != 0)
		{
			wstringstream ss;
			ss << "File not found!\nPath: ";
			ss << assetFile;
			Logger::GetInstance()->LogError(ss.str());
		}

		T* pContent = LoadContent(assetFile);
		if (pContent != nullptr)m_ContentReferences.insert(pair<wstring, T*>(assetFile, pContent));

		return pContent;
	}

	void Unload() override
	{
		--m_LoaderReferences;

		if (m_LoaderReferences <= 0)
		{
			for (pair<wstring, T*> kvp : m_ContentReferences)
			{
				Destroy(kvp.second);
			}

			m_ContentReferences.clear();
		}
	}

protected:
	virtual T* LoadContent(const wstring& assetFile) = 0;
	virtual void Destroy(T* pObjToDestroy) = 0;

private:
	static unordered_map<wstring, T*> m_ContentReferences;
	static int m_LoaderReferences;
};

template<class T>
unordered_map<wstring, T*> ContentLoader<T>::m_ContentReferences = unordered_map<wstring, T*>();

template<class T>
int ContentLoader<T>::m_LoaderReferences = 0;
