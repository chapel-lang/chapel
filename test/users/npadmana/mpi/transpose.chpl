/* An example of mixed mode Chapel-MPI programming.

We'd like to do a 2D distributed matrix transpose.
Since this is an AlltoAll textbook example, let's
not reinvent the wheel, and just use it. However, for
fun, we'll also use Chapel's distributed arrays...

*/

use MPI;
use C_MPI;
use BlockDist;

config const N=8;
config const debug=true;
assert(N%numLocales==0, "Matrix dimension must divide number of processors");
const N1 = N/numLocales;

proc main() {

  var Dom = {0.. #N, 0.. #N};
  var target : [0.. #numLocales, 0..0] locale;
  target[..,0] = Locales;
  var Space = Dom dmapped Block(Dom, targetLocales=target);

  // Debug
  if debug then coforall loc in Locales do on loc {
    writef("Locale %i --> Rank %i\n",here.id, commRank());
  }

  // Just for fun, write this out....
  if debug then for loc in Locales do on loc {
    writef("Locale %i owns %t\n",here.id, Space.localSubdomain());
  }

  // Define arrays
  var A, At : [Space] int(32);
  [(i,j) in Space] A[i,j] = (i*N + j).safeCast(int(32));
  if debug then writeln(A);

  coforall loc in Locales do on loc {
    var myspace = Space.localSubdomain();
    var low = myspace.low;
    var n1 = N1 : c_int;
    // We could make this multi-threaded
    for i in 0.. #n1 {
      var ndx = low + (i,0);
      MPI_Alltoall(A.localAccess[ndx], n1, MPI_INT, At.localAccess[ndx], n1, MPI_INT, CHPL_COMM_WORLD);
    }
    // Local transposes
    var d1 = {0.. #N1, 0.. #N1};
    forall i in 0.. #numLocales {
      var off = low + (0,i*N1);
      for (i,j) in d1 {
        if (i > j) then At.localAccess[(i,j)+off] <=> At.localAccess[(j,i)+off];
      }
    }
  }

  if debug then writeln("\n",At);

  // Do a simple check
  [(i,j) in Space] At[i,j] = abs(At[i,j] - (i + j*N).safeCast(int(32)));
  writef("Max error = %i \n",max reduce At);

}
