#pragma once
#include "windows.h"
#include "Oleauto.h"
#include "resource.h"
#import  "Flash10o.ocx" named_guids no_namespace raw_interfaces_only no_auto_exclude rename("IServiceProvider","IServiceProvider2")
#include "winver.h"
#include "MemoryModule.h"
#pragma comment(lib,"Version.lib")
//definitions
#define FLHOSTWINDOW_CLASS	L"AtlAxWin"
#define FLPROG_ID			L"ShockwaveFlash.ShockwaveFlash"
#define FLCLASS_ID			L"{D27CDB6E-AE6D-11cf-96B8-444553540000}"
#define FLTYPELIB_ID		L"{D27CDB6B-AE6D-11cf-96B8-444553540000}"
#define FLWINDOW_CLASS		L"MacromediaFlashPlayerActiveX"
#define FLDRAWTIMER_ID		1000

//typedefs
typedef BOOL	(	WINAPI	*LPAtlAxWinInit		 )();
typedef HRESULT (	WINAPI	*LPAtlAxAttachControl)(IUnknown* pControl, HWND hWnd, IUnknown** ppUnkContainer);
typedef VOID	(	WINAPI	*LPGetClassObject	 )(REFCLSID rclsid, REFIID riid, LPVOID* ppv);

class CFlashWnd:
	public  IDispatch

{
	protected:
	HWND						hWndHost;
	BOOL						isAttached;
	BOOL						isInit;
	BOOL						isMovieLoaded;
	BOOL						isTransparent;
	SIZE						drawingSize;
	POINT						drawingPoint;
	DWORD						dwConPointID;
	IConnectionPoint*			lpConPoint;
	DWORD						dwRefCount;
	BOOL						isReady;
	static HINSTANCE hInst;
private:
	IShockwaveFlash*			flash;
	IViewObjectEx*				view;
	static ATOM					RegisterHostWindowClass ();
	static LRESULT CALLBACK		HostWndProc				(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK		DlgProc					(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	static VOID CALLBACK		DrawingTimerProc		(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	HRESULT						DrawView				(SIZE windowSize, POINT windowPos, POINT layerPos);
public:
	CFlashWnd(void);
	~CFlashWnd(void);
	// main window control interface
	DWORD dwThreadId;
	HWND hwndParent;
	HWND		CreateHostWindow (
								   LPCWSTR lpWindowName = NULL,
								   DWORD dwStyle = WS_OVERLAPPEDWINDOW,
								   int nCmdShow = SW_SHOWNORMAL,
								   int x = CW_USEDEFAULT,
								   int y = 0,
								   int nWidth = CW_USEDEFAULT,
								   int nHeight = 0,
								   HWND hWndParent = NULL,
								   HMENU hMenu = NULL,
								   LPVOID lpParam = NULL
								   );
	HWND		CreateHostWindowEx(							
									LPCWSTR lpWindowName = NULL,
									DWORD dwStyle =  WS_POPUP | WS_SYSMENU,
									DWORD dwExStyle = WS_EX_LAYERED,
									int nCmdShow = SW_SHOWNORMAL,
									int X = CW_USEDEFAULT,
									int Y = 0,
									int nWidth =  CW_USEDEFAULT,
									int nHeight = 0,
									HWND hWndParent = NULL,
									HMENU hMenu = NULL,
									LPVOID lpParam = NULL
									);
	int			SetWindowSize(int cx, int cy);
	int			CenterWindow();
	HWND		GetHostWindow		();
	BOOL		SetWindowText		(WCHAR* str);
	BOOL		MessageBox			(WCHAR* text, WCHAR* title);
	BOOL		Destroy				();
	IUnknown*	GetControl			();
	HRESULT		AttachControl		();

	//static CONSTANTS
							//ScaleMode values:
	static const int		SCALEMODE_SHOWALL ;	//0
	static const int		SCALEMODE_NOBORDER;	//1
	static const int		SCALEMODE_EXACTFIT;	//2
	static const int		SCALEMODE_NOSCALE ;	//3
							//AlignMode values:
	static const int		ALIGN_C;			//0
	static const int		ALIGN_L;			//1
	static const int		ALIGN_R;			//2
	static const int		ALIGN_T;			//4
	static const int		ALIGN_B;			//8
							//SAlign values:
	static const LPOLESTR	SALIGN_L ;			//"L"
	static const LPOLESTR	SALIGN_T ;			//"T"
	static const LPOLESTR	SALIGN_R ;			//"R"
	static const LPOLESTR	SALIGN_B ;			//"B"
	static const LPOLESTR	SALIGN_TL;			//"TL"
	static const LPOLESTR	SALIGN_TR;			//"TL"
	static const LPOLESTR	SALIGN_BL;			//"BL"
	static const LPOLESTR	SALIGN_BR;			//"BR"
							//WMode values:
	static const LPOLESTR	WMODE_TRANSPARENT;	//"transparent"
	static const LPOLESTR	WMODE_OPAQUE;		//"opaque"
	static const LPOLESTR	WMODE_WINDOW;		//"window"
	// transparent window drawing functions
	HRESULT SetTransparentModeParam(SIZE viewSize, POINT viewPoint, UINT rate = 100);
	// Flash Control Init
	static HRESULT GetInstallVersion(WCHAR* ver);
	HRESULT CreateInstance();
	HRESULT CreateInstance(WCHAR* szFileOCX);
	HRESULT CreateInstance(UINT id, WCHAR* resType);


	// common 
	HRESULT LoadMovie(WCHAR* szFileName);
	HRESULT LoadMovie(UINT id, WCHAR* resType); 
	HRESULT SetWMODE(WCHAR* str);
	HRESULT SetSALIGN(WCHAR* str);
	HRESULT SetALIGNMODE(int mode);
	HRESULT SetSCALEMODE(int mode);
	
	// starting events listening
	HRESULT SetReturnValue(WCHAR* xml_responce);
	HRESULT AdviseSink();
	// IDispatch methods
	STDMETHOD(QueryInterface)( REFIID riid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();
    STDMETHOD(GetIDsOfNames)( REFIID riid, 
                              OLECHAR FAR *FAR *rgszNames,
                              unsigned int cNames, 
                              LCID lcid, 
                              DISPID FAR *rgDispId )
    { return( E_NOTIMPL ); }

    STDMETHOD(GetTypeInfo)( unsigned int iTInfo, 
                            LCID lcid, 
                            ITypeInfo FAR *FAR *ppTInfo )
    { return( E_NOTIMPL ); }

    STDMETHOD(GetTypeInfoCount)( unsigned int FAR *pctinfo )
    {
        return( E_NOTIMPL );
    }

    STDMETHOD(Invoke)( DISPID  dispIdMember,      
                       REFIID  riid,              
                       LCID  lcid,                
                       WORD  wFlags,              
                       DISPPARAMS FAR*  pDispParams,  
                       VARIANT FAR*  pVarResult,  
                       EXCEPINFO FAR*  pExcepInfo,  
                       unsigned int FAR*  puArgErr );

	// Override this methods if needed
	virtual HRESULT OnReadyStateChange	(long newState )				{return S_OK;}
    virtual HRESULT OnProgress			(long percentDone )				{return S_OK;}
    virtual HRESULT FSCommand			(_bstr_t command,_bstr_t args )	{return S_OK;}
	virtual HRESULT FlashCall			(_bstr_t xml_request )			{return S_OK;}

};
