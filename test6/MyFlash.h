#pragma once
#include "flashwnd.h"

class MyFlash :
	public CFlashWnd
{
private:
	void init();
	LPBYTE FindSharedMemBlock();
	UINT memBlockSize;
public:
	MyFlash (void){ init();};
	~MyFlash(void){};
	HRESULT FlashCall			(_bstr_t xml_request );
};
