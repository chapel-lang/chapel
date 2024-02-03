
/*****
      kdtree.chpl -
      Module holding the kdtree class and kdpoint record declarations.
      Generic in the type of data stored at each point and the dimensionality
      of the space (provide both with the constructor).  First load data into
      the tree, then assemble it - tree cannot be changed afterward.  Not a
      stand-alone program.  'use kdtree'

      Tree construction based on the paper by R. Brown, "Building a Balanced
      k-d Tree in O(kn log n) Time", Journal of Computer Graphics Techniques
      vol 4, no. 1, pp. 50-68.  http://jcgt.org/published/0004/01/03.  Paper
      claims it out-performs the O(n log n) algorithm that uses an O(n)
      median sort of the remaining points at each level of the tree if the
      number of coordinates is small (<= 4).

      Nearest point lookup based on algorithm in "Algorithms in a Nutshell",
      G. Heineman, G. Pollice, and S. Selkow, O'Reilly 2008.  This book does
      have the O(n) median sort, BTW.

      Public Interface:
        Data Structures
        Procedures
          add_node - store a point and data associated with it
          assemble_tree - build tree from stored data
          find_nearest_point - return the data stored at the nearest tree point
          dump - print a table with the tree

      c 2015-2018 Primordial Machine Vision Systems
*****/

use Math;


/**** Constants ****/

param len_insert = 16;                  /* max size for insertion sorting */


/**** Data Structures ****/

record kdpoint {
  type eltType;                         /* payload to store at node */
  param ndim : int;                     /* number of dimensions of points */
  var flags : uint(8);                  /* status flags for node */
  var pt : ndim * int;                  /* points we're storing */
  var val : eltType;                    /* user payload */
}


class kdtree {
  type dataType;                        /* what we're storing at nodes */
  param ndim : int;                     /* number of dimensions of points */
  var Ltree : domain(rank=1);           /* size of array */
  var tree : [Ltree] kdpoint(eltType=dataType,ndim=ndim);
                                        /* the sorted tree */
  var Ldata : domain(rank=1);           /* size of data to put in tree */
  var data : [Ldata] kdpoint(eltType=dataType,ndim=ndim);
                                        /* unsorted data not yet in tree */
  var sortind : [1..ndim][Ldata] int;   /* data index, sorted by coord */
  var nstored : int;                    /* number data points actually added */
  var storable : bool = true;           /* no store data after assembly */


  /* bit masks
       COORD: & mask to get coordinate
       IS_LEAF: is a leaf node, no children
       HAS_L: has a left child
       HAS_R: has a right child
       IS_NIL: node invalid, no data
  */
  /* Note: making these 'param' means that we can't access them from the
     test program. */
  const COORD   : uint(8) = 0x0f;
  const IS_LEAF : uint(8) = 0x10;
  const HAS_L   : uint(8) = 0x20;
  const HAS_R   : uint(8) = 0x40;
  const IS_NIL  : uint(8) = 0x80;

  /***
      init:    Initializer for a tree.  Provide the number of elements you
               want to store and their type and the dimensionality of the
               points used to divide the space.
      args:    nelt - number of elements to store
               dType - data to store at each point
               nDim - number of dimensions to work on
      modifies:  Ltree, Ldata, dataType, ndim
  ***/
  proc init(nelt : int, type dType, param nDim : int) {
    dataType = dType;
    ndim = nDim;

    /* NOTE: If we use COORD instead of its value, the following halts. */
    if (0x0f < ndim) {
      halt("flags not large enough to store coordinate index");
    }

    if (nelt <= 0) then {
      halt("invalid number of elements to store in tree");
    } else if (1 == nelt) {
      /* Make sure we have one left and one right node to mark IS_NIL. */
      Ltree = 1..3;
      Ldata = 1..nelt;
    } else {
      /* Allocate a full tree that can hold all the elements, ie. + 1 for the
         next biggest power-of-two.  log2 in Chapel with an integer argument
         returns the integer part of the log. */
      const size                        /* number of nodes in tree */
        = 2 ** (1 + log2(nelt)) - 1;
      Ltree = 1..size;
      Ldata = 1..nelt;
    }
  }

  /***
      add_node:  Stage a piece of data for sorting later.
      args:      pt - coordinates of data
                 val - value to store at point
      modifies:  data, nstored
  ***/
  proc add_node(pt : ndim * int, val : dataType) {
    if (!storable) then halt("cannot add node after tree has been assembled");

    nstored += 1;
    data(nstored).pt = pt;
    data(nstored).val = val;
  }

  /***
      assemble_tree:  Take the data provided and build a kd-tree with it.
      modifies:  sortind, storable
  ***/
  proc assemble_tree() {

    if (nstored != Ldata.dim(0).high) {
      writeln("warning - not all elements added to tree before assembly");
    }
    if (!storable) then return;

    for param c in 1..ndim {
      for i in Ldata by -1 do sortind(c)(i) = i;
      sort_kdpoints(Ldata.dim(0), c);
    }

    place_node(Ldata.dim(0), 1, 1);

    storable = false;
  }

  /***
      find_nearest_point:  Find the nearest point in the tree to the input and
                           return the payload value that's been stored with it.
      args:                pt - search point
      returns:   value stored at nearest point
  ***/
  proc find_nearest_point(pt : ndim * int) : dataType {
    var parnode : int;                  /* tree node above point (at if ==) */
    var d2min : int;                    /* distance squared between points */
    var match : bool;                   /* true if nearer point found */
    var nearnode : int;                 /* node nearer than parnode */

    if (storable) then halt("tree not yet assembled");

    if traverse_to_point(pt, parnode) then return tree(parnode).val;

    d2min = dist2(pt, tree(parnode).pt);

    if check_nearest(pt, d2min, 1, nearnode) then return tree(nearnode).val;
    return tree(parnode).val;
  }

  /***
      dump - Print a table with the assembled tree.
  ***/
  proc dump() {
    if (storable) then halt("tree not yet generated");

    writeln("\nkd Tree");
    for i in Ldata {
      if (tree(i).flags & IS_NIL) then continue;

      writef("node %3i   c %i   pt %4i", i,
             tree(i).flags & COORD, tree(i).pt(1));
      for param c in 2..ndim do writef(", %4i", tree(i).pt(c));
      if (tree(i).flags & HAS_L) {
        writef("    L %3i", 2*i);
      } else {
        writef("    L ---");
      }
      if (tree(i).flags & HAS_R) {
        writef("  R %3i", 2*i+1);
      } else {
        writef("  R ---");
      }
      writef("\n");
    }
  }


  /**** Private Methods - Assembly ****/

  /***
      sort_kdpoints:  Order the points in the data member by sorting on the
                      coordinate given, then cyclically after that ie. 123,
                      231, 312.
      args:           Lsort - range of array sortind(c) to sort
                      cbase - primary sort coordinate
      modifies:  sortind
  ***/
  proc sort_kdpoints(Lsort : range, cbase : int) {
    var pivot : int;                    /* index of pivot */

    /* A local implementation of quicksort (see ex_quicksort) that allows
       us to use indices to the data and to provide the base coordinate. */
    if (Lsort.first < Lsort.last) {
      if ((Lsort.last - Lsort.first + 1) <= len_insert) {
        insertsort(Lsort, cbase);
      } else {
        pivot = partition(Lsort, cbase);
        cobegin {
          sort_kdpoints(Lsort[..pivot-1], cbase);
          sort_kdpoints(Lsort[pivot+1..], cbase);
        }
      }
    }
  }

  /***
      partition:  Pick a value and shuffle the array so that smaller points
                  are to the left, larger to the right.
      args:       Lsort - range of value in sortind(cbase) to process
                  cbase - primary sort coordinate
      returns:   position where the pivot value ends up
      modifies:  sortind
  ***/
  proc partition(Lsort : range, cbase : int) : int {
    /* Make pivot median of first, last, middle point.  Might as well put
       it in the last position while we're at it. */
    var first = Lsort.first;          /* first point of array */
    const last = Lsort.last;          /* last point of array */
    const midpt = (last + first) / 2; /* midpoint of array */

    if dataind_lt(cbase, last, cbase, first, cbase) then
      sortind(cbase)(first) <=> sortind(cbase)(last);
    if dataind_lt(cbase, midpt, cbase, first, cbase) then
      sortind(cbase)(first) <=> sortind(cbase)(midpt);
    /* Note the smaller point - the median - goes in the last position. */
    if dataind_lt(cbase, midpt, cbase, last, cbase) then
      sortind(cbase)(midpt) <=> sortind(cbase)(last);

    /* Loop will pre-increment first, so reduce by one now. */
    first -= 1;
    for i in (first+1)..(last-1) do
      if dataind_lteq(cbase, i, cbase, last, cbase) {
        first += 1;
        sortind(cbase)(first) <=> sortind(cbase)(i);
      }

    first += 1;
    sortind(cbase)(first) <=> sortind(cbase)(last);

    return first;
  }

  /***
      insertsort: Perform an in-place insertion sort on an array.
      args:       Lsort - sub-array of sortind(cbase) to process
                  cbase - primary sort coordinate
      modifies:  sortind
  ***/
  proc insertsort(Lsort : range, cbase : int) {

    for j in Lsort.first+1..Lsort.last {
      const key = sortind(cbase)(j);  /* value to put in place */
      var i = j - 1;
      while ((i >= Lsort.first) &&
             kdpoint_gt(data(sortind(cbase)(i)), data(key), cbase)) {
        sortind(cbase)(i+1) = sortind(cbase)(i);
        i -= 1;
      }
      sortind(cbase)(i+1) = key;
    }
  }

  /***
      place_node:  Put the median node according to the current primary
                   coordinate into the tree and build its children.
      args:        Lsort - subarray of sortind to process
                   cbase - primary coordinate
                   pos - position to place node in tree
      modifies:  tree, sortind
  ***/
  proc place_node(Lsort : range, cbase : int, pos : int) {
    const left = 2 * pos;               /* left child */
    const right = 2 * pos + 1;          /* right child */
    const first = Lsort.first;
    const last = Lsort.last;
    const midpt = (first + last) / 2;   /* median value in range here */
    const cnext =                       /* cbase for next level */
      if (cbase == ndim) then 1 else (cbase + 1);

    if (3 == Lsort.size) {
      tree(pos) = data(sortind(cbase)(midpt));
      tree(pos).flags = cbase : uint(8) | HAS_L | HAS_R;
      tree(left) = data(sortind(cbase)(first));
      tree(left).flags = cnext : uint(8) | IS_LEAF;
      tree(right) = data(sortind(cbase)(last));
      tree(right).flags = cnext : uint(8) | IS_LEAF;
    } else if (2 == Lsort.size) {
      tree(pos) = data(sortind(cbase)(last));
      tree(pos).flags = cbase : uint(8) | HAS_L;
      tree(left) = data(sortind(cbase)(first));
      tree(left).flags = cnext : uint(8) | IS_LEAF;
      tree(right).flags = IS_NIL;
    } else if (1 == Lsort.size) {
      tree(pos) = data(sortind(cbase)(first));
      tree(pos).flags = cbase : uint(8) | IS_LEAF;
      tree(left).flags = IS_NIL;
      tree(right).flags = IS_NIL;
    } else {
      shuffle_sortind(Lsort, cbase, midpt);

      tree(pos) = data(sortind(cbase)(midpt));
      tree(pos).flags = cbase : uint(8) | HAS_L | HAS_R;
      cobegin {
        place_node(Lsort[..midpt-1], cnext, left);
        place_node(Lsort[midpt+1..], cnext, right);
      }
    }
  }

  /***
      shuffle_sortind:  Re-order the indices for the other coordinates using
                        the base for comparing the points.  This only moves
                        indices above/below the midpoint; the sort order is
                        not touched.  Similar to the partition function for
                        quicksort except midpt is already in place.
      args:             Lsort - subarray of sortind to process
                        cbase - primary coordinate for sorting
                        midpt - fixed point of shuffle
      modifies:  sortind
  ***/
  proc shuffle_sortind(Lsort : range, cbase : int, midpt : int) {
    var lt : [Ldata] bool;              /* points less than midpt */
    var tmp : [Lsort] int;              /* temporary storage for indices */

    /* Each half of Lsort will contain the same indices for all coordinates,
       just in different order.  We only need to compare the points once
       and use the less than flag to avoid having to compare each point
       to the midpoint.  Since the primary coordinate is already ordered,
       it's easy to mark this set. */
    for i in Lsort[..midpt-1] do lt(sortind(cbase)(i)) = true;
    for i in Lsort[midpt..] do lt(sortind(cbase)(i)) = false;

    for param c in 1..ndim {
      /* Primary coordinate already sorted about midpt. */
      if (cbase != c) {
        var inslo = Lsort.first;          /* insertion point below midpt */
        var inshi = midpt + 1;            /* insertion point above midpt */

        for i in Lsort {
          if (sortind(c)(i) == sortind(cbase)(midpt)) {
            tmp(midpt) = sortind(cbase)(midpt);
          } else if lt(sortind(c)(i)) {
            tmp(inslo) = sortind(c)(i);
            inslo += 1;
          } else {
            tmp(inshi) = sortind(c)(i);
            inshi += 1;
          }
        }

        for i in Lsort {
          sortind(c)(i) = tmp(i);
        }
      }
    }
  }


  /**** Private Methods - Search ****/

  /***
      traverse_to_point:  Traverse the tree to the point indicated.  If it
                          does not exist in the tree then return the node,
                          which is its parent were it to be inserted.
      args:               pt - point to look for
                          parent - result of search for point in tree
      returns:   true if point matches node in tree, else false
      modifies:  parent
  ***/
  proc traverse_to_point(pt : ndim * int, out parent : int) : bool {
    var c : uint(8);                    /* coordinate being split at node */
    var node : int;                     /* position in tree */

    node = 1;
    do {
      if (tree(node).pt == pt) {
        parent = node;
        return true;
      }

      if (tree(node).flags & IS_LEAF) {
        parent = node;
        return false;
      }

      if point_lt(pt, tree(node).pt, tree(node).flags & COORD) {
        if !(tree(node).flags & HAS_L) {
          parent = node;
          return false;
        }
        node = 2 * node;
      } else {
        if !(tree(node).flags & HAS_R) {
          parent = node;
          return false;
        }
        node = (2 * node) + 1;
      }
    } while true;

    /* This never gets reached, but the compiler doesn't see it. */
    return false;
  }

  /***
      dist2:  Return the distance squared (ie. no square root) between two
              points.
      args:   pt1, pt2 - points to measure distance between
  ***/
  proc dist2(pt1 : ndim * int, pt2 : ndim * int) : int {
    var d2 : int;                       /* distance squared */

    for param c in 0..(ndim-1) {
      d2 += (pt1(c) - pt2(c)) ** 2;
    }

    return d2;
  }

  /***
      perpdist2:  Return the distance squared between the point and the
                  dividing line of the node, ie. the difference between
                  the primary coordinates only.
      args:       pt - point to find distance of
                  node - node in tree defining plane
  ***/
  proc perpdist2(pt : ndim * int, node : int) : int {
    const cbase                         /* the primary coordinate */
      = (tree(node).flags & COORD) - 1;

    return (pt(cbase) - tree(node).pt(cbase)) ** 2;
  }

  /***
      check_nearer:  Look for a node in the tree that's closer than the
                     distance given.  nearnode will contain garbage if none
                     is found.
      args:          pt - point to find closest node to
                     d2min - smallest distance so far
                     node - node in tree we're checking
                     nearnode - closer node
      returns:   true if a node closer than d2min found, false if not
      modifies:  nearnode
  ***/
  proc check_nearest(pt : ndim * int, in d2min : int, node : int,
                     inout nearnode : int) : bool {
    var d2 : int;                       /* distance from point to node */
    var d2perp : int;                   /* perpendicular dist to node plane */
    var d2neigh : int;                  /* distance to neighboring node */
    var neighbor : int;                 /* nearer node found in neighbor */
    var cbase : int;                    /* primary coordinate at node */

    /* An invalid node number, will also serve as flag for return value. */
    nearnode = -1;

    d2 = dist2(pt, tree(node).pt);
    if (d2 < d2min) {
      nearnode = node;
      d2min = d2;
    }

    d2perp = perpdist2(pt, node);
    if (d2perp < d2min) {
      if (tree(node).flags & HAS_R) {
        if check_nearest(pt, d2min, 2*node+1, neighbor) {
          d2neigh = dist2(pt, tree(neighbor).pt);
          if (d2neigh < d2min) {
            nearnode = neighbor;
            d2min = d2neigh;
          }
        }
      }
      if (tree(node).flags & HAS_L) {
        if check_nearest(pt, d2min, 2*node, neighbor) {
          d2neigh = dist2(pt, tree(neighbor).pt);
          if (d2neigh < d2min) {
            nearnode = neighbor;
            d2min = d2neigh;
          }
        }
      }
    } else {
      cbase = tree(node).flags & COORD;
      if (point_lt(pt, tree(node).pt, cbase)) {
        if (tree(node).flags & HAS_L) {
          if check_nearest(pt, d2min, 2*node, neighbor) then
            nearnode = neighbor;
        }
      } else {
        if (tree(node).flags & HAS_R) {
          if check_nearest(pt, d2min, 2*node+1, neighbor) then
            nearnode = neighbor;
        }
      }
    }

    if (nearnode < 0) then return false;
    else return true;
  }



  /**** Private Methods - Comparison ****/

  /***
      Comparison functions for kdpoint, cyclically comparing coordinates
      starting with the base.  Optional flag noeq toggles error if points
      are equal (this is implied when assemblying the tree but not while
      querying).
      args:  pt1, pt2 - points to compare
             cbase - primary sort coordinate
             noeq - true halts program if pt1 == pt2
  ***/

  inline proc kdpoint_lteq(pt1 : kdpoint, pt2 : kdpoint, cbase : int,
                           noeq = true) : bool {
    return point_lteq(pt1.pt, pt2.pt, cbase, noeq);
  }

  inline proc kdpoint_lt(pt1 : kdpoint, pt2 : kdpoint, cbase : int,
                         noeq = true) : bool {
    return point_lt(pt1.pt, pt2.pt, cbase, noeq);
  }

  inline proc kdpoint_gt(pt1 : kdpoint, pt2 : kdpoint, cbase : int,
                         noeq = true) : bool {
    return point_gt(pt1.pt, pt2.pt, cbase, noeq);
  }

  inline proc kdpoint_gteq(pt1 : kdpoint, pt2 : kdpoint, cbase : int,
                           noeq = true) : bool {
    return point_gteq(pt1.pt, pt2.pt, cbase, noeq);
  }

  proc kdpoint_eq(pt1 : kdpoint, pt2 : kdpoint) : bool {
    return pt1.pt == pt2.pt;
  }


  /***
      Comparison functions for kdpoint via data and sortind, cyclically
      comparing coordinates starting with the base.  Points are looked up
      first in sortind and then data.  Optional flag noeq toggles error if
      points are equal (this is implied when assemblying the tree but not
      while querying).
      args:   c1, ind1 - coordinate and index in sortind for first point
              c2, ind2 - coordinate and index in sortind for second point
              cbase - primary sort coordinate
              noeq - true halts program if pt1 == pt2
  ***/

  inline proc dataind_lteq(c1 : int, ind1 : int, c2 : int, ind2 : int,
                           cbase : int, noeq = true) : bool {
    const pt1 = data(sortind(c1)(ind1)).pt;
    const pt2 = data(sortind(c2)(ind2)).pt;
    return point_lteq(pt1, pt2, cbase, noeq);
  }

  inline proc dataind_lt(c1 : int, ind1 : int, c2 : int, ind2 : int,
                         cbase : int, noeq = true) : bool {
    const pt1 = data(sortind(c1)(ind1)).pt;
    const pt2 = data(sortind(c2)(ind2)).pt;
    return point_lt(pt1, pt2, cbase, noeq);
  }

  inline proc dataind_gt(c1 : int, ind1 : int, c2 : int, ind2 : int,
                         cbase : int, noeq = true) : bool {
    const pt1 = data(sortind(c1)(ind1)).pt;
    const pt2 = data(sortind(c2)(ind2)).pt;
    return point_gt(pt1, pt2, cbase, noeq);
  }

  inline proc dataind_gteq(c1 : int, ind1 : int, c2 : int, ind2 : int,
                           cbase : int, noeq = true) : bool {
    const pt1 = data(sortind(c1)(ind1)).pt;
    const pt2 = data(sortind(c2)(ind2)).pt;
    return point_gteq(pt1, pt2, cbase, noeq);
  }

  proc dataind_eq(c1 : int, ind1 : int, c2 : int, ind2 : int) : bool {
    const pt1 = data(sortind(c1)(ind1));
    const pt2 = data(sortind(c2)(ind2));
    return pt1.pt == pt2.pt;
  }


  /***
      Comparison functions for a point against a kdpoint, cyclically comparing
      coordinates starting with the base.  Optional flag noeq toggles error
      if points are equal.
      args:   pt1, pt2 - point coordinates to compare
              cbase - primary sort coordinate
  ***/

  inline proc point_lteq(pt1 : ndim * int, pt2 : ndim * int, cbase : int,
                         noeq = false) : bool {
    for craw in cbase..(cbase+ndim-1) {
      const coff                        /* cycle through coordinates */
        = if (craw > ndim) then (craw - ndim - 1) else (craw - 1);
      if (pt1(coff) != pt2(coff)) then
        return pt1(coff) <= pt2(coff);
    }
    if (noeq) then halt("kd-tree does not accept duplicate points");
    else return true;
  }

  inline proc point_lt(pt1 : ndim * int, pt2 : ndim * int, cbase : int,
                       noeq = false) : bool {
    for craw in cbase..(cbase+ndim-1) {
      const coff                        /* cycle through coordinates */
        = if (craw > ndim) then (craw - ndim - 1) else (craw - 1);
      if (pt1(coff) != pt2(coff)) then
        return pt1(coff) < pt2(coff);
    }
    if (noeq) then halt("kd-tree does not accept duplicate points");
    else return false;
  }

  inline proc point_gt(pt1 : ndim * int, pt2 : ndim * int, cbase : int,
                       noeq = false) : bool {
    for craw in cbase..(cbase+ndim-1) {
      const coff                        /* cycle through coordinates */
        = if (craw > ndim) then (craw - ndim - 1) else (craw - 1);
      if (pt1(coff) != pt2(coff)) then
        return pt1(coff) > pt2(coff);
    }
    if (noeq) then halt("kd-tree does not accept duplicate points");
    else return false;
  }

  inline proc point_gteq(pt1 : ndim * int, pt2 : ndim * int, cbase : int,
                         noeq = false) : bool {
    for craw in cbase..(cbase+ndim-1) {
      const coff                        /* cycle through coordinates */
        = if (craw > ndim) then (craw - ndim - 1) else (craw - 1);
      if (pt1(coff) != pt2(coff)) then
        return pt1(coff) >= pt2(coff);
    }
    if (noeq) then halt("kd-tree does not accept duplicate points");
    else return true;
  }
}
