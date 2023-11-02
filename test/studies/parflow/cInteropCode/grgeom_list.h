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

#ifndef _GRGEOM_LIST_HEADER
#define _GRGEOM_LIST_HEADER


/*----------------------------------------------------------------
 * List Structure
 *----------------------------------------------------------------*/

typedef struct list_member {
  double value;
  int normal_component;
  int triangle_id;
  struct list_member   *next_list_member;
} ListMember;

#include "parflow.h"

/*--------------------------------------------------------------------------
 * Accessors : ListMember
 *--------------------------------------------------------------------------*/
#define ListMemberValue(list_member)           ((list_member)->value)
#define ListMemberNormalComponent(list_member) ((list_member)->normal_component)
#define ListMemberTriangleID(list_member)      ((list_member)->triangle_id)
#define ListMemberNextListMember(list_member)  ((list_member)->next_list_member)

/*--------------------------------------------------------------------------
 * Macros : ListMember
 *--------------------------------------------------------------------------*/
#define ListValueInClosedInterval(current_member, lower_point, upper_point) \
  (((current_member) != NULL) ? (((ListMemberValue((current_member)) >= (lower_point)) && (ListMemberValue((current_member)) <= (upper_point))) ? TRUE : FALSE) : FALSE)

#define ListValueLEPoint(current_member, point) \
  (((current_member) != NULL) ? ((ListMemberValue((current_member)) <= (point)) ? TRUE : FALSE) : FALSE)

#define ListValueLTPoint(current_member, point) \
  (((current_member) != NULL) ? ((ListMemberValue((current_member)) < (point)) ? TRUE : FALSE) : FALSE)

#define ListValueEQPoint(current_member, point) \
  (((current_member) != NULL) ? ((ListMemberValue((current_member)) == (point)) ? TRUE : FALSE) : FALSE)

#define ListValueGTPoint(current_member, point) \
  (((current_member) != NULL) ? ((ListMemberValue((current_member)) > (point)) ? TRUE : FALSE) : FALSE)

#define ListValueGEPoint(current_member, point) \
  (((current_member) != NULL) ? ((ListMemberValue((current_member)) >= (point)) ? TRUE : FALSE) : FALSE)

#endif
