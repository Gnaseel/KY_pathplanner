#include "hgMath.h"
#include<iostream>
long long hgMath::dist(const Pos* p1, const Pos* p2) {
	return (long long)(p1->x - p2->x) * (p1->x - p2->x) + (long long)(p1->y - p2->y) * (p1->y - p2->y);
}
int hgMath::ccw(const Pos* p1, const Pos* p2, const Pos* p3) {

	int cross_product = (p2->x - p1->x) * (p3->y - p1->y) - (p3->x - p1->x) * (p2->y - p1->y);

	if (cross_product > 0) {
		return 1;
	}
	else if (cross_product < 0) {
		return -1;
	}
	else {
		return 0;
	}
}
int hgMath::comparator(const Pos* left, const Pos* right, const Pos p) {
	int ret;
	int direction = ccw(&p, left, right);
	if (direction == 0) {
		ret = (dist(&p, left) < dist(&p, right));
	}
	else if (direction == 1) {
		ret = 1;
	}
	else {
		ret = 0;
	}
	return ret;
}
void hgMath::QuickSort(vector<Pos>& a, int lo, int hi, Pos init) {
	if (hi - lo <= 0) {
		return;
	}

	// ���� �迭 ������ �߾Ӱ��� �ǹ����� �����Ѵ�.
	// Select the median as pivot in the current array range.
	Pos pivot = a[lo + (hi - lo + 1) / 2];
	int i = lo, j = hi;

	// ���� ����
	// Conquer process
	while (i <= j) {
		// �ǹ��� ���ʿ��� comparator(Ÿ��, "�ǹ�")�� �������� �ʴ� �ε����� ���� (i)
		// On the left side of the pivot, select an index that doesn't satisfy the comparator(target, "pivot"). (i)
		while (comparator(&a[i], &pivot, init)) i++;

		// �ǹ��� �����ʿ��� comparator("�ǹ�", Ÿ��)�� �������� �ʴ� �ε����� ���� (j)
		// On the right side of the pivot, select an index that doesn't satisfy the comparator("pivot", target). (j)
		while (comparator(&pivot, &a[j], init)) j--;
		// (i > j) �ǹ��� ���ʿ��� ��� ���� �ǹ����� �۰� �ǹ��� �����ʿ��� ��� ���� �ǹ����� ū ���°� �Ǿ���.
		// (i > j) On the left side of the pivot, all values are smaller than the pivot, and on the right side of the pivot, all values are larger than the pivot.
		if (i > j) {
			break;
		}

		// i��° ���� �ǹ� ���� ũ�� j��° ���� �ǹ����� �����Ƿ� �� ���� �����Ѵ�.
		// The i-th value is larger than the pivot and the j-th value is smaller than the pivot, so swap the two values.
		Pos temp = a[i];
		a[i] = a[j];
		a[j] = temp;

		// �ε��� i�� 1���� ��Ű�� �ε��� j�� 1 ���� ���Ѽ� Ž�� ������ �������� ������.
		// Narrow the search inward by increasing index i by one and decreasing index j by one.
		i++;
		j--;
	}

	// ���� ����
	// Divide process
	QuickSort(a, lo, j, init);
	QuickSort(a, i, hi, init);
}
double hgMath::getDist(Pos A) {
	return sqrt(A.x * A.x + A.y * A.y);
}
double hgMath::inner(Pos A, Pos B) {
	return A.x * B.x + A.y * B.y;
}
double hgMath::integral(Pos pivot, Pos a, Pos b) {

	Pos A = a - pivot;
	Pos B = b - pivot;
	double distA = sqrt(A.x * A.x + A.y * A.y);
	double distB = sqrt(B.x * B.x + B.y * B.y);

	double inner = hgMath::inner(A, B);
	double cos = inner / abs(distA * distB);
	double sin = sqrt(1 - cos * cos);
	double area = sin * distA * distB * 0.5;

	return area;
}
double hgMath::integral_sum(vector<Pos> poses) {
	
	Pos center = hgMath::getCenterPoint(poses);
	poses.push_back(poses[0]);

	double area_total = 0;
	for (vector<Pos>::iterator i = poses.begin(); i < poses.end() - 1; i++) {
		double area = hgMath::integral(center, *i, *(i + 1));
		area_total += area;
	}
	//cout << "AREA_TOTAL " << area_total << endl;

	return area_total;
}
Pos hgMath::getCenterPoint(vector<Pos> poses) {
	Pos center;
	for (vector<Pos>::iterator i = poses.begin(); i < poses.end(); i++) {
		center.x += i->x;
		center.y += i->y;
	}
	center.x /= poses.size();
	center.y /= poses.size();
	return center;
}
bool hgMath::getIntersectPoint(const Pos& AP1, const Pos& AP2,
	const Pos& BP1, const Pos& BP2, Pos* IP)
{
	double t;
	double s;
	double under = (BP2.y - BP1.y) * (AP2.x - AP1.x) - (BP2.x - BP1.x) * (AP2.y - AP1.y);
	if (under == 0) return false;

	double _t = (BP2.x - BP1.x) * (AP1.y - BP1.y) - (BP2.y - BP1.y) * (AP1.x - BP1.x);
	double _s = (AP2.x - AP1.x) * (AP1.y - BP1.y) - (AP2.y - AP1.y) * (AP1.x - BP1.x);

	t = _t / under;
	s = _s / under;

	if (t <= 0.0 || t>=1.0 || s <= 0.0 || s>=1.0) return false;
	if (_t == 0 && _s == 0) return false;

	IP->x = AP1.x + t * (double)(AP2.x - AP1.x);
	IP->y = AP1.y + t * (double)(AP2.y - AP1.y);

	return true;
}
vector<Pos> hgMath::PCA(vector<Pos> points) {
	//points.clear();
	//points.push_back(Pos(170, 70));
	//points.push_back(Pos(150, 45));
	//points.push_back(Pos(160, 55));
	//points.push_back(Pos(180, 60));
	//points.push_back(Pos(172, 80));

	int n = points.size();

	vector<Pos> pca;
	double a, b, c, d, x_sum, y_sum;
	a = b = c = d = x_sum = y_sum = 0;
	for (int i = 0; i < n; i++) {
		x_sum += points[i].x;
		y_sum += points[i].y;
	}
	double x_mean = x_sum / n;
	double y_mean = y_sum / n;
	for (int i = 0; i < n; i++){
		a += (points[i].x-x_mean) * (points[i].x-x_mean);
		b += (points[i].x-x_mean) * (points[i].y-y_mean);
		c += (points[i].y-y_mean) * (points[i].x-x_mean);
		d += (points[i].y-y_mean) * (points[i].y-y_mean);
	}
	cout << x_mean << " " << y_mean << endl;
	cout << a << " " << b << " " << c << " " << d << endl;
	Pos pc1(a / n, b / n);
	Pos pc2(c / n, d / n);
	pca.push_back(pc1);
	pca.push_back(pc2);
	cout << " PCA " << pca[0].x << "  " << pca[0].y << "  " << pca[1].x << "  " << pca[1].y << endl;
	return pca;
}


vector<Pos> hgMath::makeConvex(vector<Pos> poses) {
	int stack[100];
	vector<Pos> stack2;


	int idx = -1;
	stack[++idx] = 0;
	stack[++idx] = 1;

	int next = 2;
	while (next < poses.size())
	{
		while ((idx + 1) >= 2)
		{
			int second = stack[idx--];
			int first = stack[idx];
			cout << "FIRST " << poses[first].x << " " << poses[first].y << endl;
			cout << "Secon " << poses[second].x << " " << poses[second].y << endl;
			cout << "Next  " << poses[next].x << " " << poses[next].y << endl;
			if (hgMath::ccw(&poses[first], &poses[second], &poses[next])>0)
			{
				cout << "HERE!! "<< endl;
				stack[++idx] = second;
				break;
			}
		}
		stack[++idx] = next++;
	}
	for (int i = 0; i < idx+1; i++) {
		cout << "IDX " << stack[i] << endl;
		cout << "		 " << poses[stack[i]].x<<" "<< poses[stack[i]].y << endl;
		stack2.push_back(poses[stack[i]]);
	}
	return stack2;
	//return idx + 1;
}

//vector<int> hgMath::makeConvex(const vector<Pos> poses){
//
//	int stack[100];
//	int idx = -1;
//	stack[++idx] = 0;
//	stack[++idx] = 1;
//
//	int next = 2;
//	while (next < poses.size())
//	{
//
//		while ((idx + 1) >= 2)
//		{
//			int second = stack[idx--];
//			int first = stack[idx];
//
//			if (hgMath::ccw(&poses[first], &poses[second], &poses[next]))
//			{
//				stack[++idx] = second;
//				break;
//			}
//		}
//		stack[++idx] = next++;
//		// cout<<"idx = "<<idx<<" next "<<next-1<<endl;
//
//	}
//	for (int i = 0; i < idx; i++) {
//		cout << "IDX " << stack[i] << endl;
//	}
//	vector<int> stack2;
//	return stack2;
//
//
//
//	//int idx = 1;
//	//vector<int> stack;
//	//stack.push_back(0);
//	//stack.push_back(1);
//	////stack[++idx] = 1;
//	//
//	//int next = 2;
//	//while (next < poses.size())
//	//{
//	//
//	//	while ((idx + 1) >= 2)
//	//	{
//	//		int second = stack[idx--];
//	//		int first = stack[idx];
//	//
//	//		if (hgMath::ccw(&poses[first], &poses[second], &poses[next]) > 0)
//	//		//if (leftTurn(p[first], p[second], p[next]))
//	//		{
//	//			stack.push_back(second);
//	//			idx++;
//	//			break;
//	//		}
//	//	}
//	//	idx++;
//	//	stack.push_back(next);
//	//	next++;
//	//	// cout<<"idx = "<<idx<<" next "<<next-1<<endl;
//	//}
//	//return stack;
//}