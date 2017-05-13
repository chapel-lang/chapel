/*
 * Copyright 2004-2017 Cray Inc.
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


// TODO -- performance test sort routines and optimize (see other TODO's)
/*

The Sort module is designed to support standard sort routines.

.. _comparators:

Comparators
-----------

Comparators allow sorting data by a mechanism other than the
default comparison operations between array elements. To use a comparator,
define a record with either a ``key(a)`` or ``compare(a, b)`` method, and pass
an instance of that record to the sort function (examples shown below).

If both methods are implemented on the record passed as the comparator, the
``key(a)`` method will take priority over the ``compare(a, b)`` method.

Key Comparator
~~~~~~~~~~~~~~

The ``key(a)`` method accepts 1 argument, which will be an element from the
array being sorted.

The default key method would look like this:

.. code-block:: chapel

  proc DefaultComparator.key(a) {
    return a;
  }


As an example, if the user wants to sort an array by the absolute values of its
elements, the user can define a comparator with a key method as follows:

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record Comparator { }

  // key method maps an element to the value to be used for comparison
  proc Comparator.key(a) { return abs(a); }

  var absComparator: Comparator;

  sort(Array, comparator=absComparator);

  // This will output: -1, 2, 3, -4
  writeln(Array);

The return type of ``key(a)`` must support the ``<``
operator, which is used by the base compare method of all sort routines. If the
``<`` operator is not defined for the return type, the user may define it
themselves like so:

.. code-block:: chapel

  proc op<(a: returnType, b: returnType): bool {
    ...
  }


Compare Comparator
~~~~~~~~~~~~~~~~~~

The ``compare(a, b)`` method accepts 2 arguments, which will be 2 elements from
the array being sorted. The return value should be a numeric signed type
indicating how a and b compare to each other. The conditions between ``a`` and
``b`` should result in the following return values for ``compare(a, b)``:

  ============ ==========
  Return Value Condition
  ============ ==========
  ``> 0``      ``a > b``
  ``0``        ``a == b``
  ``< 0``      ``a < b``
  ============ ==========

The default compare method for a numeric signed type would look like this:

.. code-block:: chapel

    proc DefaultComparator.compare(a, b) {
      return a - b;
    }


The absolute value comparison example from above can alternatively be
implemented with a compare method:

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record Comparator { }

  // compare method defines how 2 elements are compared
  proc Comparator.compare(a, b) {
    return abs(a) - abs(b);
  }

  var absComparator: Comparator;

  sort(Array, comparator=absComparator);

  // This will output: -1, 2, 3, -4
  writeln(Array);

Reverse Comparator
~~~~~~~~~~~~~~~~~~

Sort functions in Chapel do not have a ``reverse`` argument. Instead, reverse
sorting is handled through the comparator interface.

A module-defined :const:`reverseComparator` can be passed to a sort function to
reverse the default sorting order.

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Using module-defined 'reverseComparator'
  sort(Array, comparator=reverseComparator)

  // This will output: 3, 2, -1, -4
  writeln(Array);


To reverse the sort order of a user-defined comparator, pass the user-defined
comparator to the constructor of the module-defined
:record:`ReverseComparator` record, which can be passed to the sort function.

.. code-block:: chapel

  var Array = [-1, -4, 2, 3];

  // Empty record serves as comparator
  record Comparator { }

  // compare method defines how 2 elements are compared
  proc Comparator.compare(a, b) {
    return abs(a) - abs(b);
  }

  var absReverseComparator: ReverseComparator(Comparator);

  sort(Array, comparator=absReverseComparator);

  // This will output: -4, 3, 2, -1
  writeln(Array);

 */
module Sort {


/* Module-defined comparators */

/*
  Instance of :record:`DefaultComparator` used as default ``comparator=``
  argument when no comparator is passed to a sort function
*/
const defaultComparator: DefaultComparator;


/*
   Instance of :record:`ReverseComparator`. Pass this as the ``comparator=``
   argument of a sort function to reverse the sort order.
 */
const reverseComparator: ReverseComparator(DefaultComparator);


/* Private methods */

pragma "no doc"
/*
   Base compare method of all sort functions.

   By default, it returns the value of defaultComparator.compare(a, b).

   If a comparator with a key method is passed, it will return the value of
   defaultComparator(comparator.key(a), comparator.key(b)).

   If a comparator with a compare method is passed, it will return the value of
   comparator.compare(a, b).

   Return values conventions:

     a < b : returns value < 0
     a > b : returns value > 0
     a == b: returns 0
*/
inline proc chpl_compare(a, b, comparator:?rec=defaultComparator) {
  use Reflection;

  // TODO -- In cases where values are larger than keys, it may be faster to
  //         key data once and sort the keyed data, mirroring swaps in data.
  // Compare results of comparator.key(a) if is defined by user
  if canResolveMethod(comparator, "key", a) && canResolveMethod(comparator, "key", b) {
    return chpl_compare(comparator.key(a), comparator.key(b));
  // Use comparator.compare(a, b) if is defined by user
  } else if canResolveMethod(comparator, "compare", a, b) {
    return comparator.compare(a ,b);
  } else {
    compilerError("The comparator record requires a 'key(a)' or 'compare(a, b)' method");
  }
}


pragma "no doc"
/*
    Check if a comparator was passed and confirm that it will work, otherwise
    throw a compile-time error.

   :arg a: Sample data passed to confirm that comparator methods can resolve
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc chpl_check_comparator(comparator, type eltType) {
  use Reflection;

  // Dummy data for checking method resolution
  // This may need updating when constructors support non-default args
  const data: eltType;

  if comparator.type == DefaultComparator {}
  // Check for valid comparator methods
  else if canResolveMethod(comparator, "key", data) {
    // Check return type of key
    const keydata = comparator.key(data);
    type keytype = keydata.type;
    if !(canResolve("<", keydata, keydata)) then
      compilerError("The key method must return an object that supports the '<' function");
  }
  else if canResolveMethod(comparator, "compare", data, data) {
    // Check return type of compare
    type comparetype = comparator.compare(data, data).type;
    if !(isNumericType(comparetype)) then
      compilerError("The compare method must return a numeric type");
  }
  else {
    // If we make it this far, the passed comparator was defined incorrectly
    compilerError("The comparator record requires a 'key(a)' or 'compare(a, b)' method");
  }
}


/* Basic Functions */

/*
   General purpose sorting interface.

   .. note:: Currently this method calls a sequential :proc:`quickSort`, but
             this may change the future as other algorithms are implemented.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc sort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  quickSort(Data, comparator=comparator);
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc sort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("sort() requires 1-D array");
}


/*
   Check if array `Data` is in sorted order

   :arg Data: The array to verify
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
   :returns: ``true`` if array is sorted
   :rtype: `bool`
 */
proc isSorted(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator): bool {
  chpl_check_comparator(comparator, eltType);
  const stride = if Dom.stridable then abs(Dom.stride) else 1;

  for i in Dom.low..Dom.high-stride by stride do
    if chpl_compare(Data[i+stride], Data[i], comparator) < 0 then
      return false;
  return true;
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc isSorted(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("isSorted() requires 1-D array");
}


//
// This is a first draft "sorterator" which is designed to take some
// other iterator/iterable and yield its elements, in sorted order.
//
// The main limitations in the current code are (1) it should put some
// sort of constraint on 'x' to limit it to types for which this makes
// sense; and (2) there should be some generic way to say "y is an
// array of x's element type" (or to infer its element type) without
// saying a priori how big it is.  Without these mods, the result is
// that the sorterator works when it does and probably is confusing
// when it doesn't.
//
// TODO - Make standalone or leader/follower parallel iterator
/*
   Yield the elements of argument `x` in sorted order, using sort
   algorithm.

   .. note:

      This is currently implemented as a serial iterator, but will eventually
      support parallel iteration.

   :arg x: An iterable value to be sorted and yielded element by element
   :type x: `iterable`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   :yields: The elements of x in sorted order
   :ytype: x's element type

 */
iter sorted(x, comparator:?rec=defaultComparator) {
  var y = x;
  quickSort(y, comparator=comparator);
  for i in y do
    yield i;
}


/* Sort Functions */

/*
   Sort the 1D array `Data` in-place using a sequential bubble sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc bubbleSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  const low = Dom.low,
        high = Dom.high,
        stride = abs(Dom.stride);

  var swapped = true;

  while (swapped) {
    swapped = false;
    for i in low..high-stride by stride {
      if chpl_compare(Data(i), Data(i+stride), comparator) > 0 {
        Data(i) <=> Data(i+stride);
        swapped = true;
      }
    }
  }
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc bubbleSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("bubbleSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a sequential heap sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc heapSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  const low = Dom.low,
        high = Dom.high,
        size = Dom.size,
        stride = abs(Dom.stride);

  // Heapify
  var start = if high == low then high
              else if size % 2 then low + ((size - 1)/2) * stride
              else low + (size/2 - 1) * stride;

  while (start >= low) {
    SiftDown(start, high, comparator);
    start = start - stride;
  }

  // Sort, moving max element to end and re-heapifying the rest
  var end = high;
  while (end > low) {
    Data(end) <=> Data(low);
    end = end - stride;
    SiftDown(low, end, comparator);
  }

  proc SiftDown(start, end, comparator:?rec=defaultComparator) {
    var root = start;
    while ((2*root - low + stride) <= end) {
      const child = 2*root - low + stride;
      var swap = root;

      if chpl_compare(Data(swap), Data(child), comparator) < 0 then
        swap = child;

      if (child + stride <= end) && (chpl_compare(Data(swap), Data(child + stride), comparator) < 0) then
        swap = child + stride;

      if swap != root {
        Data(root) <=> Data(swap);
        root = swap;
      } else {
        return;
      }
    }
  }
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc heapSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("heapSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a sequential insertion sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc insertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  const low = Dom.low,
        high = Dom.high,
        stride = abs(Dom.stride);

  for i in low..high by stride {
    var ithVal = Data[i];
    var inserted = false;
    for j in low..i-stride by -stride {
      if chpl_compare(ithVal, Data[j], comparator) < 0 {
        Data[j+stride] = Data[j];
      } else {
        Data[j+stride] = ithVal;
        inserted = true;
        break;
      }
    }
    if (!inserted) {
      Data[low] = ithVal;
    }
  }
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc insertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("insertionSort() requires 1-D array");
}

/*
   Sort the 1D array `Data` in-place using a binary insertion sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */

proc binaryInsertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  use Search;
  chpl_check_comparator(comparator, eltType);
  const low = Dom.low,
        high = Dom.high;
        
  for i in low..high{
    var ithVal = Data[i];
    var inserted=false;
    var (found,loc) = binarySearch(Data,ithVal,comparator=comparator,lo=low,hi=i);
    
    while(found && loc!=i){      
      (found,loc) = binarySearch(Data,ithVal,comparator=comparator,lo=loc+1,hi=i);      
    }
    
    var j:int = i-1;
    
    while(j>=loc)
    {
      
      Data[j+1]=Data[j];
      j-=1;
    }
    Data[j+1]=ithVal;
  }        
}

pragma "no doc"
/*Error message for multi-dimension arrays */
proc binaryInsertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("binaryInsertionSort() requires 1-D array");
}

pragma "no doc"
proc binaryInsertionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.stridable {
  const D = {0..#Data.size};
  ref newData = Data.reindex(D);
  binaryInsertionSort(newData, comparator);
}

/*
   Sort the 1D array `Data` in-place using a timSort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */

proc timSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  use Search; 
  chpl_check_comparator(comparator, eltType);
  if(Dom.size<=64){
    binaryInsertionSort(Data,comparator=comparator);
    return;
  }
  const low = Dom.low,
        high = Dom.high;
  

  var (runs,count)=getRuns(Data,getMinrun(Dom.size)); 
  
  //reusing runs array for stack
  var top = 1; //array stack. Add first 3 runs
  var next = 2;//not part of the stack. next run to be added
  
  while(top>0){
    if(top<2){
      if(next>=count){
        _TimSortMerge(Data,runs[0],runs[1],comparator=comparator);
        top-=1;
      }else{
        top+=1;
        runs[top]=runs[next];
        next+=1;
      }
    }else{
      if(runs[top].size> runs[top-1].size+runs[top-2].size && runs[top-1].size>runs[top-2].size){
        top+=1;
        runs[top]=runs[next];
        next+=1;
      }else{
        if(runs[top].size<runs[top-2].size){
          top-=1;
          runs[top]=_TimSortMerge(Data,runs[top],runs[top+1],comparator=comparator);
        }else{         
          top-=1;
          runs[top-1]=_TimSortMerge(Data,runs[top-1],runs[top],comparator=comparator);
          runs[top]=runs[top+1];
        }
      }
    }
  }
  
}

pragma "no doc"
/*Error message for multi-dimension arrays */
proc timSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("timSort() requires 1-D array");
}

pragma "no doc"
proc timSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.stridable {
  const D = {0..#Data.size};
  ref newData = Data.reindex(D);
  timSort(newData, comparator);
}

/*
  Calculates the minrun length.
  
  :arg n: Number of elements in the data in question.
  :returns: The minrun length.
*/
proc getMinrun(in n: int) : int{
  var r: int;
  while(n>=64){
    r|=n&1;
    n>>=1;    
  }
  return n+r;
}
 /*
   Gets sorted sub-arrays present in the data.
   Performs binary insertion sort if the runlength is smaller than minrun   
   
   :arg Data: The array to be scanned
   :type Data: [] `eltType`
   :arg minrun: length of the smallest run. Default=1.
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
   :returns: a touple of an array of domains representing runs and the number of runs (array,number)

 */

proc getRuns(Data:[?Dom] ?eltType, minrun:int=1, comparator:?rec=defaultComparator){
  
  const low = Dom.low,
        high = Dom.high;
        
  var base=low;
  var maxRuns=(high-low)/(minrun) +1;
  
  //var s=false;
  //if (stride >1) then s=true;
  var runs:[0..maxRuns] domain(1);
  var n=0;
  
  do{
    var subD=countRun(Data,base,comparator=comparator);
    var sz=subD.size;
    if(sz<minrun){
      var h=(minrun)-1;
      var l=subD.low;
      if(l+h > high){      
        subD = {l..high };        
        binaryInsertionSort(Data[subD], comparator=comparator);        
      }else{        
        subD = {l..l+h };        
        binaryInsertionSort(Data[subD],comparator=comparator);          
      }
    }    
    runs[n]=subD;
    n+=1;
    base=subD.high+1;    
  }while(base<high);  
  return (runs,n);   
}

pragma "no doc"
private proc countRun(Data:[?Dom], start=Dom.low, comparator:?rec=defaultComparator){
  const low = Dom.low,
      high = Dom.high;
      
  if(start>=Dom.last) then return {start..start };
  
  var last=start;
  
  if(chpl_compare(Data[start],Data[start+1],comparator)<=0){
    //ascending
    while(last+1<=high && chpl_compare(Data[last],Data[last+1],comparator)<=0){
      last+=1;
    }
  }
  else {
    //descending
    while(last+1<=high && chpl_compare(Data[last],Data[last+1],comparator)>0){
      last+=1;
    }
    
    var l=start,h=last;
    //reverse
    while(l<h){
      Data[l]<=>Data[h];
      l+=1;
      h-=1;
    }
  }
  return {start..last };
}

/*
   Merges consecutive runs for timSort
   
   :arg Data: The array to be scanned
   :type Data: [] `eltType`
   :arg run1: domain of first run
   :arg run2: domain of second run
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
 */
 
private proc _TimSortMerge(Data:[?Dom] ?eltType,run1:domain,run2:domain,comparator:?rec=defaultComparator) {
  
  var s1,s2:int;
  var MIN_GALLOP=7;
  
  if(run1.size<=run2.size){    
    var tmp = Data[run1];
    var i=run1.first;
    var j=run2.first;
    var k=run1.first;
    
    while(i<=run1.last && j<=run2.last){
      if(chpl_compare(tmp[i],Data[j],comparator=comparator)<0){
        Data[k]=tmp[i];
        i+=1;
        k+=1;
        
        s1+=1;
        s2=0;
        if(s1>=MIN_GALLOP){
          var d=1,b=i-1, t=Data[j];
          while(b+d<run1.last && chpl_compare(tmp[b+d],t,comparator=comparator)<0){
            d<<=1;
          }
          if(d>1){
            d>>=1;
            Data[k..k+d ]= tmp[i..i+d ];
            i+=d;
            k+=d;
          }
          s1=0;
        }
      }else{
        Data[k]=Data[j];
        j+=1;
        k+=1;
        
        s2+=1;
        s1=0;
        if(s2>=MIN_GALLOP){
          var d=1,t=tmp[i],b=j-1;
          while(b+d<run1.last && chpl_compare(t,Data[b+d],comparator=comparator)>=0){
            d<<=1;
          }
          if(d>1){
            d>>=1;
            d-=1;
            //serial Data[k..k+d by stride] = Data[j..j+d by stride];
            for (x,y) in zip(k..k+d ,j..j+d ){
              Data[x]=Data[y];
            }
            j+=d;
            k+=d;
          }
          s2=0;
        }
      }      
    }
    
    if(j>run2.last){
      Data[k..run2.high ]=tmp[i..run1.high ];
    }
    
  }else{
    var tmp = Data[run2];
    var i=run2.last;
    var j=run1.last;
    var k=run2.last;
    while(i>=run2.first && j >=run1.first){
      if(chpl_compare(tmp[i],Data[j],comparator=comparator)>=0){
        Data[k]=tmp[i];
        i-=1;
        k-=1;
        
        s1+=1;
        s2=0; 
        if(s1>=MIN_GALLOP){
          var d=1,b=i+1,t=Data[j];
          while(b-d>=run2.low && chpl_compare(tmp[b-d],t,comparator=comparator)>=0){
            d<<=1;
          }
          if(d>1){
            d>>=1;
            d-=1;
            Data[k-d..k ] = tmp[i-d..i ];
            i-=d;
            k-=d;
          }
          s1=0;
        } 
      }else{
        Data[k]=Data[j];
        j-=1;
        k-=1;
        
        s2+=1;
        s1=0;
        if(s2>=MIN_GALLOP){          
          var d=1,b=j+1,t=tmp[i];
          while(b-d>=run1.low && chpl_compare(t,Data[b-d],comparator=comparator)<0){
            d<<=1;
          }
          if(d>1){             
            d>>=1;
            d-=1;            
            for (x,y) in zip(k-d..k by -1,j-d..j by -1){
              Data[x]=Data[y];
            }
            j-=d;
            k-=d;
          }
          s2=0;
        }
      }      
    }
    
    if(j<run1.first){
      Data[run1.low..k ]=tmp[run2.low..i ];
    }
  }
  return {run1.low..run2.high }; 
}

/*
   Sort the 1D array `Data` in-place using a parallel merge sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg minlen: When the array size is less than `minlen` use :proc:`insertionSort` algorithm
   :type minlen: `integral`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc mergeSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  _MergeSort(Data, minlen, comparator);
}

private proc _MergeSort(Data: [?Dom], minlen=16, comparator:?rec=defaultComparator)
  where Dom.rank == 1 {
  const lo = Dom.dim(1).low;
  const hi = Dom.dim(1).high;
  if hi-lo < minlen {
    insertionSort(Data, comparator);
    return;
  }
  const mid = (hi-lo)/2+lo;
  var A1 = Data[lo..mid];
  var A2 = Data[mid+1..hi];
  cobegin {
    { _MergeSort(A1, minlen, comparator); }
    { _MergeSort(A2, minlen, comparator); }
  }

  // TODO -- This iterator causes unnecessary overhead - we can do without it
  for (a, _a) in zip(Data[lo..hi], _MergeIterator(A1, A2, comparator=comparator)) do a = _a;
}


private iter _MergeIterator(A1: [] ?eltType, A2: [] eltType, comparator:?rec=defaultComparator) {
  var a1 = A1.domain.dim(1).low;
  const a1hi = A1.domain.dim(1).high;
  var a2 = A2.domain.dim(1).low;
  const a2hi = A2.domain.dim(1).high;
  while ((a1 <= a1hi) && (a2 <= a2hi)) {
    while (chpl_compare(A1(a1), A2(a2), comparator) <= 0) {
      yield A1(a1);
      a1 += 1;
      if a1 > a1hi then break;
    }
    if a1 > a1hi then break;
    while (chpl_compare(A2(a2), A1(a1), comparator) <= 0) {
      yield A2(a2);
      a2 += 1;
      if a2 > a2hi then break;
    }
  }
  if a1 == a1hi then yield A1(a1);
  else if a2 == a2hi then yield A2(a2);
  if a1 < a1hi then for a in A1[a1..a1hi] do yield a;
  else if a2 < a2hi then for a in A2[a2..a2hi] do yield a;
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc mergeSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("mergeSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a sequential quick sort algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg minlen: When the array size is less than `minlen` use :proc:`insertionSort` algorithm
   :type minlen: `integral`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc quickSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator) {
  chpl_check_comparator(comparator, eltType);
  // grab obvious indices
  const stride = abs(Dom.stride),
        lo = Dom.low,
        hi = Dom.high,
        size = Dom.size,
        mid = if hi == lo then hi
              else if size % 2 then lo + ((size - 1)/2) * stride
              else lo + (size/2 - 1) * stride;

  // base case -- use insertion sort
  if (hi - lo < minlen) {
    insertionSort(Data, comparator=comparator);
    return;
  }

  // find pivot using median-of-3 method
  if (chpl_compare(Data(mid), Data(lo), comparator) < 0) then
    Data(mid) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(lo), comparator) < 0) then
    Data(hi) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(mid), comparator) < 0) then
    Data(hi) <=> Data(mid);

  const pivotVal = Data(mid);
  Data(mid) = Data(hi-stride);
  Data(hi-stride) = pivotVal;
  // end median-of-3 partitioning

  var loptr = lo,
      hiptr = hi-stride;
  while (loptr < hiptr) {
    do { loptr += stride; } while (chpl_compare(Data(loptr), pivotVal, comparator) < 0);
    do { hiptr -= stride; } while (chpl_compare(pivotVal, Data(hiptr), comparator) < 0);
    if (loptr < hiptr) {
      Data(loptr) <=> Data(hiptr);
    }
  }

  Data(hi-stride) = Data(loptr);
  Data(loptr) = pivotVal;

  // TODO -- Get this cobegin working and tested
  //  cobegin {
    quickSort(Data[..loptr-stride], minlen, comparator);  // could use unbounded ranges here
    quickSort(Data[loptr+stride..], minlen, comparator);
  //  }
}

pragma "no doc"
/* Non-stridable quickSort */
proc quickSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator)
  where !Dom.stridable {
  chpl_check_comparator(comparator, eltType);

  // grab obvious indices
  const lo = Dom.low,
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  // base case -- use insertion sort
  if (hi - lo < minlen) {
    insertionSort(Data, comparator=comparator);
    return;
  }

  // find pivot using median-of-3 method
  if (chpl_compare(Data(mid), Data(lo), comparator) < 0) then
    Data(mid) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(lo), comparator) < 0) then
    Data(hi) <=> Data(lo);
  if (chpl_compare(Data(hi), Data(mid), comparator) < 0) then
    Data(hi) <=> Data(mid);

  const pivotVal = Data(mid);
  Data(mid) = Data(hi-1);
  Data(hi-1) = pivotVal;
  // end median-of-3 partitioning

  var loptr = lo,
      hiptr = hi-1;
  while (loptr < hiptr) {
    do { loptr += 1; } while (chpl_compare(Data(loptr), pivotVal, comparator) < 0);
    do { hiptr -= 1; } while (chpl_compare(pivotVal, Data(hiptr), comparator) < 0);
    if (loptr < hiptr) {
      Data(loptr) <=> Data(hiptr);
    }
  }

  Data(hi-1) = Data(loptr);
  Data(loptr) = pivotVal;

  // TODO -- Get this cobegin working and tested
  //  cobegin {
    quickSort(Data[..loptr-1], minlen, comparator);  // could use unbounded ranges here
    quickSort(Data[loptr+1..], minlen, comparator);
  //  }
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc quickSort(Data: [?Dom] ?eltType, minlen=16, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("quickSort() requires 1-D array");
}


/*
   Sort the 1D array `Data` in-place using a sequential selection sort
   algorithm.

   :arg Data: The array to be sorted
   :type Data: [] `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

 */
proc selectionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator) {
  const low = Dom.low,
        high = Dom.high,
        stride = abs(Dom.stride);

  for i in low..high-stride by stride {
    var jMin = i;
    // TODO -- should be a minloc reduction, when they can support comparators
    for j in i..high by stride {
      if chpl_compare(Data[j], Data[jMin], comparator) < 0 then
        jMin = j;
    }
    Data(i) <=> Data(jMin);
  }
}


pragma "no doc"
/* Error message for multi-dimension arrays */
proc selectionSort(Data: [?Dom] ?eltType, comparator:?rec=defaultComparator)
  where Dom.rank != 1 {
    compilerError("selectionSort() requires 1-D array");
}


/* Comparators */

/* Default comparator used in sort functions.*/
record DefaultComparator {

  /*
   Default compare method used in sort functions.

   :arg a: Array element
   :type a: `eltType`
   :arg b: Array element
   :type b: `eltType`
   :returns: 1 if ``b < a``
   :returns: 0 if ``a == b``
   :returns: -1 if ``a < b``

   */
  proc compare(a, b) {
    if a < b { return -1; }
    else if b < a { return 1; }
    else return 0;
  }
}

/* Reverse comparator built from another comparator.*/
record ReverseComparator {

  /* Generic comparator defined in constructor.*/
  var comparator;

  /*
   Constructor - builds a comparator with a compare method that reverses the sort order of
   the argument-provided comparator.

   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.

   */
  proc ReverseComparator(comparator:?rec=defaultComparator) {}

  /*
   Reversed compare method defined based on ``comparator.key`` if defined,
   otherwise ``comparator.compare``.

   :arg a: Array element
   :type a: `eltType`
   :arg b: Array element
   :type b: `eltType`
   :returns: -1 if ``b < a``
   :returns: 0 if ``a == b``
   :returns: 1 if ``a < b``
   */
  proc compare(a, b) {
    use Reflection;

    chpl_check_comparator(this.comparator, a.type);

    // Key defined
    if canResolveMethod(this.comparator, "key", a) && canResolveMethod(this.comparator, "key", b) {
      const A = this.comparator.key(a),
            B = this.comparator.key(b);
      if B < A { return -1; }
      else if A < B { return 1; }
      else return 0;

    // Compare defined
    } else if canResolveMethod(this.comparator, "compare", a, b) && canResolveMethod(this.comparator, "compare", a, b) {
      return this.comparator.compare(b, a);
    } else {
      compilerError("The comparator record requires a 'key(a)' or 'compare(a, b)' method");
    }
  }
}
} // Sort Module
