#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>


double const INFTY = 1e9;

const double TURN_ANGLE = 0.01;

struct Point {
    double x;
    double y;
    double z;
    int index;
    Point *next, *prev;

    Point(double x = 0, double y = 0, double z = 0, int index = -1)
            : x(x), y(y), z(z)
            , index(index)
            , next(nullptr)
            , prev(nullptr) {}

    bool PerformEvent() {
        if (prev->next != this) {
            prev->next = this;
            next->prev = this;
            return true;
        } else {
            prev->next = next;
            next->prev = prev;
            return false;
        }
    }

    friend bool operator < (const Point& p1, const Point& p2) {
        return p1.x < p2.x;
    }

    void rotate(double angle) {
        // Y
        double new_x = x * cos(angle) + z * sin(angle);
        double new_z = -x * sin(angle) + z * cos(angle);
        x = new_x;
        z = new_z;

        // X
        new_z = z * cos(angle) + y * sin(angle);
        double new_y = -z * sin(angle) + y * cos(angle);
        z = new_z;
        y = new_y;

        // Z
        new_x = x * cos(angle) + y * sin(angle);
        new_y = -x * sin(angle) + y * cos(angle);
        x = new_x;
        y = new_y;
    }
};


typedef Point Event;


struct Face {
    int first;
    int second;
    int third;

    Face(int first, int second, int third)
            : first(first)
            , second(second)
            , third(third) {}

    void FixOrder() {
        if (second < first && second < third) {
            std::swap(first, second);
            std::swap(second, third);
        } else if (third < first && third < second) {
            std::swap(second, third);
            std::swap(first, second);
        }
    }

    friend bool operator<(const Face& f1, const Face& f2) {
        if (f1.first < f2.first) {
            return true;
        } else if (f1.first > f2.first) {
            return false;
        } else {
            if (f1.second < f2.second) {
                return true;
            } else if (f1.second > f2.second) {
                return false;
            } else {
                return f1.third < f2.third;
            }
        }
    }
};



double sign(const Point* a, const Point* b, const Point* c) {
    if (!a || !b || !c) {
        return INFTY;
    }

    return (b->x - a->x) * (c->y - b->y) - (b->y - a->y) * (c->x - b->x);
}



double time(const Point* a, const Point* b, const Point* c) {
    if (a == nullptr || b == nullptr || c == nullptr) {
        return INFTY;
    }
    return ((b->x - a->x) * (c->z - b->z) - (b->z - a->z) * (c->x - b->x)) / sign(a, b, c);
}






std::vector<Event*> BuildDownHull(std::vector<Point>& points, size_t l, size_t r) {
    if (r - l == 1) {
        return std::vector<Event*>();
    }

    size_t m = (l + r) / 2;
    std::vector<Event*> evs[2] = {
            BuildDownHull(points, l, m),
            BuildDownHull(points, m, r)
    };
    std::vector<Event*> result;

    Point* u = &points[m - 1];
    Point* v = &points[m];
    while (true) {
        if (sign(u, v, v->next) < 0) {
            v = v->next;
        } else if (sign(u->prev, u, v) < 0) {
            u = u->prev;
        } else {
            break;
        }
    }

    size_t p1 = 0, p2 = 0;
    for (double current_t = -INFTY;;) {
        Point* left = nullptr;
        Point* right = nullptr;
        std::vector<double> next_t(6, INFTY);

        if (p1 < evs[0].size()) {
            left = evs[0][p1];
            next_t[0] = time(left->prev, left, left->next);
        }
        if (p2 < evs[1].size()) {
            right = evs[1][p2];
            next_t[1] = time(right->prev, right, right->next);
        }
        next_t[2] = time(u, v, v->next);
        next_t[3] = time(u, v->prev, v);
        next_t[4] = time(u->prev, u, v);
        next_t[5] = time(u, u->next, v);

        int min_i = -1;
        double min_t = INFTY;
        for (int i = 0; i < 6; i++) {
            if (next_t[i] > current_t && next_t[i] < min_t) {
                min_t = next_t[i];
                min_i = i;
            }
        }
        if (min_i == -1 || min_t >= INFTY) {
            break;
        }

        switch (min_i) {
            case 0:
                if (left->x < u->x) {
                    result.push_back(left);
                }
                left->PerformEvent();
                p1++;
                break;
            case 1:
                if (right->x > v->x) {
                    result.push_back(right);
                }
                right->PerformEvent();
                p2++;
                break;
            case 2:
                result.push_back(v);
                v = v->next;
                break;
            case 3:
                v = v->prev;
                result.push_back(v);
                break;
            case 4:
                result.push_back(u);
                u = u->prev;
                break;
            case 5:
                u = u->next;
                result.push_back(u);
                break;
            default:
                break;
        }
        current_t = min_t;
    }
    u->next = v;
    v->prev = u;
    for (int i = static_cast<int>(result.size() - 1); i >= 0; i--) {
        Point* current = result[i];
        if (current->x > u->x && current->x < v->x) {
            u->next = v->prev = current;
            current->prev = u;
            current->next = v;
            if (current->x <= points[m - 1].x) {
                u = current;
            } else {
                v = current;
            }
        } else {
            current->PerformEvent();
            if (current == u) {
                u = u->prev;
            }
            if (current == v) {
                v = v->next;
            }
        }
    }
    return result;
}



std::vector<Face> BuildConvexHull(std::vector<Point> points) {
    size_t num = points.size();
    std::vector<Face> result;

    std::sort(points.begin(), points.end());

    std::vector<Event*> events = BuildDownHull(points, 0, num);
    for (Event* e : events) {
        Face current(e->prev->index, e->index, e->next->index);

        if (!e->PerformEvent()) {
            std::swap(current.first, current.second);
        }

        result.push_back(current);
    }

    for (Point& p : points) {
        p.next = nullptr;
        p.prev = nullptr;
        p.z = -p.z;
    }
    events = BuildDownHull(points, 0, num);

    for (Event* e : events) {
        Face current(e->prev->index, e->index, e->next->index);

        if (e->PerformEvent()) {
            std::swap(current.first, current.second);
        }

        result.push_back(current);
    }

    return result;
}



int main() {
    int num = 0;
    scanf("%d", &num);

    for (int i = 0; i < num; ++i) {
        int m = 0;
        std::vector<Point> points;

        scanf("%d", &m);
        for (int j = 0; j < m; ++j) {
            Point p;

            scanf("%lf %lf %lf", &p.x, &p.y, &p.z);
            p.index = j;

            p.rotate(TURN_ANGLE);
            points.push_back(p);
        }

        std::vector<Face> hull = BuildConvexHull(points);
        for (Face& f : hull) {
            f.FixOrder();
        }

        std::sort(hull.begin(), hull.end());

        printf("%zu\n", hull.size());

        for (Face& f : hull) {
            printf("3 %d %d %d\n", f.first, f.second, f.third);
        }
    }

    return 0;
}
