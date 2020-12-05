#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>
#include <vector>


const double EPS = 1e-9;

struct Point {
    double x;
    double y;
};

struct seg {
    Point begin, end;
    int index;

    double y (double x) const {
        if (fabs (begin.x - end.x) < EPS)
            return begin.y;
        
        return begin.y + (end.y - begin.y) * (x - begin.x) / (end.x - begin.x);
    }
};


inline bool intersect1d (double l1, double r1, double l2, double r2) {
    if (l1 > r1)
        std::swap (l1, r1);

    if (l2 > r2)
        std::swap (l2, r2);

    return std::max (l1, l2) <= std::min (r1, r2) + EPS;
}

inline int vec (const Point & a, const Point & b, const Point & c) {
    double s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);

    return abs(s)<EPS ? 0 : s>0 ? +1 : -1;
}

bool intersect (const seg & a, const seg & b) {
    return intersect1d (a.begin.x, a.end.x, b.begin.x, b.end.x)
           && intersect1d (a.begin.y, a.end.y, b.begin.y, b.end.y)
           && vec (a.begin, a.end, b.begin) * vec (a.begin, a.end, b.end) <= 0
           && vec (b.begin, b.end, a.begin) * vec (b.begin, b.end, a.end) <= 0;
}


bool operator< (const seg & a, const seg & b) {
    double x = std::max (std::min (a.begin.x, a.end.x), std::min (b.begin.x, b.end.x));
    return a.y(x) < b.y(x) - EPS;
}


struct event {
    double x;
    int tp, index;

    event (double x, int tp, int index)
            : x(x), tp(tp), index(index)
    { }

    bool operator< (const event & e) const {
        if (abs (x - e.x) > EPS)  return x < e.x;
        return tp > e.tp;
    }
};

std::set<seg> s;
std::vector < std::set<seg>::iterator > where;

inline std::set<seg>::iterator prev (std::set<seg>::iterator it) {
    return it == s.begin() ? s.end() : --it;
}

inline std::set<seg>::iterator next (std::set<seg>::iterator it) {
    return ++it;
}

std::pair<int,int> solve (const std::vector<seg> & a) {
    int n = (int) a.size();
    std::vector<event> e;
    for (int i=0; i<n; ++i) {
        e.emplace_back(std::min (a[i].begin.x, a[i].end.x), +1, i);
        e.emplace_back(std::max (a[i].begin.x, a[i].end.x), -1, i);
    }
    
    std::sort (e.begin(), e.end());

    s.clear();
    where.resize (a.size());
    for (size_t i=0; i<e.size(); ++i) {
        int index = e[i].index;
        if (e[i].tp == +1) {
            std::set<seg>::iterator
                    nxt = s.lower_bound (a[index]),
                    prv = prev (nxt);
            if (nxt != s.end() && intersect (*nxt, a[index]))
                return std::make_pair (nxt->index, index);
            
            if (prv != s.end() && intersect (*prv, a[index]))
                return std::make_pair (prv->index, index);
            
            where[index] = s.insert (nxt, a[index]);
        }
        else {
            std::set<seg>::iterator
                    nxt = next (where[index]),
                    prv = prev (where[index]);
            if (nxt != s.end() && prv != s.end() && intersect (*nxt, *prv))
                return std::make_pair (prv->index, nxt->index);
            s.erase (where[index]);
        }
    }

    return std::make_pair (-1, -1);
}

int main() {
    std::vector<seg> v;

    int num = 0;
    scanf("%d", &num);

    for (int i = 0; i < num; ++i) {
        seg temp = {};
        scanf("%lf %lf %lf %lf", &temp.begin.x, &temp.begin.y, &temp.end.x, &temp.end.y);
        temp.index = i;

        v.push_back(temp);
    }

    std::pair <int, int> p = solve(v);

    if (p.first == -1 && p.second == -1) {
        printf("NO\n");
    } else {
        printf("YES\n");
        printf("%d %d\n", p.first + 1, p.second + 1);
    }

    return 0;
}
