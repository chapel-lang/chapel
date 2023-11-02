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

#ifndef _INPUT_DATABASE_HEADER
#define _INPUT_DATABASE_HEADER

#include "hbt.h"

/**
 * Maximum length for the key (including the trailing NUL).
 * User can thus enter 1024 characters.
 */
#define IDB_MAX_KEY_LEN 1025

/**
 * Maximum length for the value (including the trailing NUL).
 * User can thus enter 4096 characters.
 */
#define IDB_MAX_VALUE_LEN 65536

/**
 * Entry value for the HBT.  Contains the key and the value pair.
 */
typedef struct _IDB_Entry {
  char *key;
  char *value;

  /* Flag indicating if the key was used */
  char used;
} IDB_Entry;

/**
 * The input database type.  Currently uses a HBT (height balanced tree) for
 * storage.
 */
typedef HBT IDB;

typedef struct NameArray__ {
  int num;
  char **names;

  char *tok_string;
  char *string;
} NameArrayStruct;

typedef NameArrayStruct *NameArray;

#define WHITE " \t\n"

#define NA_MAX_KEY_LENGTH 2048

#endif
