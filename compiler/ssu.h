/* -*-Mode: c++;-*-
 Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#ifndef _ssu_h_
#define _ssu_h_

class PNode;
class BracketList;
class Region;
class CQClass;
class UNode;

// internal classes used during SSU conversion
//   UEdge - undirected CFG graph edge for computing Control Regions
//   UNode - undirected CFG graph node for computing Control Regions

class UEdge : public gc {
 public:
  PNode *from, *to;
  Region *entry, *exit;
  CQClass *cq_class;
  int recent_size;
  CQClass *recent_class;
  SLink<UEdge> backedges_link;   
  SLink<UEdge> capping_link;   
  SLink<UEdge> dfs_children_link;   
  Link<UEdge> bracket_link;

  PNode *other(PNode *e) { return (e == to) ? from : to; }
  inline UNode *other(UNode *e);

  UEdge(PNode *afrom, PNode *ato) : from(afrom), to(ato), entry(0), exit(0), cq_class(0), 
    recent_size(0), recent_class(0) {}
};
#define forv_UEdge(_p, _v) forv_Vec(UEdge, _p, _v)
#define forl_UEdge(_p, _l) forl_LL(UEdge, _p, _l)

class CQClass : public gc { 
 public:
  Vec<UEdge *> edges;
  Vec<PNode *> nodes;
};
#define forv_CQClass(_p, _v) forv_Vec(CQClass, _p, _v)

class UNode : public gc {
 public:
  int dfs_num;
  PNode *pn;
  Vec<UEdge *> edges;
  SList(UEdge, backedges_link) backedges;
  SList(UEdge, capping_link) capping;
  UEdge *dfs_parent;
  SList(UEdge, dfs_children_link) dfs_children;
  UNode *hi;
  CountQue(UEdge,bracket_link) blist;

  inline int p();

  UNode(PNode *n) : dfs_num(-1), pn(n), dfs_parent(0), hi(0) {}
};
#define forv_UNode(_p, _v) forv_Vec(UNode, _p, _v)

#endif
