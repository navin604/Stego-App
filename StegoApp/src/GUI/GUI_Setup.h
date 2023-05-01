#pragma once

#include <iostream>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>
#include <iostream>

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

class GUI
{
	public:
		GUI();
		~GUI();
		void Run();
		void Init();
		void cleanup();
		void render(ImGuiIO& io, ImVec4& clear_color);
		WNDCLASSEXW wc;
		HWND hwnd;
		
};
