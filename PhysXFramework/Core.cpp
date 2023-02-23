#include "stdafx.h"
#include "Core.h"
#include <chrono>
#include "SceneManager.h"
#include "InputManager.h"
#include "PhysxManager.h"
#include "ContentManager.h"
#include "SoundManager.h"
#include "GameScene.h"
#include "SceneSelector.h"

GameSettings Core::m_GameSettings{};

Core::Core()
	: m_WindowClassName{ L"GP2_PhysXFramework_2022" }
	, m_IsActive{ true }
	, m_pSceneManager{ new SceneManager{} }

{
	if (!XMVerifyCPUSupport()) Logger::GetInstance()->LogError(L"XMVerifyCPUSupport: failed");
}

Core::~Core()
{
	SafeRelease(m_pDXGIAdapter);
	SafeRelease(m_pDXGIOutput);

	//Game Cleanup
	delete m_pSceneManager;

	// delete singletons to prevent memleak reports
	delete PhysxManager::GetInstance();
	delete Logger::GetInstance();
	delete ContentManager::GetInstance();
	delete SoundManager::GetInstance();

	SafeRelease(m_pRenderTargetView);
	SafeRelease(m_pDepthStencilView);
	SafeRelease(m_pTexDepthStencilBuffer);
	SafeRelease(m_pDXGIFactory);
	SafeRelease(m_pSwapChain);

	if (m_D3D11.pDeviceContext)
	{
		m_D3D11.pDeviceContext->ClearState();
		m_D3D11.pDeviceContext->Flush();
		SafeRelease(m_D3D11.pDeviceContext);
	}

	SafeRelease(m_D3D11.pDevice);
}

void Core::AddScenes()
{
	dae::AddScenes(m_pSceneManager);
	//GameScene::EnablePhysXFrameStepping(true);

	Logger::GetInstance()->LogInfo(L"Press \'Page Up\' or \'Page Down\' to cycle between the different scenes");
	Logger::GetInstance()->LogInfo(L"Press \'I\' to toggle the debug renderer.");
	Logger::GetInstance()->LogInfo(L"Press \'M\' to do physics stepping.");
	Logger::GetInstance()->LogInfo(L"Press \'L\' to stop doing physics stepping.");

	m_pSceneManager->Initialize();
	Logger::GetInstance()->LogDebug(L"Scenes Initialized!");
}

void Core::StateChanged(StateType state, bool active)
{
	switch (state)
	{
		//WINDOW ACTIVE/INACTIVE
	case StateType::window:
		if (m_IsActive != active)
		{
			m_IsActive = active;
			m_pSceneManager->WindowStateChanged(active);
		}
		break;
		//INPUT ACTIVE/INACTIVE
	case StateType::input:
		InputManager::SetEnabled(active);
		break;
	}
}

HRESULT Core::RunGame(HINSTANCE hInstance)
{
	// provide the locator with a reference to my D3D11 object
	// objects that need D3D11, use the Locator service
	// preventing having to clutter code by passing the d3d11 to everywhere

	Locator::SetD3D11(m_D3D11);

	m_hInstance = hInstance;
	HRESULT hr;

	//Initialize Debug Logger (Needed for logging during Initialization)
	Logger::GetInstance()->Initialize();
#ifdef _DEBUG
	Logger::GetInstance()->StartFileLogging(L"Log.txt");
#endif

	//PREPARE GAME SETTINGS:
	m_GameSettings.isVSyncOn = true; // bij true, cubes bounce
	m_GameSettings.clearColor = XMFLOAT3(DirectX::Colors::CornflowerBlue);
	m_GameSettings.width = 1280;
	m_GameSettings.height = 720;

	//0. Query directx for adapters and output screen information (dpi aware)
	hr = InitalizeAdapterAndOutput();
	if (FAILED(hr)) return hr;
	Logger::GetInstance()->LogDebug(L"InitalizeAdapterAndOutput ok.");

	//1. Create the Window Class
	hr = CreateWindowClass();
	if (FAILED(hr)) return hr;
	Logger::GetInstance()->LogDebug(L"Window Class Created");

	//2. Open the window
	hr = OpenWindow();
	if (FAILED(hr)) return hr;
	Logger::GetInstance()->LogDebug(L"Window Opened");

	//3. Direct3D Init
	hr = InitializeDirect3D();
	if (FAILED(hr)) return hr;
	Logger::GetInstance()->LogDebug(L"Direct3D Initialized");

	//4. Initialize Managers
	ContentManager::GetInstance()->Initialize(m_D3D11.pDevice);
	PhysxManager::GetInstance();
	SoundManager::GetInstance();
	Logger::GetInstance()->LogDebug(L"Managers Initialized!");

	//5. Add Game scenes and initialize the scenes
	AddScenes(); // pass width/height for camera aspect ratio

	//6. Start the message pump
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// prepare for rendering, clear background, etc
		XMVECTOR clearColor = XMLoadFloat3(&m_GameSettings.clearColor);
		m_D3D11.pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor.m128_f32);
		m_D3D11.pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_pSceneManager->Update();
		SoundManager::GetInstance()->Update();
		m_pSceneManager->Draw();

		// swap front and backbuffer: Display next rendered frame
		m_pSwapChain->Present(m_GameSettings.isVSyncOn ? 1 : 0, 0);
	}

	//7. Finished
	Logger::GetInstance()->LogDebug(L"Core Initialization Finished");
	return (int)msg.wParam;
}

HRESULT Core::InitalizeAdapterAndOutput()
{
	HRESULT hr;

	//Create DXGIFactory
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&m_pDXGIFactory));
	if (Logger::GetInstance()->LogHResult(hr, L"InitializeDirect3D::CreateDXGIFactory"))
	{
		return hr;
	}

	//Allows DXGI to monitor an application's message queue for the alt-enter key sequence
	//(which causes the application to switch from windowed to full screen or vice versa).
	//hr = pDXGIFactory->MakeWindowAssociation(m_WindowHandle, 0);
	//if(AlertOnFail(hr, L"InitializeDirect3D: ::MakeWindowAssociation(...)"))return hr;

	//Get all Graphics Adapters
	UINT i = 0;
	while (m_pDXGIFactory->EnumAdapters(i, &m_pDXGIAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC adapterDescriptor;
		m_pDXGIAdapter->GetDesc(&adapterDescriptor);
		Logger::GetInstance()->LogFormat(LogLevel::Debug, L"Adapter %i : %s", i, adapterDescriptor.Description);

		++i;
	}

	Logger::GetInstance()->LogFormat(LogLevel::Debug, L"%i Adapter(s) found", i);

	// use the first adapter
	hr = m_pDXGIFactory->EnumAdapters(0, &m_pDXGIAdapter);

	// store the gi output adapter (has info on desktop coordinates)
	IDXGIOutput* tempOutput{};
	hr = m_pDXGIAdapter->EnumOutputs(0, &tempOutput);
	if (FAILED(hr)) return hr;

	hr = tempOutput->QueryInterface(__uuidof(IDXGIOutput), reinterpret_cast<void**>(&m_pDXGIOutput));
	if (FAILED(hr)) return hr;
	tempOutput->Release();

	return hr;
}

HRESULT Core::CreateWindowClass()
{
	m_WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WindowClass.hIcon = nullptr;
	m_WindowClass.hbrBackground = nullptr;
	m_WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	m_WindowClass.lpfnWndProc = WindowsProcedureStatic;
	m_WindowClass.hInstance = GetModuleHandle(0);
	m_WindowClass.lpszClassName = m_WindowClassName.c_str();

	if (!RegisterClass(&m_WindowClass))
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}
	return S_OK;
}

HRESULT Core::OpenWindow()
{
	DXGI_OUTPUT_DESC outputDesc;
	auto hr = m_pDXGIOutput->GetDesc(&outputDesc);
	if (FAILED(hr))return hr;
	Logger::GetInstance()->LogFormat(LogLevel::Debug, L"Desktop resolution: %i x %i", outputDesc.DesktopCoordinates.right, outputDesc.DesktopCoordinates.bottom);
	//Calculate position (centered)
	RECT R = { 0,0,m_GameSettings.width,m_GameSettings.height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	LONG windowWidth = R.right - R.left;
	LONG windowHeight = R.bottom - R.top;

	//Calculate the position of the window to be centered on the desktop
	int posX = outputDesc.DesktopCoordinates.left + ((outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left) / 2) - windowWidth / 2;
	int posY = (outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top) / 2 - windowHeight / 2;

	m_D3D11.windowHandle = CreateWindowW(m_WindowClassName.c_str(),
		m_GameSettings.windowTitle.c_str(),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		posX,
		posY,
		windowWidth,
		windowHeight,
		NULL,
		0,
		m_hInstance,
		this);//'this' is retrieved in WindowsProcedureStatic by lpCreateParams

	if (m_D3D11.windowHandle == NULL)
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}

	//Show the window
	ShowWindow(m_D3D11.windowHandle, SW_SHOWDEFAULT);
	return S_OK;
}

HRESULT Core::InitializeDirect3D()
{
	HRESULT hr;

	//4.2.1 Create the Device and Context
	UINT createDeviceFlags{ 0 };

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	hr = D3D11CreateDevice(m_pDXGIAdapter,//when pDXGIAdapter is used, nex param must be D3D_DRIVER_TYPE_UNKNOWN
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&m_D3D11.pDevice,
		&featureLevel,
		&m_D3D11.pDeviceContext
	);

	if (Logger::GetInstance()->LogHResult(hr, L"InitializeDirect3D: D3D11CreateDevice"))
	{
		return hr;
	}

	if (featureLevel < D3D_FEATURE_LEVEL_10_0)
	{
		Logger::GetInstance()->LogError(L"Feature level 10.0+ not supported on this device!");
		exit(-1);
	}
	if (featureLevel < D3D_FEATURE_LEVEL_11_0)
	{
		Logger::GetInstance()->LogWarning(L"Feature level 10.1, some DirectX11 specific features won't be available on this device!");
	}

	//Create Swapchain descriptor
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferDesc.Width = m_GameSettings.width;
	swapChainDesc.BufferDesc.Height = m_GameSettings.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_D3D11.windowHandle;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hr = m_pDXGIFactory->CreateSwapChain(m_D3D11.pDevice, &swapChainDesc, &m_pSwapChain);
	if (FAILED(hr))	return hr;

	//release temporary resources
	m_pDXGIFactory->Release();

	//4.2.5 Create the Render Target View
	void* pRenderTargetBuffer{ nullptr };

	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pRenderTargetBuffer);
	if (Logger::GetInstance()->LogHResult(hr, L"InitializeDirect3D: m_pSwapChain->GetBuffer"))
	{
		return hr;
	}

	hr = m_D3D11.pDevice->CreateRenderTargetView(static_cast<ID3D11Resource*>(pRenderTargetBuffer), 0, &m_pRenderTargetView);
	if (Logger::GetInstance()->LogHResult(hr, L"InitializeDirect3D: m_pDevice->CreateRenderTargetView"))
	{
		return hr;
	}

	if (pRenderTargetBuffer)static_cast<ID3D11Resource*>(pRenderTargetBuffer)->Release();

	//4.2.6 Create the Depth/Stencil Buffer and View
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = m_GameSettings.width;
	depthStencilDesc.Height = m_GameSettings.height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = m_D3D11.pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pTexDepthStencilBuffer);
	if (Logger::GetInstance()->LogHResult(hr, L"InitializeDirect3D: m_pDevice->CreateTexture2D  m_pTexDepthStencilBuffer"))
	{
		return hr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = depthStencilDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_D3D11.pDevice->CreateDepthStencilView(m_pTexDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if (Logger::GetInstance()->LogHResult(hr, L"InitializeDirect3D: m_pDevice->CreateDepthStencilView"))
	{
		return hr;
	}

	//4.2.7 Bind the Views to the Output Merger Stage
	m_D3D11.pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//4.2.8 Set the Viewport
	m_Viewport.Width = float(m_GameSettings.width);
	m_Viewport.Height = float(m_GameSettings.height);
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_D3D11.pDeviceContext->RSSetViewports(1, &m_Viewport);

	return S_OK;
}

#pragma region
LRESULT CALLBACK Core::WindowsProcedureStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CREATE)
	{
		CREATESTRUCT* pCS = (CREATESTRUCT*)lParam;
		auto par = (LONG_PTR)pCS->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pCS->lpCreateParams);
	}
	else
	{
		Core* pThisGame = (Core*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (pThisGame) return pThisGame->WindowsProcedure(hWnd, message, wParam, lParam);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Core::SetWindowTitle(const std::wstring& title)
{
	SetWindowText(Locator::GetD3D11()->windowHandle, title.c_str());
}

LRESULT Core::WindowsProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_pSceneManager->ProcessMessage(message, wParam, lParam);

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYUP:
	{
		if ((lParam & 0x80000000) != 0x80000000)
			return -1;

		// NextScene
		if (wParam == VK_NEXT || wParam == 'P')
		{
			m_pSceneManager->NextScene();
			return 0;
		}

		// PreviousScene
		if (wParam == VK_PRIOR || wParam == 'O')
		{
			m_pSceneManager->PreviousScene();
			return 0;
		}

		// swap physx line rendering
		if (wParam == 'I')
		{
			m_pSceneManager->ToggleDebugRenderer();
			return 0;
		}

		// PhysX enable Time Stepping and make a step
		else if (wParam == 'M')
		{
			GameScene::EnablePhysXFrameStepping(true);
			GameScene::SetPhysicsStepTime(1.0f / 60);
			return 0;
		}

		// PhysX disable timestepping
		else if (wParam == 'L')
		{
			GameScene::EnablePhysXFrameStepping(false);
			return 0;
		}
	}
	// https://docs.microsoft.com/en-us/windows/desktop/inputdev/wm-activate
	case WM_ACTIVATE:
		//If the low - order word of wParam is WA_ACTIVE or WA_CLICKACTIVE, lParam is the handle to the window being deactivated.
		if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
		{
			if (HWND(wParam) == m_D3D11.windowHandle)
			{
				StateChanged(StateType::input, false);
			}
			else StateChanged(StateType::input, true);
		}
		//If the low-order word of wParam is WA_INACTIVE, lParam is the handle to the window being activated.
		if (wParam == WA_INACTIVE)
		{
			if (HWND(wParam) == m_D3D11.windowHandle)
			{
				StateChanged(StateType::input, true);
			}
			else StateChanged(StateType::input, false);
		}
		return 0;

		//Sent to a window after its size has changed.
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			StateChanged(StateType::input, false);
		else if (wParam == SIZE_RESTORED)
			StateChanged(StateType::input, true);
		return 0;

		// Sent to a window after it has gained the keyboard focus. wParam: A handle to the window that has lost the keyboard focus.
	case WM_SETFOCUS:
		StateChanged(StateType::input, true);
		return 0;

		// Sent to a window immediately before it loses the keyboard focus. wParam: A handle to the window that receives the keyboard focus.
	case WM_KILLFOCUS:
		StateChanged(StateType::input, false);
		return 0;

	case WM_ENTERSIZEMOVE:
		StateChanged(StateType::input, false);
		StateChanged(StateType::window, false);
		return 0;

	case WM_EXITSIZEMOVE:
		StateChanged(StateType::input, true);
		StateChanged(StateType::window, true);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

#pragma endregion Windows Procedures
