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
	// CreateWindow가 완료되기 전에는 mhMainWnd는 유효하지 않습니다.
	// 하지만 CreateWindow가 완료되기 전에 메세지 (예를 들어 WM_CREATE)를
	// 받을 수 있기 때문에 hwnd를 전달합니다.
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
		// 처리해야할 윈도우 메세지들이 있는지 확인합니다.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 처리해야할 메세지가 없는 경우, 에니메이션과 게임을 처리합니다.
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
		// WM_ACTIVATE는 윈도우가 활성화 되거나 비활성화 될때 보내집니다.
		// 윈도우가 비활성화 되었을 때는 게임을 중지시키고
		// 다시 활성화 되었을 때는 게임을 다시 재게합니다.
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

		// WM_SIZE는 사용자가 윈도우 크기를 변경할 때 보내집니다.
	case WM_SIZE:
		// 새로운 윈도우 크기를 저장합니다.
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
				// 최소화 상태로부터 다시 복원되는 겁니까?
				if (m_Minimized)
				{
					m_AppPaused = false;
					m_Minimized = false;
					//OnResize();
				}
				// 최대화 상태로부터 다시 복원되는 겁니까?
				else if (m_Maximized)
				{
					m_AppPaused = false;
					m_Maximized = false;
					//OnResize();
				}
				else if (m_Resizing)
				{
					// 사용자가 크기 조정 바를 드래그하는 경우마다 매번 리사이즈 처리를 해주면
					// 성능이 크게 하락하게 됩니다. 그러므로 매번 리사이즈 처리를 하지 않고
					// 사용자가 크기 조정을 끝냈을 때 발생하는 WM_EXITSIZEMOVE에서 리사이즈 처리를 합니다.
				}
				else // SetWindowPos 또는 mSwapChain->SetFullscreenState에 의해서 API가 호출됬습니다.
				{
					//OnResize();
				}
			}
		}
		return 0;

		// 사용자가 크기 조정을 시작했을 때 WM_EXITSIZEMOVE가 보내집니다.
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing = true;
		m_pTimer->Stop();
		return 0;

		// 사용자가 크기 조정을 끝마쳤을 때 WM_EXITSIZEMOVE가 보내집니다.
		// 여기서 새로운 윈도우 크기에 대한 처리를 모두 합니다.
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing = false;
		m_pTimer->Start();
		//OnResize();
		return 0;

		// 윈도우가 파괴될 때 WM_DESTORY가 보내집니다.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// WM_MENUCAR는 메뉴가 활성화 될때 보내집니다.
		// 사용자가 니모닉, 가속기 키에 해당하지 않는 키를 누릅니다.
	case WM_MENUCHAR:
		// 알트 + 엔터를 입력시 비프음이 발생하는것을 방지합니다.
		return MAKELRESULT(0, MNC_CLOSE);

		// 윈도우가 너무 작아지는것을 방지합니다.
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
			Set4xMsaaState(!m4xMsaaState); // 그 뭐야 안티앨리어싱 비스무리한 거 키는 거.
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

	// 클라이언트의 크기를 기반으로 윈도우 사각형을 계산합니다.
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

	// 1초 동안의 평균을 계산합니다.
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

		// 다음 평균을 위해 초기화 합니다.
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
