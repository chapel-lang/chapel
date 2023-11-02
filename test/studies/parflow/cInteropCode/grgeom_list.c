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

#include "parflow.h"

/*****************************************************************************
*
* The functions in this file are for manipulating the List data structure.
*
*****************************************************************************/

/*--------------------------------------------------------------------------
 * NewListMember
 *--------------------------------------------------------------------------*/

ListMember *NewListMember(
                          double value,
                          int    normal_component,
                          int    triangle_id)
{
  ListMember *new_member;

  new_member = talloc(ListMember, 1);

  ListMemberValue(new_member) = value;
  ListMemberNormalComponent(new_member) = normal_component;
  ListMemberTriangleID(new_member) = triangle_id;
  ListMemberNextListMember(new_member) = NULL;

  return new_member;
}

/*--------------------------------------------------------------------------
 * FreeListMember
 *--------------------------------------------------------------------------*/

void FreeListMember(
                    ListMember *member)
{
  tfree(member);
}

/*--------------------------------------------------------------------------
 * ListInsert
 *--------------------------------------------------------------------------*/

void ListInsert(
                ListMember **head,
                ListMember * member)
{
  ListMember *previous_member, *current_member, *next_member = NULL;
  int not_eol, searching;

  not_eol = TRUE;
  searching = TRUE;
  previous_member = NULL;
  current_member = *head;
  if (current_member != NULL)
  {
    next_member = ListMemberNextListMember(current_member);
  }

  while (not_eol && searching)
  {
    if (current_member == NULL)
    {
      not_eol = FALSE;
    }
    else
    {
      if (ListMemberValue(current_member) > ListMemberValue(member))
      {
        searching = FALSE;
      }
      else if (ListMemberValue(current_member) == ListMemberValue(member))
      {
        if (ListMemberTriangleID(current_member) >= ListMemberTriangleID(member))
        {
          searching = FALSE;
        }
      }
      if (searching)
      {
        previous_member = current_member;
        current_member = next_member;
        if (current_member != NULL)
        {
          next_member = ListMemberNextListMember(current_member);
        }
      }
    }
  }

  ListMemberNextListMember(member) = current_member;
  if (previous_member != NULL)
  {
    ListMemberNextListMember(previous_member) = member;
  }
  else
  {
    *head = member;
  }
}

/*--------------------------------------------------------------------------
 * ListDelete
 *--------------------------------------------------------------------------*/

int ListDelete(
               ListMember **head,
               ListMember * member)
{
  ListMember *previous_member, *current_member;
  int deleted, not_eol, searching;

  previous_member = NULL;
  current_member = *head;
  not_eol = TRUE;
  searching = TRUE;
  while (not_eol && searching)
  {
    if (current_member != NULL)
    {
      if (current_member == member)
      {
        searching = FALSE;
      }
      else
      {
        previous_member = current_member;
        current_member = ListMemberNextListMember(current_member);
      }
    }
    else
    {
      not_eol = FALSE;
    }
  }

  if (not_eol)
  {
    if (previous_member != NULL)
    {
      ListMemberNextListMember(previous_member) = ListMemberNextListMember(member);
    }
    else
    {
      *head = NULL;
    }
    FreeListMember(member);
    deleted = TRUE;
  }
  else
  {
    deleted = FALSE;
  }

  return deleted;
}

/*--------------------------------------------------------------------------
 * ListSearch
 *--------------------------------------------------------------------------*/

ListMember *ListSearch(
                       ListMember *head,
                       double      value,
                       int         normal_component,
                       int         triangle_id)
{
  ListMember *list_member, *current_member;
  int not_eol, searching;

  current_member = head;
  not_eol = TRUE;
  searching = TRUE;
  while (not_eol && searching)
  {
    if (current_member != NULL)
    {
      if ((ListMemberValue(current_member) == value)
          && (ListMemberNormalComponent(current_member) == normal_component)
          && (ListMemberTriangleID(current_member) == triangle_id))
      {
        searching = FALSE;
      }
      else
      {
        current_member = ListMemberNextListMember(current_member);
      }
    }
    else
    {
      not_eol = FALSE;
    }
  }
  if (not_eol)
  {
    list_member = current_member;
  }
  else
  {
    list_member = NULL;
  }
  return list_member;
}

/*--------------------------------------------------------------------------
 * ListValueSearch
 *--------------------------------------------------------------------------*/

ListMember *ListValueSearch(
                            ListMember *head,
                            double      value)
{
  ListMember *list_member, *current_member;
  int not_eol, searching;

  current_member = head;
  not_eol = TRUE;
  searching = TRUE;
  while (not_eol && searching)
  {
    if (current_member != NULL)
    {
      if (ListMemberValue(current_member) == value)
      {
        searching = FALSE;
      }
      else
      {
        current_member = ListMemberNextListMember(current_member);
      }
    }
    else
    {
      not_eol = FALSE;
    }
  }
  if (not_eol)
  {
    list_member = current_member;
  }
  else
  {
    list_member = NULL;
  }
  return list_member;
}

/*--------------------------------------------------------------------------
 * ListValueNormalComponentSearch
 *--------------------------------------------------------------------------*/

ListMember *ListValueNormalComponentSearch(
                                           ListMember *head,
                                           double      value,
                                           int         normal_component)
{
  ListMember *list_member, *current_member;
  int not_eol, searching;

  current_member = head;
  not_eol = TRUE;
  searching = TRUE;
  while (not_eol && searching)
  {
    if (current_member != NULL)
    {
      if ((ListMemberValue(current_member) == value)
          && (ListMemberNormalComponent(current_member) == normal_component))
      {
        searching = FALSE;
      }
      else
      {
        current_member = ListMemberNextListMember(current_member);
      }
    }
    else
    {
      not_eol = FALSE;
    }
  }
  if (not_eol)
  {
    list_member = current_member;
  }
  else
  {
    list_member = NULL;
  }
  return list_member;
}

/*--------------------------------------------------------------------------
 * ListTriangleIDSearch
 *--------------------------------------------------------------------------*/

ListMember *ListTriangleIDSearch(
                                 ListMember *head,
                                 int         triangle_id)
{
  ListMember *list_member, *current_member;
  int not_eol, searching;

  current_member = head;
  not_eol = TRUE;
  searching = TRUE;
  while (not_eol && searching)
  {
    if (current_member != NULL)
    {
      if (ListMemberTriangleID(current_member) == triangle_id)
      {
        searching = FALSE;
      }
      else
      {
        current_member = ListMemberNextListMember(current_member);
      }
    }
    else
    {
      not_eol = FALSE;
    }
  }
  if (not_eol)
  {
    list_member = current_member;
  }
  else
  {
    list_member = NULL;
  }
  return list_member;
}


/*--------------------------------------------------------------------------
 * ListFree
 *--------------------------------------------------------------------------*/

void ListFree(
              ListMember **head)
{
  ListMember *current_member, *next_member;
  int not_eol;

  current_member = *head;
  not_eol = TRUE;
  while (not_eol)
  {
    if (current_member != NULL)
    {
      next_member = ListMemberNextListMember(current_member);
      FreeListMember(current_member);
      current_member = next_member;
    }
    else
    {
      not_eol = FALSE;
    }
  }
  *head = NULL;
}

/*--------------------------------------------------------------------------
 * ListLength
 *--------------------------------------------------------------------------*/

int ListLength(
               ListMember *head)
{
  ListMember *current_member;
  int not_eol;
  int length;

  length = 0;
  current_member = head;
  not_eol = TRUE;
  while (not_eol)
  {
    if (current_member != NULL)
    {
      length++;
      current_member = ListMemberNextListMember(current_member);
    }
    else
    {
      not_eol = FALSE;
    }
  }

  return(length);
}

/*--------------------------------------------------------------------------
 * ListPrint
 *--------------------------------------------------------------------------*/

void ListPrint(
               ListMember *head)
{
  ListMember *current_member;
  int not_eol;
  double value;
  int normal_component;
  int triangle_id;

  current_member = head;
  not_eol = TRUE;
  while (not_eol)
  {
    if (current_member != NULL)
    {
      value = ListMemberValue(current_member);
      normal_component = ListMemberNormalComponent(current_member);
      triangle_id = ListMemberTriangleID(current_member);
      amps_Printf(" List Member = [%f, %d, %d]\n", value, normal_component, triangle_id);
      current_member = ListMemberNextListMember(current_member);
    }
    else
    {
      not_eol = FALSE;
    }
  }
}
