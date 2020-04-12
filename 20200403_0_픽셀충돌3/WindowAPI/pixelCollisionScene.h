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
	POINT pt;		//_x, _y��ǥ
	image* _bg;		//��׶��� �̹���
	image* _floor;		//��׶��� �̹���
	image* _player;			//���̹���
	RECT _rc;				//����Ʈ

	camera* _cam;

	RECT cam;
	RECT charge;
	POINT fortstart;
	POINT fortend;
	float _x, _y;			//��ġ��ǥ
	float chargeSpd;
	float _playerAngle;
	float _probeY, _probeX;			//�ȼ��浹 Ž���� Y��
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

