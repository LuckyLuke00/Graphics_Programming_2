#pragma once

struct D3D11
{
	ID3D11Device* pDevice{};
	ID3D11DeviceContext* pDeviceContext{};
	HWND windowHandle{};
};
