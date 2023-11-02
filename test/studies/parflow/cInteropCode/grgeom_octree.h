/*BHEADER*********************************************************************
 *
 *  Copyright (c) 1995-2009, Lawrence Livermore National Security,
 *  LLC. Produced at the Lawrence Livermore National Laboratory. Written
 *  by the Parflow Team (see the CONTRIBUTORS file)
 *  <parflow@lists.llnl.gov> CODE-OCEC-08-103. All rights reserved.
 *
 *  This file is part of Parflow. For details, see
 *  http://www.llnl.gov/casc/parflow
 *
 *  Please read the COPYRIGHT file or Our Notice and the LICENSE file
 *  for the GNU Lesser General Public License.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License (as published
 *  by the Free Software Foundation) version 2.1 dated February 1999.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the terms
 *  and conditions of the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 *  USA
 **********************************************************************EHEADER*/

#ifndef _GRGEOM_OCTREE_HEADER
#define _GRGEOM_OCTREE_HEADER

/** @file
 * @brief Octree for looping over geometries
 */

/**
 * @brief A node in the octree.
 *
 * Octrees are used to loop over indices in an index space that are on
 * the interior and exterior of a geometric object.  Boundaries of
 * geometries are useful (e.g. setting boundary conditions); the
 * octree represents a single index space around the inside and
 * outside of the geometry bourndary.
 *
 * An octree node is either a leaf node (no children) or a branch
 * node.  Each branch node has 8 children (surprise!)  A leaf node in
 * the octree represents a range of the index space that is all the
 * same state (interior, exterior, inside boundary or outside
 * boundary).
 *
 * A leaf node can have only one of the following flags set:
 *
 *  Flags                   | State of all indices in the index space in the octree node
 *  ----------------------- | ---------------------------------
 *  GrGeomOctreeNodeEmpty   | Exterior to the geometry, not on outside boundary
 *  GrGeomOctreeNodeOutside | Outside boundary
 *  GrGeomOctreeNodeInside  | Inside boundary
 *  GrGeomOctreeNodeFull    | Interior to the geomery, not on inside boundary
 *
 *
 * In most cases the leaf nodes with inside/outside boundary flags
 * will be single points due to the boundary being a single layer.  It
 * will be rare for a cube of 8 points to all lie on a boundary.
 *
 * A branch node can have one or more flags set to indicate the state of
 * children in the subtree.
 *
 *  Flags                | Subtree index space state(s)
 *  -------------------- | ---------------------------------
 *  Empty                | subtree is exterior to the geometry
 *  Empty & Outside      | subtree is exterior and has at least one outside boundary index
 *  Full                 | subtree is interior to the geometry
 *  Full & Inside        | subtree is interior to the geometry and has at least one inside boundary index
 *
 * Branch node flags are used to improve tree searching speed.  For
 * example searching for interior points (Full | Inside boundary) can
 * stop as soon as a branch subtree flag state is marked as Full &
 * Inside.  Searching for inside boundary only will require searching
 * children of a branch node with mixed state to find only the nodes
 * marked as Inside.
 *
 * The following images illustrate the octree label in along a single dimension.
 *
 * \image html Octree-1.png
 *
 * \image html Octree-2.png
 *
 * Spacial accessors are consistent with the labeling in the literature :
 *
 * Short Name | Name  | Description
 * ---------- | ----- | ----------------
 * L          | left  | x coord. small
 * R          | right | x coord. big
 * D          | down  | y coord. small
 * U          | up    | y coord. big
 * B          | back  | z coord. small
 * F          | front | z coord. big
 */
typedef struct grgeom_octree {
  /** State flags for this node */
  unsigned char flags;

  /** Faces for this node */
  unsigned char faces;

  /** Pointer to the parent node */
  struct grgeom_octree   *parent;

  /** Array of pointers to children of this node.  Order of children is
   *  well defined using octant constants */
  struct grgeom_octree  **children;
} GrGeomOctree;

/**
 * @name GrGeomOctreeSizes
 * @brief Octree size constants
 *
 * @{
 */
#define GrGeomOctreeNumFaces        6
#define GrGeomOctreeNumChildren     8
/** @} */

/**
 * @name GrGeomOctreeFlags
 * @brief Octree flag constants.
 *
 * Used to access bits in the GrGeomOctree#flags field.  Indicates states
 * in the subtree or leaf node.
 * @{
 */
#define GrGeomOctreeNodeEmpty     ((unsigned char)0x01)
#define GrGeomOctreeNodeOutside   ((unsigned char)0x02)
#define GrGeomOctreeNodeInside    ((unsigned char)0x04)
#define GrGeomOctreeNodeFull      ((unsigned char)0x08)
#define GrGeomOctreeNodeLeaf      ((unsigned char)0x10)
/** @} */

/**
 * @name GrGeomOctreeFaces
 * @brief Octree face index constants.
 *
 * Used to access index the GrGeomOctree#faces array.
 * @{
 */
#define GrGeomOctreeFaceL 0
#define GrGeomOctreeFaceR 1
#define GrGeomOctreeFaceD 2
#define GrGeomOctreeFaceU 3
#define GrGeomOctreeFaceB 4
#define GrGeomOctreeFaceF 5
/** @} */

/**
 * @name GrGeomOctreeOctants
 * @brief Octree children index constants.
 *
 * Used to access index the GrGeomOctree#children array.
 * @{
 */
#define GrGeomOctreeOctantLDB 0
#define GrGeomOctreeOctantRDB 1
#define GrGeomOctreeOctantLUB 2
#define GrGeomOctreeOctantRUB 3
#define GrGeomOctreeOctantLDF 4
#define GrGeomOctreeOctantRDF 5
#define GrGeomOctreeOctantLUF 6
#define GrGeomOctreeOctantRUF 7
/** @} */

/**
 * @name GrGeomOctreeMisc
 * @brief Miscellaneous Octree constants.
 *
 * @{
 */
#define OUTSIDE 0
#define INSIDE  1
/** @} */

/**
 * @name GrGeomOctreeAccessors
 * @brief GrGeomOctree member accessor macros
 *
 * @param octree the octree node
 * @param i child index
 * @{
 */
#define GrGeomOctreeFlag(octree)      ((octree)->flags)
#define GrGeomOctreeParent(octree)    ((octree)->parent)
#define GrGeomOctreeChildren(octree)  ((octree)->children)
#define GrGeomOctreeChild(octree, i)  ((octree)->children[i])
#define GrGeomOctreeFaces(octree)     ((octree)->faces)
/** @} */

/**
 * @brief Set leaf flag
 *
 * Sets value of a leaf flag to the specified value and overrides
 * previous values.  A leaf flag can only be in one state either
 * Inside, Outside, Full, or Empty.  This method enforces this constraint.
 *
 * @param octree node to set
 * @param flag_value flag to set on node
 */
#define GrGeomOctreeSetLeafFlag(octree, flag_value) \
  GrGeomOctreeFlag(octree) =                        \
    (GrGeomOctreeFlag(octree) & GrGeomOctreeNodeLeaf) | (flag_value)

/**
 * @brief Set branch flag
 *
 * Sets specified branch flag.
 *
 * @param octree node to set
 * @param flag_value flag to set on node
 */
#define GrGeomOctreeSetBranchFlag(octree, flag_value) \
  GrGeomOctreeFlag(octree) =                          \
    (GrGeomOctreeFlag(octree) | (flag_value))

/**
 * @brief Clear branch flag
 *
 * Clear the specified branch flag
 *
 * @param octree node to set
 * @param flag_value flag to set clear node
 */
#define GrGeomOctreeClearBranchFlag(octree, flag_value) \
  GrGeomOctreeFlag(octree) &= ~(flag_value)

/**
 * @brief Set branch node flag value
 *
 * Adds flag to a branch node.  Branch flags may have more than one
 * value set: Inside, Outside, Full, or Empty.
 *
 * @param octree node to set
 * @param flag_value flag to set on node
 */
#define GrGeomOctreeSetBranchNode(octree, flag_value) \
  GrGeomOctreeFlag(octree) |= (flag_value)

/**
 * @brief Clear the branch flag value
 *
 * @param octree node to set
 * @param flag_value flag to clear
 */
#define GrGeomOctreeClearBranchNode(octree, flag_value) \
   GrGeomOctreeFlag(octree) &= ~(flag_value)

/**
 * @brief Set leaf indicator
 *
 * Set flag to be a Leaf flag.   A flag may be either a leaf or branch flag.
 *
 * @param octree node to make a leaf
 */
#define GrGeomOctreeSetNodeLeaf(octree) \
  (GrGeomOctreeFlag(octree) |= GrGeomOctreeNodeLeaf)

/**
 * @brief Clear leaf indicator and set to branch flag.
 *
 * Set flag to be a branch flag.   A flag may be either a leaf or branch flag.
 *
 * @param octree node to make a branch.
 */
#define GrGeomOctreeClearNodeLeaf(octree) \
  (GrGeomOctreeFlag(octree) &= ~GrGeomOctreeNodeLeaf)

/**
 * @name GrGeomOctreeFlagAccessors
 *
 * @brief Face accessors
 *
 * @param octree node to set/inspect
 * @param face_index face index
 */
#define GrGeomOctreeSetFace(octree, face_index) \
  (GrGeomOctreeFaces(octree) |= GrGeomOctreeFaceValue(face_index))
#define GrGeomOctreeFaceValue(face_index) \
  (((unsigned char)0x01) << face_index)
#define GrGeomOctreeClearFace(octree, face_index) \
  (GrGeomOctreeFaces(octree) &= ~GrGeomOctreeFaceValue(face_index))
/** @} */

/**
 * @brief Return face index from offsets.
 *
 * Compute the face index based on the supplied offsets.
 *
 * @param i offset in x (-1, 1)
 * @param j offset in y (-1, 1)
 * @param k offset in z (-1, 1)
 * @return face index
 */
#define GrGeomOctreeFaceIndex(i, j, k) \
  (int)(((i * (2 * i + 1)) + (j * (6 * j + 1)) + (k * (10 * k + 1))) / 2)

/**
 * @name GrGeomOctreeStateTests
 * @brief Test state of node
 *
 * @param octree node to inspect
 * @param flag_value flag to return
 * @param face_index Face index, should use defined constant
 * @return boolean indicating state of the octree node
 * @{
 */
#define GrGeomOctreeFlagIs(octree, flag_value) \
  ((GrGeomOctreeFlag(octree) & flag_value) == flag_value)
#define GrGeomOctreeNodeIsEmpty(octree) \
  GrGeomOctreeFlagIs(octree, GrGeomOctreeNodeEmpty)
#define GrGeomOctreeNodeIsOutside(octree) \
  GrGeomOctreeFlagIs(octree, GrGeomOctreeNodeOutside)
#define GrGeomOctreeNodeIsInside(octree) \
  GrGeomOctreeFlagIs(octree, GrGeomOctreeNodeInside)
#define GrGeomOctreeNodeIsFull(octree) \
  GrGeomOctreeFlagIs(octree, GrGeomOctreeNodeFull)
#define GrGeomOctreeNodeIsLeaf(octree) \
  GrGeomOctreeFlagIs(octree, GrGeomOctreeNodeLeaf)
#define GrGeomOctreeHasChildren(octree) \
  (GrGeomOctreeChildren(octree) != NULL)
#define GrGeomOctreeHasNoChildren(octree) \
  (GrGeomOctreeChildren(octree) == NULL)
#define GrGeomOctreeHasChild(octree, i) \
  (GrGeomOctreeChild(octree, i) != NULL)
#define GrGeomOctreeHasNoChild(octree, i) \
  (GrGeomOctreeChild(octree, i) == NULL)
#define GrGeomOctreeHasFaces(octree) \
  (GrGeomOctreeFaces(octree) != ((unsigned char)0x00))
#define GrGeomOctreeHasNoFaces(octree) \
  (GrGeomOctreeFaces(octree) == ((unsigned char)0x00))
#define GrGeomOctreeHasFace(octree, face_index)                       \
  ((GrGeomOctreeFaces(octree) & GrGeomOctreeFaceValue(face_index)) != \
   ((unsigned char)0x00))
#define GrGeomOctreeHasNoFace(octree, face_index)                     \
  ((GrGeomOctreeFaces(octree) & GrGeomOctreeFaceValue(face_index)) == \
   ((unsigned char)0x00))
/** @} */

/**
 * @brief Internal generic octree looping
 *
 * @note Do not call directly!  Not intended for use code.
 *
 * Basic Octree looping macro.  Octree is traversed executing the
 * level_body when a node is at the supplied level of interest and
 * executing the leaf_body when a leaf node is reached.  value_test is
 * evaluated to determine if the bodies should be executed.
 *
 * i,j,k should be initialized to the starting indices and modified as the
 * tree is traversed to the starting offset of the current octree node.
 *
 * The current level is maintained in the l parmeter.
 *
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] l current octree level
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level level of interest to limit tree traversal
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] level_body code to execute if level has been reached.
 * @param[in] leaf_body code to execute for leaf nodes
 */
#define GrGeomOctreeLoop(i, j, k, l, node, octree, level, value_test, \
                         level_body, leaf_body)                       \
  {                                                                   \
    int PV_level = level;                                             \
    unsigned int PV_inc;                                              \
    int           *PV_visiting;                                       \
    int PV_visit_child;                                               \
                                                                      \
                                                                      \
    node = octree;                                                    \
                                                                      \
    l = 0;                                                            \
    PV_inc = 1 << PV_level;                                           \
    PV_visiting = ctalloc(int, PV_level + 2);                         \
    PV_visiting++;                                                    \
    PV_visiting[0] = 0;                                               \
                                                                      \
    while (l >= 0)                                                    \
    {                                                                 \
      /* if at the level of interest */                               \
      if (l == PV_level)                                              \
      {                                                               \
        if (value_test)                                               \
          level_body;                                                 \
                                                                      \
        PV_visit_child = FALSE;                                       \
      }                                                               \
                                                                      \
      /* if this is a leaf node */                                    \
      else if (GrGeomOctreeNodeIsLeaf(node))                          \
      {                                                               \
        if (value_test)                                               \
          leaf_body;                                                  \
                                                                      \
        PV_visit_child = FALSE;                                       \
      }                                                               \
                                                                      \
      /* have I visited all of the children? */                       \
      else if (PV_visiting[l] < GrGeomOctreeNumChildren)              \
        PV_visit_child = TRUE;                                        \
      else                                                            \
        PV_visit_child = FALSE;                                       \
                                                                      \
      /* visit either a child or the parent node */                   \
      if (PV_visit_child)                                             \
      {                                                               \
        node = GrGeomOctreeChild(node, PV_visiting[l]);               \
        PV_inc = PV_inc >> 1;                                         \
        i += (int)(PV_inc) * ((PV_visiting[l] & 1) ? 1 : 0);          \
        j += (int)(PV_inc) * ((PV_visiting[l] & 2) ? 1 : 0);          \
        k += (int)(PV_inc) * ((PV_visiting[l] & 4) ? 1 : 0);          \
        l++;                                                          \
        PV_visiting[l] = 0;                                           \
      }                                                               \
      else                                                            \
      {                                                               \
        l--;                                                          \
        i -= (int)(PV_inc) * ((PV_visiting[l] & 1) ? 1 : 0);          \
        j -= (int)(PV_inc) * ((PV_visiting[l] & 2) ? 1 : 0);          \
        k -= (int)(PV_inc) * ((PV_visiting[l] & 4) ? 1 : 0);          \
        PV_inc = PV_inc << 1;                                         \
        node = GrGeomOctreeParent(node);                              \
        PV_visiting[l]++;                                             \
      }                                                               \
    }                                                                 \
                                                                      \
    tfree(PV_visiting - 1);                                           \
  }

/**
 * @brief Internal generic octree looping
 *
 * @note Do not call directly!  Not intended for use code.
 *
 * Basic Octree looping template macro for interiror and exterior
 * looping.  Traversal is stopped when the traversal_stop_test
 * expression is true.  body_test is evaluated before the code block
 * is exectued on leaf nodes or branch nodes that meet the traversal_stop_test
 * criteria.
 *
 * value_test is evaluated to determine if the bodies should be
 * executed.
 *
 * i,j,k should be initialized to the starting indices and modified as the
 * tree is traversed to the starting offset of the current octree node.
 *
 * The current level is maintained in the l parmeter.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] l current octree level
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level level of interest to limit tree traversal
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] level_body code to execute if level has been reached.
 * @param[in] code_body code to execute for leaf nodes or branch nodes
 * @param[in] traversal_stop_test boolean expression evalutated to determine if a branch node should terminate traversal
 *  @param[in] body_test boolean expression evalutated to determine if code_body should be executed.
 */
#define GrGeomOctreeLoopTemplate(i, j, k, l, node, octree, level, value_test,           \
                                 level_body, code_body, traversal_stop_test, body_test) \
  {                                                                                     \
    int PV_level = level;                                                               \
    unsigned int PV_inc;                                                                \
    int           *PV_visiting;                                                         \
    int PV_visit_child;                                                                 \
                                                                                        \
    node = octree;                                                                      \
                                                                                        \
    l = 0;                                                                              \
    PV_inc = 1 << PV_level;                                                             \
    PV_visiting = ctalloc(int, PV_level + 2);                                           \
    PV_visiting++;                                                                      \
    PV_visiting[0] = 0;                                                                 \
                                                                                        \
    while (l >= 0)                                                                      \
    {                                                                                   \
      /* if at the level of interest */                                                 \
      if (l == PV_level)                                                                \
      {                                                                                 \
        if ((body_test) && (value_test))                                                \
          level_body;                                                                   \
                                                                                        \
        PV_visit_child = FALSE;                                                         \
      }                                                                                 \
                                                                                        \
      /* if this is a full node or a leaf node */                                       \
      else if (GrGeomOctreeNodeIsLeaf(node) || (traversal_stop_test))                   \
      {                                                                                 \
        if ((body_test) && (value_test))                                                \
          code_body;                                                                    \
                                                                                        \
        PV_visit_child = FALSE;                                                         \
      }                                                                                 \
                                                                                        \
      /* have I visited all of the children? */                                         \
      else if (PV_visiting[l] < GrGeomOctreeNumChildren)                                \
        PV_visit_child = TRUE;                                                          \
      else                                                                              \
        PV_visit_child = FALSE;                                                         \
                                                                                        \
      /* visit either a child or the parent node */                                     \
      if (PV_visit_child)                                                               \
      {                                                                                 \
        node = GrGeomOctreeChild(node, PV_visiting[l]);                                 \
        PV_inc = PV_inc >> 1;                                                           \
        i += (int)(PV_inc) * ((PV_visiting[l] & 1) ? 1 : 0);                            \
        j += (int)(PV_inc) * ((PV_visiting[l] & 2) ? 1 : 0);                            \
        k += (int)(PV_inc) * ((PV_visiting[l] & 4) ? 1 : 0);                            \
        l++;                                                                            \
        PV_visiting[l] = 0;                                                             \
      }                                                                                 \
      else                                                                              \
      {                                                                                 \
        l--;                                                                            \
        i -= (int)(PV_inc) * ((PV_visiting[l] & 1) ? 1 : 0);                            \
        j -= (int)(PV_inc) * ((PV_visiting[l] & 2) ? 1 : 0);                            \
        k -= (int)(PV_inc) * ((PV_visiting[l] & 4) ? 1 : 0);                            \
        PV_inc = PV_inc << 1;                                                           \
        node = GrGeomOctreeParent(node);                                                \
        PV_visiting[l]++;                                                               \
      }                                                                                 \
    }                                                                                   \
                                                                                        \
    tfree(PV_visiting - 1);                                                             \
  }


/**
 * @brief Internal use generic octree looping for interior indices only.
 *
 * @note Do not call directly!  Not intended for use code.
 *
 * Basic Octree looping macro for interior (Full and Inside) indices
 * only.  This is more efficient than the GrGeomOctreeLoop looping.
 * It will stop the octree traversel at nodes that are fully in the interior
 * without distingishing inside indices.
 *
 * Octree is traversed executing the level_body when a node is at the
 * supplied level of interest and executing the code_body when a node
 * with points in the interior is reached.  value_test is evaluated to
 * determine if the bodies should be executed.
 *
 * i,j,k should be initialized to the starting indices and modified as the
 * tree is traversed to the starting offset of the current octree node.
 *
 * The current level is maintained in the l parmeter.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] l current octree level
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level level of interest to limit tree traversal
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] level_body code to execute if level has been reached.
 * @param[in] code_body code to execute for interior nodes
 */
#define GrGeomOctreeInteriorLoop(i, j, k, l, node, octree, level, value_test,                   \
                                 level_body, code_body)                                         \
  {                                                                                             \
    GrGeomOctreeLoopTemplate(i, j, k, l, node, octree, level, value_test,                       \
                             level_body, code_body,                                             \
                             GrGeomOctreeNodeIsFull(node),                                      \
                             (GrGeomOctreeNodeIsInside(node) || GrGeomOctreeNodeIsFull(node))); \
  }

/**
 * @brief Internal use generic octree looping for Exteriror indices only.
 *
 * @note Do not call directly!  Not intended for use code.
 *
 * Basic Octree looping macro for exteriorr (Empty and Outside) indices
 * only.  This is more efficient than the GrGeomOctreeLoop looping.
 * It will stop the octree traversel at nodes that are fully in the interior
 * without distingishing inside indices.
 *
 * Octree is traversed executing the level_body when a node is at the
 * supplied level of interest and executing the code_body when a node
 * with points in in interior is reached.  value_test is evaluated to
 * determine if the bodies should be executed.
 *
 * i,j,k should be initialized to the starting indices and modified as the
 * tree is traversed to the starting offset of the current octree node.
 *
 * The current level is maintained in the l parmeter.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] l current octree level
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level level of interest to limit tree traversal
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] level_body code to execute if level has been reached.
 * @param[in] code_body code to execute for exterior nodes
 */
#define GrGeomOctreeExteriorLoop(i, j, k, l, node, octree, level, value_test,                     \
                                 level_body, code_body)                                           \
  {                                                                                               \
    GrGeomOctreeLoopTemplate(i, j, k, l, node, octree, level, value_test,                         \
                             level_body, code_body,                                               \
                             GrGeomOctreeNodeIsEmpty(node),                                       \
                             (GrGeomOctreeNodeIsOutside(node) || GrGeomOctreeNodeIsEmpty(node))); \
  }

/**
 * @brief Internal use generic octree looping for Inside indices only.
 *
 * @note Do not call directly!  Not intended for use code.
 *
 * Basic Octree looping macro for Inside indices
 * only.  This is more efficient than the GrGeomOctreeLoop looping.
 * It will stop the octree traversel at nodes that do not contain
 * any Inside indices.
 *
 * Octree is traversed executing the level_body when a node is at the
 * supplied level of interest and executing the code_body when a node
 * with points in in interior is reached.  value_test is evaluated to
 * determine if the bodies should be executed.
 *
 * i,j,k should be initialized to the starting indices and modified as the
 * tree is traversed to the starting offset of the current octree node.
 *
 * The current level is maintained in the l parmeter.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] l current octree level
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level level of interest to limit tree traversal
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] level_body code to execute if level has been reached.
 * @param[in] code_body code to execute for inside nodes
 */
#define GrGeomOctreeInsideLoop(i, j, k, l, node, octree, level, value_test, \
                               level_body, code_body)                       \
  {                                                                         \
    fprintf(stdout, "OctreeInsideLoop\n");\
    int PV_level = level;                                                   \
    unsigned int PV_inc;                                                    \
    int           *PV_visiting;                                             \
    int PV_visit_child;                                                     \
                                                                            \
    node = octree;                                                          \
                                                                            \
    l = 0;                                                                  \
    PV_inc = 1 << PV_level;                                                 \
    PV_visiting = ctalloc(int, PV_level + 2);                               \
    PV_visiting++;                                                          \
    PV_visiting[0] = 0;                                                     \
                                                                            \
    while (l >= 0)                                                          \
    {                                                                       \
      /* if at the level of interest */                                     \
      if (l == PV_level)                                                    \
      {                                                                     \
        if ((GrGeomOctreeNodeIsInside(node)) && (value_test))               \
          level_body;                                                       \
                                                                            \
        PV_visit_child = FALSE;                                             \
      }                                                                     \
                                                                            \
      /* if this is a full node or a leaf node */                           \
      else if (GrGeomOctreeNodeIsLeaf(node))                                \
      {                                                                     \
        if (GrGeomOctreeNodeIsInside(node) && (value_test))                 \
          code_body;                                                        \
                                                                            \
        PV_visit_child = FALSE;                                             \
      }                                                                     \
                                                                            \
      /* Branch node and not inside, don't need to continue traversal */    \
      else if (!GrGeomOctreeNodeIsInside(node))                             \
      {                                                                     \
        PV_visit_child = FALSE;                                             \
      }                                                                     \
                                                                            \
      /* have I visited all of the children? */                             \
      else if (PV_visiting[l] < GrGeomOctreeNumChildren)                    \
        PV_visit_child = TRUE;                                              \
      else                                                                  \
        PV_visit_child = FALSE;                                             \
                                                                            \
      /* visit either a child or the parent node */                         \
      if (PV_visit_child)                                                   \
      {                                                                     \
        node = GrGeomOctreeChild(node, PV_visiting[l]);                     \
        PV_inc = PV_inc >> 1;                                               \
        i += (int)(PV_inc) * ((PV_visiting[l] & 1) ? 1 : 0);                \
        j += (int)(PV_inc) * ((PV_visiting[l] & 2) ? 1 : 0);                \
        k += (int)(PV_inc) * ((PV_visiting[l] & 4) ? 1 : 0);                \
        l++;                                                                \
        PV_visiting[l] = 0;                                                 \
      }                                                                     \
      else                                                                  \
      {                                                                     \
        l--;                                                                \
        i -= (int)(PV_inc) * ((PV_visiting[l] & 1) ? 1 : 0);                \
        j -= (int)(PV_inc) * ((PV_visiting[l] & 2) ? 1 : 0);                \
        k -= (int)(PV_inc) * ((PV_visiting[l] & 4) ? 1 : 0);                \
        PV_inc = PV_inc << 1;                                               \
        node = GrGeomOctreeParent(node);                                    \
        PV_visiting[l]++;                                                   \
      }                                                                     \
    }                                                                       \
                                                                            \
    tfree(PV_visiting - 1);                                                 \
  }

/**
 * @brief Loop over the interior index space of an octree.
 *
 * General octree macro to loop over the index space (i,j,k values) in the octree.
 * Octree is traversed executing the code specified in body for
 * indices at the supplied level of interest or indices leaf nodes.
 * value_test is evaluated to determine if the body should be
 * executed.
 *
 * i,j,k should be initialized to the starting indices and will
 * modified as the tree is traversed to the starting offset of the
 * current octree node.
 *
 * ix,iy,iz and nx,ny,nz specify a bounding box to constrain the the iteration.
 *
 * The current level is maintained in the l parmeter.
 *
 * @note Level of interest doesn't seem to make sense.  The body will
 * be executed for the first point (i,j,k) in the octree nodes at the
 * level of interest.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level level to limit tree traversal
 * @param[in] ix x lower bound of index space to process
 * @param[in] iy y lower bound of index space to process
 * @param[in] iz z lower bound of index space to process
 * @param[in] nx x upper bound of index space to process
 * @param[in] ny y upper bound of index space to process
 * @param[in] nz z upper bound of index space to process
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] body code to execute
 */
#define GrGeomOctreeInteriorNodeLoop(i, j, k, node, octree, level,                    \
                                     ix, iy, iz, nx, ny, nz, value_test,              \
                                     body)                                            \
  {                                                                                   \
    int PV_i, PV_j, PV_k, PV_l;                                                       \
    int PV_ixl, PV_iyl, PV_izl, PV_ixu, PV_iyu, PV_izu;                               \
                                                                                      \
                                                                                      \
    PV_i = i;                                                                         \
    PV_j = j;                                                                         \
    PV_k = k;                                                                         \
                                                                                      \
    GrGeomOctreeInteriorLoop(PV_i, PV_j, PV_k, PV_l, node, octree, level, value_test, \
    {                                                                                 \
      if ((PV_i >= ix) && (PV_i < (ix + nx)) &&                                       \
          (PV_j >= iy) && (PV_j < (iy + ny)) &&                                       \
          (PV_k >= iz) && (PV_k < (iz + nz)))                                         \
      {                                                                               \
        i = PV_i;                                                                     \
        j = PV_j;                                                                     \
        k = PV_k;                                                                     \
        body;                                                                         \
      }                                                                               \
    },                                                                                \
    {                                                                                 \
      /* find octree and region intersection */                                       \
      PV_ixl = pfmax(ix, PV_i);                                                       \
      PV_iyl = pfmax(iy, PV_j);                                                       \
      PV_izl = pfmax(iz, PV_k);                                                       \
      PV_ixu = pfmin((ix + nx), (PV_i + (int)PV_inc));                                \
      PV_iyu = pfmin((iy + ny), (PV_j + (int)PV_inc));                                \
      PV_izu = pfmin((iz + nz), (PV_k + (int)PV_inc));                                \
                                                                                      \
      /* loop over indexes and execute the body */                                    \
      for (k = PV_izl; k < PV_izu; k++)                                               \
        for (j = PV_iyl; j < PV_iyu; j++)                                             \
          for (i = PV_ixl; i < PV_ixu; i++)                                           \
          {                                                                           \
            body;                                                                     \
          }                                                                           \
    })                                                                                \
  }


/**
 * @brief Loop over the exterior index space of an octree.
 *
 * General octree macro to loop over the index space (i,j,k values) in the octree.
 * Octree is traversed executing the code specified in body for
 * indices at the supplied level of interest or indices leaf nodes.
 * value_test is evaluated to determine if the body should be
 * executed.
 *
 * i,j,k should be initialized to the starting indices and will
 * modified as the tree is traversed to the starting offset of the
 * current octree node.
 *
 * ix,iy,iz and nx,ny,nz specify a bounding box to constrain the the iteration.
 *
 * The current level is maintained in the l parmeter.
 *
 * @note Level of interest doesn't seem to make sense.  The body will
 * be executed for the first point (i,j,k) in the octree nodes at the
 * level of interest.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level level to limit tree traversal
 * @param[in] ix x lower bound of index space to process
 * @param[in] iy y lower bound of index space to process
 * @param[in] iz z lower bound of index space to process
 * @param[in] nx x upper bound of index space to process
 * @param[in] ny y upper bound of index space to process
 * @param[in] nz z upper bound of index space to process
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] body code to execute
 */
#define GrGeomOctreeExteriorNodeLoop_default(i, j, k, node, octree, level,            \
                                     ix, iy, iz, nx, ny, nz, value_test,              \
                                     body)                                            \
  {                                                                                   \
    int PV_i, PV_j, PV_k, PV_l;                                                       \
    int PV_ixl, PV_iyl, PV_izl, PV_ixu, PV_iyu, PV_izu;                               \
                                                                                      \
                                                                                      \
    PV_i = i;                                                                         \
    PV_j = j;                                                                         \
    PV_k = k;                                                                         \
                                                                                      \
    GrGeomOctreeExteriorLoop(PV_i, PV_j, PV_k, PV_l, node, octree, level, value_test, \
    {                                                                                 \
      if ((PV_i >= ix) && (PV_i < (ix + nx)) &&                                       \
          (PV_j >= iy) && (PV_j < (iy + ny)) &&                                       \
          (PV_k >= iz) && (PV_k < (iz + nz)))                                         \
      {                                                                               \
        i = PV_i;                                                                     \
        j = PV_j;                                                                     \
        k = PV_k;                                                                     \
        body;                                                                         \
      }                                                                               \
    },                                                                                \
    {                                                                                 \
      /* find octree and region intersection */                                       \
      PV_ixl = pfmax(ix, PV_i);                                                       \
      PV_iyl = pfmax(iy, PV_j);                                                       \
      PV_izl = pfmax(iz, PV_k);                                                       \
      PV_ixu = pfmin((ix + nx), (PV_i + (int)PV_inc));                                \
      PV_iyu = pfmin((iy + ny), (PV_j + (int)PV_inc));                                \
      PV_izu = pfmin((iz + nz), (PV_k + (int)PV_inc));                                \
                                                                                      \
      /* loop over indexes and execute the body */                                    \
      for (k = PV_izl; k < PV_izu; k++)                                               \
        for (j = PV_iyl; j < PV_iyu; j++)                                             \
          for (i = PV_ixl; i < PV_ixu; i++)                                           \
          {                                                                           \
            body;                                                                     \
          }                                                                           \
    })                                                                                \
  }

/**
 * @brief Loop over the inside index space of an octree.
 *
 * General octree macro to loop over the index space (i,j,k values) in the octree.
 * Octree is traversed executing the code specified in body for
 * indices at the supplied level of interest or indices leaf nodes.
 * value_test is evaluated to determine if the body should be
 * executed.
 *
 * i,j,k should be initialized to the starting indices and will
 * modified as the tree is traversed to the starting offset of the
 * current octree node.
 *
 * ix,iy,iz and nx,ny,nz specify a bounding box to constrain the the iteration.
 *
 * The current level is maintained in the l parmeter.
 *
 * @note Level of interest doesn't seem to make sense.  The body will
 * be executed for the first point (i,j,k) in the octree nodes at the
 * level of interest.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level level to limit tree traversal
 * @param[in] ix x lower bound of index space to process
 * @param[in] iy y lower bound of index space to process
 * @param[in] iz z lower bound of index space to process
 * @param[in] nx x upper bound of index space to process
 * @param[in] ny y upper bound of index space to process
 * @param[in] nz z upper bound of index space to process
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] body code to execute
 */
#define GrGeomOctreeInsideNodeLoop(i, j, k, node, octree, level,                    \
                                   ix, iy, iz, nx, ny, nz, value_test,              \
                                   body)                                            \
  {                                                                                 \
    int PV_i, PV_j, PV_k, PV_l;                                                     \
    int PV_ixl, PV_iyl, PV_izl, PV_ixu, PV_iyu, PV_izu;                             \
                                                                                    \
                                                                                    \
    PV_i = i;                                                                       \
    PV_j = j;                                                                       \
    PV_k = k;                                                                       \
                                                                                    \
    GrGeomOctreeInsideLoop(PV_i, PV_j, PV_k, PV_l, node, octree, level, value_test, \
    {                                                                               \
      if ((PV_i >= ix) && (PV_i < (ix + nx)) &&                                     \
          (PV_j >= iy) && (PV_j < (iy + ny)) &&                                     \
          (PV_k >= iz) && (PV_k < (iz + nz)))                                       \
      {                                                                             \
        i = PV_i;                                                                   \
        j = PV_j;                                                                   \
        k = PV_k;                                                                   \
        body;                                                                       \
      }                                                                             \
    },                                                                              \
    {                                                                               \
      /* find octree and region intersection */                                     \
      PV_ixl = pfmax(ix, PV_i);                                                     \
      PV_iyl = pfmax(iy, PV_j);                                                     \
      PV_izl = pfmax(iz, PV_k);                                                     \
      PV_ixu = pfmin((ix + nx), (PV_i + (int)PV_inc));                              \
      PV_iyu = pfmin((iy + ny), (PV_j + (int)PV_inc));                              \
      PV_izu = pfmin((iz + nz), (PV_k + (int)PV_inc));                              \
                                                                                    \
      /* loop over indexes and execute the body */                                  \
      for (k = PV_izl; k < PV_izu; k++)                                             \
        for (j = PV_iyl; j < PV_iyu; j++)                                           \
          for (i = PV_ixl; i < PV_ixu; i++)                                         \
          {                                                                         \
            body;                                                                   \
          }                                                                         \
    })                                                                              \
  }

/**
 * @brief Loop over the index space of an octree with strides.
 *
 * Same looping as GrGeomOctreeNodeLoop but with non-unitary strides
 * specified by sx, sy, sz
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level level to limit tree traversal
 * @param[in] ix x lower bound of index space to process
 * @param[in] iy y lower bound of index space to process
 * @param[in] iz z lower bound of index space to process
 * @param[in] nx x upper bound of index space to process
 * @param[in] ny y upper bound of index space to process
 * @param[in] nz z upper bound of index space to process
 * @param[in] sx x stride
 * @param[in] sy y stride
 * @param[in] sz z stride
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] body code to execute for leaf nodes or nodes at the specified level
 */
#define GrGeomOctreeNodeLoop2(i, j, k, node, octree, level,                   \
                              ix, iy, iz, nx, ny, nz, sx, sy, sz, value_test, \
                              body)                                           \
  {                                                                           \
    int PV_i, PV_j, PV_k, PV_l;                                               \
    int PV_ixl, PV_iyl, PV_izl, PV_ixu, PV_iyu, PV_izu;                       \
                                                                              \
                                                                              \
    PV_i = i;                                                                 \
    PV_j = j;                                                                 \
    PV_k = k;                                                                 \
                                                                              \
    GrGeomOctreeLoop(PV_i, PV_j, PV_k, PV_l, node, octree, level, value_test, \
    {                                                                         \
      if ((PV_i >= ix) && (PV_i < (ix + nx)) &&                               \
          (PV_j >= iy) && (PV_j < (iy + ny)) &&                               \
          (PV_k >= iz) && (PV_k < (iz + nz)) &&                               \
          ((PV_i - ix) % sx == 0) &&                                          \
          ((PV_j - iy) % sy == 0) &&                                          \
          ((PV_k - iz) % sz == 0))                                            \
      {                                                                       \
        i = PV_i;                                                             \
        j = PV_j;                                                             \
        k = PV_k;                                                             \
                                                                              \
        body;                                                                 \
      }                                                                       \
    },                                                                        \
    {                                                                         \
      /* find octree and region intersection */                               \
      PV_ixl = pfmax(ix, PV_i);                                               \
      PV_iyl = pfmax(iy, PV_j);                                               \
      PV_izl = pfmax(iz, PV_k);                                               \
      PV_ixu = pfmin((ix + nx), (PV_i + (int)PV_inc));                        \
      PV_iyu = pfmin((iy + ny), (PV_j + (int)PV_inc));                        \
      PV_izu = pfmin((iz + nz), (PV_k + (int)PV_inc));                        \
                                                                              \
      /* project intersection onto strided index space */                     \
      PV_ixl = PV_ixl + ix;                                                   \
      PV_ixu = PV_ixu + ix;                                                   \
      PV_ixl = ((int)((PV_ixl + (sx - 1)) / sx)) * sx - ix;                   \
      PV_ixu = ((int)((PV_ixu + (sx - 1)) / sx)) * sx - ix;                   \
      PV_iyl = PV_iyl + iy;                                                   \
      PV_iyu = PV_iyu + iy;                                                   \
      PV_iyl = ((int)((PV_iyl + (sy - 1)) / sy)) * sy - iy;                   \
      PV_iyu = ((int)((PV_iyu + (sy - 1)) / sy)) * sy - iy;                   \
      PV_izl = PV_izl + iz;                                                   \
      PV_izu = PV_izu + iz;                                                   \
      PV_izl = ((int)((PV_izl + (sz - 1)) / sz)) * sz - iz;                   \
      PV_izu = ((int)((PV_izu + (sz - 1)) / sz)) * sz - iz;                   \
                                                                              \
      /* loop over indexes and execute the body */                            \
      for (k = PV_izl; k < PV_izu; k += sz)                                   \
        for (j = PV_iyl; j < PV_iyu; j += sy)                                 \
          for (i = PV_ixl; i < PV_ixu; i += sx)                               \
          {                                                                   \
            body;                                                             \
          }                                                                   \
    })                                                                        \
  }

// \todo SGS 2019/04/18 Currently faces are stored on single point leaf nodes.
// Would be better to store planes.

//  \todo SGS 12/3/2008 can optimize fdir by using 1 assignment to static.  Should
// elimiate 2 assignment statements and switch and replace with table:
// fdir = FDIR[PV_f] type of thing.
//

/**
 * @brief Loop over the faces of an octree.
 *
 * General octree macro to loop over the faces in the octree.  Octree
 * is traversed executing the code specified in body for indices at
 * the supplied level interest or in leaf nodes.  value_test is
 * evaluated to determine if the body should be executed.
 *
 * General octree face looping macro.  Octree is traversed executing the
 * code specified in body for indices at the supplied level or in leaf
 * nodes.
 *
 * i,j,k should be initialized to the starting indices and will
 * modified as the tree is traversed to the starting offset of the
 * current octree node.
 *
 * The fdir int[3] array indicates the face direction.  Array is
 * indexed by 0 (x), 1 (y), or 2 (z).  Value is -1 for face on left
 * side of the cell, 0 for no face, and 1 for right face on the cell.
 * The array will have only one face set at a time but the same index
 * point may be iterated over multiple times if the cell has multiple
 * faces.
 *
 * ix,iy,iz and nx,ny,nz specify a bounding box for the iteration
 * for cases when the entire index space should not be iterated.
 *
 * The current level is maintained in the l parmeter.
 *
 * @note level_of_intest doesn't make a lot of sense, only leaf nodes
 * have faces.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] fdir face direction array
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] level_of_interest level to limit tree traversal
 * @param[in] ix x lower bound of index space to process
 * @param[in] iy y lower bound of index space to process
 * @param[in] iz z lower bound of index space to process
 * @param[in] nx x upper bound of index space to process
 * @param[in] ny y upper bound of index space to process
 * @param[in] nz z upper bound of index space to process
 * @param[in] body code to execute
 */
#define GrGeomOctreeFaceLoop(i, j, k, fdir, node, octree, level_of_interest, \
                             ix, iy, iz, nx, ny, nz, body)                   \
  {                                                                          \
    int PV_f;                                                                \
    int PV_fdir[3];                                                          \
                                                                             \
                                                                             \
    fdir = PV_fdir;                                                          \
    GrGeomOctreeInsideNodeLoop(i, j, k, node, octree, level_of_interest,     \
                               ix, iy, iz, nx, ny, nz,                       \
                               TRUE,                                         \
    {                                                                        \
      for (PV_f = 0; PV_f < GrGeomOctreeNumFaces; PV_f++)                    \
        if (GrGeomOctreeHasFace(node, PV_f))                                 \
        {                                                                    \
          switch (PV_f)                                                      \
          {                                                                  \
            case GrGeomOctreeFaceL:                                          \
              fdir[0] = -1; fdir[1] = 0; fdir[2] = 0;                        \
              break;                                                         \
            case GrGeomOctreeFaceR:                                          \
              fdir[0] = 1; fdir[1] = 0; fdir[2] = 0;                         \
              break;                                                         \
            case GrGeomOctreeFaceD:                                          \
              fdir[0] = 0; fdir[1] = -1; fdir[2] = 0;                        \
              break;                                                         \
            case GrGeomOctreeFaceU:                                          \
              fdir[0] = 0; fdir[1] = 1; fdir[2] = 0;                         \
              break;                                                         \
            case GrGeomOctreeFaceB:                                          \
              fdir[0] = 0; fdir[1] = 0; fdir[2] = -1;                        \
              break;                                                         \
            case GrGeomOctreeFaceF:                                          \
              fdir[0] = 0; fdir[1] = 0; fdir[2] = 1;                         \
              break;                                                         \
            default:                                                         \
              fdir[0] = -9999; fdir[1] = -9999; fdir[2] = -99999;            \
              break;                                                         \
          }                                                                  \
                                                                             \
          body;                                                              \
        }                                                                    \
    })                                                                       \
  }

/**
 * @brief Loop over the faces of an octree without use of fdir array
 */
#define GrGeomOctreeFaceLoopNoFdir(i, j, k,                             \
                                   node, octree, level_of_interest,     \
                                   ix, iy, iz, nx, ny, nz,              \
                                   locals, setup,                       \
                                   f_left, f_right,                     \
                                   f_down, f_up,                        \
                                   f_back, f_front,                     \
                                   finalize)                            \
  {                                                                     \
    int PV_f;                                                           \
    UNPACK(locals);                                                     \
    GrGeomOctreeInsideNodeLoop(i, j, k, node, octree, level_of_interest, \
                               ix, iy, iz, nx, ny, nz,                  \
                               TRUE,                                    \
    {                                                                   \
      for (PV_f = 0; PV_f < GrGeomOctreeNumFaces; PV_f++)               \
        if (GrGeomOctreeHasFace(node, PV_f))                            \
        {                                                               \
          setup;                                                        \
                                                                        \
          switch (PV_f)                                                 \
          {                                                             \
            f_left;                                                     \
            f_right;                                                    \
            f_down;                                                     \
            f_up;                                                       \
            f_back;                                                     \
            f_front;                                                    \
          }                                                             \
                                                                        \
          finalize;                                                     \
        }                                                               \
    })                                                                  \
  }

/*==========================================================================
 *==========================================================================*/

/*
 * Geometry Loop over boxes.
 * i,j,k are starting indexes of the found box.
 * num_i, num_j, num_k are number of nodes in found box.
 */

/**
 * @brief Loop over the boxes of an octree
 *
 * Octree macro to loop over the boxes in the octree.  Octree is
 * traversed executing the code specified in body for boxes at the
 * supplied level interest or in leaf nodes.  value_test is evaluated
 * to determine if the body should be executed.  Bodies execute per
 * octree node traversed (a box), not over the index space.
 *
 * i,j,k should be initialized to the starting indices and will
 * modified as the tree is traversed to the starting offset of the
 * current octree node.
 *
 * The current level is maintained in the l parmeter.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[out] l current octree level
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] levels_in_octree number of levels in the octree
 * @param[in] level_of_interest level to limit tree traversal
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] level_body code to execute if level has been reached.
 * @param[in] leaf_body code to execute for leaf nodes
 */
#define GrGeomOctreeBoxLoop(i, j, k, l, node,              \
                            octree, levels_in_octree,      \
                            level_of_interest,             \
                            value_test,                    \
                            level_body, leaf_body)         \
  {                                                        \
    unsigned int PV_inc;                                   \
    int           *PV_visiting;                            \
    int PV_visit_child;                                    \
                                                           \
    node = octree;                                         \
    l = 0;                                                 \
                                                           \
    PV_inc = 1 << (levels_in_octree);                      \
    PV_visiting = ctalloc(int, (levels_in_octree) + 2);    \
    PV_visiting++;                                         \
    PV_visiting[0] = 0;                                    \
                                                           \
    while (l >= 0)                                         \
    {                                                      \
      /* if at the level of interest */                    \
      if (l == (level_of_interest))                        \
      {                                                    \
        if (value_test)                                    \
          level_body;                                      \
                                                           \
        PV_visit_child = FALSE;                            \
      }                                                    \
                                                           \
      /* if this is a leaf node */                         \
      else if (GrGeomOctreeNodeIsLeaf(node))               \
      {                                                    \
        if (value_test)                                    \
          leaf_body;                                       \
                                                           \
        PV_visit_child = FALSE;                            \
      }                                                    \
                                                           \
      /* have I visited all of the children? */            \
      else if (PV_visiting[l] < GrGeomOctreeNumChildren)   \
        PV_visit_child = TRUE;                             \
      else                                                 \
        PV_visit_child = FALSE;                            \
                                                           \
      /* visit either a child or the parent node */        \
      if (PV_visit_child)                                  \
      {                                                    \
        node = GrGeomOctreeChild(node, PV_visiting[l]);    \
        PV_inc = PV_inc >> 1;                              \
        i += (int)PV_inc * ((PV_visiting[l] & 1) ? 1 : 0); \
        j += (int)PV_inc * ((PV_visiting[l] & 2) ? 1 : 0); \
        k += (int)PV_inc * ((PV_visiting[l] & 4) ? 1 : 0); \
        l++;                                               \
        PV_visiting[l] = 0;                                \
      }                                                    \
      else                                                 \
      {                                                    \
        l--;                                               \
        i -= (int)PV_inc * ((PV_visiting[l] & 1) ? 1 : 0); \
        j -= (int)PV_inc * ((PV_visiting[l] & 2) ? 1 : 0); \
        k -= (int)PV_inc * ((PV_visiting[l] & 4) ? 1 : 0); \
        PV_inc = PV_inc << 1;                              \
        node = GrGeomOctreeParent(node);                   \
        PV_visiting[l]++;                                  \
      }                                                    \
    }                                                      \
                                                           \
    tfree(PV_visiting - 1);                                \
  }


/**
 * @brief Loop over the boxes in an octree constrained by a bounding box.
 *
 * General octree macro to loop over the boxes (i,j,k
 * num_x,num_y,num_z) in the index space on of the octree.  Octree is
 * traversed executing the code specified in body for each box at the
 * supplied level interest or in leaf nodes.  value_test is evaluated
 * to determine if the body should be executed at each box.
 *
 * i,j,k should be initialized to the starting indices and will
 * modified as the tree is traversed to the starting offset of the
 * current octree node.
 *
 * ix,iy,iz and nx,ny,nz specify a bounding box to constrain the the iteration.
 *
 * The current level is maintained in the l parmeter.
 *
 * @param[in,out] i X index
 * @param[in,out] j Y index
 * @param[in,out] k k index
 * @param[in,out] num_i size of index space in X
 * @param[in,out] num_j size of index space in Y
 * @param[in,out] num_k size of index space in Z
 * @param[out] node octree node
 * @param[in] octree octree to traverse
 * @param[in] levels_in_octree number of levels in the octree
 * @param[in] level_of_interest level to limit tree traversal
 * @param[in] ix x lower bound of index space to process
 * @param[in] iy y lower bound of index space to process
 * @param[in] iz z lower bound of index space to process
 * @param[in] nx x upper bound of index space to process
 * @param[in] ny y upper bound of index space to process
 * @param[in] nz z upper bound of index space to process
 * @param[in] value_test boolean if tests evaluated before body execution
 * @param[in] body code to execute
 */
#define GrGeomOctreeNodeBoxLoop(i, j, k,                            \
                                num_i, num_j, num_k,                \
                                node, octree,                       \
                                levels_in_octree,                   \
                                level_of_interest,                  \
                                ix, iy, iz, nx, ny, nz, value_test, \
                                body)                               \
  {                                                                 \
    int PV_i, PV_j, PV_k, PV_l;                                     \
    int PV_ixl, PV_iyl, PV_izl, PV_ixu, PV_iyu, PV_izu;             \
                                                                    \
                                                                    \
    PV_i = i;                                                       \
    PV_j = j;                                                       \
    PV_k = k;                                                       \
                                                                    \
    GrGeomOctreeBoxLoop(PV_i, PV_j, PV_k, PV_l,                     \
                        node, octree, levels_in_octree,             \
                        level_of_interest, value_test,              \
    {                                                               \
      /* find octree and region intersection */                     \
      PV_ixl = pfmax(ix, PV_i);                                     \
      PV_iyl = pfmax(iy, PV_j);                                     \
      PV_izl = pfmax(iz, PV_k);                                     \
      PV_ixu = pfmin((ix + nx), (PV_i + (int)PV_inc));              \
      PV_iyu = pfmin((iy + ny), (PV_j + (int)PV_inc));              \
      PV_izu = pfmin((iz + nz), (PV_k + (int)PV_inc));              \
                                                                    \
      i = PV_ixl;                                                   \
      j = PV_iyl;                                                   \
      k = PV_izl;                                                   \
      num_i = PV_ixu - PV_ixl;                                      \
      num_j = PV_iyu - PV_iyl;                                      \
      num_k = PV_izu - PV_izl;                                      \
      if (num_i > 0 && num_j > 0 && num_k > 0) {                    \
        body;                                                       \
      }                                                             \
    },                                                              \
    {                                                               \
      /* find octree and region intersection */                     \
      PV_ixl = pfmax(ix, PV_i);                                     \
      PV_iyl = pfmax(iy, PV_j);                                     \
      PV_izl = pfmax(iz, PV_k);                                     \
      PV_ixu = pfmin((ix + nx), (PV_i + (int)PV_inc));              \
      PV_iyu = pfmin((iy + ny), (PV_j + (int)PV_inc));              \
      PV_izu = pfmin((iz + nz), (PV_k + (int)PV_inc));              \
                                                                    \
      i = PV_ixl;                                                   \
      j = PV_iyl;                                                   \
      k = PV_izl;                                                   \
      num_i = PV_ixu - PV_ixl;                                      \
      num_j = PV_iyu - PV_iyl;                                      \
      num_k = PV_izu - PV_izl;                                      \
      if (num_i > 0 && num_j > 0 && num_k > 0) {                    \
        body;                                                       \
      }                                                             \
    })                                                              \
  }


#endif
