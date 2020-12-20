#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>


struct Point {
    double x;
    double y;

    friend std::istream& operator>> (std::istream& in, Point& p);
};


bool comp(const Point& a, const Point& b);

double cross(const Point& O, const Point& A, const Point& B);

std::vector<Point> ConvexHull(std::vector<Point> P);

double dist(Point x, Point y);

std::istream& operator>> (std::istream& in, Point& p);

double perimeter(const std::vector<Point>& convex);


int main() {
    int num = 0;

    scanf("%d", &num);

	std::vector<Point> v(num);
    for (int i = 0; i < num; ++i) {
        std::cin >> v[i];
    }

    std::vector<Point> convex = ConvexHull(v);

    printf("%lf\n", perimeter(convex));
}


double dist(Point x, Point y) {
    return sqrt((y.x - x.x) * (y.x - x.x) + (y.y  - x.y) * (y.y - x.y));
}


std::vector<Point> ConvexHull(std::vector<Point> P) {
    size_t n = P.size();
    size_t k = 0;

    if (n <= 3)
        return P;

    std::vector<Point> hull(2 * n);

    std::sort(P.begin(), P.end(), comp);

    for (size_t i = 0; i < n; ++i) {
        while (k >= 2 && cross(hull[k - 2], hull[k - 1], P[i]) <= 0)
            --k;

        hull[k++] = P[i];
    }

    for (size_t i = n - 1, t = k + 1; i > 0; --i) {
        while (k >= t && cross(hull[k - 2], hull[k - 1], P[i - 1]) <= 0)
            --k;

        hull[k++] = P[i - 1];
    }

    hull.resize(k - 1);
    return hull;
}


double cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}


bool comp(const Point& a, const Point& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

std::istream& operator>> (std::istream& in, Point& p) {
	in >> p.x >> p.y;

	return in;
}

double perimeter(const std::vector<Point>& convex) {
	double dst = 0;

    for (int i = 0; i < convex.size() - 1; ++i)
        dst += dist(convex[i], convex[i+1]);

    dst += dist(convex[0], convex[convex.size() - 1]);

	return dst;
}
