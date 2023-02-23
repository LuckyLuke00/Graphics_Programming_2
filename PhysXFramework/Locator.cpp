#include "stdafx.h"
#include "Locator.h"

D3D11* Locator::m_pD3D11{};

D3D11* Locator::GetD3D11()
{
	return m_pD3D11;
}

void Locator::SetD3D11(D3D11& d3d11)
{
	m_pD3D11 = &d3d11;
}
