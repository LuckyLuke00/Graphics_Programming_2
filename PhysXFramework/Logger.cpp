#include "stdafx.h"

#include "Logger.h"
#include <fcntl.h>
#include <iomanip>
#include <comdef.h>

Logger::~Logger()
{
	StopFileLogging();
}

void Logger::Initialize()
{
	m_os = &std::wcout;

	for (int i = 0; i < MAX_PERFORMANCE_TIMERS; ++i)
		m_PerformanceTimerArr[i] = -1;

	LARGE_INTEGER li;
	if (QueryPerformanceFrequency(&li))
	{
		m_PcFreq = double(li.QuadPart) / 1000.0;
		//LOG ERROR
	}

	//#if defined(DEBUG) | defined(_DEBUG)
	if (AllocConsole())
	{
		// Redirect the CRT standard input, output, and error handles to the console
		FILE* pCout;
		freopen_s(&pCout, "CONIN$", "r", stdin);
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		freopen_s(&pCout, "CONOUT$", "w", stderr);

		//Clear the error state for each of the C++ standard stream objects. We need to do this, as
		//attempts to access the standard streams before they refer to a valid target will cause the
		//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
		//to always occur during startup regardless of whether anything has been read from or written to
		//the console or not.
		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();
		std::cin.clear();

		//Set ConsoleHandle
		m_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

		//Disable Close-Button
		HWND hwnd = GetConsoleWindow();
		if (hwnd != NULL)
		{
			HMENU hMenu = GetSystemMenu(hwnd, FALSE);
			if (hMenu != NULL) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}
	//#endif
}

int Logger::StartPerformanceTimer()
{
	int counter = 0;
	while (m_PerformanceTimerArr[counter] != -1)
	{
		++counter;
		if (counter == MAX_PERFORMANCE_TIMERS)
		{
			counter = -1;
			break;
		}
	}

	if (counter >= 0)
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		m_PerformanceTimerArr[counter] = li.QuadPart;
	}

	return counter;
}

double Logger::StopPerformanceTimer(int timerId)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double diff = double(li.QuadPart - m_PerformanceTimerArr[timerId]) / m_PcFreq;

	m_PerformanceTimerArr[timerId] = -1;

	return diff;
}

void Logger::LogDebug(const std::wstring& msg)
{
	Log(LogLevel::Debug, msg);
}

void Logger::LogInfo(const std::wstring& msg)
{
	Log(LogLevel::Info, msg);
}

void Logger::LogWarning(const std::wstring& msg)
{
	Log(LogLevel::Warning, msg);
}

void Logger::LogError(const std::wstring& msg)
{
	Log(LogLevel::Error, msg);
}

void Logger::LogToDo(const std::wstring& source)
{
	Log(LogLevel::ToDo, source);
}

bool Logger::LogHResult(HRESULT hr, const std::wstring& origin)
{
	if (FAILED(hr))
	{
		if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
			hr = HRESULT_CODE(hr);

		std::wstringstream ss;
		if (origin.size() != 0)
		{
			ss << ">ORIGIN:\n";
			ss << origin;
			ss << "\n\n";
		}
		ss << "HResult Error:\n(0x";

		_com_error err(hr);
		ss << std::hex << hr << ")\n" << err.ErrorMessage();

		Log(LogLevel::Error, ss.str());
		return true;
	}

	return false;
}

void Logger::StartFileLogging(const std::wstring& fileName)
{
	m_FileOutStream = new std::wofstream(fileName.c_str());
}

void Logger::StopFileLogging()
{
	if (m_FileOutStream)
	{
		m_FileOutStream->close();
		delete m_FileOutStream;
		m_FileOutStream = nullptr;
	}
}

void Logger::ClearConsole()
{
	std::system("cls");
}

void Logger::LogFormat(LogLevel level, const wchar_t* format, ...)
{
	va_list ap;

	va_start(ap, format);
	_vsnwprintf_s(&m_ConvertBuffer[0], m_ConvertBufferSize, m_ConvertBufferSize, format, ap);
	va_end(ap);
	Log(level, std::wstring(&m_ConvertBuffer[0]));
}

void Logger::Log(LogLevel level, const std::wstring& msg)
{
#ifdef NDEBUG
	if (level == LogLevel::Debug || level == LogLevel::ToDo)
	{
		return;
	}
#endif

	std::wstringstream stream;

	if (m_AppendTimestamp)
	{
		SYSTEMTIME st;
		GetSystemTime(&st);
		stream << std::setw(20) << std::left;
		stream << L"[" << st.wYear << L"-" << st.wMonth << L"-" << st.wDay << L" - ";
		stream << st.wHour << L":" << st.wMinute << L":" << st.wSecond << L":" << st.wMilliseconds << L"]";
	}
	else
	{
		stream << std::setw(10) << std::left;
	}

	switch (level)
	{
	case LogLevel::Debug:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		stream << L"[DEBUG]";
		break;
	case LogLevel::Info:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		stream << L"[INFO]";
		break;
	case LogLevel::Warning:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		stream << L"[WARNING]";
		break;
	case LogLevel::Error:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		stream << L"[ERROR]";
		break;
	case LogLevel::ToDo:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
		stream << L"[FIX-ME]";
		break;
	}

	stream << msg << std::endl;

	LogToFile(stream.str());

	//if error, break
	if (level == LogLevel::Error)
	{
		MessageBox(0, msg.c_str(), L"ERROR", MB_OK | MB_ICONERROR);
	}

	if ((m_BreakBitField & static_cast<int>(level)) == static_cast<int>(level))
	{
#if _DEBUG
		__debugbreak();
#else
		exit(-1);
#endif
	}
}

void Logger::LogToFile(const std::wstring& message)
{
	if (m_os)
	{
		(*m_os) << message;
		m_os->flush();
	}

	if (m_FileOutStream)
	{
		SYSTEMTIME st;
		GetSystemTime(&st);

		(*m_FileOutStream) << L"[" << st.wYear << L"-" << st.wMonth << L"-" << st.wDay << L" - " << st.wHour << L":" << st.wMinute << L":" << st.wSecond << L":" << st.wMilliseconds << L"]" << message;
	}
}
