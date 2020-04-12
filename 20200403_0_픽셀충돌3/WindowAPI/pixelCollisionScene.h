#pragma once
#include "gameNode.h"
#include "camera.h"
#define BULLET_MAX 10

struct fortBullet {

	float x, y;
	float startx, starty;
	RECT rc;
	float speed;
	float angle;
	float gravity;
	bool fire;
	int camCount;
};
class pixelCollisionScene : public gameNode
{
private:
	POINT pt;		//_x, _y좌표
	image* _bg;		//백그라운드 이미지
	image* _floor;		//백그라운드 이미지
	image* _player;			//공이미지
	RECT _rc;				//공렉트

	camera* _cam;

	RECT cam;
	RECT charge;
	POINT fortstart;
	POINT fortend;
	float _x, _y;			//위치좌표
	float chargeSpd;
	float _playerAngle;
	float _probeY, _probeX;			//픽셀충돌 탐지할 Y값
	int fortlength;
	fortBullet bullet[BULLET_MAX];

	int _count;
	float temp;
	bool isFall;
	bool bulletCam;
	bool isLeft;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void fire();
	void bulletMove();

	pixelCollisionScene() {}
	~pixelCollisionScene() {}
};

