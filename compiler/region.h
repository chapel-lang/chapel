/* -*-Mode: c++;-*-
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#ifndef _region_H_
#define _region_H_

#include "ssu.h"

// Control Region

class Region : public gc {
 public:
  UEdge *entry;
  UEdge *exit;
  Vec<PNode *> nodes;
  Vec<UEdge *> edges;
  Region *parent;
  Vec<Region *> children;

  Region *copy(PNodeMap &map);
  Region(UEdge *aentry, UEdge *aexit) : entry(aentry), exit(aexit), parent(NULL) {}
  Region() : entry(NULL), exit(NULL), parent(NULL) {}
};
#define forv_Region(_p, _v) forv_Vec(Region, _p, _v)

#endif
