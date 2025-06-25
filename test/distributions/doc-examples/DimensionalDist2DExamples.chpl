//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
use DimensionalDist2D, ReplicatedDim, BlockCycDim;

const Space = {1..3, 1..8};

// Compute N_1 and N_2 and reshapes Locales correspondingly.

var (N_1, N_2) =
  if numLocales == 1
    then (1, 1)
    else (2, numLocales/2);

var MyLocaleView = {0..N_1-1, 0..N_2-1};
var MyLocales = reshape(Locales[0..N_1*N_2-1], MyLocaleView);

const D = Space
  dmapped new dimensionalDist2D(MyLocales,
                            new ReplicatedDim(numLocales = N_1),
                            new BlockCyclicDim(numLocales = N_2,
                                               lowIdx     = 1,
                                               blockSize  = 2));
var A: [D] int;

for loc in MyLocales do on loc {

  // The ReplicatedDim specifier always accesses the local replicand.
  //
  // Therefore, 'forall a in A' when executed on MyLocales[loc1,loc2]
  // visits only the replicands on MyLocales[loc1,0..N_2-1].

  forall a in A do
    a = here.id;

  // Technicality: 'writeln(A)' would read A always on Locale 0.
  // Since we want to see what A contains on the current locale,
  // we use default-distributed 'Helper'.
  // 'Helper = A' captures the view of A on the current locale,
  // which we then print out.

  writeln("On ", here, ":");
  const Helper: [Space] int = A;
  writeln(Helper);
  writeln();
}
/* STOP_EXAMPLE */
