#pragma once
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>


#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <string>
#include <Windows.h>
#include <wrl.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include "d3dUtil.h"
#include "MathHelper.h"

// �ʿ��� d3d12 ���̺귯�� ��ũ
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

BEGIN(Engine)
	enum MOUSEKEYSTATE { LBTN, RBTN, MBTN, MKS_END };
	enum MOUSEMOVESTATE { X, Y, Z };
END

using namespace std;
using namespace Engine;
using namespace DirectX;