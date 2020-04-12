#pragma once
#include "gameNode.h"

#define CAMSIZEX 600
#define CAMSIZEY 400

class camera : public gameNode
{
private:
	float pX, pY;
	POINT p;
	image* _img;		//��׶��� �̹���
	RECT _rcCam;		//ī�޶� ��Ʈ
	float speedControl;

public:
	HRESULT init(float x, float y, const char* bgName);
	void release();
	void update(float x, float y);
	void render();

	//ī�޶� Ư�� ��ġ��ǥ�� �ű�� + ������ǥ ����
	void setCam(float xPos, float yPos);
	void loadprevCam() {};

	float getcX() { return pX; }
	float getcY() { return pY; }

	RECT getcam() { return _rcCam; }

	camera() {}
	~camera() {}
};

