#include "VertexTypes.h"

const unsigned long VertexPositionColor::fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const unsigned long VertexPositionNormal::fvf = D3DFVF_XYZ | D3DFVF_NORMAL;
const unsigned long VertexPosition::fvf = D3DFVF_XYZ;
const unsigned long VertexPositionTexture::fvf = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
const unsigned long VertexPositionNormalTexture::fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
const unsigned long VertexPositionColorTexture::fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;