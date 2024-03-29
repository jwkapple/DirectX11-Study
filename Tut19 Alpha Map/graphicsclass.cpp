
#include "graphicsclass.h"



GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_MultiShader = 0;
	m_Light = 0;
	
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//  Direct3D 객체를 생성합니다.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Direct3D 객체를 초기화합니다.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)L"Could not initialize Direct3D", (LPCSTR)L"Error", MB_OK);
		return false;
	}

	// Create the Camera Object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}
	const wchar_t* texturenames[3];

	texturenames[0] = L"../Tut19 Alpha Map/Data/stone01.dds";
	texturenames[1] = L"../Tut19 Alpha Map/Data/dirt01.dds";
	texturenames[2] = L"../Tut19 Alpha Map/Data/alpha01.dds";

	result = m_Model->Initialize(m_D3D->GetDevice(),m_D3D->GetDeviceContext(),(char*)"../Tut19 Alpha Map/Data/Cube.txt",texturenames );
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Could not initialize the model obejct", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_MultiShader = new MultiShaderClass;
	if (!m_MultiShader)
	{
		return false;
	}

	result = m_MultiShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, (LPCSTR)"Aould not initialize the shader obejct", (LPCSTR)"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	
	
	return true;
}

void GraphicsClass::Shutdown()
{
	// D3D 객체를 변환합니다.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	if (m_MultiShader)
	{
		m_MultiShader->Shutdown();
		delete m_MultiShader;
		m_MultiShader = 0;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	//그래픽 렌더링을 수행합니다.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{	// 씬 그리기를 시작하기 위해 버퍼의 내용을 지웁니다.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f,1.0f);
	
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();


	XMMATRIX worldMatrix = XMMatrixIdentity(), viewMatrix = XMMatrixIdentity(), projectionMatrix = XMMatrixIdentity();
	bool result;

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value
	worldMatrix = XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_MultiShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureArray()
	,m_Light->GetDirection(),m_Light->GetDiffuseColor());

	// 버퍼에 그려진 씬을 화면에 표시합니다.
	m_D3D->EndScene();

	return true;
}