/*
 * Copyright 2004-2020 Cray Inc.
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
  public use Sort only defaultComparator, DefaultComparator,
                       reverseComparator, ReverseComparator;
  private use Sort;


/*
  A note about lo/hi arguments:

  These args were kept in the interface since passing sliced arrays is
  (currently) very expensive, and many fundamental module codes depend on
  passing sliced arrays to binarySearch. For interface consistency, other
  search algorithms also support lo/hi arguments.

  TODO -- add performance testing for Search algorithms, and compare
          passing sliced arrays vs. unsliced arrays with lo/hi args
          and strided vs unstrided performance
*/

/*
   General purpose searching interface for searching through a 1D array.
   For pre-sorted arrays, denoted by passing ``sorted=true`` as an argument,
   this function wraps :proc:`binarySearch`, otherwise it wraps
   :proc:`linearSearch`.

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
proc search(Data:[?Dom], val, comparator:?rec=defaultComparator, lo=Dom.alignedLow, hi=Dom.alignedHigh, sorted=false) {
  if sorted then
    return binarySearch(Data, val, comparator, lo, hi);
  else
    return linearSearch(Data, val, comparator, lo, hi);
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc search(Data:[?Dom], val, comparator:?rec=defaultComparator, lo=Dom.alignedLow, hi=Dom.alignedHigh, sorted=false)
  where Dom.rank != 1 {
    compilerError("search() requires 1-D array");
}



/*
   Searches through the array `Data` looking for the value `val` using
   a sequential linear search.  Returns a tuple indicating (1) whether or not
   the value was found and (2) the location of the first occurrence of the
   value if it was found, or ``hi+abs(Dom.stride)`` if it was not found.

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
      of the value if it was found or ``hi+abs(Dom.stride)`` if it was not
      found.
   :rtype: (`bool`, `Dom.idxType`)

 */
proc linearSearch(Data:[?Dom], val, comparator:?rec=defaultComparator, lo=Dom.alignedLow, hi=Dom.alignedHigh) {

 chpl_check_comparator(comparator, Data.eltType);

 const stride = if Dom.stridable then abs(Dom.stride) else 1;
 if Dom.stridable {
   for i in lo..hi by stride {
     if chpl_compare(Data[i], val, comparator=comparator) == 0 then
       return (true, i);
   }
 } else {
   for i in lo..hi {
     if chpl_compare(Data[i], val, comparator=comparator) == 0 then
       return (true, i);
   }
 }

 return (false, hi+stride);
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc linearSearch(Data:[?Dom], val, comparator:?rec=defaultComparator, lo=Dom.alignedLow, hi=Dom.alignedHigh)
  where Dom.rank != 1 {
    compilerError("linearSearch() requires 1-D array");
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
proc binarySearch(Data:[?Dom], val, comparator:?rec=defaultComparator, in lo=Dom.alignedLow, in hi=Dom.alignedHigh) {
  chpl_check_comparator(comparator, Data.eltType);

  const stride = if Dom.stridable then abs(Dom.stride) else 1;

  while (lo <= hi) {
    const size = (hi - lo) / stride,
          mid = if hi == lo then hi
                else if size % 2 then lo + ((size - 1)/2) * stride
                else lo + (size/2 - 1) * stride;

    if chpl_compare(Data[mid], val, comparator=comparator) == 0 then
        return (true, mid);
    else if chpl_compare(val, Data[mid], comparator=comparator) > 0 then
      lo = mid + stride;
    else
      hi = mid - stride;
  }
  return (false, lo);
}


pragma "no doc"
/* Non-stridable binarySearch */
proc binarySearch(Data:[?Dom], val, comparator:?rec=defaultComparator, in lo=Dom.alignedLow, in hi=Dom.alignedHigh)
  where !Dom.stridable {
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


pragma "no doc"
/* Error message for multi-dimension arrays */
proc binarySearch(Data:[?Dom], val, comparator:?rec=defaultComparator, in lo=Dom.alignedLow, in hi=Dom.alignedHigh)
  where Dom.rank != 1 {
    compilerError("binarySearch() requires 1-D array");
}
} // Search module
