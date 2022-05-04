#pragma once
#include "Engine.h"
#include <math.h>
#include "hgMath.h"
#include "PathPlanner.h"
#include <fstream>
void Engine::setCarnum(int car_num) {
	this->car_num = car_num;
}
void Engine::setWaypointRand(int waypoint_num) {
	cout << "Way Points" << endl<<endl;
	//3개: (280, 475)  (150.096, 250)  (409.904, 250)
	//	4개 : (280, 475)  (130, 325)  (280, 175)  (430, 325)
	//	5개 : (280, 475)  (137.342, 371.353)  (191.832, 203.647)  (368.168, 203.647)  (422.658, 371.353)
	//	6개 : (280, 475)  (150.096, 400)  (150.096, 250)  (280, 175)  (409.904, 250)  (409.904, 400)
	// 오~각형
	//this->wayPoints.push_back(Pos(20,0));
	//this->wayPoints.push_back(Pos(40,0));
	//this->wayPoints.push_back(Pos(50,20 ));
	//this->wayPoints.push_back(Pos(30,40 ));
	//this->wayPoints.push_back(Pos(10,20));

	// 사~각형
	//this->wayPoints.push_back(Pos(-40, 0));
	//this->wayPoints.push_back(Pos(40, 0));
	//this->wayPoints.push_back(Pos(70, 80));
	//this->wayPoints.push_back(Pos(-10, 80));
	// 
	// 마룸몽
	//this->wayPoints.push_back(Pos(280, 503));
	//this->wayPoints.push_back(Pos(130, 353));
	//this->wayPoints.push_back(Pos(280, 203));
	//this->wayPoints.push_back(Pos(430, 353));
	// 
	// 정~사각형
	//this->wayPoints.push_back(Pos(0, 0));
	//this->wayPoints.push_back(Pos(20, 0));
	//this->wayPoints.push_back(Pos(20, 20));
	//this->wayPoints.push_back(Pos(0, 20));

	// 길쭊사각형
	//this->wayPoints.push_back(Pos(0, 0));
	//this->wayPoints.push_back(Pos(10, 0));
	//this->wayPoints.push_back(Pos(10, 100));
	//this->wayPoints.push_back(Pos(0, 100));

	// 별별
	//this->wayPoints.push_back(Pos(10, 0));
	//this->wayPoints.push_back(Pos(20, 20));
	//this->wayPoints.push_back(Pos(30, 0));
	//this->wayPoints.push_back(Pos(27, 20));
	//this->wayPoints.push_back(Pos(40, 40));
	//this->wayPoints.push_back(Pos(25, 40));
	//this->wayPoints.push_back(Pos(20, 60));
	//this->wayPoints.push_back(Pos(15, 40));
	//this->wayPoints.push_back(Pos(0, 40));
	//this->wayPoints.push_back(Pos(10, 0));

	//NOP 
	//this->wayPoints.push_back(Pos(280, 475));
	//this->wayPoints.push_back(Pos(130, 325));
	//this->wayPoints.push_back(Pos(280, 175));
	//this->wayPoints.push_back(Pos(430, 325));

	this->wayPoints.push_back(Pos(280, 475));
	this->wayPoints.push_back(Pos(150, 400));
	this->wayPoints.push_back(Pos(150, 250));
	this->wayPoints.push_back(Pos(280, 175));
	this->wayPoints.push_back(Pos(410, 250));
	this->wayPoints.push_back(Pos(700, 400));
}
std::vector<std::vector<Pos>> Engine::getPath2() {

	//std::vector<int> hi = hgMath::makeConvex(this->wayPoints);
	//cout << "RRR" << endl;
	//
	//std::vector<std::vector<Pos>> rere;
	//return rere;
	Pos center = hgMath::getCenterPoint(this->wayPoints);

	cout << "Center = " << center.x << " " << center.y << endl;

	PathPlanner::printPathpoints(this->wayPoints);

	//hgMath::QuickSort(this->wayPoints, 1, this->wayPoints.size() - 1, this->wayPoints[0]);
	cout << "HERE!!" << endl;
	PathPlanner::printPathpoints(this->wayPoints);

	std::vector<Pos> inter_points = PathPlanner::getInnerPoint_polygon(wayPoints);
	cout << inter_points[0].x << " " << inter_points[0].y << endl;
	cout << inter_points[0].x << " " << inter_points[0].y << endl;
	cout << "N = " << inter_points.size() << endl;

	std::vector<Pos> pca = hgMath::PCA(inter_points);
	if (pca[0].x == 0) pca[0].x += 0.000001;
	if (pca[1].y == 0) pca[1].y += 0.000001;
	double ratio = pca[0].x / (pca[1].y);
	if (ratio < 1) ratio = 1 / (ratio);

	cout << "RATIO " << ratio << endl;

	std::vector<std::vector<Pos>> route;
	//return route;
	// 공분산 큰 경우
	if (ratio > 0) {
		cout << "SIBVAL!!!!!!!!!!" << endl;
		//KMean_clustering km(this->car_num, wayPoints.size());
		//this->wayPoints = km.clustering(wayPoints);
		KMean_clustering km(this->car_num, inter_points.size());
		this->wayPoints = km.clustering(inter_points);
		//cout << "      KMeans Result " << endl << endl;
		std::vector<std::vector<Pos>> bf_convex_route;
		bf_convex_route = PathPlanner::divide_waypoints(wayPoints);

		//PathPlanner::printWaypoints(bf_convex_route);
		//cv::Mat My_Mat1(500, 500, CV_8SC1, cv::Scalar::all(0));

		int temp[500][500];
		int temp_idx = 0;
		for (std::vector<std::vector<Pos>>::iterator it = bf_convex_route.begin(); it < bf_convex_route.end(); it++) {
			//hgMath::QuickSort(*it, 1, it->size()-1, (*it)[0]);
			std::vector<Pos> convex_route = hgMath::makeConvex(*it);
			route.push_back(convex_route);

			//for (std::vector<Pos>::iterator itt = it->begin(); itt < it->end(); itt++) {
			//	temp[int(itt->y)][int(itt->x)] = 0;
			//}
			//temp_idx++;
		}
		//cout << My_Mat1 << endl;

		//cv::Mat drawing = cv::Mat::zeros(500,500, CV_8UC1);
		// 소스 이미지로 부터 윤곽선을 찾고, contour 벡터 안에 Point정보를 저장한다.
		//for (int i = 0; i < contours.size(); i++)
		//{
		//	cout << contours[i] << endl;
		//	//cv::Scalar color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		//	//drawContours(drawing, contours, (int)i, color, 2, cv::LINE_8, hierarchy, 0);
		//}
		cout << "CARACACAR   " << this->car_num << endl;
		cout << "SIXZER   " << route.size() << endl;
		cout << "SIXZER   " << route.size() << endl;
		cout << "SIXZER   " << bf_convex_route.size() << endl;
		cout << "SIXZER   " << bf_convex_route.size() << endl;
		//std::vector<int> hi = hgMath::makeConvex(route[0]);

		//cout << "COUNT!! " << _countof(hi) << endl;
		//for (int i =0; i<)
	}
	// 일반적인 경우
	else {
		double area = hgMath::integral_sum(this->wayPoints);
		double area_per_unit = area / this->car_num;
		double area_th = 0.2;
		route = PathPlanner::divide_intergral_center(this->wayPoints, area_per_unit, this->car_num);
	}
	PathPlanner::printWaypoints(route);
	return route;
}

// Unused
std::vector<std::vector<VEdge>> Engine::waypoints2vector(std::vector<Pos> way) {
	std::vector<std::vector<VEdge>> edges_vector(this->car_num);
	std::vector<std::vector<Pos>> pos_vector(this->car_num);
	std::vector<Pos> area_center;	// Voronoi center

	//cout << "SIZE " << way.size() << endl;
	//cout << "SIZE " << this->car_num <<endl;
	
	for (std::vector<Pos>::iterator i = way.begin(); i != way.end(); i++) {
		//pos_vector[i->group].push_back(Pos(i->x, i->y));
		pos_vector[i->group].push_back(Pos(i->x, i->y));
	}
	for (int g = 0; g < this->car_num; g++) {
		int count = 0;
		area_center.push_back(Pos(0, 0));
		for (std::vector<Pos>::iterator i = this->wayPoints.begin(); i != this->wayPoints.end(); i++) {
			if (i->group == g) {
				count++;
				area_center[g].x += i->x;
				area_center[g].y += i->y;
			}
		}
		area_center[g].x /= count;
		area_center[g].y /= count;
	}
	//for (std::vector<Pos>::iterator k = area_center.begin(); k != area_center.end(); k++) {
	//	cout << "!! " << k->x << " " << k->y << endl;
	//}
	int group = 0;
	for (std::vector<Pos*>::iterator i = this->ver.begin(); i != this->ver.end(); i++)
	{
		for (std::vector<VEdge>::iterator j = this->edges.begin(); j != this->edges.end(); j++)
		{
			if (((j->Left_Site.x == (*i)->x) && (j->Left_Site.y == (*i)->y)) || ((j->Right_Site.x == (*i)->x) && (j->Right_Site.y == (*i)->y)))
			{
				float min_dist = 10000;
				Pos AB;
				Pos AC;
				Pos newPose;

				//for (std::vector<Pos>::iterator k = area_center.begin(); k != area_center.end(); k++) {
				Pos k = area_center[group];
				//cout << "k pose = " << k.x << "  " << k.y << endl;

				AB.x = j->VertexB.x - j->VertexA.x;
				AB.y = j->VertexB.y - j->VertexA.y;
				AC.x = j->VertexB.x - k.x;
				AC.y = j->VertexB.y - k.y;

				float inner = AB.x * AC.x + AB.y * AC.y;
				float inner_self = AB.x * AB.x + AB.y * AB.y;

				AB.x *= inner / inner_self;
				AB.y *= inner / inner_self;
				Pos tempPose;

				tempPose.x = j->VertexA.x + AB.x;
				tempPose.y = j->VertexA.y + AB.y;
				float dist = (tempPose.x - k.x) * (tempPose.x - k.x) + (tempPose.y - k.y) * (tempPose.y - k.y);
				if (dist < min_dist) {
					newPose = tempPose;
					min_dist = dist;
				}
				//cout << "Added pose = "<<newPose.x<<"  "<< newPose.y<< endl;
				pos_vector[group].push_back(newPose);
				//pos_vector[group].push_back(Pos((j->VertexA.x+ j->VertexB.x)/2, (j->VertexA.y+ j->VertexB.y)/2));
			}
		}
		//cout << "POSE vector  "<<group << endl;
		for (std::vector<Pos>::iterator i = pos_vector[group].begin(); i != pos_vector[group].end(); i++) {
			//cout << "!! " << i->x << " " << i->y << endl;
		}
		group++;
	}
	group = 0;
	for (std::vector<std::vector<Pos>>::iterator i = pos_vector.begin(); i != pos_vector.end(); i++) {
		cout << "------------ Start" << endl;
		for (std::vector<Pos>::iterator a = i->begin(); a != i->end(); a++) {
			cout << a->x << " " << a->y << endl;
		}

		cout << "------------ SOrt" << endl;
		//QuickSort(*i, 1, i->size()-1);
		for (std::vector<Pos>::iterator a = i->begin(); a != i->end(); a++) {
			cout << a->x << " " << a->y << endl;
		}
		cout << "------------" << endl;
		for (std::vector<Pos>::iterator j = i->begin(); j != i->end(); j++) {
			VEdge  edges;
			edges.VertexA = Pos(j->x, j->y);

			if (j + 1 == i->end()) {
				edges.VertexB = Pos((i->begin())->x, (i->begin())->y);
			}
			else {
				edges.VertexB = Pos((j+1)->x, (j+1)->y);
			}
			edges_vector[group].push_back(edges);
		}
		group++;
	}
	for (std::vector<std::vector<VEdge>>::iterator i = edges_vector.begin(); i != edges_vector.end(); i++)
	{
		cout << "GROUP " << endl;

		for (std::vector<VEdge>::iterator j = i->begin(); j != i->end(); j++)
		{
			cout << "vector" << endl;
			cout << "		vector " << j->VertexA.x << " " << j->VertexA.y << endl;
			cout << "		vector " << j->VertexB.x << " " << j->VertexB.y << endl;
			//cout << "		vector "<<j->VertexA << " " << " " << j->VertexB << endl;
		}
	}

	return edges_vector;
}
//Temp
void Engine::printWaypoints(string fileName) {
	for (int i = 0; i < wayPoints.size(); i++) {
		cout << "COORD = (" << wayPoints[i].x << " , " << wayPoints[i].y << ")  GROUP = " << wayPoints[i].group << endl;
	}
	std::ofstream writeFile;            //쓸 목적의 파일 선언
	writeFile.open("text\\words"+fileName+".txt");


	for (int i = 0; i < wayPoints.size(); i++) {
		string str = to_string(int(wayPoints[i].x))+"\n";
		writeFile.write(str.c_str(), str.size());
		str = to_string(int(wayPoints[i].y)) + "\n";
		writeFile.write(str.c_str(), str.size());
		str = to_string(int(wayPoints[i].group)) + "\n";
		writeFile.write(str.c_str(), str.size());
	}
}
// Unused
void Engine::getPath_voronoi() {
	KMean_clustering km(this->car_num, this->wayPoints.size());
	this->wayPoints = km.clustering(wayPoints);
	cout << endl << " ---------------------------------------------------------- " << endl;
	cout << "      KMeans Result " << endl << endl;
	//this->printWaypoints();

	//km.k = center of each waypoints group
	//ver  = center of each waypoints group (Pos type)
	Voronoi* vdg;
	for (int i = 0; i < km.K_COUNT; i++) {
		this->ver.push_back(new Pos(km.k[i].x, km.k[i].y));
		ver_val.push_back(Pos(km.k[i].x, km.k[i].y));
	}

	vdg = new Voronoi();
	double minY = 0;
	double maxY = 100;
	this->edges = vdg->ComputeVoronoiGraph(this->ver, minY, maxY);
	delete vdg;
	cout << endl << " --------------------------------------------------------- " << endl;
	cout << "      Voronoi Result " << endl << endl;
	for (std::vector<VEdge>::iterator j = this->edges.begin(); j != this->edges.end(); j++)
		cout << "(" << j->VertexA.x << ", " << j->VertexA.y << ")\t(" << j->VertexB.x << ", " << j->VertexB.y << ")\n";

	cout << " -------------------- " << endl;

	std::vector<std::vector<VEdge>> edges_vector;

	for (std::vector<Pos*>::iterator i = this->ver.begin(); i != this->ver.end(); i++)
	{
		std::vector<VEdge> edges;
		cout << "\t\tSite =  (" << (*i)->x << ", " << (*i)->y << ")\n";
		for (std::vector<VEdge>::iterator j = this->edges.begin(); j != this->edges.end(); j++)
		{
			if (((j->Left_Site.x == (*i)->x) && (j->Left_Site.y == (*i)->y)) || ((j->Right_Site.x == (*i)->x) && (j->Right_Site.y == (*i)->y)))
			{
				cout << "(" << j->VertexA.x << ", " << j->VertexA.y << ")\t(" << j->VertexB.x << ", " << j->VertexB.y << ")\n";
				edges.push_back(*j);
			}
		}
		edges_vector.push_back(edges);
		cout << "\n";
	}
	cout << endl << " --------------------------------------------------------- " << endl;
	cout << "      Convexhull  Result " << endl << endl;
	Pos center_pos;
	center_pos.x = 50;
	center_pos.y = 50;
	//getInnerPoints(ver_val, edges_vector, center_pos);
	//return 0;
}