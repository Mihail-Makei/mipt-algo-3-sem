#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

const double SMALL_NUM = 1e-9;

/*====== Vector2D ======*/

bool isZero(double a) {
    return fabs(a) < SMALL_NUM;
}

class Vector2D {
public:
    Vector2D(double x, double y);

    Vector2D operator-();

    friend Vector2D operator*(double c, const Vector2D & v);

    friend Vector2D operator+(const Vector2D & v1, const Vector2D & v2);
    friend Vector2D operator-(const Vector2D & v1, const Vector2D & v2);

    double norm();

    friend double operator*(const Vector2D& v1, const Vector2D& v2);
    
    bool LessPolarAngle(Vector2D v2) {
        if (x > 0 && v2.x < 0) {
            return true;
        }
        
        if (x < 0 && v2.x > 0) {
            return false;
        }

        double tg1 = y / x;
        double tg2 = v2.y / v2.x;

        if (isZero(x)) {
            return y > 0 && v2.x < 0;
        }
        if (isZero(v2.x)) {
            return v2.y > 0 ? x > 0 : !isZero(tg1 - tg2);
        }

        if (isZero(tg1 - tg2)) {
            return false;
        }

        return tg1 < tg2;
    }

    double x;
    double y;
};

/*============*/


std::vector<Vector2D> minkowskiSum(std::vector<Vector2D> & polygon1, std::vector<Vector2D> & polygon2) {
    polygon1.push_back(polygon1[0]);
    polygon2.push_back(polygon2[0]);

    std::vector<Vector2D> result;
    size_t i = 0, j = 0;

    while (i < polygon1.size() - 1 && j < polygon2.size() - 1) {
        result.push_back(polygon1[i] + polygon2[j]);
        if ((polygon1[i + 1] - polygon1[i]).LessPolarAngle(polygon2[j + 1] - polygon2[j])) {
            ++i;
        } else if ((polygon2[j + 1] - polygon2[j]).LessPolarAngle(polygon1[i + 1] - polygon1[i])) {
            ++j;
        } else {
            ++i;
            ++j;
        }
    }

    return result;
}

/*============*/

bool differentSigns(double a, double b) {
    return a * b < 0;
}

bool containsZero(const std::vector<Vector2D> & polygon) {
    std::vector<double> x0;

    if (differentSigns(polygon[0].y, polygon[polygon.size() - 1].y)) {
        x0.push_back((polygon[0].x + polygon[polygon.size() - 1].x) / 2);
    }

    for (size_t i = 0; i < polygon.size() - 1; i++) {
        if (differentSigns(polygon[i].y, polygon[i + 1].y)) {
            x0.push_back((polygon[i].x + polygon[i + 1].x) / 2);
        }
    }

    for (auto i : polygon) {
        if (isZero(i.y)) {
            x0.push_back(i.x);
        }
    }

    if (x0.empty()) {
        return false;
    }

    double min = x0[0];
    double max = x0[0];
    for (double x: x0) {
        if (x < min) {
            min = x;
        }
        if (x > max) {
            max = x;
        }
    }

    return isZero(max) || isZero(min) || differentSigns(max, min);
}

/*============*/

std::vector<Vector2D> normalize(const std::vector<Vector2D> & v) {
    std::vector<Vector2D> r;

    size_t min = 0;
    for (size_t i = 1; i < v.size(); i++) {
        if (v[i].x < v[min].x || isZero(v[i].x - v[min].x) && v[i].y < v[min].y) {
            min = i;
        }
    }

    for (size_t i = 0; i < v.size(); i++) {
        r.push_back(v[(i + min + 1) % v.size()]);
    }

    std::reverse(r.begin(), r.end());
    return r;
}

/*============*/

int main() {
    std::vector<Vector2D> polygon1;
    std::vector<Vector2D> polygon2;

    int num1 = 0;
    scanf("%d", &num1);
    for (size_t i = 0; i < num1; i++) {
        double x, y;
        std::cin >> x >> y;
        polygon1.emplace_back(x, y);
    }

    int num2 = 0;
    scanf("%d", &num2);
    for (size_t i = 0; i < num2; i++) {
        double x, y;
        scanf("%lf %lf", &x, &y);
        polygon2.emplace_back(-x, -y);
    }

    polygon1 = normalize(polygon1);
    polygon2 = normalize(polygon2);

    std::vector<Vector2D> minksum = minkowskiSum(polygon1, polygon2);

    if (containsZero(minksum))
        printf("YES\n");
    else
        printf("NO\n");

    return 0;
}


/*====== Vector2D ======*/

Vector2D::Vector2D(double x, double y)
        : x(x)
        , y(y) {}



Vector2D Vector2D::operator-() {
    return Vector2D(-x, -y);
}



Vector2D operator+(const Vector2D & v1, const Vector2D & v2) {
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}



Vector2D operator-(const Vector2D & v1, const Vector2D & v2) {
    return Vector2D(v1.x - v2.x, v1.y - v2.y);
}


double Vector2D::norm() {
    return sqrt(*this * *this);
}


Vector2D operator*(double c, const Vector2D & v) {
    return Vector2D(c * v.x, c * v.y);
}

double operator*(const Vector2D &v1, const Vector2D &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
