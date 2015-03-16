/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
   The `Search` module is designed to support standard search routines. The
   current interface is minimal and should be expected to grow and evolve
   over time.
 */


/*
   Searches through the pre-sorted array Data looking for the value val using
   a sequential linear search.  Returns a tuple indicating (1) whether or not
   the value was found and (2) the location of the value if it was found, or
   the location where the value should have been if it was not found.

   :arg Data: The sorted array to search
   :arg val: The value to find in the array

   :returns: A tuple indicating (1) if the value was found and (2) the location of the value if it was found or the location where the value should have been if it was not found.


 */
proc LinearSearch(Data:[?Dom], val) {
  for i in Dom {
    if (Data(i) == val) {
      return (true, i);
    } else if (Data(i) > val) {
      return (false, i);
    }
  }
  return (false,Dom.high+1);
}


// would really like to drop the lo/hi arguments here, but right now
// that causes too big of a memory leak
/*
   Searches through the pre-sorted array `Data` looking for the value `val`
   using a sequential binary search.  If provided, only the indices `lo`
   through `hi` will be considered, otherwise the whole array will be
   searched. Returns a tuple indicating (1) whether or not the value was
   found and (2) the location of the value if it was found, or the location
   where the value should have been if it was not found.

   :arg Data: The sorted array to search
   :arg val: The value to find in the array
   :arg lo: The lowest index to consider while searching
   :type lo: `integral`
   :arg hi: The highest index to consider while searching
   :type hi: `integral`

   :returns: A tuple indicating (1) if the value was found and (2) the location of the value if it was found or the location where the value should have been if it was not found.

 */
proc BinarySearch(Data:[?Dom], val, in lo = Dom.low, in hi = Dom.high) {
  while (lo <= hi) {
    const mid = (hi - lo)/2 + lo;
    if (Data(mid) == val) {
      return (true, mid);
    } else if (val > Data(mid)) {
      lo = mid+1;
    } else {
      hi = mid-1;
    }
  }
  return (false, lo);
}
