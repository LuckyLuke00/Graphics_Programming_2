#pragma once
#include "Singleton.h"
#include <array>
#include <chrono>

enum class LogLevel
{
	Debug = 0x1,
	Info = 0x2,
	Warning = 0x4,
	Error = 0x8,
	ToDo = 0x16
};

class Logger final : public Singleton<Logger>
{
public:
	~Logger();
	Logger(const Logger& other) = delete;
	Logger(Logger&& other) noexcept = delete;
	Logger& operator=(const Logger& other) = delete;
	Logger& operator=(Logger&& other) noexcept = delete;

	void Initialize();
	int StartPerformanceTimer();
	double StopPerformanceTimer(int timerId);

	void Log(LogLevel level, const std::wstring& msg);
	void LogFormat(LogLevel level, const wchar_t* format, ...);
	void LogDebug(const std::wstring& msg);
	void LogInfo(const std::wstring& msg);
	void LogWarning(const std::wstring& msg);
	void LogError(const std::wstring& msg);
	void LogToDo(const std::wstring& source);
	bool LogHResult(HRESULT hr, const std::wstring& origin);
	void StartFileLogging(const std::wstring& fileName);
	void StopFileLogging();
	void ClearConsole();

	void BreakOnLog(LogLevel level, bool doBreak) { doBreak ? m_BreakBitField |= static_cast<int>(level) : m_BreakBitField &= ~static_cast<int>(level); }

	void AppendTimestamp(bool append) { m_AppendTimestamp = append; }

private:
	Logger() = default;
	void LogToFile(const std::wstring& message);

	std::wostream* m_os{};
	std::wofstream* m_FileOutStream{};

	double m_PcFreq{};
	static constexpr int MAX_PERFORMANCE_TIMERS{ 10 };
	__int64 m_PerformanceTimerArr[MAX_PERFORMANCE_TIMERS]{};
	std::chrono::high_resolution_clock::time_point m_TimePoints[MAX_PERFORMANCE_TIMERS]{};

	int m_BreakBitField{ static_cast<int>(LogLevel::Error) };
	bool m_AppendTimestamp{ false };

	static constexpr size_t m_ConvertBufferSize{ 1024 };
	std::array<wchar_t, m_ConvertBufferSize> m_ConvertBuffer{};

	HANDLE m_ConsoleHandle{ nullptr };

	friend class Game;
	friend Singleton<Logger>;
};
