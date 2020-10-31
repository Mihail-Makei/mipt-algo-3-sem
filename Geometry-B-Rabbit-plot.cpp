#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>


struct Point {
    double x;
    double y;
};


bool comp(const Point& a, const Point& b);

double cross(const Point &O, const Point &A, const Point &B);

std::vector<Point> convex_hull(std::vector<Point> P);

double dist(Point x, Point y);


int main() {
    std::vector<Point> v;
    int num = 0;

    double x = 0;
    double y = 0;

    scanf("%d", &num);

    for (int i = 0; i < num; ++i) {
        scanf("%lf %lf", &x, &y);
        v.push_back({x, y});
    }

    std::vector<Point> convex = convex_hull(v);

    double dst = 0;

    for (int i = 0; i < convex.size() - 1; ++i)
        dst += dist(convex[i], convex[i+1]);

    dst += dist(convex[0], convex[convex.size() - 1]);

    printf("%lf", dst);
}


double dist(Point x, Point y) {
    return sqrt((y.x - x.x) * (y.x - x.x) + (y.y  - x.y) * (y.y - x.y));
}


std::vector<Point> convex_hull(std::vector<Point> P) {
    size_t n = P.size();
    size_t k = 0;

    if (n <= 3)
        return P;

    std::vector<Point> hull(2*n);

    std::sort(P.begin(), P.end(), comp);

    for (size_t i = 0; i < n; ++i) {
        while (k >= 2 && cross(hull[k-2], hull[k-1], P[i]) <= 0)
            --k;

        hull[k++] = P[i];
    }

    for (size_t i = n-1, t = k+1; i > 0; --i) {
        while (k >= t && cross(hull[k-2], hull[k-1], P[i-1]) <= 0)
            --k;

        hull[k++] = P[i-1];
    }

    hull.resize(k-1);
    return hull;
}


double cross(const Point &O, const Point &A, const Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}


bool comp(const Point& a, const Point& b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}
