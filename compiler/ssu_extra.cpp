/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

static void
print_classes_nodes(Vec<CQClass*> &cq_classes) {
  forv_CQClass(c, cq_classes) {
    printf("Node Class\n");
    forv_PNode(n, c->nodes)
      if1_dump_code(stdout, n->code, 2);
  }
}

static char *SPACES = "                                        ";
#define SP(_fp, _n) fputs(&SPACES[40-(_n)], _fp)

static void
print_regions_nodes(Region *r, int indent) {
  SP(stdout, indent); printf("Region\n");
  forv_PNode(n, r->nodes)
    if1_dump_code(stdout, n->code, indent + 3);
  forv_Region(rr, r->children)
    print_regions_nodes(rr, indent + 2);
}

static void
print_classes_edges(Vec<CQClass *> &v) {
  forv_CQClass(c, v) {
    printf("Edge Class\n");
    forv_UEdge(e, c->edges) {
      if (e->from->code) 
	if1_dump_code(stdout, e->from->code, 2);
      else printf("FAKE\n");
      printf("->"); 
      if (e->to->code) 
	if1_dump_code(stdout, e->to->code, 0);
      else printf("FAKE\n");
    }
  }
}

static void
print_pnode(PNode *n, char *s) {
  printf("  %s: used %d RVALS ", s, n->used);
  forv_Var(v, n->rvals) {
    printf("%X:", (int)v);
    if1_dump_sym(stdout, v->sym);
  }
  printf(" LVALS ");
  forv_Var(v, n->lvals) {
    printf("%X:", (int)v);
    if (v)
      if1_dump_sym(stdout, v->sym);
    else
      printf("NULL");
  }
  printf("\n");
}

static void
print_renamed(Fun *f) {
  Vec<PNode *> nodes;
  f->collect_PNodes(nodes);
  forv_PNode(n, nodes) {
    forv_PNode(p, n->phi) print_pnode(p, "phi");
    if1_dump_code(stdout, n->code, 0);
    if (n->rvals.n || n->lvals.n)
      print_pnode(n, "node");
    forv_PNode(p, n->phy) print_pnode(p, "phy");
  }
}

static void
print_ssu(Fun *f, Vec<CQClass*> cq_classes, Region *region, Vec<PNode *> nodes) {
  if (verbose_level) {
    printf("%d cq classes\n", cq_classes.n);
    int phi = 0, phy = 0;
    forv_PNode(n, nodes) {
      phi += n->phi.n;
      phy += n->phy.n;
    }
    printf("%d phi nodes\n", phi);
    printf("%d phy nodes\n", phi);
  }
  if (verbose_level > 1) {
    print_classes_nodes(cq_classes);
    print_classes_edges(cq_classes);
    print_regions_nodes(region, 0);
    print_renamed(f);
  }
}

