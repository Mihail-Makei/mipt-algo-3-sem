#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <algorithm>

const double SMALL_NUM = 1e-6;

struct point {
    double x, y;

    friend std::istream& operator>> (std::istream& in, point& p);
};

std::istream& operator>> (std::istream& in, point& p) {
    in >> p.x >> p.y;
    return in;
}

struct segment {
    point begin, end;

    segment() = delete;
    segment(point begin, point end): begin(begin), end(end) {}
};


bool intersect (segment s1, segment s2, point& p) {
    double dy1 = s1.begin.y - s1.end.y;
    double dx1 = s1.end.x - s1.begin.x;
    double dot1 = (s1.begin.y - s1.end.y) * s1.begin.x + (s1.end.x - s1.begin.x) * s1.begin.y;

    double dy2 = s2.begin.y - s2.end.y;
    double dx2 = s2.end.x - s2.begin.x;
    double dot2 = (s2.begin.y - s2.end.y) * s2.begin.x + (s2.end.x - s2.begin.x) * s2.begin.y;

    double temp = dy1 * dx2 - dx1 * dy2;

    if (abs(temp) < SMALL_NUM)
        return false;

    p.x = (dot1 * dx2 - dx1 * dot2) / temp;
    p.y = (dy1 * dot2 - dot1 * dy2) / temp;

    return p.x >= s1.begin.x - SMALL_NUM && p.x <= s1.end.x + SMALL_NUM && p.x >= s2.begin.x - SMALL_NUM && p.x <= s2.end.x + SMALL_NUM;
}

double segment_y (segment s, double x) {
    return s.begin.y + (s.end.y - s.begin.y) * (x - s.begin.x) / (s.end.x - s.begin.x);
}

struct section {
    double y1, y2;
    int triangle_num;
};


bool comp_sections(section a, section b) {
    return a.y1 < b.y1 - SMALL_NUM || abs(a.y1 - b.y1) < SMALL_NUM && a.y2 < b.y2 - SMALL_NUM;
}


bool comp_points(point a, point b) {
    return a.x < b.x - SMALL_NUM;
}

bool intersections_equiv(point a, point b) {
    return fabs(a.x - b.x) < SMALL_NUM;
}

int main() {
    std::vector<segment> edges;
    for (int i = 0; i < 2; ++i) {
        point a{}, b{}, c{};
        std::cin >> a >> b >> c;

        segment AB(a, b);
        edges.emplace_back(AB);

        segment AC(a, c);
        edges.emplace_back(AC);

        segment BC(b, c);
        edges.emplace_back(BC);
    }

    for (auto& edge : edges) {
        if (edge.begin.x > edge.end.x) {
            double temp = edge.begin.x;
            edge.begin.x = edge.end.x;
            edge.end.x = temp;

            temp = edge.begin.y;
            edge.begin.y = edge.end.y;
            edge.end.y = temp;
        }
    }

    std::vector<point> intersections;
    for (int i = 0; i < edges.size(); ++i) {
        for (int j = i + 1; j < edges.size(); ++j) {
            point temp{};
            if (intersect(edges[i], edges[j], temp)) {
                intersections.emplace_back(temp);
            }
        }
    }

    std::sort(intersections.begin(), intersections.end(), comp_points);
    intersections.erase(std::unique(intersections.begin(), intersections.end(), intersections_equiv), intersections.end());

    double square = 0;
    bool used[2] = {};

    for (int i = 0; i + 1 < intersections.size(); ++i) {
        double x1 = intersections[i].x,  x2 = intersections[i + 1].x;
        std::vector<section> sections;

        for (int j = 0; j < edges.size(); ++j) {
            if (edges[j].begin.x != edges[j].end.x) {
                if (edges[j].begin.x <= x1 && edges[j].end.x >= x2) {
                    section sect = {segment_y(edges[j], x1), segment_y(edges[j], x2), static_cast<int>(j) / 3};
                    sections.emplace_back(sect);
                }
            }
        }

        std::sort (sections.begin(), sections.end(), comp_sections);

        double mid_line = 0;
        for (int j = 0; j < sections.size(); ) {
            section lower = sections[j];
            ++j;

            used[lower.triangle_num] = true;
            int counter = 1;

            while (counter && j < sections.size()) {
                used[sections[j].triangle_num] = !used[sections[j].triangle_num];

                if (used[sections[j].triangle_num]) {
                    ++counter;
                } else {
                    --counter;
                }

                ++j;
            }
            section upper = sections[j - 1];
            mid_line += upper.y1 - lower.y1 + upper.y2 - lower.y2;
        }
        square += mid_line * (x2 - x1) / 2;
    }

    printf("%.4lf\n", square);

    return 0;
}
