
/*BHEADER*********************************************************************
 *
 *  Copyright (c) 1995-2019, Lawrence Livermore National Security,
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

#ifndef _INDEX_SPACE_HEADER
#define _INDEX_SPACE_HEADER

/**
 * Dimension for index space.
 *
 * Currently index spaces are hard-coded for 3D.
 */
#define DIM 3

/**
 * Hold a point in index space.
 */
typedef int Point[DIM];

/**
 * A box in index space.
 *
 * Defined by an upper and lower index.
 */
typedef struct {
  Point lo;
  Point up;
} Box;

/**
 * Element in a box list.
 *
 * Holds a box and list pointers.
 */
typedef struct _BoxListElement {
  Box box;
  struct _BoxListElement* next;
  struct _BoxListElement* prev;
} BoxListElement;

/**
 * List of boxes.
 *
 * Doubly linked list for storing boxes.
 */
typedef struct _BoxList {
  BoxListElement* head;
  BoxListElement* tail;
  unsigned int size;
} BoxList;

/**
 * Array of boxes.
 */
typedef struct _BoxArray {
  Box* boxes;
  int boxlimits[2 * DIM];
  unsigned int size;
} BoxArray;

/**
 * Copy point from src to dst.
 *
 * @param dst destination point
 * @param src source point
 */
void PointCopy(Point dst, Point src);

/**
 * The size of the box.
 *
 * Returns number of points in the index that are contained in the box.
 *
 * @param box input box
 * @return size of box
 */
int BoxSize(Box *box);

/**
 * Compute number of index points in each dimension.
 *
 * @param box input box
 * @param number_cells number of cells in each dimension.
 */
void BoxNumberCells(Box* box, Point* number_cells);

/**
 * Clear box.
 *
 * Resets box upper and lower index points to 0.
 *
 * @param box input box
 */
void BoxClear(Box *box);

/**
 * Set lower and upper points of the box.
 *
 * @param box box to set
 * @param lo lower point that defines the box
 * @param up upper point that defines the box
 */
void BoxSet(Box *box, Point lo, Point up);

/**
 * Copy box from src to dst.
 *
 * @param dst destination box
 * @param src destination box
 */
void BoxCopy(Box *dst, Box *src);

/**
 * Print Box to stdout.
 *
 * Debugging utility to print a box.
 *
 * @param box box to print
 */
void BoxPrint(Box* box);

/**
 * Allocate an empty box list
 *
 * @return newly constructed box list
 */
BoxList* NewBoxList(void);

/**
 * Free box list
 *
 * Delete all storage associated with the provide box array.  Pointer
 * becomes an invalid reference.
 *
 * @param box_list box list to free
 */
void FreeBoxList(BoxList *box_list);

/**
 * Size of box list
 *
 * @param box_list box list
 * @return number of elements in the box list
 */
int BoxListSize(BoxList *box_list);

/**
 * Is box list empty.
 *
 * @param box_list box list
 * @return boolean true if box list is empty, false otherwise
 */
int BoxListIsEmpty(BoxList *box_list);

/**
 * Return first element on the box list.
 *
 * @param box_list box list
 * @return first element in box list
 */
Box* BoxListFront(BoxList *box_list);

/**
 * Append box to box list.
 *
 * Box list is copied into box list, reference is not kept.
 *
 * @param box_list box list
 * @param box box to append to end of list
 */
void BoxListAppend(BoxList* box_list, Box* box);

/**
 * Concatenate box list to a box list.
 *
 * Box list is copied into box list, reference is not kept.
 *
 * @param box_list box list to concatenate to
 * @param concatenate_list list to concatenate to end of box_list
 */
void BoxListConcatenate(BoxList *box_list, BoxList *concatenate_list);

/**
 * Remove all elements from the box list.
 *
 * @param box_list box list
 */
void BoxListClearItems(BoxList* box_list);

/**
 * Debugging utility to print box list to stdout.
 *
 * @param box_list box list
 */
void BoxListPrint(BoxList* box_list);

/**
 * Create a new box array with elements from a box list.
 *
 * The new box array constructed will have same size as the provided box list.
 *
 * @param box_list list of boxes to insert into the new box array
 * @return new box array
 */
BoxArray* NewBoxArray(BoxList *box_list);

/**
 * Free box array.
 *
 * Delete all storage associated with the provide box array.  Pointer
 * becomes an invalid reference.
 *
 * @param box_array box array to free.
 */
void FreeBoxArray(BoxArray* box_array);

/**
 * Get the ith box in the array.
 *
 * @param box_array the box array
 * @param i index into the array (not ranged checked)
 * @return the ith box in the array
 */
#define BoxArrayGetBox(box_array, i) (box_array->boxes[i])

/**
 * Maximum cell coordinates of any box in box array
 *
 * @param box_array box array
 * @param dim dimension
 * @return cell index of the respective dimension
 */
#define BoxArrayMaxCell(box_array, dim) (box_array->boxlimits[DIM + dim])

/**
 * Minimum cell coordinates of any box in box array
 *
 * @param box_array box array
 * @param dim dimension
 * @return cell index of the respective dimension
 */
#define BoxArrayMinCell(box_array, dim) (box_array->boxlimits[dim])

/**
 * Size of box array
 *
 * @param box_array box array
 * @return number of elements in the box array
 */
#define BoxArraySize(box_array) (box_array->size)

#endif

