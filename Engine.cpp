#pragma once
#include "Engine.h"
#include <math.h>
#include "hgMath.h"
#include "PathPlanner.h"
#include <fstream>
#include <time.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
void Engine::setCarnum(int car_num) {
	this->car_num = car_num;
}
void Engine::setWaypointRand(int waypoint_num) {
	std::cout << "Way Points" << std::endl<<std::endl;
	//3��: (280, 475)  (150.096, 250)  (409.904, 250)
	//	4�� : (280, 475)  (130, 325)  (280, 175)  (430, 325)
	//	5�� : (280, 475)  (137.342, 371.353)  (191.832, 203.647)  (368.168, 203.647)  (422.658, 371.353)
	//	6�� : (280, 475)  (150.096, 400)  (150.096, 250)  (280, 175)  (409.904, 250)  (409.904, 400)
	// ��~����
	//this->wayPoints.push_back(Pos(200,0));
	//this->wayPoints.push_back(Pos(400,0));
	//this->wayPoints.push_back(Pos(500,200 ));
	//this->wayPoints.push_back(Pos(300,400 ));
	//this->wayPoints.push_back(Pos(100,200));

	// ��~����
	//this->wayPoints.push_back(Pos(120, 50));
	//this->wayPoints.push_back(Pos(280, 50));
	//this->wayPoints.push_back(Pos(340, 210));
	//this->wayPoints.push_back(Pos(280, 190));
	//this->wayPoints.push_back(Pos(180, 210));
	// 
	// �����
	//this->wayPoints.push_back(Pos(280, 503));
	//this->wayPoints.push_back(Pos(130, 353));
	//this->wayPoints.push_back(Pos(280, 203));
	//this->wayPoints.push_back(Pos(430, 353));
	// 
	// ��~�簢��
	//this->wayPoints.push_back(Pos(0, 0));
	//this->wayPoints.push_back(Pos(20, 0));
	//this->wayPoints.push_back(Pos(20, 20));
	//this->wayPoints.push_back(Pos(0, 20));

	// �槄�簢��
	//this->wayPoints.push_back(Pos(10, 10));
	//this->wayPoints.push_back(Pos(110, 10));
	//this->wayPoints.push_back(Pos(110, 600));
	//this->wayPoints.push_back(Pos(10, 600));

	// ����
	//this->wayPoints.push_back(Pos(100, 0));
	//this->wayPoints.push_back(Pos(200, 200));
	//this->wayPoints.push_back(Pos(300, 0));
	//this->wayPoints.push_back(Pos(270, 200));
	//this->wayPoints.push_back(Pos(400, 400));
	//this->wayPoints.push_back(Pos(250, 400));
	//this->wayPoints.push_back(Pos(200, 600));
	//this->wayPoints.push_back(Pos(150, 400));
	//this->wayPoints.push_back(Pos(0, 400));
	//this->wayPoints.push_back(Pos(130, 200));
	//NOP 
	this->wayPoints.push_back(Pos(280, 475));
	this->wayPoints.push_back(Pos(130, 325));
	this->wayPoints.push_back(Pos(280, 175));
	this->wayPoints.push_back(Pos(430, 325));
	//this->wayPoints.push_back(Pos(280, 475));
	//this->wayPoints.push_back(Pos(150, 400));
	//this->wayPoints.push_back(Pos(150, 250));
	//this->wayPoints.push_back(Pos(280, 175));
	//this->wayPoints.push_back(Pos(410, 250));
	//this->wayPoints.push_back(Pos(700, 400));
}
std::vector<std::vector<Pos>> Engine::getPath2() {
	double threshold = 5.0;
	Pos center = hgMath::getCenterPoint(this->wayPoints);

	std::cout << "Center = " << center.x << " " << center.y << std::endl;

	std::vector<Pos> inter_points = PathPlanner::getInnerPoint_polygon(wayPoints);
	std::cout << inter_points[0].x << " " << inter_points[0].y << std::endl;
	std::cout << inter_points[0].x << " " << inter_points[0].y << std::endl;
	std::cout << "N = " << inter_points.size() << std::endl;

	std::vector<Pos> pca = hgMath::PCA(inter_points);
	if (pca[0].x == 0) pca[0].x += 0.000001;
	if (pca[1].y == 0) pca[1].y += 0.000001;
	double ratio = pca[0].x / (pca[1].y);
	if (ratio < 1) ratio = 1 / (ratio);
	std::cout << "RATIO " << ratio << std::endl;

	std::vector<std::vector<Pos>> route;
	std::vector<std::vector<Pos>> route_integral;
	if (ratio < threshold) {
		std::cout << "Kmeans mode" << std::endl;
		// clock_t start, end;
		// start = clock(); 
		KMean_clustering km(this->car_num, inter_points.size());
		// end = clock();
		// cout << "Time  = " << (double)(end - start) << endl;
		std::vector<std::vector<Pos>> bf_convex_route;
		bf_convex_route = PathPlanner::divide_waypoints(km.clustering(inter_points));
		for (std::vector<std::vector<Pos>>::iterator it = bf_convex_route.begin(); it < bf_convex_route.end(); it++) {
			cout << "SIZE ======= "<<(*it).size() << endl;
			std::vector<Pos> convex_route = PathPlanner::save_clustering_img(*it);
			route.push_back(convex_route);
		}
		//std::cout << "CAR NUM   		" << this->car_num << std::endl;
		//std::cout << "Route length    " << route.size() << << "   " << bf_convex_route.size() << std::endl;
	}
	else{
		std::cout << "Integral" << std::endl;
		clock_t start, end;
		start = clock();
		double area = hgMath::integral_sum(this->wayPoints);
		double area_per_unit = area / this->car_num;
		double area_th = 0.2;
		route_integral = PathPlanner::divide_intergral_center(this->wayPoints, area_per_unit, this->car_num);
		end = clock();
		cout << "TIME = " << (double)(end - start) << endl;

	}
	//cout << "Len = " << route.size()<< endl;
	//cout << "Len = " << route_integral.size()<< endl;
	if (ratio < threshold0) return route = PathPlanner::samplingAllRoute(route);
	else return route_integral = PathPlanner::samplingAllRoute(route_integral);

	//std::vector<std::vector<float>> cur1 = PathPlanner::getCurvatureFromRoute(route);
	//cout << "-----------------" << endl;
	//std::vector<std::vector<float>> cur2 = PathPlanner::getCurvatureFromRoute(route_integral);
	//PathPlanner::printWaypoints(route, cur1);
	//
	//float max_cur1, max_cur2;
	//max_cur1 = max_cur2 = 0;
	//for (int i = 0; i < cur1.size(); i++) {
	//	for (int j = 0; j < cur1[i].size(); j++) if (cur1[i][j] > max_cur1) max_cur1 = cur1[i][j];
	//	for (int j = 0; j < cur2[i].size(); j++) if (cur2[i][j] > max_cur2) max_cur2 = cur2[i][j];
	//}
	//std::cout << "CUR1 " << max_cur1 << " CUR2  " << max_cur2 << std::endl;
	//for (int i = 0; i < route.size(); i++) {
	//std::cout << "CS " << cur1[i].size() << " RS  " << route[i].size() << std::endl;
	//}
	//return route_integral;
}