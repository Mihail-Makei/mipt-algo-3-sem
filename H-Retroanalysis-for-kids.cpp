#include <vector>
#include <stdio.h>


enum state {
    WIN = 0,
    LOSE,
    DRAW
};


void DFS(
        int v,
        const std::vector<std::vector<int> >& graph,
        std::vector<bool>& visited,
        std::vector<state>& type,
        std::vector<int>& degree,
        std::vector<int>& c
) {
    if (visited[v])
        return;

    visited[v] = true;
    for (auto u : graph[v]) {
        if (!visited[u]) {
            if (type[v] == LOSE) {
                type[u] = WIN;
                c[u] = c[v] + 1;
            } else if (--degree[u] == 0) {
                type[u] = LOSE;
                c[u] = c[v] + 1;
            } else {
                continue;
            }
        }

        DFS(u, graph, visited, type, degree, c);
    }
}


std::vector<state> LaunchGame(int vertex_num, int edges_num) {
    std::vector<std::vector<int> > graph(vertex_num);
    std::vector<int> degree(vertex_num, 0);

    for (int i = 0; i < edges_num; ++i) {
        int v = 0, u = 0;
        scanf("%d %d", &v, &u);
        --u;
        --v;
        ++degree[v];

        graph[u].push_back(v);
    }

    std::vector<bool> visited(vertex_num, false);
    std::vector<int> c(vertex_num, 0);
    std::vector<state> type(vertex_num, DRAW);
    std::vector<int> terminate;

    for (int i = 0; i < vertex_num; ++i) {
        if (degree[i] == 0) {
            type[i] = LOSE;
            terminate.push_back(i);
        }
    }

    for (int i : terminate) {
        DFS(i, graph, visited, type, degree, c);
    }

    return type;
}


int main() {
    int vertex_num = 0, edges_num = 0;

    while (scanf("%d %d", &vertex_num, &edges_num) != EOF) {
        std::vector<state> type = LaunchGame(vertex_num, edges_num);

        for (int i = 0; i < vertex_num; ++i) {
            switch (type[i]) {
                case WIN:
                    printf("FIRST\n");
                    break;

                case LOSE:
                    printf("SECOND\n");
                    break;

                case DRAW:
                    printf("DRAW\n");
                    break;
            }
        }

        printf("\n");
    }

    return 0;
}
