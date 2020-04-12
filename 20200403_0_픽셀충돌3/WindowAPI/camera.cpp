#include "stdafx.h"
#include "camera.h"

HRESULT camera::init(float x, float y, const char* bgName)
{

	_img = IMAGEMANAGER->findImage(bgName);

	pX = x;
	pY = y;

	//카메라 렉트 위치 초기화
	_rcCam = RectMakeCenter(pX, pY, WINSIZEX, WINSIZEY);

	return S_OK;
}

void camera::release()
{

}

void camera::update(float x, float y)
{
	//pX = (float)_img->getWidth() * x;
	//pY = (float)_img->getHeight() * y;
	//pX = x;
	//pY = y;

	if (pX <= WINSIZEX/2)
	{
		pX = WINSIZEX/2;
	}
	else if (pX >= _img->getWidth() - WINSIZEX/2)
	{
		pX = _img->getWidth() - WINSIZEX/2;
	}

	if (pY <= WINSIZEY/2)
	{
		pY = WINSIZEY/2;
	}
	else if (pY >= _img->getHeight() - WINSIZEY/2)
	{
		pY = _img->getHeight() - WINSIZEY/2;
	}


	_rcCam = RectMakeCenter(pX, pY, WINSIZEX, WINSIZEY);

}

void camera::render()
{
	FrameRect(getMemDC(), _rcCam, RGB(255, 255, 0));
}
