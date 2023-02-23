// Bart: service locator design pattern
// http://gameprogrammingpatterns.com/service-locator.html
// Provide a global point of access to a service without
// coupling users to the concrete class that implements it.

#pragma once
#include "D3D11.h"
#include "Core.h"

class Locator
{
	friend Core;
public:
	static D3D11* GetD3D11();
private:
	void static SetD3D11(D3D11& d3d11);
	static D3D11* m_pD3D11;
};
