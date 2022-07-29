use CTypes;
use Communication;

enum errMode { getNegLoc, getHiLoc, getNegSize,
               putNegLoc, putHiLoc, putNegSize };

config const mode: errMode;

const n = 10;
var A: [1..n] int;
var B: [1..n] int;

var Aptr = c_ptrTo(A);
var Bptr = c_ptrTo(B);

select mode {
  when errMode.getNegLoc  do get(Aptr, Bptr, -1, n);
  when errMode.getHiLoc   do get(Aptr, Bptr, numLocales, n);
  when errMode.getNegSize do get(Aptr, Bptr, 0, -n);
  when errMode.putNegLoc  do put(Aptr, Bptr, -1, n);
  when errMode.putHiLoc   do put(Aptr, Bptr, numLocales, n);
  when errMode.putNegSize do put(Aptr, Bptr, 0, -n);
}
