#pragma once
#include <typeinfo>
#include "ContentLoader.h"
#include "Singleton.h"

class ContentManager : public Singleton<ContentManager>
{
public:
	~ContentManager();
	ContentManager(const ContentManager& t) = delete;
	ContentManager& operator=(const ContentManager& t) = delete;

	void Initialize(ID3D11Device* pDevice);
	void AddLoader(BaseLoader* loader);

	template<class T>
	T* Load(const wstring& assetFile)
	{
		const type_info& ti = typeid(T);
		for (BaseLoader* loader : m_Loaders)
		{
			const type_info& loadertype = loader->GetType();
			if (loader->GetType() == ti)
			{
				return (static_cast<ContentLoader<T>*>(loader))->GetContent(assetFile);
			}
		}

		return nullptr;
	}

protected:
	ContentManager();
	friend Singleton<ContentManager>; // allow Singleton to call the constructor

private:

	std::vector<BaseLoader*> m_Loaders;
	ID3D11Device* m_pDevice;
	bool m_IsInitialized;
};
