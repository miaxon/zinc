#include "FlashWnd.h"

HINSTANCE CFlashWnd::hInst = NULL;
//ScaleMode values:
const int		 CFlashWnd::SCALEMODE_SHOWALL  = 0;
const int		 CFlashWnd::SCALEMODE_NOBORDER = 1;
const int		 CFlashWnd::SCALEMODE_EXACTFIT = 2;
const int		 CFlashWnd::SCALEMODE_NOSCALE  = 3;
//AlignMode values:
const int		 CFlashWnd::ALIGN_C = 0;//default
const int		 CFlashWnd::ALIGN_L = 1;
const int		 CFlashWnd::ALIGN_R = 2;
const int		 CFlashWnd::ALIGN_T = 4;
const int		 CFlashWnd::ALIGN_B = 8;	
//SAlign values:
const LPOLESTR	 CFlashWnd::SALIGN_L  = L"L";			
const LPOLESTR   CFlashWnd::SALIGN_T  = L"T";			
const LPOLESTR   CFlashWnd::SALIGN_R  = L"R";			
const LPOLESTR   CFlashWnd::SALIGN_B  = L"B";			
const LPOLESTR   CFlashWnd::SALIGN_TL = L"TL";			
const LPOLESTR   CFlashWnd::SALIGN_TR = L"TR";	
const LPOLESTR   CFlashWnd::SALIGN_BL = L"BL";
const LPOLESTR   CFlashWnd::SALIGN_BR = L"BR";
//WMode values:
const LPOLESTR   CFlashWnd::WMODE_TRANSPARENT  = L"transparent";
const LPOLESTR   CFlashWnd::WMODE_OPAQUE	   = L"opaque";
const LPOLESTR   CFlashWnd::WMODE_WINDOW	   = L"window";

CFlashWnd::CFlashWnd(void)
{
	isTransparent	= FALSE;
	hWndHost		= NULL;
	if(!hInst)
		hInst	  = GetModuleHandle(NULL);
	flash		  = NULL;
	view		  = NULL;
	isInit		  = FALSE;
	isAttached	  = FALSE;
	isReady		  = FALSE;
	isMovieLoaded = FALSE;
	dwThreadId = GetCurrentThreadId();
}

CFlashWnd::~CFlashWnd(void)
{
	if(isInit)
	{
		flash->Release();
	}
}
int CFlashWnd::CenterWindow()
{
	RECT rc = {0,0};
	GetWindowRect(hWndHost,&rc);
	int _x = (GetSystemMetrics(SM_CXSCREEN) - rc.right + rc.left)/ 2;
	int _y = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom + rc.top)/ 2;
	return SetWindowPos(hWndHost,HWND_TOP, _x ,_y, 0, 0, SWP_NOSIZE);

}
int	CFlashWnd::SetWindowSize(int cx, int cy)
{
	return ::SetWindowPos(hWndHost,HWND_TOP,0,0,cx,cy,SWP_NOMOVE);
}
HWND CFlashWnd::GetHostWindow()
{
	return hWndHost;
}
BOOL CFlashWnd::SetWindowText(WCHAR* str)
{
	if(hWndHost == NULL)
		return FALSE;
	return ::SetWindowText(hWndHost,str);
}
BOOL CFlashWnd::MessageBox(WCHAR* text, WCHAR* title)
{
	return ::MessageBox(hWndHost,text,title,0);
}
BOOL CFlashWnd::Destroy()
{
	return ::DestroyWindow(hWndHost);
}
HRESULT CFlashWnd::AttachControl()
{
	if(isAttached)
		return S_FALSE;

	if(hWndHost == NULL)
		return S_FALSE;

	HMODULE hModule = ::LoadLibrary(L"atl");
	if(!hModule)
		return S_FALSE;

	LPAtlAxAttachControl pAtlAxAttachControl = (LPAtlAxAttachControl)::GetProcAddress(hModule,"AtlAxAttachControl");
	if(!pAtlAxAttachControl)
	{
		::FreeLibrary(hModule);
		return S_FALSE;
	}
	HRESULT hr = pAtlAxAttachControl(flash,hWndHost,NULL);
	if(hr != S_OK)
		return hr;

	isAttached = TRUE;
	return hr;
}
HWND CFlashWnd::CreateHostWindow(LPCWSTR lpWindowName,
										DWORD dwStyle,
										int nCmdShow,
										int x,
										int y,
										int nWidth,
										int nHeight,
										HWND hWndParent,
										HMENU hMenu,
										LPVOID lpParam
										)
{
	if(hWndHost != NULL)
		return hWndHost;
	
	ATOM reg = RegisterHostWindowClass();
	if( reg == 0)
		return NULL;

	HMODULE hModule = ::LoadLibrary(L"atl");
	if(!hModule)
		return NULL;

	LPAtlAxWinInit pAtlAxWinInit    = (LPAtlAxWinInit)::GetProcAddress(hModule,"AtlAxWinInit");
	if(!pAtlAxWinInit)
	{
		::FreeLibrary(hModule);
		return NULL;
	}
	BOOL  res = pAtlAxWinInit();
	if(res == FALSE)
		return NULL;

	hWndHost = ::CreateWindow(FLHOSTWINDOW_CLASS, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInst, lpParam);	
	if(hWndHost == NULL)
		return NULL;

	::SetWindowLongPtr(hWndHost,GWL_USERDATA,(int)this);
	::ShowWindow(hWndHost, nCmdShow);
	::UpdateWindow(hWndHost);
	return hWndHost;
}
HWND CFlashWnd::CreateHostWindowEx( LPCWSTR lpWindowName,
										DWORD dwStyle,
										DWORD dwExStyle,
										int nCmdShow,
										int X,
										int Y,
										int nWidth,
										int nHeight,
										HWND hWndParent,
										HMENU hMenu,
										LPVOID lpParam
										)
{
	if(hWndHost != NULL)
		return hWndHost;

	ATOM reg = RegisterHostWindowClass();
	if( reg == 0)
		return NULL;

	HMODULE hModule = ::LoadLibrary(L"atl");
	if(!hModule)
		return NULL;

	LPAtlAxWinInit pAtlAxWinInit    = (LPAtlAxWinInit)::GetProcAddress(hModule,"AtlAxWinInit");
	if(!pAtlAxWinInit)
	{
		::FreeLibrary(hModule);
		return NULL;
	}

	BOOL  res = pAtlAxWinInit();
	if(res == FALSE)
		return NULL;

	hWndHost = ::CreateWindowEx(dwExStyle,FLHOSTWINDOW_CLASS, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInst, lpParam);
	if(hWndHost == NULL)
		return NULL;

	::SetWindowLongPtr(hWndHost,GWL_USERDATA,(int)this);
	isTransparent = TRUE;
	::ShowWindow(hWndHost, nCmdShow);
	::UpdateWindow(hWndHost);
	return hWndHost;
}

ATOM CFlashWnd::RegisterHostWindowClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= HostWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= ::LoadIcon(hInst, MAKEINTRESOURCE(IDC_MYICON));
	wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TEST);
	wcex.lpszClassName	= FLHOSTWINDOW_CLASS;
	wcex.hIconSm		= ::LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return ::RegisterClassEx(&wcex);
}

LRESULT CALLBACK CFlashWnd::HostWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	CFlashWnd* f=(CFlashWnd*)GetWindowLongPtr(hWnd,GWL_USERDATA);
	
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			::DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, DlgProc);
			break;
		case IDM_EXIT:
			::DestroyWindow(hWnd);
			break;
		default:
			return ::DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = ::BeginPaint(hWnd, &ps);
		// TODO: добавьте любой код отрисовки...
		::EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


INT_PTR CALLBACK CFlashWnd::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			::EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
///************************** Flash Control Method's**************************///

IUnknown* CFlashWnd::GetControl()
{
	return flash;
}

HRESULT CFlashWnd::SetWMODE(WCHAR* str)
{
	if(!isInit)
		return S_FALSE;
	return flash->put_WMode(str);
}
HRESULT CFlashWnd::SetSALIGN(WCHAR* str)
{
	if(!isInit)
		return S_FALSE;
	return flash->put_SAlign(str);
}
HRESULT CFlashWnd::SetALIGNMODE(int mode)
{
	if(!isInit)
		return S_FALSE;
	return flash->put_AlignMode(mode);
}
HRESULT CFlashWnd::SetSCALEMODE(int mode)
{
	if(!isInit)
		return S_FALSE;
	return flash->put_ScaleMode(mode);
}

HRESULT CFlashWnd::CreateInstance()
{
	if(isInit)
		return S_FALSE;

	HRESULT hr = CoCreateInstance(CLSID_ShockwaveFlash, 0, CLSCTX_ALL, __uuidof(IShockwaveFlash), (void **)&flash); 
	isInit = hr == S_OK;
	return hr;
}
HRESULT CFlashWnd::CreateInstance(WCHAR* szFileOCX)
{
	if(isInit)
		return S_FALSE;

	HMODULE hModule = LoadLibrary(szFileOCX);
	if(!hModule)
		return S_FALSE;

	LPGetClassObject fLPGetClassObject = (LPGetClassObject)GetProcAddress(hModule, "DllGetClassObject");
	if(!fLPGetClassObject )
		return S_FALSE;
	
	IClassFactory* clf = NULL;
	fLPGetClassObject(CLSID_ShockwaveFlash, IID_IClassFactory, (LPVOID*)&clf);
	if(!clf )
		return S_FALSE;

	HRESULT hr = clf->CreateInstance(NULL, __uuidof(IShockwaveFlash),(void**)&flash);
	clf->Release();
	isInit = hr == S_OK;
	return hr;
}
HRESULT CFlashWnd::CreateInstance(UINT id, WCHAR* resType)
{
	if(hInst == NULL)
		return S_FALSE;
	if(isInit)
		return S_FALSE;

	IClassFactory* clf = NULL;
	HRSRC hResInfo = ::FindResource(hInst, MAKEINTRESOURCE(id), resType);
	if(!hResInfo)
		return S_FALSE;

	DWORD dwFlashOCXCodeSize = ::SizeofResource(hInst, hResInfo);
	if(dwFlashOCXCodeSize == 0)
		return S_FALSE;

	HGLOBAL data = ::LoadResource(hInst, hResInfo);
	if(!data)
		return S_FALSE;

	LPVOID lpFlashOCXCode = ::LockResource(data);
	if(!lpFlashOCXCode)
		return S_FALSE;

	HMEMORYMODULE hMem = MemoryLoadLibrary(lpFlashOCXCode);
	if(!hMem )
		return S_FALSE;

	LPGetClassObject fLPGetClassObject = (LPGetClassObject)MemoryGetProcAddress(hMem, "DllGetClassObject");
	if(!fLPGetClassObject )
		return S_FALSE;

	fLPGetClassObject(CLSID_ShockwaveFlash, IID_IClassFactory, (LPVOID*)&clf);
	if(!clf )
		return S_FALSE;

	HRESULT hr = clf->CreateInstance(NULL, __uuidof(IShockwaveFlash),(void**)&flash);
	clf->Release();
	isInit = hr == S_OK;
	return hr;

}
HRESULT CFlashWnd::GetInstallVersion(WCHAR* buf)
{
	CLSID clsid = GUID_NULL;
	HRESULT hr = CLSIDFromString(FLPROG_ID,&clsid);
	if(hr != S_OK)
		return hr;
	if(buf == NULL)
		return S_FALSE;

	GUID guid;
	hr = IIDFromString(FLTYPELIB_ID, &guid);
	if(hr != S_OK)
		return hr;

	BSTR szVersionFile; 
	hr = QueryPathOfRegTypeLib(guid, 1, 0, 0x0409, &szVersionFile);
	if(hr == S_OK)
	{
		DWORD  verHandle = NULL;
		UINT   size      = 0;
		LPBYTE lpBuffer  = NULL;
		DWORD  verSize   = GetFileVersionInfoSize( szVersionFile, &verHandle);
		if (verSize != NULL)
		{
			LPSTR verData = new char[verSize];
			if (GetFileVersionInfo( szVersionFile, verHandle, verSize, verData))
			{
				if (VerQueryValue(verData,L"\\",(VOID FAR* FAR*)&lpBuffer,&size))
				{
					if (size)
					{
						VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
						if (verInfo->dwSignature == 0xfeef04bd)
						{
							int major = HIWORD(verInfo->dwFileVersionMS);
							int minor = LOWORD(verInfo->dwFileVersionMS);
							int build_major = HIWORD(verInfo->dwFileVersionLS);
							int build_minor = LOWORD(verInfo->dwFileVersionLS);
							wsprintf(buf,L"%d.%d.%d.%d",major,minor,build_major,build_minor);
						}
					}
				}
			}
		}
	}
    SysFreeString(szVersionFile); 
	return hr;
}

HRESULT CFlashWnd::LoadMovie(WCHAR* szFileName)
{
	if(isMovieLoaded == TRUE)
		return S_FALSE;

	if(flash == NULL)
		return S_FALSE;
	
	HRESULT hr = flash->put_Movie(szFileName);
	if(hr != S_OK)
		return hr;

	isMovieLoaded = TRUE;
	return hr;
}
HRESULT CFlashWnd::LoadMovie(UINT id, WCHAR* resType)
{
	if(hInst == NULL)
		return S_FALSE;

	if(isMovieLoaded == TRUE)
		return S_FALSE;

	HRSRC hResInfo = ::FindResource(hInst, MAKEINTRESOURCE(id), resType);
	if(!hResInfo)
		return S_FALSE;

	HRSRC hResource = ::FindResource(hInst, MAKEINTRESOURCE(id), resType);
	if(!hResource)
		return S_FALSE;

	HGLOBAL hResourceData = ::LoadResource(hInst, hResource);
	if(!hResourceData)
		return S_FALSE;

	LPVOID lpResourceData = ::LockResource(hResourceData);
	if(!lpResourceData)
		return S_FALSE;

	DWORD dwResourceSize = ::SizeofResource(hInst, hResource);
	if(dwResourceSize == 0)
		return S_FALSE;

	HGLOBAL hGlobalData = ::GlobalAlloc(GPTR, dwResourceSize+8);
	if(!hGlobalData)
		return S_FALSE;

	LPBYTE pMem = (LPBYTE)::GlobalLock(hGlobalData);
	if(!pMem)
		return S_FALSE;

	((long*)pMem)[0] = 0x55665566; 
	((long*)pMem)[1] = dwResourceSize;
	::CopyMemory((void*)(&(pMem[8])), lpResourceData, dwResourceSize);
	IStream* pStream = 0;
	HRESULT hr = ::CreateStreamOnHGlobal(hGlobalData, TRUE, &pStream);
	if(hr == S_OK)
	{
		IPersistStreamInit* pPersistStream = NULL;
		hr = flash->QueryInterface(IID_IPersistStreamInit, (void**)&pPersistStream);
		if(hr == S_OK)
			hr = pPersistStream->Load(pStream);
		pPersistStream->Release();
		pPersistStream = NULL;
		pStream->Release();
		pStream = NULL;
	}
	::GlobalUnlock((HGLOBAL)pMem);
	isMovieLoaded = TRUE;
	return hr;

}
//// Transparent drawing
HRESULT CFlashWnd::DrawView(SIZE windowSize, POINT windowPos, POINT layerPos)
{
	if(view == NULL)
		return S_FALSE;
	
	HDC hdcWindow=GetDC(hWndHost);
    HDC hdcMemory = CreateCompatibleDC(hdcWindow);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcWindow, windowSize.cx, windowSize.cy);
	HBITMAP hOldObject = (HBITMAP)SelectObject(hdcMemory, hBitmap);	

	HRESULT hr = view->Draw(DVASPECT_TRANSPARENT, 0, 0, 0, 0, hdcMemory, 0, 0, 0, 0);
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	UpdateLayeredWindow(hWndHost, hdcWindow, &windowPos, &windowSize, hdcMemory, &layerPos, 0, &bf, ULW_ALPHA);
	SetWindowPos(hWndHost, HWND_TOP, windowPos.x, windowPos.y, windowSize.cx, windowSize.cy, SWP_SHOWWINDOW | SWP_FRAMECHANGED);

	SelectObject(hdcMemory, hOldObject);
	DeleteObject(hBitmap);
	ReleaseDC(hWndHost, hdcMemory);
	DeleteDC(hdcMemory);
	ReleaseDC(hWndHost, hdcWindow);

	return hr;
}
HRESULT CFlashWnd::SetTransparentModeParam(SIZE viewSize, POINT viewPoint, UINT rate)
{
	if(!isInit)
		return S_FALSE;

	HRESULT hr =  flash->QueryInterface(__uuidof(IViewObjectEx), (void**)&view) ;
	if(hr != S_OK)
		return hr;

	drawingSize = viewSize;
	drawingPoint = viewPoint;
	SetTimer(hWndHost, FLDRAWTIMER_ID, rate, (TIMERPROC)DrawingTimerProc);
	return hr;
}
VOID CALLBACK CFlashWnd::DrawingTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if(idEvent == FLDRAWTIMER_ID)
	{
		
		POINT pl = {0,0};
		CFlashWnd* f=(CFlashWnd*)GetWindowLongPtr(hwnd,GWL_USERDATA);
		if(f != NULL && f->hWndHost == hwnd)
			f->DrawView(f->drawingSize,f->drawingPoint,pl);
	}
}
// ----- Flash Control Event's interface implementation ----- //
HRESULT CFlashWnd::AdviseSink()
{
	if(!isInit)
		return S_FALSE;

	IConnectionPointContainer*	lpConCont = NULL;
	HRESULT hr = flash->QueryInterface(IID_IConnectionPointContainer, (void**)&lpConCont);
	if (hr != S_OK)
		return hr;

	hr = lpConCont->FindConnectionPoint(DIID__IShockwaveFlashEvents, &lpConPoint);
	lpConCont->Release();
	lpConCont = NULL;
	if (hr != S_OK)
		return hr;

	hr = lpConPoint->Advise(this, &dwConPointID);
	return hr;
}

HRESULT STDMETHODCALLTYPE CFlashWnd::QueryInterface(REFIID iid, void** ppvObject)
{
	if (iid == IID_IUnknown)
	{
	  dwRefCount++;
	  *ppvObject = (void *)this;
	  return S_OK;
	}
	if (iid == DIID__IShockwaveFlashEvents)
	{
	  dwRefCount++;
	  *ppvObject = (void *)this;
	  return S_OK;
	}
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CFlashWnd::AddRef()
{
	dwRefCount++;
	return dwRefCount;
}

ULONG STDMETHODCALLTYPE CFlashWnd::Release()
{
	dwRefCount--;
	return dwRefCount;
}
HRESULT CFlashWnd::Invoke(
                            DISPID  dispIdMember,      
                            REFIID  riid,              
                            LCID  lcid,                
                            WORD  wFlags,              
                            DISPPARAMS FAR*  pDispParams,  
                            VARIANT FAR*  pVarResult,  
                            EXCEPINFO FAR*  pExcepInfo,  
                            unsigned int FAR*  puArgErr )
{
    if (!pDispParams)
        return E_POINTER;

    if (pDispParams->cNamedArgs != 0)
        return DISP_E_NONAMEDARGS;

    HRESULT hr = DISP_E_MEMBERNOTFOUND;
    switch (dispIdMember)
    {
	case 0xC5:// ExternalInterface call
		return FlashCall(pDispParams->rgvarg[0].bstrVal);
		break;
	case 0x96:// fscommand call
		return FSCommand(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal );
		break;
	case 0x7A6: // OnProgress ???                 
		return OnProgress(pDispParams->rgvarg[0].intVal);
		break;
	case 0xFFFFFD9F: // OnReadyStateChange ???
		if(pDispParams->rgvarg[0].lVal == 4)
			isReady = TRUE;
		return OnReadyStateChange(pDispParams->rgvarg[0].lVal);
		break;
    }

    return  hr;
}
HRESULT CFlashWnd::SetReturnValue(WCHAR* xml_responce)
{
	if(isInit)
		return flash->SetReturnValue(xml_responce);
	
	return S_FALSE;
}