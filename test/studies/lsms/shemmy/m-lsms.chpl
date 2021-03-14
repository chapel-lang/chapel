//
// This test is pulled from intern ii02 in the codes written in the
// Summer of 2011 to measure increase in productivity using Chapel
// versus C/MPI. It is a pared down version of the Locally Self-consistent
// Multiple Scattering (LSMS) application.
//

use BlockDist, Time;

config const debug = false;
config const perfTest = false;
config const reportFrequency = 1025;
config const displayLIZ = false;
config const serialForall = true;

config const span = 5;

const span_x = span,
	  span_y = span,
	  span_z = span,
	  spanTuple = (span, span, span);

config const atomSpacing = 5.000, //spacing in X, Y, and Z
			 lizRadius   = 7.110, //radius of LIZ sphere
			 nIterations = 1024;

const reductionFactor = 1000.0;

param nExtent = 13;
type  AtomMatrix = nExtent*real;

const GridDom  = {0..#span_x, 0..#span_y, 0..#span_z};
const GridDist = GridDom dmapped Block(GridDom);

const LocalesDist = {0..#numLocales} dmapped Block({0..#numLocales});

class Cache {
	var space: sparse subdomain(GridDom);
	var atoms: [space] AtomMatrix;
}

proc main() {
	var t: Timer;

	writeln("[[ LSMS ]]");
	writeln("Problem size = [", span_x, ", ", span_y, ", ", span_z, "]");

	//parameters for each atom 
	var atoms: [GridDist] AtomMatrix;

	//list of atoms in LIZ for each atom
	var lizDoms: [GridDist] sparse subdomain(GridDom);
	//list of atoms accessed by local atoms, for each locale
	var caches: [LocalesDist] unmanaged Cache =
                      [LocalesDist] new unmanaged Cache();

	//compute LIZ and caches
        serial serialForall do // TODO: enable 'forall' in parallel
	forall (a, liz) in zip(GridDist, lizDoms) {
		for ac in GridDom do if a != ac {
			local {
				var dist = atomSpacing * sqrt( + reduce ([d in 0..2] circularDistance(a[d],ac[d],spanTuple[d])**2));
				if dist <= lizRadius {
					liz += ac;
					caches[here.id].space += ac;
				}
			}
		}
	}
	if displayLIZ then writeln("LIZ counts: ", [liz in lizDoms] liz.size);

	//initialize atom values
	forall (i, atom) in zip(GridDist, atoms) {
		local do for param e in 0..nExtent-1 do atom[e] = GridDist.indexOrder(i);
	}
	
	t.start();

	for itr in 0..#nIterations {
		if itr % reportFrequency == 0 && reportFrequency <= nIterations {
			writef("step %{####}\n", itr);
		}
		//load data into caches
		forall cache in caches {
			forall i in cache.space {
				cache.atoms[i] = atoms[i]; //local and remote loads
			}
		}
		//add up neighbors' contributions
		forall (a, liz) in zip(GridDist, lizDoms) {
			local {
				var total: AtomMatrix;
				for ac in liz {
					/*TODO this does a binary search to find the atom in the cache
					  try to find a way to loop through cache? or store the location */
					total += caches[here.id].atoms[ac];
				}
				for param e in 0..nExtent-1 {
					atoms[a][e] += total[e] / reductionFactor;
				}
			}
		}
		if itr == 0 || itr == nIterations-1 {
			checkExpected(itr, atoms);
		}
	}

	t.stop();

	writeln("Success!");
	if perfTest then writeln("Chapel time = ", t.elapsed(), " s");

        forall cache in caches do
          delete cache;
}

proc checkExpected(itr: int, atoms: [?AtomDom] AtomMatrix) {

	proc check(atom: AtomMatrix, expected): bool {
		const tolerance = max(expected * 1e-9, 0.1);

		var pass = true;
		for param e in 0..nExtent-1 {
			if abs(atom[e] - expected) > tolerance then pass = false;
		}
		if debug then writef( "%11.9r ~=~ %11.9r\n", 
                                      atom[1], expected);

		return pass;
	}

	if span != 5 {
		writeln("span != 5, expected value check disabled...");
		return;
	}
	if itr > 0 && nIterations != 1024 {
	writeln("nIterations != 1024, expected value check disabled...");
		return;
	}

	const first = AtomDom.low,
		  last  = AtomDom.high;

	var pass: bool;
	if itr == 0 {
		pass = check(atoms[first], 0.775) 
			&& check(atoms[last],  125.5);
	} else {
		pass = check(atoms[first], 5320018409.9) 
			&& check(atoms[last],  5324998555.3);
	}
	if !pass {
		writeln("Computed values do not match expected values at step ", itr);
		exit(1);
	}
}

proc circularDistance(a, b, size: int) {
	if a < b {
		return min(a+size-b, b-a);
	} else {
		return min(a-b, b-a+size);
	}
}

proc compactWriteArray(arr: [?D], fmtStr = "%{#}") where D.rank == 3 {
	for j in D.dim[2] {
		for i in D.dim[1] {
                  for a in arr[i,j,..] do writef(fmtStr, a);
                  if i < D.dim[1].high then write("   ");
		}
		writeln();
	}
}
