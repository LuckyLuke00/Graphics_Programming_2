#pragma once
#include <chrono>

class GameTime
{
public:
	GameTime() = default;
	~GameTime() = default;

	GameTime(const GameTime& other) = delete;
	GameTime(GameTime&& other) noexcept = delete;
	GameTime& operator=(const GameTime& other) = delete;
	GameTime& operator=(GameTime&& other) noexcept = delete;

	void Reset();
	void Update();

	float GetElapsed() const { return  m_ElapsedGameTime; }
	float GetTotal() const { return m_TotalGameTime; }
	int GetFPS() const { return m_FPS; }
	void ForceElapsedUpperbound(bool force, float upperBound = 0.03f) { m_ForceElapsedUpperBound = force; m_ElapsedUpperBound = upperBound; }
	bool IsRunning() const { return !m_IsStopped; }

	void Start();
	void Stop();

private:
	float m_TotalGameTime{};
	float m_ElapsedGameTime{};

	bool m_ForceElapsedUpperBound{};
	float m_ElapsedUpperBound{ 0.03f };

	std::chrono::high_resolution_clock::time_point m_BaseTime{};
	std::chrono::high_resolution_clock::time_point m_StopTime{};
	std::chrono::high_resolution_clock::time_point m_PrevTime{};
	std::chrono::high_resolution_clock::time_point m_CurrTime{};
	std::chrono::high_resolution_clock::duration m_PausedDuration{};

	bool m_IsStopped{ true };

	int m_FPS{};
	float m_FpsTimer{};
	int m_FpsCount{};
};
