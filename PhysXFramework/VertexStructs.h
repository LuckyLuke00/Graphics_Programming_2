#pragma once
#include "stdafx.h"

struct VertexBase
{
	XMFLOAT3 Position{};
	XMFLOAT4 Color{ XMFLOAT4(.3f,.4f,.4f,1.f) };
	XMFLOAT3 Normal{};
	XMFLOAT2 TexCoord{};
};

struct VertexPosCol
{
	VertexPosCol() = default;
	VertexPosCol(XMFLOAT3 pos, XMFLOAT4 col) :
		Position(pos),
		Color(col)
	{}

	VertexPosCol(const VertexBase& vertex) :
		Position(vertex.Position),
		Color(vertex.Color)
	{}

	XMFLOAT3 Position{};
	XMFLOAT4 Color{};
};

struct VertexPosColNorm
{
	VertexPosColNorm() = default;
	VertexPosColNorm(XMFLOAT3 pos, XMFLOAT4 col, XMFLOAT3 norm) :
		Position(pos),
		Color(col),
		Normal(norm)
	{}
	VertexPosColNorm(const VertexBase& vertex) :
		Position(vertex.Position),
		Color(vertex.Color),
		Normal(vertex.Normal)
	{}

	XMFLOAT3 Position{};
	XMFLOAT4 Color{};
	XMFLOAT3 Normal{};
};

struct VertexPosColNormTex
{
	VertexPosColNormTex() = default;
	VertexPosColNormTex(const VertexBase& vertex) :
		Position(vertex.Position),
		Color(vertex.Color),
		Normal(vertex.Normal),
		TexCoord(vertex.TexCoord)
	{}

	XMFLOAT3 Position{};
	XMFLOAT4 Color{};
	XMFLOAT3 Normal{};
	XMFLOAT2 TexCoord{};
};

struct VertexPosNormTex
{
	VertexPosNormTex() = default;
	VertexPosNormTex(const VertexBase& vertex) :
		Position(vertex.Position),
		Normal(vertex.Normal),
		TexCoord(vertex.TexCoord)
	{}

	XMFLOAT3 Position{};
	XMFLOAT3 Normal{};
	XMFLOAT2 TexCoord{};
};
