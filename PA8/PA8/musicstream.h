#pragma once

#include <atlbase.h>
#include <wmp.h>
#include "util.h"

class MusicStream
{
public:
	MusicStream(PWCHAR streamURL)
	{
		if(FAILED(CoInitialize(0)) ||
		   FAILED(WMPCore.CoCreateInstance(__uuidof(WindowsMediaPlayer), 0, CLSCTX_INPROC_SERVER)) ||
		   FAILED(WMPCore->put_URL(streamURL)))
		{
			throw E_AUDIO_INIT_FAILURE;
		}
	}

	~MusicStream()
	{
		CoUninitialize();
	}

private:
	// atl smart pointer to wmp root com interface
	CComPtr<IWMPCore> WMPCore;
};