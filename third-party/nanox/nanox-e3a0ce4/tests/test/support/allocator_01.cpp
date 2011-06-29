/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
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
/* DESCRIPTION: Just checking basic functionality of Allocator alternating memory
 * allocations and memory deallocations in a single thread.
 */

/*<testinfo>
test_generator="gens/mixed-generator"
</testinfo>*/

#include <iostream>
#include "allocator.hpp"

#define CHECK_VALUE 3456
#define TIMES 100000

int sizes[] = { 7, 17, 33, 63, 123 };

int main (int argc, char **argv)
{
   bool check = true;
   Allocator allocator;

   for ( int  n = 0; n < TIMES; n++ ) {
      for ( unsigned int i = 0; i < (sizeof( sizes )/sizeof(int)); i++ ) {

         int *ptr = (int *) allocator.allocate( sizes[i] * sizeof(int) );
         if ( ptr == NULL ) check = false;

         for ( int j = 0; j < sizes[i]; j++ ) ptr[j] = CHECK_VALUE; // INI
         for ( int j = 0; j < sizes[i]; j++ ) ptr[j]++; // INC
         for ( int j = 0; j < sizes[i]; j++ ) ptr[j]--; // DEC

         // Check result
         for ( int j = 0; j < sizes[i]; j++ ) {
            if ( ptr[j] != CHECK_VALUE ) exit(-1);
         }

         Allocator::deallocate( ptr );
      }
   }

   if (check) { return 0; } else { return -1; }
}

