#include <windowsx.h>
#include "MainApp.h"
#include "GameInstance.h"
#include "Timer.h"
#include "Raw_Input.h"

IMPLEMENT_SINGLETON(CMainApp)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd){
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//_CrtSetBreakAlloc(158);
	//_CrtSetBreakAlloc(159);

	/*CMainApp* mainApp = CMainApp::Get_Instance();
	if (FAILED(mainApp->Initialize(hInstance)))
		return FALSE;

	mainApp->Run();*/


	try
	{
		CMainApp* mainApp = CMainApp::Get_Instance();
		if (FAILED(mainApp->Initialize(hInstance)))
			return 0;

		return mainApp->Run();
	}
	catch (DxException& e)
	{
		//MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}

}


LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// CreateWindow�� �Ϸ�Ǳ� ������ mhMainWnd�� ��ȿ���� �ʽ��ϴ�.
	// ������ CreateWindow�� �Ϸ�Ǳ� ���� �޼��� (���� ��� WM_CREATE)��
	// ���� �� �ֱ� ������ hwnd�� �����մϴ�.
	return CMainApp::Get_Instance()->MsgProc(hwnd, msg, wParam, lParam);
}

CMainApp::CMainApp() : m_pGameInstance(CGameInstance::Get_Instance()), m_pTimer(CTimer::Get_Instance()), m_pInput_Dev(CRawInput_Device::Get_Instance())
{
}

int CMainApp::Run()
{
	MSG msg = { 0 };

	m_pTimer->Reset();

	while (msg.message != WM_QUIT)
	{
		// ó���ؾ��� ������ �޼������� �ִ��� Ȯ���մϴ�.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// ó���ؾ��� �޼����� ���� ���, ���ϸ��̼ǰ� ������ ó���մϴ�.
		else
		{
			m_pTimer->Tick();

			if (!m_AppPaused)
			{
				const float DT = m_pTimer->DeltaTime();
				CalculateFrameStats();
				Update(DT);
				Draw(DT);
			}
			else
			{
				Sleep(100);
			}
		}
	}

	RELEASE_INSTANCE(CMainApp);

	return (int)msg.wParam;
}

LRESULT CMainApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE�� �����찡 Ȱ��ȭ �ǰų� ��Ȱ��ȭ �ɶ� �������ϴ�.
		// �����찡 ��Ȱ��ȭ �Ǿ��� ���� ������ ������Ű��
		// �ٽ� Ȱ��ȭ �Ǿ��� ���� ������ �ٽ� ����մϴ�.
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_AppPaused = true;
			m_pTimer->Stop();
		}
		else
		{
			m_AppPaused = false;
			m_pTimer->Start();
		}
		return 0;

		// WM_SIZE�� ����ڰ� ������ ũ�⸦ ������ �� �������ϴ�.
	case WM_SIZE:
		// ���ο� ������ ũ�⸦ �����մϴ�.
		m_ClientWidth = LOWORD(lParam);
		m_ClientHeight = HIWORD(lParam);

		if (m_pGameInstance)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_Maximized = true;
				//OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				// �ּ�ȭ ���·κ��� �ٽ� �����Ǵ� �̴ϱ�?
				if (m_Minimized)
				{
					m_AppPaused = false;
					m_Minimized = false;
					//OnResize();
				}
				// �ִ�ȭ ���·κ��� �ٽ� �����Ǵ� �̴ϱ�?
				else if (m_Maximized)
				{
					m_AppPaused = false;
					m_Maximized = false;
					//OnResize();
				}
				else if (m_Resizing)
				{
					// ����ڰ� ũ�� ���� �ٸ� �巡���ϴ� ��츶�� �Ź� �������� ó���� ���ָ�
					// ������ ũ�� �϶��ϰ� �˴ϴ�. �׷��Ƿ� �Ź� �������� ó���� ���� �ʰ�
					// ����ڰ� ũ�� ������ ������ �� �߻��ϴ� WM_EXITSIZEMOVE���� �������� ó���� �մϴ�.
				}
				else // SetWindowPos �Ǵ� mSwapChain->SetFullscreenState�� ���ؼ� API�� ȣ�����ϴ�.
				{
					//OnResize();
				}
			}
		}
		return 0;

		// ����ڰ� ũ�� ������ �������� �� WM_EXITSIZEMOVE�� �������ϴ�.
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing = true;
		m_pTimer->Stop();
		return 0;

		// ����ڰ� ũ�� ������ �������� �� WM_EXITSIZEMOVE�� �������ϴ�.
		// ���⼭ ���ο� ������ ũ�⿡ ���� ó���� ��� �մϴ�.
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing = false;
		m_pTimer->Start();
		//OnResize();
		return 0;

		// �����찡 �ı��� �� WM_DESTORY�� �������ϴ�.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// WM_MENUCAR�� �޴��� Ȱ��ȭ �ɶ� �������ϴ�.
		// ����ڰ� �ϸ��, ���ӱ� Ű�� �ش����� �ʴ� Ű�� �����ϴ�.
	case WM_MENUCHAR:
		// ��Ʈ + ���͸� �Է½� �������� �߻��ϴ°��� �����մϴ�.
		return MAKELRESULT(0, MNC_CLOSE);

		// �����찡 �ʹ� �۾����°��� �����մϴ�.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

		/*case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;

		case WM_MOUSEMOVE:
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;*/

	case WM_INPUT:
		m_pInput_Dev->Update_InputDev(lParam);
		break;

	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		/*else if ((int)wParam == VK_F2)
		{
			Set4xMsaaState(!m4xMsaaState); // �� ���� ��Ƽ�ٸ���� �񽺹����� �� Ű�� ��.
		}*/
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

HRESULT CMainApp::Initialize_MainWindow(HINSTANCE g_hInstance)
{
	m_hAppInst = g_hInstance;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Ŭ���̾�Ʈ�� ũ�⸦ ������� ������ �簢���� ����մϴ�.
	RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	m_hMainWnd = CreateWindow(L"MainWnd", m_MainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, 0);
	if (!m_hMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);

	return true;
}

HRESULT CMainApp::Initialize(HINSTANCE g_hInstance)
{
	Initialize_MainWindow(g_hInstance);

	m_pInput_Dev->Initialize(m_hMainWnd);

	if (FAILED(m_pGameInstance->Initialize_Engine(m_hMainWnd)))
		return E_FAIL;

	m_pGameInstance->OnResize();

	return S_OK;
}

void CMainApp::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	++frameCnt;

	// 1�� ������ ����� ����մϴ�.
	if ((m_pTimer->TotalTime() - timeElapsed >= 1.0f))
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1;
		float mspf = 1000.0f / fps;

		wstring fpsStr = to_wstring(fps);
		wstring mpsfStr = to_wstring(mspf);

		wstring windowText = m_MainWndCaption +
			L"    fps: " + fpsStr +
			L"   mfps: " + mpsfStr;

		SetWindowText(m_hMainWnd, windowText.c_str());

		// ���� ����� ���� �ʱ�ȭ �մϴ�.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void CMainApp::Update(const float& DT)
{
}

void CMainApp::Draw(const float& DT)
{
	m_pGameInstance->Draw();
}

void CMainApp::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pTimer);
	Safe_Release(m_pInput_Dev);
}
