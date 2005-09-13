/* -*-Mode: c++;-*-
*/

#ifndef _region_H_
#define _region_H_

class UEdge;
class PNode;

// Control Region

class Region : public gc {
 public:
  UEdge *entry;
  UEdge *exit;
  Vec<PNode *> nodes;
  Vec<UEdge *> edges;
  Region *parent;
  Vec<Region *> children;
  int depth;

  Region *copy(Map<PNode *, PNode *> &map);
  Region(UEdge *aentry, UEdge *aexit) : entry(aentry), exit(aexit), parent(NULL), depth(0) {}
  Region() : entry(NULL), exit(NULL), parent(NULL), depth(0) {}
};
#define forv_Region(_p, _v) forv_Vec(Region, _p, _v)

#endif
