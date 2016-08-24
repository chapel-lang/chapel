/*
 * Copyright 2004-2016 Cray Inc.
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
   The `Search` module is designed to support standard search routines on 1D
   arrays
 */
module Search {
  use Sort;


/*
  A note about lo/hi arguments:

  These args were kept in the interface since passing sliced arrays is
  (currently) very expensive, and many fundamental module codes depend on
  passing sliced arrays to binarySearch. For interface consistency, other
  search algorithms also support lo/hi arguments.

  TODO -- add performance testing for Search algorithms, and compare
          passing sliced arrays vs. unsliced arrays with lo/hi args
*/

/*
   General purpose searching interface for searching through a 1D array.
   For pre-sorted arrays, denoted by passing ``sorted=true`` as an argument,
   this function wraps :proc:`binarySearch`, otherwise it wraps
   :proc`linearSearch`.

   :arg Data: The array to be searched
   :type Data: [] `eltType`
   :arg val: The value to find in the array
   :type val: `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted, and the equality operation for the array data.
   :arg lo: The lowest index to consider while searching
   :type lo: `Dom.idxType`
   :arg hi: The highest index to consider while searching
   :type hi: `Dom.idxType`
   :arg sorted: Indicate if the array is pre-sorted, determining which search
      algorithm to call
   :type sorted: `bool`

   :returns: A tuple indicating (1) if the value was found and (2) the location
      of the value if it was found or the location where the value should have
      been if it was not found.
   :rtype: (`bool`, `Dom.idxType`)
 */
proc search(Data:[?Dom], val, comparator:?rec=defaultComparator, lo=Dom.low, hi=Dom.high, sorted=false) where Dom.rank == 1 {
  if sorted then
    return binarySearch(Data, val, comparator, lo, hi);
  else
    return linearSearch(Data, val, comparator, lo, hi);
}


/*
   Searches through the array `Data` looking for the value `val` using
   a sequential linear search.  Returns a tuple indicating (1) whether or not
   the value was found and (2) the location of the first occurrence of the
   value if it was found, or ``Data.domain.high+1`` if it was not found.

   :arg Data: The array to search
   :type Data: [] `eltType`
   :arg val: The value to find in the array
   :type val: `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines the
       equality operation for the array data.
   :arg lo: The lowest index to consider while searching
   :type lo: `Dom.idxType`
   :arg hi: The highest index to consider while searching
   :type hi: `Dom.idxType`

   :returns: A tuple indicating (1) if the value was found and (2) the location
      of the value if it was found or ``Data.domain.high+1`` if it was not
      found.
   :rtype: (`bool`, `Dom.idxType`)

 */
proc linearSearch(Data:[?Dom], val, comparator:?rec=defaultComparator, lo=Dom.low, hi=Dom.high) where Dom.rank == 1 {
  chpl_check_comparator(comparator, Data.eltType);

  // Domain slicing is cheap, but avoiding it when possible helps performance
  if lo == Dom.low && hi == Dom.high {
    for i in Dom do
      if chpl_compare(Data[i], val, comparator=comparator) == 0 then
        return (true, i);
  } else {
    for i in Dom[lo..hi] do
      if chpl_compare(Data[i], val, comparator=comparator) == 0 then
        return (true, i);
  }

  return (false, Dom.high+1);
}


/*
   Searches through the pre-sorted array `Data` looking for the value `val`
   using a sequential binary search.  If provided, only the indices `lo`
   through `hi` will be considered, otherwise the whole array will be
   searched. Returns a tuple indicating (1) whether or not the value was
   found and (2) the location of the value if it was found, or the location
   where the value should have been if it was not found.

   :arg Data: The sorted array to search
   :type Data: [] `eltType`
   :arg val: The value to find in the array
   :type val: `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
   :arg lo: The lowest index to consider while searching
   :type lo: `Dom.idxType`
   :arg hi: The highest index to consider while searching
   :type hi: `Dom.idxType`

   :returns: A tuple indicating (1) if the value was found and (2) the location
      of the value if it was found or the location where the value should have
      been if it was not found.
   :rtype: (`bool`, `Dom.idxType`)

 */
proc binarySearch(Data:[?Dom], val, comparator:?rec=defaultComparator, in lo=Dom.low, in hi=Dom.high) where Dom.rank == 1 {
  chpl_check_comparator(comparator, Data.eltType);

  while (lo <= hi) {
    const mid = (hi - lo)/2 + lo;
    if chpl_compare(Data[mid], val, comparator=comparator) == 0 then
      return (true, mid);
    else if chpl_compare(val, Data[mid], comparator=comparator) > 0 then
      lo = mid + 1;
    else
      hi = mid - 1;
  }
  return (false, lo);
}


/*
    Deprecated Functions
    TODO -- deprecate in 1.15
 */


pragma "no doc"
/*
   Searches through the pre-sorted array `Data` looking for the value `val` using
   a sequential linear search.  Returns a tuple indicating (1) whether or not
   the value was found and (2) the location of the value if it was found, or
   the location where the value should have been if it was not found.

   :arg Data: The sorted array to search
   :arg val: The value to find in the array

   :returns: A tuple indicating (1) if the value was found and (2) the location of the value if it was found or the location where the value should have been if it was not found.

 */
proc LinearSearch(Data:[?Dom], val) {
  compilerWarning("LinearSearch() has been deprecated.  Please use linearSearch() instead");
  for i in Dom {
    if (Data(i) == val) {
      return (true, i);
    } else if (Data(i) > val) {
      return (false, i);
    }
  }
  return (false,Dom.high+1);
}


pragma "no doc"
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
  compilerWarning("BinarySearch() has been deprecated.  Please use binarySearch() instead");
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
}
