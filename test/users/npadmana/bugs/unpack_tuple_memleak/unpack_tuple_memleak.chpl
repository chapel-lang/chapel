/* Unpacking a tuple returned from a function in global scope 
 causes a memory leak.

There are three cases below -- 
 Case 1 : we just assign the tuple to a single variable, no memory leak
 Case 2 : we unpack the tuple, but not to global variables, no memory leak
 Case 3 : unpack the tuple into global variables, memory leak

*/
config const N=32**3;

proc mkarray() {
  var pos, vel : [0.. #N] real;
  return (pos, vel);
}

/* No leak here */
var a = mkarray();

/* No memory leak here */
{
  var (a1, a2) = mkarray();
}

/* Memory leak here */
var (b1, b2) = mkarray();
