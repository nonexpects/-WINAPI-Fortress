#include "stdafx.h"
#include "pixelCollisionScene.h"

HRESULT pixelCollisionScene::init()
{
	//백그라운드, 공 이미지 초기화
	_bg = IMAGEMANAGER->findImage("fortress_map");
	_floor = IMAGEMANAGER->findImage("fortress_f");
	_player = IMAGEMANAGER->findImage("ball");
	
	//플레이어 위치 초기화
	_x = ((float)100 /WINSIZEX) * _bg->getWidth()  ;
	_y = ((float)100 / WINSIZEY) * _bg->getHeight();
	_rc = RectMakeCenter(_x, _y, 120, 120);

	//카메라 위치 초기화
	//_cam = new camera;
	//_cam->init(_x, _y, "fortress_map");

	cam = RectMakeCenter(_x, _y, WINSIZEX, WINSIZEY);

	//Y축으로 탐지(공의 하단에서 검사함)
	_probeX = _x - _player->getHeight() / 2;
	_probeY = _y + _player->getHeight() / 2;

	//총알 초기화
	ZeroMemory(bullet, sizeof(fortBullet));
	//총알 초기화
	for (int i = 0; i < BULLET_MAX; i++)
	{
		bullet[i].fire = false;
		bullet[i].speed = 1.0f;
		bullet[i].gravity = 0.0f;
		bullet[i].camCount = 60;
	}
	fortlength = 100;

	chargeSpd = 0;

	charge = RectMake(_x, _y, chargeSpd, 10);

	//플레이어의 앵글값 초기화
	_playerAngle = PI_4;

	bulletCam = isLeft = false;

	return S_OK;
}

void pixelCollisionScene::release()
{
}

void pixelCollisionScene::update()
{
	//이동(및 예외처리)
	if (INPUT->GetKey(VK_LEFT) && _x - 30 > _bg->getX())
	{
		_x -= 5.f;
		isLeft = true;
		//확인용
		cout << "X : " << _x << endl;
		cout << "Y : " << _y << endl;
		cout << "fX : " << cam.left << endl;
		cout << "fY : " << cam.top << endl;
	}
	if (INPUT->GetKey(VK_RIGHT) && _x + 30 < _bg->getWidth())
	{
		_x += 5.f;
		isLeft = false;
		//확인용
		cout << "X : " << _x << endl;
		cout << "Y : " << _y << endl;
		cout << "fX : " <<  cam.left << endl;
		cout << "fY : " <<  cam.top << endl;
	}
	//Angle값 변경 
	if (INPUT->GetKey(VK_UP))
	{
		_playerAngle += 0.1f;
	}
	if (INPUT->GetKey(VK_DOWN))
	{
		_playerAngle -= 0.1f;
	}
	

	

	//총알의 위치 초기화
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (bullet[i].fire)
		{
			bullet[i].rc = RectMakeCenter(bullet[i].x - cam.left, bullet[i].y - cam.top, 20, 20);
		}

	}

	
	if (bulletCam)
	{
		for (int i = 0; i < BULLET_MAX; i++)
		{
			if (bullet[i].fire || bullet[i].camCount < 60)
			{
				cam = RectMakeCenter(bullet[i].x , bullet[i].y, WINSIZEX, WINSIZEY);
				if (bullet[i].camCount < 60)
				{
					bullet[i].camCount--;
					if (bullet[i].camCount == 0) bulletCam = false;
				}
			}
		}
	}
	else
	{
		cam = RectMakeCenter(_x, _y, WINSIZEX, WINSIZEY);
	}
	
	if (cam.left < 0)
	{
		cam.left = 0;
		cam.right = cam.left + WINSIZEX;
	}
	if (cam.right > _bg->getWidth())
	{
		cam.right = _bg->getWidth();
		cam.left = cam.right - WINSIZEX;
	}
	if (cam.top < 0)
	{
		cam.top = 0;
		cam.bottom = cam.top + WINSIZEY;
	}
	if (cam.bottom > _bg->getHeight())
	{
		cam.bottom = _bg->getHeight();
		cam.top = cam.bottom - WINSIZEY;
	}



	_rc = RectMakeCenter(_x - cam.left, _y - cam.top, 120, 120);


	//포트리스 포 부분
	fortstart.x = _x - cam.left;
	fortstart.y = _y - cam.top;
	fortend.x = fortstart.x + cosf(_playerAngle) * fortlength;
	fortend.y = fortstart.y + -sinf(_playerAngle) * fortlength;

	//픽셀 충돌시 충돌 확인할 픽셀
	_probeY = _y + _player->getHeight() / 2;

	/*이부분이 픽셀충돌의 핵심*/
	for (int i = _probeY - 60; i < _probeY + 60; ++i)
	{
		COLORREF color = GetPixel(_floor->getMemDC(), _x , i);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);
	
		if (!(r == 255 && g == 0 && b == 255))
		{
			_y = i - _player->getHeight() / 2;
			break;
		}
		else
		{
			_y++;
		}
	}
	
	
	//총알 발사
	if (INPUT->GetKey(VK_SPACE))
	{
		if (chargeSpd < 100)
		{
			chargeSpd++;
		}
	}
	charge = RectMake(_rc.left + 7, _rc.bottom + 5, chargeSpd, 10);

	if (INPUT->GetKeyUp(VK_SPACE))
	{
		this->fire();
	}

	bulletMove();

}

void pixelCollisionScene::render()
{
	//백그라운드 렌더
	_bg->render(getMemDC(), 0, 0, cam.left, cam.top, WINSIZEX, WINSIZEY);
	_floor->render(getMemDC(), 0, 0, cam.left, cam.top, WINSIZEX, WINSIZEY);

	//공 이미지 렌더
	if (isLeft)
	{
		_player->frameRender(getMemDC(), _rc.left, _rc.top, 1, 0);
	}
	else
	{
		_player->frameRender(getMemDC(), _rc.left, _rc.top, 0, 0);
	}
	

	//디버깅용
	if (INPUT->GetToggleKey('A'))
	{
		//충돌용 작은사각형
		RECT rc = RectMakeCenter(_x, _probeY, 10, 10);
		Rectangle(getMemDC(), _rc);
		Rectangle(getMemDC(), rc);
		FrameRect(getMemDC(), cam, RGB(255, 0, 0));
	}

	if (chargeSpd > 0)
	{
		if (chargeSpd < 50)
		{
			HBRUSH brush = CreateSolidBrush(RGB(0, 255, 0));
			FillRect(getMemDC(), &charge, brush);
		}
		else if (chargeSpd >= 50 && chargeSpd < 90)
		{
			HBRUSH brush = CreateSolidBrush(RGB(255, 255, 0));
			FillRect(getMemDC(), &charge, brush);
		}
		if (chargeSpd >= 90)
		{
			HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
			FillRect(getMemDC(), &charge, brush);
		}
	}

	//포선 보여주는거
	LineMake(getMemDC(), fortstart.x, fortstart.y, fortend.x, fortend.y);

	//총알 렌더링
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (bullet[i].fire)
		{
			IMAGEMANAGER->findImage("bullet")->render(getMemDC(), bullet[i].rc.left - 0.5f, bullet[i].rc.top - 0.5f);
			FrameRect(getMemDC(), bullet[i].rc, RGB(255, 0, 0));
		}
	}
	
}

void pixelCollisionScene::fire()
{
	//총알 발사
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (bullet[i].fire) continue;
		bullet[i].fire = true;
		bullet[i].angle = _playerAngle;
		bullet[i].x = _x;
		bullet[i].y = _y;
		bullet[i].speed = chargeSpd / 10;
		cout << "bX: " << bullet[i].x << endl;
		cout << "bY: " << bullet[i].y << endl;
		bullet[i].rc = RectMakeCenter(bullet[i].x, bullet[i].y, 20, 20);
		bulletCam = true;							 
		bullet[i].camCount = 60;
		chargeSpd = 0;
		break; 
	}
}

void pixelCollisionScene::bulletMove()
{
	//총알 이동
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (bullet[i].fire == false) continue;
		bullet[i].gravity += 0.1f;
		bullet[i].x += cosf(bullet[i].angle) * bullet[i].speed;
		bullet[i].y += -sinf(bullet[i].angle) * bullet[i].speed + bullet[i].gravity;
		
		//if (bullet[i].x > WINSIZEX)
		//{
		//	cam = RectMakeCenter(bullet[i].x, WINSIZEY/2, WINSIZEX, WINSIZEY);
		//}

		//bullet 픽셀 충돌 확인
		COLORREF color = GetPixel(_floor->getMemDC(), bullet[i].x , bullet[i].y);
		int r = GetRValue(color);
		int g = GetGValue(color);
		int b = GetBValue(color);

		if (!(r == 255 && g == 0 && b == 255))
		{
			HBRUSH brush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 255));
			HBRUSH hbrush = (HBRUSH)SelectObject(_floor->getMemDC(), brush);
			HPEN pen = CreatePen(BS_SOLID, 1, (RGB(255, 0, 255)));
			HPEN hpen = (HPEN)SelectObject(_floor->getMemDC(), pen);
			EllipseMakeCenter(_floor->getMemDC(), bullet[i].x, bullet[i].y, 100, 100);
			bullet[i].camCount--;
			bullet[i].fire = false;
			bullet[i].gravity = 0.0f;
			break;
		}
	}
}

