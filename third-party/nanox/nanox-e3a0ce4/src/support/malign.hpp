/*************************************************************************************/
/*      Copyright 2011 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/
#ifndef _NANOS_MEMORY_ALIGNMENT_HPP
#define _NANOS_MEMORY_ALIGNMENT_HPP

/* This macro computes the memory offset for a given element (ce) with taking into account its own *type*
 * and the *base* and *size* of the previous element e.g.:
 *
 *    +---------+---+------+          It is important to realize that first two parameters 
 *    |++++pe+++|···|++ce++|          refer to previous element and only *type* parameter refers
 *    +---------+---+------+          to current element (which we want to align). This is because
 *    ^         ^   ^                 we want to add padding to the previous structure in order to
 *    base   size   align (return)    aling the current one.
 *
 * If we use this macro with *base* and *size* of the last element (and with any type given as a parameter
 * we get the size of the whole chunk.
 */
#define NANOS_ALIGNED_MEMORY_OFFSET(base,size,alignment) \
   ( ((uintptr_t)(base+size+alignment-1)) & (~(uintptr_t)(alignment-1)) )

#endif
