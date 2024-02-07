#include "MyFlash.h"

void MyFlash::init()
{
	memBlockSize = 1024*1024;
	WCHAR ver[16]={0};
	GetInstallVersion(ver);
	CreateHostWindow();
	SetWindowText(ver);
	SetWindowSize(550,600);
	CenterWindow();
	CreateInstance();
	AdviseSink();
	AttachControl();

	WCHAR buf[MAX_PATH]={0};
	::GetCurrentDirectory(MAX_PATH,buf);
    lstrcat(buf,L"\\..\\common\\movie.swf");
	LoadMovie(buf);

}

HRESULT MyFlash::FlashCall (_bstr_t xml_request)
{
	LPBYTE sharedMemBlock = FindSharedMemBlock();
	lstrcpyA((CHAR*)&sharedMemBlock[16]," HELLO, BUFFER!");
	// естественно, в бинарный буфер можно запистаь не только строку :)
	SetReturnValue(_bstr_t("<string>ok</string>"));
    return S_OK;
}

LPBYTE MyFlash::FindSharedMemBlock()
{
	HANDLE hProc = GetCurrentProcess();
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	LPBYTE lpBytes = NULL;
	MEMORY_BASIC_INFORMATION mi;
	BYTE buf[16] = {0x0};
	VirtualQueryEx(hProc,si.lpMinimumApplicationAddress, &mi,sizeof(mi));

	for( LPBYTE lpAddress = (LPBYTE)si.lpMinimumApplicationAddress;	lpAddress < si.lpMaximumApplicationAddress;	lpAddress += mi.RegionSize)
	{
		if(VirtualQueryEx(hProc, (LPCVOID)lpAddress, &mi,sizeof(mi)))
		{
			if((mi.Protect & PAGE_READWRITE) || (mi.Protect & PAGE_WRITECOPY))
			{
				for( LPBYTE b = lpAddress; b < lpAddress + mi.RegionSize; b += 16)
				{
					if(ReadProcessMemory(hProc,(LPCVOID)b,&buf,16,NULL))
					{
						int i = 0;
						while( i < 8)
						{
							if( buf[i] != 0x61 + i + i || buf[i+8] != 0x61 + i + i)
								break;
							i++;
						}
						if (i == 8)
						{
							lpBytes = b;
							VirtualProtect(lpAddress,memBlockSize,PAGE_EXECUTE_READWRITE,NULL);
							break;
						}
					}
				}
			}

		}
	}
	CloseHandle(hProc);
	//DWORD oldProtect;
	//BOOL res = VirtualProtect(mi.AllocationBase, mi.RegionSize, mi.AllocationProtect, &oldProtect);
	//outputD((int)lpBytes);
	//dumpMemory(0,64,(char*)lpBytes);
	//outputD(szMem);
	//outputD(amfVer);
	
	return lpBytes;
}
