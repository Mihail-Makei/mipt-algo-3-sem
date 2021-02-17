#include <cstdio>
#include <vector>
#include <set>

int mex(const std::vector<int>& a) {
    std::set<int> b(a.begin(), a.end());
    for (int i = 0; ; ++i)
        if (!b.count(i))
            return i;
}


std::vector<int> gValues(int qlen) {
    std::vector<int> g(qlen + 1, 0);
    g[1] = 1;
    g[2] = 1;
    g[3] = 2;
    g[4] = 0;
    g[5] = 1;

    for (int i = 6; i <= qlen; ++i) {
        std::vector<int> temp;
        temp.push_back(g[i - 1]);
        temp.push_back(g[i - 2]);
        for (int j = 2; j <= i - 3; ++j) {
            temp.push_back(g[j] ^ g[i - j - 1]);
        }

        g[i] = mex(temp);
    }
    
    return g;
}


void PrintAnswer(const std::vector<int>& g, int qlen) {
    if (g[qlen] == 0) {
        printf("Mueller\n");
    } else {
        printf("Schtirlitz\n");

        if (qlen == 2) {
            printf("1\n2\n");
            return;
        }

        if (qlen == 3) {
            printf("2\n");
            return;
        }

        if (qlen == 5) {
            printf("1\n3\n5\n");
            return;
        }

        if (g[qlen - 1] == 0) {
            printf("1\n");
        }

        if (g[qlen - 2] == 0) {
            printf("2\n");
        }

        for (int i = 3; i <= qlen - 2; ++i) {
            if ((g[qlen - i] ^ g[i - 1]) == 0) {
                printf("%d\n", i);
            }
        }

        if (g[qlen - 2] == 0) {
            printf("%d\n", qlen - 1);
        }

        if (g[qlen - 1] == 0) {
            printf("%d\n", qlen);
        }
    }
}


int main() {
    int qlen = 0;
    scanf("%d", &qlen);

    std::vector<int> g = gValues(qlen);

    PrintAnswer(g, qlen);
    
    return 0;
}
