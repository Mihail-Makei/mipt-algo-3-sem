#include <cstdio>
#include <cstddef>
#include <cmath>
#include <limits>
#include <algorithm>
#include <vector>
#include <set>


long double const inf = std::numeric_limits<long double>::max();

typedef std::pair<int, int> Edge;

class Point {
public:
    int index = -1;
    long double x = 0, y = 0, z = 0;

    Point *next = nullptr, *prev = nullptr;

    Point(long double x = 0, long double y = 0, long double z = 0, int index = -1) : x(x), y(y), z(z), index(index) {}

    friend Point operator - (const Point &p1, const Point &p2);

    friend bool operator < (const Point &p1, const Point &p2);

    friend long double vec(const Point &p1, const Point &p2);

    bool performEvent();
};

typedef Point Event;

class Face {
public:
    int points[3];

    Face(int first, int second, int third);

    friend bool operator < (const Face &f1, const Face &f2);
};


bool isAcceptable(const Point *a, const Point *b, const Point *c);
long double sign(const Point *a, const Point *b, const Point *c);
long double time(const Point *a, const Point *b, const Point *c);

std::vector<Event*> BuildHull(std::vector<Point> &points, size_t left, size_t right);
std::vector<Face> BuildConvex(std::vector<Point> points);

long double VoronoiCount(std::vector<Point> &sites);


int main() {
    long double x = 0, y = 0;
    std::vector<Point> points;

    int counter = 0;

    while (scanf("%Lf %Lf", &y, &x) != EOF) {
        Point pnt(x, y, x * x + y * y, counter);
        points.emplace_back(pnt);
        ++counter;
    }

    printf("%.7Lf", VoronoiCount(points));

    return 0;
}

Point operator-(const Point &p1, const Point &p2) {
    return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

bool operator<(const Point &p1, const Point &p2) {
    return p1.x < p2.x;
}

long double vec(const Point &p1, const Point &p2) {
    return (p1.x * p2.y - p1.y * p2.x);
}

bool Point::performEvent() {
    if (prev->next != this) {
        prev->next = this;
        next->prev = this;
        return true;
    } 
    
    prev->next = next;
    next->prev = prev;
    return false;
}


bool operator<(const Face &f1, const Face &f2) {
    return f1.points[0] < f2.points[0] || f1.points[0] <= f2.points[0] &&
                                          (f1.points[1] < f2.points[1] ||
                                           f1.points[1] <= f2.points[1] &&
                                           f1.points[2] < f2.points[2]);
}

bool isAcceptable(const Point *a, const Point *b, const Point *c) {
    return vec(*b - *a, *c - *b) > 0;
}

long double sign(const Point *a, const Point *b, const Point *c) {
    if (!a || !b || !c) {
        return inf;
    }

    return (b->x - a->x) * (c->y - b->y) - (b->y - a->y) * (c->x - b->x);
}

long double time(const Point *a, const Point *b, const Point *c) {
    if (!a || !b || !c) {
        return inf;
    }

    return ((b->x - a->x) * (c->z - b->z) - (b->z - a->z) * (c->x - b->x)) / sign(a, b, c);
}


Face::Face(int first, int second, int third) {
    points[0] = first;
    points[1] = second;
    points[2] = third;
}


std::vector<Event*> BuildHull(std::vector<Point> &points, size_t left, size_t right) {
    if (right - left == 1) {
        return std::vector<Event*> ();
    }
    
    int mid = (left + right) / 2;
    int counter1 = 0, counter2 = 0;

    std::vector<Event*> p_events[2] = {
            BuildHull(points, left, mid),
            BuildHull(points, mid, right)
    };
    
    std::vector<Event*> final;

    Point *v = &points[mid];
    Point *u = &points[mid - 1];

    while (true) {
        if (sign(u, v, v->next) < 0)
            v = v->next;
        else {
            if (sign(u->prev, u, v) < 0)
                u = u->prev;
            else
                break;
        }
    }


    long double curr_t = -inf;
    while (true) {
        Point *left = nullptr;
        Point *right = nullptr;
        std::vector<long double> next(6, inf);

        if (counter1 < p_events[0].size()) {
            left = p_events[0][counter1];
            next[0] = time(left->prev, left, left->next);
        }
        
        if (counter2 < p_events[1].size()) {
            right = p_events[1][counter2];
            next[1] = time(right->prev, right, right->next);
        }

        next[2] = time(u, v, v->next);
        next[3] = time(u, v->prev, v);
        next[4] = time(u->prev, u, v);
        next[5] = time(u, u->next, v);

        int min_ind = -1;
        long double min_t = inf;

        for (int i = 0; i < 6; i++) {
            if ((next[i] > curr_t) && (next[i] < min_t)) {
                min_t = next[i];
                min_ind = i;
            }
        }

        if (min_ind == -1 || min_t >= inf) {
            break;
        }


        switch (min_ind) {
            case 0:
                if (left->x < u->x) {
                    final.emplace_back(left);
                }
                left->performEvent();
                ++counter1;
                break;

            case 1:
                if (right->x > v->x) {
                    final.emplace_back(right);
                }

                right->performEvent();
                ++counter2;
                break;

            case 2:
                final.emplace_back(v);
                v = v->next;
                break;

            case 3:
                v = v->prev;
                final.emplace_back(v);
                break;

            case 4:
                final.emplace_back(u);
                u = u->prev;
                break;

            case 5:
                u = u->next;
                final.emplace_back(u);
                break;
        }

        curr_t = min_t;
    }


    u->next = v;
    v->prev = u;

    for (int i = final.size() - 1; i >= 0; i--) {
        Point* current = final[i];
        if (current->x > u->x && current->x < v->x) {
            u->next = v->prev = current;
            current->prev = u;
            current->next = v;

            if (current->x <= points[mid - 1].x)
                u = current;
            else
                v = current;

        } else {
            current->performEvent();
            if (current == u) {
                u = u->prev;
            }

            if (current == v) {
                v = v->next;
            }
        }
    }

    return final;
}

long double VoronoiCount(std::vector<Point> &sites) {
    std::vector<Face> dimentional_hull = BuildConvex(sites);
    std::set<Edge> edges;
    std::vector<int> edgeid(sites.size());
    std::vector<bool> in_planar_hull(sites.size(), false);

    for (Face& face : dimentional_hull) {
        for (size_t j = 0; j < 3; j++) {
            Edge edge = {face.points[j], face.points[(j + 1) % 3]};
            if (edge.first > edge.second) {
                std::swap(edge.first, edge.second);
            }

            edges.insert(edge);
        }
    }

    for (const Edge& edge : edges) {
        ++edgeid[edge.first];
        ++edgeid[edge.second];
    }

    std::sort(sites.begin(), sites.end());
    std::vector<Point> planar_hull;

    for (const Point& site : sites) {
        while (static_cast<int>(planar_hull.size()) >= 2) {
            if (isAcceptable(&planar_hull[planar_hull.size() - 2], &planar_hull.back(), &site)) {
                break;
            }
            planar_hull.pop_back();
        }
        planar_hull.emplace_back(site);
    }

    for (int i = static_cast<int>(sites.size()) - 2, bottom = static_cast<int>(planar_hull.size()); i >= 0; --i) {
        while (static_cast<int>(planar_hull.size()) > bottom) {
            if (isAcceptable(&planar_hull[planar_hull.size() - 2], &planar_hull.back(), &sites[i])) {
                break;
            }
            planar_hull.pop_back();
        }
        planar_hull.emplace_back(sites[i]);
    }

    for (Point& i : planar_hull) {
        in_planar_hull[i.index] = true;
    }

    size_t delaunay_inner_pts = 0, total_degree = 0;
    for (size_t i = 0; i < sites.size(); ++i) {
        if (!in_planar_hull[i]) {
            total_degree += edgeid[i];
            ++delaunay_inner_pts;
        }
    }

    if (!delaunay_inner_pts) {
        return 0.0;
    } else {
        return (static_cast<long double>(total_degree) / static_cast<long double>(delaunay_inner_pts));
    }
}

std::vector<Face> BuildConvex(std::vector<Point> points) {
    std::vector<Face> final;
    std::sort(points.begin(), points.end());

    int num = points.size();
    std::vector<Event*> events = BuildHull(points, 0, num);

    for (Event* event : events) {
        Face current(event->prev->index, event->index, event->next->index);

        if (!event->performEvent()) {
            std::swap(current.points[0], current.points[1]);
        }
        final.emplace_back(current);
    }

    for (Point& p : points) {
        p.next = nullptr;
        p.prev = nullptr;
        p.z = -p.z;
    }

    events = BuildHull(points, 0, num);

    for (Event* event : events) {
        Face curr(event->prev->index, event->index, event->next->index);

        if (event->performEvent()) {
            std::swap(curr.points[0], curr.points[1]);
        }

        final.emplace_back(curr);
    }

    return final;
}
