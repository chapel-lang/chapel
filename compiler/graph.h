/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _graph_H_
#define _graph_H_


enum GraphType { GraphViz, VCG };

void graph(FA *fa, char *fn, int graph_type = VCG);

extern char graph_fun[80];
extern char graph_var[80];

#endif
