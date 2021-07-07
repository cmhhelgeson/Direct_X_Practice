#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_system.h>
#include "C:\SDL2-w64\include\SDL2\SDL_image.h"
#include <iostream>
#include <stdio.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;

typedef struct Vec4 {
	float x, y, z, w;
} Vec4;

void InitializeVec4(Vec4& v, float _x, float _y, float _z, float _w) {
	v.x = _x;
	v.y = _y;
	v.z = _z;
	v.w = _w;
}


/* Graphics helper functions are forward declared
	Sometimes we do this for reasons relating to classes? */
bool CreateDeviceD3D(HWND hWnd);
void CleanDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

static ID3D11Device* p_3D_DEVICE = NULL;
static ID3D11DeviceContext* p_3D_DEVICE_CONTEXT = NULL;
static IDXGISwapChain* p_SWAP_CHAIN = NULL;
static ID3D11RenderTargetView* p_RENDER_TARGET_VIEW = NULL;



int main(int argc, char* args[]) {
	Vec4 v;
	InitializeVec4(v, 0.45f, 0.55f, 0.60f, 1.00f);
	SDL_Window* window;
	SDL_SysWMinfo info;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		std::cout << "SDL Initialization failed" << std::endl;
		std::cout << "Press Enter to exit program: " << std::endl;
		std::cin.get();
		return -1;
	}

	SDL_WindowFlags window_flags =
		(SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	window = SDL_CreateWindow("SDL2",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
		window_flags);

	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo(window, &info);
	HWND window_handle = (HWND)info.info.win.window;

	//Initialize Direct3D
	if (!CreateDeviceD3D(window_handle)) {
		CleanDeviceD3D();
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	};

	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(window))
			{
				// Release all outstanding references to the swap chain's buffers before resizing.
				CleanupRenderTarget();
				p_SWAP_CHAIN->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
				CreateRenderTarget();
			}
		}

		// Rendering
		const float clear_color_with_alpha[4] = { v.x * v.w, v.y * v.w, v.z * v.w, v.w };
		p_3D_DEVICE_CONTEXT->OMSetRenderTargets(1, &p_RENDER_TARGET_VIEW, NULL);
		p_3D_DEVICE_CONTEXT->ClearRenderTargetView(p_RENDER_TARGET_VIEW, clear_color_with_alpha);



		p_SWAP_CHAIN->Present(1, 0); // Present with vsync
		//g_pSwapChain->Present(0, 0); // Present without vsync
	}



	CleanDeviceD3D();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

bool CreateDeviceD3D(HWND win_handle) {
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	//Only one back buffer.
	sd.BufferCount = 1;
	//Set width and height automatically
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	//Colors
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//For frame rate
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	//Allows switching between full-screen and windowed modes
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.OutputWindow = win_handle;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	//Discard the contents of the back buffer after they are displaye to screen
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	if (D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&p_SWAP_CHAIN,
		&p_3D_DEVICE,
		nullptr,
		&p_3D_DEVICE_CONTEXT
	) != S_OK) {
		return false;
	}

	//UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//D3D_FEATURE_LEVEL featureLevel;
	//const D3D_FEATURE_LEVEL featureLevelArray[2] = 
	//	{ D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

	//if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 
	//createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		//return false;
	return true;

}

void CleanDeviceD3D() {
	CleanupRenderTarget();
	if (p_SWAP_CHAIN) {
		p_SWAP_CHAIN->Release();
		p_SWAP_CHAIN = NULL;
	}

	if (p_3D_DEVICE_CONTEXT) {
		p_3D_DEVICE_CONTEXT->Release();
		p_3D_DEVICE_CONTEXT = NULL;
	}

	if (p_3D_DEVICE) {
		p_3D_DEVICE->Release();
		p_3D_DEVICE = NULL;
	}
}

void CreateRenderTarget() {
	//Creates the back buffer
	ID3D11Texture2D* p_back_buffer;
	p_SWAP_CHAIN->GetBuffer(0, IID_PPV_ARGS(&p_back_buffer));
	p_3D_DEVICE->CreateRenderTargetView(p_back_buffer, nullptr,
		&p_RENDER_TARGET_VIEW);
	p_back_buffer->Release();
}

void CleanupRenderTarget() {
	if (p_RENDER_TARGET_VIEW) {
		p_RENDER_TARGET_VIEW->Release();
		p_RENDER_TARGET_VIEW = NULL;
	}
}