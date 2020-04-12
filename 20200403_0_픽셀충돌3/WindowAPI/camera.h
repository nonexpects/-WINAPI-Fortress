#pragma once
#include "gameNode.h"

#define CAMSIZEX 600
#define CAMSIZEY 400

class camera : public gameNode
{
private:
	float pX, pY;
	POINT p;
	image* _img;		//백그라운드 이미지
	RECT _rcCam;		//카메라 렉트
	float speedControl;

public:
	HRESULT init(float x, float y, const char* bgName);
	void release();
	void update(float x, float y);
	void render();

	//카메라를 특정 위치좌표로 옮기기 + 이전좌표 저장
	void setCam(float xPos, float yPos);
	void loadprevCam() {};

	float getcX() { return pX; }
	float getcY() { return pY; }

	RECT getcam() { return _rcCam; }

	camera() {}
	~camera() {}
};

