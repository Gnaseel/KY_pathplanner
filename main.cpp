#include<iostream>
#include"Engine.h"

using namespace std;

int main() {
	Engine engine;

	//���Ƿ� 10�� waypoint ���� (���� = xy 0~100)
	engine.setCarnum(3);
	engine.setWaypointRand(10);
	engine.getPath();
}
