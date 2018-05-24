#pragma once

#ifdef _MSC_VER 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib") 
#endif

/* DirectX 11 */
#include <d3d11.h>                  // Direct3D 11�� ����ϱ� ���� ���̺귯��
#include <d3dcompiler.h>            // Shader �ڵ� �������� ���� ���̺귯��

// 2D�� DirectX11 �� DeferredContext�� �׸���.
enum class DIMENSION : int { D2, D3 };