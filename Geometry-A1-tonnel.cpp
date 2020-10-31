#include <iostream>
#include <iomanip>
#include <cmath>

const double SMALL_NUM = 1e-7;

struct Vector3D {
    Vector3D(double x, double y, double z): x(x), y(y), z(z) {};

    Vector3D operator-();

    friend Vector3D operator*(double c, const Vector3D & v);
    friend Vector3D operator+(const Vector3D & v1, const Vector3D & v2);
    friend Vector3D operator-(const Vector3D & v1, const Vector3D & v2);
    friend double operator*(const Vector3D & v1, const Vector3D & v2);

    double length();

    double x;
    double y;
    double z;
};



struct Segment3D {
    Vector3D begin;
    Vector3D end;

    Segment3D(const Vector3D& begin, const Vector3D& end): begin(begin), end(end) {};
};


double dabs(double n);

double SegmentDist(const Segment3D& S1, const Segment3D& S2);


int main() {
    double x1 = 0, y1 = 0, z1 = 0;
    double x2 = 0, y2 = 0, z2 = 0;
    double x3 = 0, y3 = 0, z3 = 0, x4 = 0, y4 = 0, z4 = 0;

    scanf("%lf %lf %lf", &x1, &y1, &z1);
    scanf("%lf %lf %lf", &x2, &y2, &z2);
    scanf("%lf %lf %lf", &x3, &y3, &z3);
    scanf("%lf %lf %lf", &x4, &y4, &z4);

    printf("%.6f", SegmentDist(Segment3D(Vector3D(x1, y1, z1), Vector3D(x2, y2, z2)),
                                        Segment3D(Vector3D(x3, y3, z3), Vector3D(x4, y4, z4))));
}





Vector3D Vector3D::operator-() {
    return Vector3D(-x, -y, -z);
}



Vector3D operator+(const Vector3D & v1, const Vector3D & v2) {
    return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}



Vector3D operator-(const Vector3D & v1, const Vector3D & v2) {
    return Vector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}



double Vector3D::length() {
    return sqrt(*this * *this);
}


Vector3D operator*(double c, const Vector3D & v) {
    return Vector3D(c * v.x, c * v.y, c * v.z);
}

double operator*(const Vector3D &v1, const Vector3D &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3D operator/(double c, const Vector3D & v) {
    return Vector3D(c * v.x, c * v.y, c * v.z);
}

double SegmentDist(const Segment3D& S1, const Segment3D& S2) {
    Vector3D s1 = S1.end - S1.begin;
    Vector3D s2 = S2.end - S2.begin;
    Vector3D dst = S1.begin - S2.begin;
    double s1s1 = s1 * s1;
    double s1s2 = s1 * s2;
    double s2s2 = s2 * s2;
    double s1dst = s1 * dst;
    double s2dst = s2 * dst;
    double D = s1s1*s2s2 - s1s2*s1s2;
    double s, sN, sD = D;
    double t, tN, tD = D;

    if (D <= 0.0) {
        sN = 0.0;
        sD = 1.0;
        tN = s2dst;
        tD = s2s2;
    } else {
        sN = (s1s2*s2dst - s2s2*s1dst);
        tN = (s1s1*s2dst - s1s2*s1dst);
        if (sN <= 0.0) {
            sN = 0.0;
            tN = s2dst;
            tD = s2s2;
        } else if (sN >= sD) {
            sN = sD;
            tN = s2dst + s1s2;
            tD = s2s2;
        }
    }

    if (tN <= 0.0) {
        tN = 0.0;
        if (-s1dst <= 0.0) {
            sN = 0.0;
        } else if (-s1dst >= s1s1) {
            sN = sD;
        } else {
            sN = -s1dst;
            sD = s1s1;
        }
    } else if (tN >= tD) {
        tN = tD;
        if ((-s1dst + s1s2) <= 0.0) {
            sN = 0;
        } else if ((-s1dst + s1s2) >= s1s1) {
            sN = sD;
        } else {
            sN = (-s1dst +  s1s2);
            sD = s1s1;
        }
    }
    s = (dabs(sN) <= SMALL_NUM ? 0.0 : sN / sD);
    t = (dabs(tN) <= SMALL_NUM ? 0.0 : tN / tD);

    Vector3D   dP = dst + (s * s1) - (t * s2);

    return dP.length();
}

double dabs(double n) {
    return (n > 0 ? n: -n);
}
