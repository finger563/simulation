//***************************************************************************************
// Link Libraries & Include Headers
//***************************************************************************************
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

// FONT
#pragma comment (lib, "D3D10_1.lib")
#pragma comment (lib, "DXGI.lib")
#pragma comment (lib, "D2D1.lib")
#pragma comment (lib, "dwrite.lib")
#include <D3D10_1.h>
#include <DXGI.h>
#include <D2D1.h>
#include <sstream>
#include <dwrite.h>

// Error checking
#pragma comment (lib, "DXErr.lib")
#include <DXErr.h>

// Direct Input
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

//Skybox
#include <vector>

//***************************************************************************************
// Global Interface Declarations
//***************************************************************************************
IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;

ID3D11Buffer* squareIndexBuffer;
ID3D11Buffer* squareVertBuffer;

// Interface Object to store depth and stencil view
ID3D11DepthStencilView* depthStencilView;
// Interface Object to store depth and stencil buffer
ID3D11Texture2D* depthStencilBuffer;

ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D10Blob* VS_Buffer;
ID3D10Blob* PS_Buffer;
ID3D11InputLayout* vertLayout;

/*
 * A constant buffer is a structure in an effect file which holds variables 
 * we are able to update from our game code. 
 * We can create a constant buffer using the cbuffer type. 
 *
 * Constant buffers should be seperated by how often they are updated. 
 * This way we can call them as least as possible since it takes processing time to do it. 
 * Examples of different frequencies we call a constant buffer are:
 * per scene (call the buffer only once per scene, such as lighting that does not change throughout the scene)
 * per frame (such as lighting that does change position or something every frame, like the sun moving across the sky)
 * per object (Like what we will do. We update the WVP per object, since each object has a different World space, or position, rotation, or scaling).
 */
ID3D11Buffer* cbPerObjectBuffer;

/*
 * First new interface is an object which will hold our texture we load from the file. 
 * The second will hold our sampler state information.
 */
ID3D11ShaderResourceView* CubesTexture;
ID3D11SamplerState* CubesTexSamplerState;

/*
 * Declaration of the interface object which we will use to set the render state of the RS stage of the pipeline.
 */
ID3D11RasterizerState* WireFrame;

/*
 * We create one interface to hold our blend state, 
 * and two interfaces for our rasterizer states, one for counter clockwise culling, and the other for clockwise.
 */
ID3D11BlendState* Transparency;
ID3D11RasterizerState* CCWcullMode;
ID3D11RasterizerState* CWcullMode;

/*
 * Interfaces required to render text
 */
ID3D10Device1 *d3d101Device;	
IDXGIKeyedMutex *keyedMutex11;
IDXGIKeyedMutex *keyedMutex10;	
ID2D1RenderTarget *D2DRenderTarget;	
ID2D1SolidColorBrush *Brush;
ID3D11Texture2D *BackBuffer11;
ID3D11Texture2D *sharedTex11;	
ID3D11Buffer *d2dVertBuffer;
ID3D11Buffer *d2dIndexBuffer;
ID3D11ShaderResourceView *d2dTexture;
IDWriteFactory *DWriteFactory;
IDWriteTextFormat *TextFormat;
std::wstring printText;

/*
 * Interfaces for Simple Lighting
 */
ID3D11Buffer* cbPerFrameBuffer;
ID3D11PixelShader* D2D_PS;
ID3D10Blob* D2D_PS_Buffer;

/*
 * Interfaces for Keyboard and Mouse Input
 */
IDirectInputDevice8* DIKeyboard;
IDirectInputDevice8* DIMouse;

/*
 * Interfaces for Skybox - Cube Mapping
 */
ID3D11Buffer* sphereIndexBuffer;
ID3D11Buffer* sphereVertBuffer;

ID3D11VertexShader* SKYMAP_VS;
ID3D11PixelShader* SKYMAP_PS;
ID3D11PixelShader* TESSELLATE_PS;
ID3D11HullShader* TESS_HS;
ID3D11DomainShader *TESS_DS;
ID3D10Blob* SKYMAP_VS_Buffer;
ID3D10Blob* SKYMAP_PS_Buffer;
ID3D10Blob* TESSELLATE_PS_Buffer;
ID3D10Blob* TESS_HS_Buffer;
ID3D10Blob* TESS_DS_Buffer;

ID3D11ShaderResourceView* smrv;

ID3D11DepthStencilState* DSLessEqual;
ID3D11RasterizerState* RSCullNone;
//***************************************************************************************
// Other Global Declarations
//***************************************************************************************
LPCTSTR WndClassName = L"Application_Window";
HWND hwnd = NULL;
HRESULT hr;

const int Width  = 1920;
const int Height = 1000;

/*
 * Four matrices & three vectors. 
 * The four matrices represent each of the different spaces our objects vertices need to go through to get onto the screen.
 * The three vectors are used to define the position, target, and up direction of our camera.
 */
XMMATRIX WVP;
XMMATRIX World;
XMMATRIX camView;
XMMATRIX camProjection;

// World space matrices for two cubes
XMMATRIX cube1World;
XMMATRIX cube2World;
XMMATRIX Rotation;
XMMATRIX Scale;
XMMATRIX Translation;
float rot = 0.01f;

XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;

/*
 * First Person Camera
 */
XMVECTOR DefaultForward = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
XMVECTOR DefaultRight = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);
XMVECTOR camForward = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
XMVECTOR camRight = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);

XMMATRIX camRotationMatrix;
XMMATRIX groundWorld;

float moveLeftRight = 0.0f;
float moveBackForward = 0.0f;

float camYaw = 0.0f;
float camPitch = 0.0f;

/*
 * Skybox
 */
int NumSphereVertices;
int NumSphereFaces;

XMMATRIX sphereWorld;

/*
 * High Resolution Timer variables
 */
double countsPerSecond = 0.0;
__int64 CounterStart = 0;

int frameCount = 0;
int fps = 0;

__int64 frameTimeOld = 0;
double frameTime;

/*
 * Mouse-specific variables
 */
DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;

float rotx = 0;
float rotz = 0;
float scaleX = 1.0f;
float scaleY = 1.0f;

XMMATRIX Rotationx;
XMMATRIX Rotationy;
XMMATRIX Rotationz;

//***************************************************************************************
// Primary Function Prototypes
//***************************************************************************************
bool InitializeDirect3d11App(HINSTANCE hInstance);
void CleanUp();
bool InitScene();
void UpdateScene(double time);
void DrawScene();

/*
 * Camera Update Function
 */
void UpdateCamera();

/*
 * Skybox Sphere
 */
void CreateSphere(int LatLines, int LongLines);

/*
 * Functions to Render Text:
 * 
 * The first function will initailize D3D10.1, D2D, and DirectWrite, using a shared surface. 
 * The only parameter here is the Adapter, since we need to make sure we use the same adapter as the D3D11 Device. 
 * The second function will create a square and a shader resource view from the shared texture 
 * so we can render it onto the square.
 */
bool InitD2D_D3D101_DWrite(IDXGIAdapter1 *Adapter);
void InitD2DScreenTexture();
void RenderText(std::wstring text, int inInt);

/*
 * High Resolution Timer Functions
 */
void StartTimer();
double GetTime();
double GetFrameTime();

bool InitializeWindow(HINSTANCE hInstance,
					  int ShowWnd,
					  int width, int height,
					  bool windowed);
int messageloop();

LRESULT CALLBACK WndProc(HWND hWnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam);

/*
 * Our constant buffer in code is the same exact layout of the structure in the constant buffer in the effect file. 
 * Then we create a constant buffer object.
 */
struct cbPerObject
{
	XMMATRIX  WVP;
    XMMATRIX  World;
};

cbPerObject cbPerObj;


/*
 * Functions to Initialize and Detect Input 
 */
bool InitDirectInput(HINSTANCE hInstance);
void DetectInput(double time);

//***************************************************************************************
// Light
//***************************************************************************************
struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 dir;
	float pad;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

Light light;

struct cbPerFrame
{
	Light  light;
};

cbPerFrame constbuffPerFrame;
//***************************************************************************************
// Vertex Structure and Vertex (Input) Layout
//***************************************************************************************
struct Vertex	//Overloaded Vertex Structure
{
	Vertex(){}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x,y,z), texCoord(u, v), normal(nx, ny, nz){}

	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
};

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },  
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },  
	{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

UINT numElements = ARRAYSIZE(layout);

//***************************************************************************************
// High Resolution Timer Functions
//***************************************************************************************
void StartTimer()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	countsPerSecond = double(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	CounterStart = frequencyCount.QuadPart;
}

double GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return double(currentTime.QuadPart-CounterStart)/countsPerSecond;
}

double GetFrameTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart-frameTimeOld;
	frameTimeOld = currentTime.QuadPart;

	if(tickCount < 0.0f)
		tickCount = 0.0f;

	return float(tickCount)/countsPerSecond;
}

//***************************************************************************************
// WinMain - MAIN FUNCTION OF THE APPLICATION
//***************************************************************************************
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine,
				   int nShowCmd)
{
	if(!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBox(0, L"Window Initialization - Failed",
						L"Error", MB_OK);
		return 0;
	}

	if(!InitializeDirect3d11App(hInstance))	
	{
		MessageBox(0, L"Direct3D Initialization - Failed",
						L"Error", MB_OK);
		return 0;
	}

	if(!InitScene())	
	{
		MessageBox(0, L"Scene Initialization - Failed",
						L"Error", MB_OK);
		return 0;
	}

	if(!InitDirectInput(hInstance))
	{
		MessageBox(0, L"Direct Input Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}

	messageloop();
	CleanUp();    
	return 0;
}

bool InitializeWindow(HINSTANCE hInstance,
					  int ShowWnd,
					  int width, int height,
					  bool windowed)
{
	typedef struct _WNDCLASS {
		UINT cbSize;
		UINT style;
		WNDPROC lpfnWndProc;
		int cbClsExtra;
		int cbWndExtra;
		HANDLE hInstance;
		HICON hIcon;
		HCURSOR hCursor;
		HBRUSH hbrBackground;
		LPCTSTR lpszMenuName;
		LPCTSTR lpszClassName;
	} WNDCLASS;

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WndClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	/* 
	 * Now we register the class. If it fails we will get an error. 
	 * If it passes, we move on to create the window.
	 */
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",	
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	hwnd = CreateWindowEx( NULL,
						   WndClassName,
						   L"Rendering Engine",
						   WS_OVERLAPPEDWINDOW, // WS_OVERLAPPEDWINDOW
						   CW_USEDEFAULT, CW_USEDEFAULT,
						   width, height,
						   NULL,
						   NULL,
						   hInstance,
						   NULL);

	if (!hwnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, SW_MAXIMIZE); 
	UpdateWindow(hwnd);

	return true;
}


//***************************************************************************************
// Heart of our direct3d initialization - All initailization done here
//***************************************************************************************
bool InitializeDirect3d11App(HINSTANCE hInstance)
{
	/*
	 * Describe our back buffer
	 */
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	/*
	 * Describe our swap chain
	 */
	DXGI_SWAP_CHAIN_DESC swapChainDesc; 

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd; 
	swapChainDesc.Windowed = TRUE; 
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	/*
	 * Init steps to render text
	 * (1) Create a DXGI Factory to enumarate adapters
	 * (2) Use the first adapter
	 * (3) Create our D3D11 Device and SwapChain
	 * (4) Initialize Direct2D, Direct3D 10.1, DirectWrite
	 * (5) Release the Adapter interface
	 */
	IDXGIFactory1 *DXGIFactory;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory);	
	IDXGIAdapter1 *Adapter;
	hr = DXGIFactory->EnumAdapters1(0, &Adapter);
	DXGIFactory->Release();	
	hr = D3D11CreateDeviceAndSwapChain( Adapter, 
										D3D_DRIVER_TYPE_UNKNOWN, 
										NULL, 
										D3D11_CREATE_DEVICE_DEBUG |	D3D11_CREATE_DEVICE_BGRA_SUPPORT,		
										NULL, 
										NULL,	
										D3D11_SDK_VERSION, 
										&swapChainDesc, 
										&SwapChain, 
										&d3d11Device, 
										NULL, 
										&d3d11DevCon);
	InitD2D_D3D101_DWrite(Adapter);
	Adapter->Release();

	/*
	 * Create Back buffer
	 */
	hr = SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&BackBuffer11 );

	/*
	 * Error checking after creating back buffer
	 */
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),
			TEXT("SwapChain->GetBuffer"), MB_OK);	
		return 0;	
	}

	/*
	 * Create our render target view
	 */
	hr = d3d11Device->CreateRenderTargetView( BackBuffer11, NULL, &renderTargetView );

	/*
	 * Error checking after creating render target view
	 */
	if(FAILED(hr))
	{	
		MessageBox(NULL, DXGetErrorDescription(hr),
			TEXT("d3d11Device->CreateRenderTargetView"), MB_OK);	
		return 0;	
	}

	/*
	 * Describe our Depth/Stencil Buffer
	 */
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width     = Width;
	depthStencilDesc.Height    = Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	/*
	 * Create our Depth/Stencil View
	 */
	d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

	/*
	 * Bind Depth/Stencil View to the OM stage of the pipeline
	 */
	d3d11DevCon->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );

	return true;
}


bool InitD2D_D3D101_DWrite(IDXGIAdapter1 *Adapter)
{
	// Create our Direc3D 10.1 Device
	hr = D3D10CreateDevice1( Adapter, 
							 D3D10_DRIVER_TYPE_HARDWARE, 
							 NULL,
							 D3D10_CREATE_DEVICE_DEBUG | D3D10_CREATE_DEVICE_BGRA_SUPPORT, 
							 D3D10_FEATURE_LEVEL_9_3, 
							 D3D10_1_SDK_VERSION, 
							 &d3d101Device);	

	// Create Shared Texture that Direct3D 10.1 will render on
	D3D11_TEXTURE2D_DESC sharedTexDesc;	

	ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));

	sharedTexDesc.Width = Width;
	sharedTexDesc.Height = Height;	
	sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sharedTexDesc.MipLevels = 1;	
	sharedTexDesc.ArraySize = 1;
	sharedTexDesc.SampleDesc.Count = 1;
	sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
	sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;	
	sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;	

	hr = d3d11Device->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);	

	// Get the keyed mutex for the shared texture (for D3D11)
	hr = sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex11);	

	// Get the shared handle needed to open the shared texture in D3D10.1
	IDXGIResource *sharedResource10;
	HANDLE sharedHandle10;	

	hr = sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10);

	hr = sharedResource10->GetSharedHandle(&sharedHandle10);	

	sharedResource10->Release();

	// Open the surface for the shared texture in D3D10.1
	IDXGISurface1 *sharedSurface10;	

	hr = d3d101Device->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)(&sharedSurface10));

	hr = sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex10);	

	// Create D2D factory
	ID2D1Factory *D2DFactory;	
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);	

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;

	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));

	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);	

	hr = D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &D2DRenderTarget);

	sharedSurface10->Release();
	D2DFactory->Release();	

	// Create a solid color brush to draw something with		
	hr = D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f, 1.0f), &Brush);

	// DirectWrite
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&DWriteFactory));

	hr = DWriteFactory->CreateTextFormat( L"Script",
										  NULL,
										  DWRITE_FONT_WEIGHT_REGULAR,
										  DWRITE_FONT_STYLE_NORMAL,
										  DWRITE_FONT_STRETCH_NORMAL,
										  24.0f,
										  L"en-us",
										  &TextFormat);

	hr = TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	hr = TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	d3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);	
	return true;
}

//***************************************************************************************
// Direct Input
//***************************************************************************************
bool InitDirectInput(HINSTANCE hInstance)
{
	hr = DirectInput8Create(hInstance,
							DIRECTINPUT_VERSION,
							IID_IDirectInput8,
							(void**)&DirectInput,
							NULL); 

	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
								   &DIKeyboard,
								   NULL);

	hr = DirectInput->CreateDevice(GUID_SysMouse,
								   &DIMouse,
								   NULL);

	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;
}

void UpdateCamera()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix );
	camTarget = XMVector3Normalize(camTarget);

	/*
	// First-Person Camera
	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);*/
	
	// Free-Look Camera
	camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camUp = XMVector3Cross(camForward, camRight);

	camPosition += moveLeftRight*camRight;
	camPosition += moveBackForward*camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;	

	camView = XMMatrixLookAtLH( camPosition, camTarget, camUp );
}

void DetectInput(double time)
{
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(keyboardState),(LPVOID)&keyboardState);

	if(keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hwnd, WM_DESTROY, 0, 0);

	float speed = 15.0f * time;

	if(keyboardState[DIK_A] & 0x80)
	{
		moveLeftRight -= speed;
	}
	if(keyboardState[DIK_D] & 0x80)
	{
		moveLeftRight += speed;
	}
	if(keyboardState[DIK_W] & 0x80)
	{
		moveBackForward += speed;
	}
	if(keyboardState[DIK_S] & 0x80)
	{
		moveBackForward -= speed;
	}
	if((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		camYaw += mouseLastState.lX * 0.001f;

		camPitch += mouseCurrState.lY * 0.001f;

		mouseLastState = mouseCurrState;
	}

	UpdateCamera();

	return;
}

/*
 * Release Interfaces when we are done with them
 */
void CleanUp()
{
	SwapChain->SetFullscreenState(false, NULL);
	PostMessage(hwnd, WM_DESTROY, 0, 0);

	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
	renderTargetView->Release();
	squareVertBuffer->Release();
	squareIndexBuffer->Release();
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	vertLayout->Release();
	depthStencilView->Release();
	depthStencilBuffer->Release();
	cbPerObjectBuffer->Release();
	// Wireframe cleanup
	WireFrame->Release();
	// Transparency cleanup
	Transparency->Release();
	CCWcullMode->Release();
	CWcullMode->Release();
	// Font Interface Objects clean up
	d3d101Device->Release();
	keyedMutex11->Release();
	keyedMutex10->Release();
	D2DRenderTarget->Release();	
	Brush->Release();
	BackBuffer11->Release();
	sharedTex11->Release();
	DWriteFactory->Release();
	TextFormat->Release();
	d2dTexture->Release();
	// Light perFrameBuffer clean up
	cbPerFrameBuffer->Release();
	// Direct Input clean up
	DIKeyboard->Unacquire();
	DIMouse->Unacquire();
	DirectInput->Release();
	// Skybox interfaces clean up
	sphereIndexBuffer->Release();
	sphereVertBuffer->Release();

	SKYMAP_VS->Release();
	SKYMAP_PS->Release();
	TESSELLATE_PS->Release();
	TESS_DS->Release();
	TESS_HS->Release();
	SKYMAP_VS_Buffer->Release();
	SKYMAP_PS_Buffer->Release();
	TESSELLATE_PS_Buffer->Release();
	TESS_HS_Buffer->Release();
	TESS_DS_Buffer->Release();

	smrv->Release();

	DSLessEqual->Release();
	RSCullNone->Release();
}


//***************************************************************************************
// CREATE A SPHERE
//***************************************************************************************
void CreateSphere(int LatLines, int LongLines)
{
	NumSphereVertices = ((LatLines-2) * LongLines) + 2;
	NumSphereFaces  = ((LatLines-3)*(LongLines)*2) + (LongLines*2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for(DWORD i = 0; i < LatLines-2; ++i)
	{
		spherePitch = (i+1) * (3.14/(LatLines-1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for(DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28/(LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal( XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy) );	
			currVertPos = XMVector3Normalize( currVertPos );
			vertices[i*LongLines+j+1].pos.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines+j+1].pos.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines+j+1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices-1].pos.x =  0.0f;
	vertices[NumSphereVertices-1].pos.y =  0.0f;
	vertices[NumSphereVertices-1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData; 

	ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
	vertexBufferData.pSysMem = &vertices[0];
	hr = d3d11Device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for(DWORD l = 0; l < LongLines-1; ++l)
	{
		indices[k] = 0;
		indices[k+1] = l+1;
		indices[k+2] = l+2;
		k += 3;
	}

	indices[k] = 0;
	indices[k+1] = LongLines;
	indices[k+2] = 1;
	k += 3;

	for(DWORD i = 0; i < LatLines-3; ++i)
	{
		for(DWORD j = 0; j < LongLines-1; ++j)
		{
			indices[k]   = i*LongLines+j+1;
			indices[k+1] = i*LongLines+j+2;
			indices[k+2] = (i+1)*LongLines+j+1;

			indices[k+3] = (i+1)*LongLines+j+1;
			indices[k+4] = i*LongLines+j+2;
			indices[k+5] = (i+1)*LongLines+j+2;

			k += 6; // next quad
		}

		indices[k]   = (i*LongLines)+LongLines;
		indices[k+1] = (i*LongLines)+1;
		indices[k+2] = ((i+1)*LongLines)+LongLines;

		indices[k+3] = ((i+1)*LongLines)+LongLines;
		indices[k+4] = (i*LongLines)+1;
		indices[k+5] = ((i+1)*LongLines)+1;

		k += 6;
	}

	for(DWORD l = 0; l < LongLines-1; ++l)
	{
		indices[k] = NumSphereVertices-1;
		indices[k+1] = (NumSphereVertices-1)-(l+1);
		indices[k+2] = (NumSphereVertices-1)-(l+2);
		k += 3;
	}

	indices[k] = NumSphereVertices-1;
	indices[k+1] = (NumSphereVertices-1)-LongLines;
	indices[k+2] = NumSphereVertices-2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

}

void InitD2DScreenTexture()
{
	//Create the vertex buffer
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),
	};

	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &d2dIndexBuffer);


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData; 

	ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
	vertexBufferData.pSysMem = v;
	hr = d3d11Device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &d2dVertBuffer);

	/*
	 * Create a shader resource view from the texture D2D wil render to
	 * So we can use it to texture a square which overlays our scene
	 */
	d3d11Device->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);

}

/*
 * Init Scene
 * Place things that will change throughout the course of the game, 
 * but will not change throughout the scene
 */
bool InitScene()
{

	InitD2DScreenTexture();

	/*
	 * Create Skybox sphere
	 */
	CreateSphere(5, 5);

	/*
	 * Start Initializing scene by creating the shaders - compiled from Effects.fx
	 */
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "VS", "vs_5_0", 0, 0, 0, &VS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_5_0", 0, 0, 0, &PS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "D2D_PS", "ps_5_0", 0, 0, 0, &D2D_PS_Buffer, 0, 0);

	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_VS", "vs_5_0", 0, 0, 0, &SKYMAP_VS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_PS", "ps_5_0", 0, 0, 0, &SKYMAP_PS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "TESSELLATE_PS", "ps_5_0", 0, 0, 0, &TESSELLATE_PS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "TESS_DS", "ds_5_0", 0, 0, 0, &TESS_DS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "TESS_HS", "hs_5_0", 0, 0, 0, &TESS_HS_Buffer, 0, 0);

	/*
	 * Create the shader objects
	 */
	hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	hr = d3d11Device->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);

	hr = d3d11Device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	hr = d3d11Device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
	hr = d3d11Device->CreatePixelShader(TESSELLATE_PS_Buffer->GetBufferPointer(), TESSELLATE_PS_Buffer->GetBufferSize(), NULL, &TESSELLATE_PS);
	hr = d3d11Device->CreateDomainShader(TESS_DS_Buffer->GetBufferPointer(), TESS_DS_Buffer->GetBufferSize(), NULL, &TESS_DS);
	hr = d3d11Device->CreateHullShader(TESS_HS_Buffer->GetBufferPointer(), TESS_HS_Buffer->GetBufferSize(), NULL, &TESS_HS);

	/*
	 * Set the shaders as the current pipeline shaders
	 */
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);

	/*
	 * Defining Light direction, ambient and difuse values.
	 */
	light.dir = XMFLOAT3(0.25f, 0.5f, -1.0f);
	light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = XMFLOAT4(2.0f, 2.0f, 2.0f, 2.0f);

	/*
	 * Create vertex buffer - start by making an array of vertices
	 * using our vertex structure
	 */
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		Vertex( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
		Vertex( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		Vertex( 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
		Vertex( 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		// Right Face
		Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex( 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};

	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	/*
	 * Setting up the Index Buffer Byte Width
	 * sizeof(DWORD) * (Number of Triangles/Faces) * (Each Triangle/Face contains 3 vertices in the Index array)
	 */
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);

	d3d11DevCon->IASetIndexBuffer( squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	// Multiply by number of vertices 
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData; 

	ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
	vertexBufferData.pSysMem = v;
	hr = d3d11Device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &squareVertBuffer);

	/*
	 * Set the vertex buffer
	 * Bind vertex buffer to input assembler (IA)
	 */
	UINT stride = sizeof( Vertex );
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers( 0, 1, &squareVertBuffer, &stride, &offset );

	/*
	 * Create the Input (Vertex Layout)
	 */
	hr = d3d11Device->CreateInputLayout( layout, numElements, VS_Buffer->GetBufferPointer(), 
		VS_Buffer->GetBufferSize(), &vertLayout );

	/*
	 * Set the Input (Vertex Layout)
	 */
	d3d11DevCon->IASetInputLayout( vertLayout );

	/*
	 * Set the Primitive Topology
	 */
	d3d11DevCon->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	/*
	 * Create the Viewport
	 * Tells the RS stage of the pipeline what to draw
	 */
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	/*
	 * Set the ViewPort
	 */
	d3d11DevCon->RSSetViewports(1, &viewport);

	/*
	 *  Create a buffer which will hold the information we want to pass to the constant buffer in the effect file. 
	 * We first create the buffer description - change the bind flags member to D3D11_BIND_CONSTANT_BUFFER. 
	 * This says that the buffer will be bound to a constant buffer in the effect file. 
	 * After that, we create the buffer cbPerObjectBuffer using the description we just made.
	 */
	D3D11_BUFFER_DESC cbbd;	
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);


	//Create the buffer to send to the cbuffer per frame in effect file
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);



	/*
	 * Define our cameras position, Target, and up vectors.
	 * Move camera back using camPosition
	 */
	camPosition = XMVectorSet( 0.0f, 3.0f, -20.0f, 0.0f );
	camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	/*
	 * We can create the view space matrix using the XMMatrixLookAtLH() function from the xna library.
	 */
	camView = XMMatrixLookAtLH( camPosition, camTarget, camUp );

	/*
	 * Create the projection space matrix using the XMMatrixPerspectiveFovLH() xna function. 
	 * This usually does not have to be updated every frame, but sometimes you might want to update it for a certain effect.
	 */
	camProjection = XMMatrixPerspectiveFovLH( 0.4f*3.14f, (float)Width/Height, 1.0f, 1000.0f);

	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	// FillMode either D3D11_FILL_SOLID or D3D11_FILL_WIREFRAME  
	wfdesc.FillMode = D3D11_FILL_SOLID;
	wfdesc.CullMode = D3D11_CULL_NONE;
	hr = d3d11Device->CreateRasterizerState(&wfdesc, &WireFrame);

	/*
	 * We have created the render state, we need to set it, or bind it to the RS stage of the pipeline.
	 */
	d3d11DevCon->RSSetState(WireFrame);

	/*
	 *  We are loading our texture from a file using the function D3DX11CreateShaderResourceViewFromFile()
	 */
	hr = D3DX11CreateShaderResourceViewFromFile( d3d11Device, 
												 L"Texture.jpg",
												 NULL, 
												 NULL, 
												 &CubesTexture, 
												 NULL );

	/*
	 * Describe the sampler state, or how the shader will render the texture.
	 */
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory( &sampDesc, sizeof(sampDesc) );
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    
	/*
	 * We have described our sampler. Now we need to create it. 
	 * We can do this by using the CreateSamplerState() method
	 * The first parameter is our described sampler state.
	 * The second is an interface to put the sampler state into.
	 */

	hr = d3d11Device->CreateSamplerState( &sampDesc, &CubesTexSamplerState );


	/*
	 * We can create the blending State by calling the ID3D11Device::CreateBlendState(), 
	 * where the first parameter is a pointer to our blending description, 
	 * and the second is a pointer to an ID3D11BlendState interface.
	 */
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof(blendDesc) );

	/*
	 * Render Target Blend Descriptor.
	 */
	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory( &rtbd, sizeof(rtbd) );

	rtbd.BlendEnable			 = true;
	rtbd.SrcBlend				 = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend				 = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha			 = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha			 = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask	 = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	// Tell D3D we will be loading a cube texture
	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	// Load the texture
	ID3D11Texture2D* SMTexture = 0;
	hr = D3DX11CreateTextureFromFile(d3d11Device, L"skybox.dds", 
		&loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);

	//Create the textures description
	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	//Tell D3D We have a cube texture, which is an array of 2D textures
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	//Create the Resource view
	hr = d3d11Device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);



	d3d11Device->CreateBlendState(&blendDesc, &Transparency);

	/*
	 * We create the two rasterizer states, CW and CCW cull modes. 
	 * This is so we can draw our boxes twice each, so we can see the backside of the boxes 
	 * through the front side while they spin. 
	 * We fill out a rasterizer state description, which is a D3D11_RASTERIZER_DESC structure. 
	 * Then we can create the rasterizer state by calling the method ID3D11Device::CreateRasterizerState()
	 */
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
    
	// CHANGE FROM WIREFRAME TO SOLID OR VICE-VERSA
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
    
	cmdesc.FrontCounterClockwise = true;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CWcullMode);



	cmdesc.CullMode = D3D11_CULL_NONE;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	d3d11Device->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	return true;
}

void UpdateScene(double time)
{
	//Keep the cubes rotating
	rot += 1.0f * time;
	if(rot > 6.26f)
		rot = 0.0f;

	XMVECTOR rotyaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR rotzaxis = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR rotxaxis = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	//Reset cube1World
	cube1World = XMMatrixIdentity();

	Rotation = XMMatrixRotationAxis(rotyaxis, rot);
	Rotationx = XMMatrixRotationAxis(rotxaxis, rotx);
	Rotationz = XMMatrixRotationAxis(rotzaxis, rotz);
	// Move object 
	Translation = XMMatrixTranslation( 0.0f, 0.0f, 10.0f );

	//Set cube1's world space using the transformations
	cube1World = Translation * Rotation * Rotationx * Rotationz;



	//Reset cube2World
	cube2World = XMMatrixIdentity();

	//Define cube2's world space matrix
	Rotation = XMMatrixRotationAxis( rotyaxis, -rot);
	Scale = XMMatrixScaling( scaleX, scaleY, 1.3f );

	//Set cube2's world space matrix
	cube2World = (XMMatrixScaling(2.0f, 2.0f, 2.0f)) * Rotation * Scale;

	//Reset sphereWorld
	sphereWorld = XMMatrixIdentity();

	//Define sphereWorld's world space matrix
	Scale = XMMatrixScaling( 5.0f, 5.0f, 5.0f );

	//Make sure the sphere is always centered around camera
	Translation = XMMatrixTranslation( XMVectorGetX(camPosition), XMVectorGetY(camPosition), XMVectorGetZ(camPosition) );

	//Set sphereWorld's world space using the transformations
	sphereWorld = Scale * Translation;


}

void RenderText(std::wstring text, int inInt)
{
	// Release the D3D 11 Device
	keyedMutex11->ReleaseSync(0);

	// Use D3D10.1 device
	keyedMutex10->AcquireSync(0, 5);			

	// Draw D2D content		
	D2DRenderTarget->BeginDraw();	

	// Clear D2D Background
	D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

	// Create our string
	std::wostringstream printString; 
	printString << text << inInt;
	printText = printString.str();

	// Set the Font Color
	D2D1_COLOR_F FontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the brush color D2D will use to draw with
	Brush->SetColor(FontColor);	

	// Create the D2D Render Area
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, Width, Height);

	// Draw the Text
	D2DRenderTarget->DrawText( printText.c_str(),
							   wcslen(printText.c_str()),
							   TextFormat,
							   layoutRect,
							   Brush);

	D2DRenderTarget->EndDraw();	

	// Release the D3D10.1 Device
	keyedMutex10->ReleaseSync(1);

	// Use the D3D11 Device
	keyedMutex11->AcquireSync(1, 5);

	/*
	 * Use the shader resource representing the direct2d render target
	 * to texture a square which is rendered in screen space so it
	 * overlays on top of our entire scene. We use alpha blending so
	 * that the entire background of the D2D render target is "invisible",
	 * And only the stuff we draw with D2D will be visible (the text)
	 */

	// Set the blend state for D2D render target texture objects
	d3d11DevCon->OMSetBlendState(Transparency, NULL, 0xffffffff);

	// Set the d2d Index buffer
	d3d11DevCon->IASetIndexBuffer( d2dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the d2d vertex buffer
	UINT stride = sizeof( Vertex );
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers( 0, 1, &d2dVertBuffer, &stride, &offset );

	WVP =  XMMatrixIdentity();
	cbPerObj.World = XMMatrixTranspose(WVP);


	cbPerObj.WVP = XMMatrixTranspose(WVP);	
	d3d11DevCon->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );
	d3d11DevCon->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );
	d3d11DevCon->PSSetShaderResources( 0, 1, &d2dTexture );
	d3d11DevCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );

	d3d11DevCon->RSSetState(CWcullMode);
	//Draw the second cube
	d3d11DevCon->DrawIndexed( 6, 0, 0 );	
}

void DrawScene()
{
	// Clear our backbuffer
	float bgColor[4] = {(0.0f, 0.0f, 0.0f, 0.0f)};
	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);

	UINT stride = sizeof( Vertex );
	UINT offset = 0;

	// Set the default blend state (no blending) for opaque objects
	d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff);

	
	// Refresh the Depth/Stencil view
	d3d11DevCon->ClearDepthStencilView( depthStencilView, 
										D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 
										1.0f, 
										0);
	//Set our Render Target
	d3d11DevCon->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );

	/*
	 * SKYBOX
	 */
	d3d11DevCon->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	//Set the spheres index buffer
	d3d11DevCon->IASetIndexBuffer( sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//Set the spheres vertex buffer
	d3d11DevCon->IASetVertexBuffers( 0, 1, &sphereVertBuffer, &stride, &offset );

	//Set the WVP matrix and send it to the constant buffer in effect file
	WVP = sphereWorld * camView * camProjection;
	cbPerObj.WVP = XMMatrixTranspose(WVP);	
	cbPerObj.World = XMMatrixTranspose(sphereWorld);	
	d3d11DevCon->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );
	d3d11DevCon->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );
	//Send our skymap resource view to pixel shader
	d3d11DevCon->PSSetShaderResources( 0, 1, &smrv );
	d3d11DevCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );
	
	//Set the new VS and PS shaders
	d3d11DevCon->VSSetShader(SKYMAP_VS, 0, 0);
	d3d11DevCon->PSSetShader(SKYMAP_PS, 0, 0);
	//Set the new depth/stencil and RS states
	d3d11DevCon->OMSetDepthStencilState(DSLessEqual, 0);
	d3d11DevCon->RSSetState(RSCullNone);
	d3d11DevCon->DrawIndexed( NumSphereFaces * 3, 0, 0 );

	//Set the default VS shader and depth/stencil state
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->OMSetDepthStencilState(NULL, 0);

	constbuffPerFrame.light = light;
	d3d11DevCon->UpdateSubresource( cbPerFrameBuffer, 0, NULL, &constbuffPerFrame, 0, 0 );
	d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);	
    
    //Reset Vertex and Pixel Shaders
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);


	/*
	 * Blending Note: 
	 * -------------
	 * Remember, blending works by taking whats already on the render target, 
	 * and blending the current objects colors with whats already there. 
	 * So we need to make sure we render the opaque objects first, 
	 * so that the transparent objects can blend with the opaque objects.
	 */

#if 1

	// Fine-tune the blending equation
	float blendFactor[] = {0.75f, 0.75f, 0.75f, 1.0f};

	//Set the cubes index buffer
	d3d11DevCon->IASetIndexBuffer( squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//Set the cubes vertex buffer
	/*
	UINT stride = sizeof( Vertex );
	UINT offset = 0;*/
	d3d11DevCon->IASetVertexBuffers( 0, 1, &squareVertBuffer, &stride, &offset );

	// RENDER OPAQUE OBJECTS HERE
	
	d3d11DevCon->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST );
	//d3d11DevCon->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//Set the new VS and PS shaders
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->HSSetShader(TESS_HS, 0, 0);
	d3d11DevCon->DSSetShader(TESS_DS, 0, 0);
	d3d11DevCon->PSSetShader(TESSELLATE_PS, 0, 0);
	
	// Set the blend state for transparent objects
	d3d11DevCon->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

	/*
	 * Transparency Depth Ordering:
	 * Find the transparent object thats furthest from the camera
	 * Render the objects in the depth order to the render target
	 */
    
	// Find distance from first cube to camera
	XMVECTOR cubePos = XMVectorZero();
	cubePos = XMVector3TransformCoord(cubePos, cube1World);
	float distX = XMVectorGetX(cubePos) - XMVectorGetX(camPosition);
	float distY = XMVectorGetY(cubePos) - XMVectorGetY(camPosition);
	float distZ = XMVectorGetZ(cubePos) - XMVectorGetZ(camPosition);
	float cube1Dist = distX*distX + distY*distY + distZ*distZ;

	// Clean up CubePos
	cubePos = XMVectorZero();

	// Find distance from second cube to camera
	cubePos = XMVector3TransformCoord(cubePos, cube2World);
	distX = XMVectorGetX(cubePos) - XMVectorGetX(camPosition);
	distY = XMVectorGetY(cubePos) - XMVectorGetY(camPosition);
	distZ = XMVectorGetZ(cubePos) - XMVectorGetZ(camPosition);
	float cube2Dist = distX*distX + distY*distY + distZ*distZ;

	// If the first cubes distance is less than the second cubes
	if(cube1Dist < cube2Dist)
	{
		// Switch the order in which the cubes are drawn
		XMMATRIX tempMatrix = cube1World;
		cube1World = cube2World;
		cube2World = tempMatrix;	
	}

	/*
	 * Create the WVP matrix which will be sent to the vertex shader to reposition the objects vertices correctly. 
	 * Every object will have their own world space, so this should be done for each object in the scene. 
	 * We reset the World Matrix by using the XMMatrixIdentity() function, which returns a blank matrix. 
	 * Then we define the WVP - multiply the world, view, and projection matrices in that order.
	 */
	WVP = cube1World * camView * camProjection;

	cbPerObj.World = XMMatrixTranspose(cube1World);	

	/*
	 * When sending matrices to the effect file in direct3d 11, 
	 * we must send the matrices "transpose", where the rows and colums are switched. 
	 * We set our buffers WVP matrix to the transpose of our WVP matrix. 
	 * We then update our applications Constant Buffers Buffer with the cbPerObj structure containing our 
	 * updated WVP matrix using the UpdateSubresource() method of the ID3D11DeviceContext interface. 
	 * After that, we set the Vertex Shaders constant buffer to our applications constant buffers buffer 
	 * using the method ID3D11DeviceContext::VSSetConstantBuffers()
	 */
	cbPerObj.WVP = XMMatrixTranspose(WVP);	
	d3d11DevCon->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );
	d3d11DevCon->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );

	/*
	 * Sending the Sampler State and Texture to the Shader
	 */
	d3d11DevCon->PSSetShaderResources( 0, 1, &CubesTexture );
	d3d11DevCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );

	/*
	 * Counter clockwise culling first because we need the back side of
	 * the cube to be rendered first, so the front side can blend with it
	 */
	d3d11DevCon->RSSetState(CCWcullMode);

	d3d11DevCon->DrawIndexed( 36, 0, 0 );

	d3d11DevCon->RSSetState(CWcullMode);
	d3d11DevCon->DrawIndexed( 36, 0, 0 );

	WVP = cube2World * camView * camProjection;
	cbPerObj.World = XMMatrixTranspose(cube2World);
	cbPerObj.WVP = XMMatrixTranspose(WVP);	
	d3d11DevCon->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );
	d3d11DevCon->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );
	
	/*
	 * Sending the Sampler State and Texture to the Shader
	 */
	d3d11DevCon->PSSetShaderResources( 0, 1, &CubesTexture );
	d3d11DevCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );

	/*
	 * Counter clockwise culling first because we need the back side of
	 * the cube to be rendered first, so the front side can blend with it
	 */
	d3d11DevCon->RSSetState(CCWcullMode);

	/*
	 * The first parameter is the number of indices to draw. 
	 * The second is an offset from the start of the index buffer to start drawing. 
	 * We could specify a "4" here if we only wanted to draw the second triangle. 
	 * The third parameter is the offset from the start of the vertex buffer to start drawing. 
	 * You might have two index buffers, one describing a sphere, and one describing a box. 
	 * But maybe you have both in a single vertex buffer, where the sphere is the first set of vertices 
	 * in the vertex buffer, and the box is the second set. So to draw the box, you would need to 
	 * set this third parameter to the number of vertices in your sphere, 
	 * so that the box would be the first thing to start drawing.
	 */
	d3d11DevCon->DrawIndexed( 36, 0, 0 );

	d3d11DevCon->RSSetState(CWcullMode);
	d3d11DevCon->DrawIndexed( 36, 0, 0 );

#endif
	// Render this text
	
	//d3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);	
	RenderText(L"FPS: ", fps);


	//Present the backbuffer to the screen
	SwapChain->Present(0, 0);
}

int messageloop(){
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(true)
	{
		BOOL PeekMessageL( LPMSG lpMsg,
						   HWND hWnd,
						   UINT wMsgFilterMin,
						   UINT wMsgFilterMax,
						   UINT wRemoveMsg);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		}
		else{
			// MAIN GAME CODE HERE    

			/*
			 * Measure FPS
			 */
			frameCount++;
			if(GetTime() > 1.0f)
			{
				fps = frameCount;
				frameCount = 0;
				StartTimer();
			}	
			frameTime = GetFrameTime();

			/*
			 * Update Application Window with FPS
			 */
			std::wostringstream window_title; 
			window_title << "Rendering Engine " << "[FPS: " << fps << " ]";
			SetWindowText(hwnd, window_title.str().c_str());

			// Detect Direct Input
			DetectInput(frameTime);

			// Call UpdateScene with new FPS
			UpdateScene(frameTime);

			DrawScene();
		}
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,
						 UINT msg,
						 WPARAM wParam,
						 LPARAM lParam)
{
	switch( msg )
	{
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE ){
			DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}