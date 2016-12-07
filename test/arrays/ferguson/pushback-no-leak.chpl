/*
  The following code demonstrates a memory leak
  that was present at one point.
*/

var wrappedArrays: [1..0] WrappedArray;

const n = 10; // n is parsed from a file

extend(wrappedArrays, n);

for i in 1..n do
 wrappedArrays[i].push_back(i);


record WrappedArray {
 var Array: [1..0] int;

 proc push_back(node) {
   this.Array.push_back(node);
 }
}

/* Extend an array by n elements */
proc extend(ref A: [?D] ?t, n: int) {
 var default: t;
 for 1..n do
   A.push_back(default);
}

