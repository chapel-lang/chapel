#ifndef _graph_H_
#define _graph_H_

#include "ifa.h"

class FA;

void graph(FA *fa, char *fn, int graph_type = VCG);

void graph_node(FILE *fp, void *id, char *label, int options = 0);
void graph_edge(FILE *fp, void *a, void *b, int options = 0);

extern char graph_fun[80];
extern char graph_var[80];

#endif
