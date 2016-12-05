/*
  The following code leaks memory
  This was distilled down from the SABNA-Chapel port
*/

var wrappedArrays: [1..0] WrappedArray ; // declared before parsing (is record field)

const n = 10; // n is parsed from a file

extend(wrappedArrays, n);

// i is also parsed from a file, so we need to access any nodeArrays[i]
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

