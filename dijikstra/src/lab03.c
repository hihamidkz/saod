#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "binaryheap.h"
#include "graph.h"

struct graph *read_graph(char *filename);
void shortestpath_dijkstra(struct graph *gr, int src, int *d, int *prev);
void search_path(int *prev, int src, int dst, int *pathlen, int *path);

int main(int argc, char **argv)
{
	int i;
	int j;
	int src;
	int pathlen;
	int *d;
	int *prev;

	struct graph *gr;

	if (argc < 2) {
		fprintf(stderr, "Missing file name\n");
		exit(EXIT_FAILURE);
	}

	if (argc < 3) {
		fprintf(stderr, "Missing source node index\n");
		exit(EXIT_FAILURE);
	}

	src = atoi(argv[2]);

	if (src < 1) {
		fprintf(stderr, "Source node index <%s> must be greater than 0\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	gr = read_graph(argv[1]);

	// Выделяем место под массив минимальных весов
	d = (int*) malloc((gr->nvertices + 1) * sizeof(int));

	// Выделяем место под массив кратчайших путей
	prev = (int*) malloc((gr->nvertices + 1) * sizeof(int));

	shortestpath_dijkstra(gr, src, d, prev);

	int path[gr->nvertices];

	for (j = 1; j <= gr->nvertices; j++) {
		if (j != src) {
			search_path(prev, src, j, &pathlen, path);

			printf("Min path from <%d> to <%d> : ", src, j);

			for (i = 1; i <= pathlen; i++) {
				printf("%d ", path[i]);
			}

			printf("weight : %d\n", d[j]);
		}
	}

	graph_free(gr);
	free(prev);
	free(d);

	return 0;
}


struct graph *read_graph(char *filename)
{
	int i;
	int first_ver;
	int second_ver;
	int edge_count;
	int edge_weight;
	int vertex_count;

	FILE *input_file;

	struct graph *gr;

	input_file = fopen(filename, "r");

	if (input_file == NULL) {
		fprintf(stderr, "Cannot open the file <%s> for reading\n", filename);
		exit(EXIT_FAILURE);
	}

	// Прочитали из файла количество вершин
	fscanf(input_file, "%d", &vertex_count);

	// Создали пустой граф
	gr = graph_create(vertex_count);

	// Прочитали из файла количество ребер
	fscanf(input_file, "%d", &edge_count);

	for (i = 0; i < edge_count; i++) {
		fscanf(input_file, "%d %d %d", &first_ver, &second_ver, &edge_weight);
		graph_set_edge(gr, first_ver, second_ver, edge_weight);
	}

	fclose(input_file);
	return gr;
}


void shortestpath_dijkstra(struct graph *gr, int src, int *d, int *prev)
{
	int i;
	int j;

	struct heap *h;

	struct heapnode v;

	// Создаем пустую кучу
	h = heap_create(gr->nvertices);

	for (i = 1; i <= gr->nvertices; i++) {
		d[i] = (i != src) ? INT_MAX : 0;
		prev[i] = -1;
		heap_insert(h, d[i], i);
	}

	for (i = 1; i <= gr->nvertices; i++) {
		// Извлекаем узел ближайший к начальному
		v = heap_extract_min(h);

		// Отмечаем v как посещенный
		gr->visited[v.value - 1] = 1;

			// Цикл по смежным вершинам узла v
			for (j = 1; j <= gr->nvertices; j++) {
				if (graph_get_edge(gr, v.value, j) && !gr->visited[j - 1]) {
					if (d[v.value] + graph_get_edge(gr, v.value, j) < d[j]) {
						d[j] = d[v.value] + graph_get_edge(gr, v.value, j);
						heap_decrease_key(h, j, d[j]);
						prev[j] = v.value;
					}
				}
			}
	}
	heap_free(h);
}


void search_path(int *prev, int src, int dst, int *pathlen, int *path)
{
	int i;
	int j;

	*pathlen = 1;
	i = dst;

	
	while (i != src) {
		(*pathlen)++;
		i = prev[i];
	}

	j = 0;
	i = dst;

	while (i != src) {
		path[*pathlen - j] = i;
		i = prev[i];
		j++;
	}
	path[*pathlen - j] = i;
}
