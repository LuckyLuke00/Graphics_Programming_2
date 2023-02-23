#pragma once
#include "Singleton.h"

class SoundManager : public Singleton<SoundManager>
{
public:
	SoundManager();
	~SoundManager();
	SoundManager(const SoundManager& obj) = delete;
	SoundManager& operator=(const SoundManager& obj) = delete;

	bool ErrorCheck(FMOD_RESULT res);
	FMOD::System* GetSystem() const { return m_pFmodSystem; }
	void Update();
protected:
	friend Singleton<SoundManager>; // allow Singleton to call the constructor

private:
	void Initialize();
	FMOD::System* m_pFmodSystem;
};
