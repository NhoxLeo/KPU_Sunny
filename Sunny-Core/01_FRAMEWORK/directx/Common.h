#pragma once

#ifdef _MSC_VER 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib") 
#endif

/* DirectX 11 */
#include <d3d11.h>                  // Direct3D 11�� ����ϱ� ���� ���̺귯��
#include <d3dcompiler.h>            // Shader �ڵ� �������� ���� ���̺귯��

enum DIMENSION { D2 = 0, D3 = 1};