#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF INT_MAX

int graph[MAX_VERTICES][MAX_VERTICES];
int visited[MAX_VERTICES];
int distance[MAX_VERTICES];
int previous[MAX_VERTICES];

void dijkstra(int n, int start) {
    for (int i = 0; i < n; i++) {
        distance[i] = INF;
        visited[i] = 0;
        previous[i] = -1;
    }

    distance[start] = 0;

    for (int count = 0; count < n - 1; count++) {
        int min = INF, u = -1;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && distance[i] <= min) {
                min = distance[i];
                u = i;
            }
        }

        if (u == -1) break;

        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] && distance[u] != INF &&
                distance[u] + graph[u][v] < distance[v]) {
                distance[v] = distance[u] + graph[u][v];
                previous[v] = u;
            }
        }
    }
}

void print_full_path_edges(int path[], int len, FILE *fout) {
    for (int i = 0; i < len - 1; i++) {
        fprintf(fout, "  %d -> %d [color=red penwidth=3];\n", path[i], path[i + 1]);
    }
}

void export_graphviz(int n, const char *filename, int path[], int path_len) {
    FILE *fout = fopen(filename, "w");
    fprintf(fout, "digraph G {\n");
    fprintf(fout, "  node [shape=circle style=filled fillcolor=lightgray];\n");

    for (int i = 0; i < n; i++)
        fprintf(fout, "  %d;\n", i);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (graph[i][j]) {
                fprintf(fout, "  %d -> %d [label=\"%d\"];\n", i, j, graph[i][j]);
            }

    if (path_len > 1) {
        print_full_path_edges(path, path_len, fout);
    }

    fprintf(fout, "}\n");
    fclose(fout);
}

int main() {
    int n, edges;
    int path[MAX_VERTICES], len = 0;

    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    if (n <= 0 || n > MAX_VERTICES) {
        printf("Invalid number of vertices.\n");
        return 1;
    }

    // Инициализация графа
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            graph[i][j] = 0;

    printf("Enter the number of edges: ");
    scanf("%d", &edges);

    printf("Enter edges in format: from to weight\n");
    for (int i = 0; i < edges; i++) {
        int u, v, w;

        while (1) {
            printf("Edge %d: ", i + 1);
            if (scanf("%d %d %d", &u, &v, &w) != 3) {
                printf("Invalid input. Please enter three integers.\n");
                while (getchar() != '\n'); // очистить буфер
                continue;
            }

            if (u < 0 || u >= n || v < 0 || v >= n) {
                printf("Invalid edge (%d -> %d). Vertices must be in [0, %d]. Try again.\n", u, v, n - 1);
                continue;
            }

            if (graph[u][v] != 0) {
                printf("Warning: edge %d → %d already exists with weight %d. Overwrite? (y/n): ", u, v, graph[u][v]);
                char confirm;
                scanf(" %c", &confirm);
                if (confirm != 'y' && confirm != 'Y') {
                    i--; // повтор итерации
                    continue;
                }
            }

            break; // корректный ввод
        }

        graph[u][v] = w;
        graph[v][u] = w; // если направленный граф — закомментируй эту строку
    }

    int start, end;
    while (1) {
        printf("Enter the start and end vertices: ");
        if (scanf("%d %d", &start, &end) != 2) {
            printf("Invalid input. Please enter two integers.\n");
            while (getchar() != '\n');
            continue;
        }

        if (start < 0 || start >= n || end < 0 || end >= n) {
            printf("Invalid vertices. Must be in [0, %d]. Try again.\n", n - 1);
            continue;
        }

        break;
    }

    dijkstra(n, start);

    if (distance[end] == INF) {
        printf("No path exists.\n");
    } else {
        printf("Shortest path: ");
        int curr = end;
        len = 0;
        while (curr != -1) {
            path[len++] = curr;
            curr = previous[curr];
        }
        for (int i = len - 1; i >= 0; i--)
            printf("%d ", path[i]);
        printf("\nPath length: %d\n", distance[end]);
    }

    export_graphviz(n, "graph.dot", path, len);
    printf("DOT file generated: graph.dot\n");
    printf("Use `dot -Tpng graph.dot -o graph.png` to visualize.\n");

    return 0;
}
