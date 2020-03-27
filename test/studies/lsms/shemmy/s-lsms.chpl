// 
// This test is pulled from intern ii02 in the codes written in the 
// Summer of 2011 to measure increase in productivity using Chapel 
// versus C/MPI.It is a pared down version of the Locally Self-consistent
// Multiple Scattering (LSMS) application.
//

use Time;

config const debug = false;
config const reportFrequency = 1025;
config const perfTest = false;
config const displayLIZ = false;

config const span = 5;

const span_x = span,
	  span_y = span,
	  span_z = span,
	  nAtoms = span_x * span_y * span_z,
	  spanT  = (span, span, span);

config
const atomSpacing = 5.000, //spacing in X, Y, and Z
	  lizRadius   = 7.110, //radius of LIZ sphere
	  nIterations = 1024;

const reductionFactor = 1000.0;

param nExtent = 13;
type AtomMatrix = nExtent*real;
//type AtomMatrix = [0..#nExtent] real;

const GridDom  = {0..#span_x, 0..#span_y, 0..#span_z};

proc main() {
	var t: Timer;

	writeln("[[ LSMS ]]");
	writeln("Problem size = [", span_x, ", ", span_y, ", ", span_z, "]");

	//calculate Local Interaction Zones
	var lizDoms: [GridDom] sparse subdomain(GridDom);
	for (a, liz) in zip(GridDom, lizDoms) {
		for ac in GridDom do if a != ac {
			//compute dist between 2 points
			var diff: 3*int;
			for param d in 1..3 do diff[d] = circularDistance(a[d], ac[d], spanT[d]);
			var dist = atomSpacing * sqrt(diff[1]**2 + diff[2]**2 + diff[3]**2);
			if dist <= lizRadius {
				liz += ac;
			}
		}
	}
	if displayLIZ then writeln("LIZ counts: ", [liz in lizDoms] liz.size);

	//initialize atoms
	var atoms: [GridDom] AtomMatrix;
	for (i, atom) in zip(GridDom, atoms) {
		for param e in 1..nExtent do atom[e] = GridDom.indexOrder(i);
	}

	t.start();

	for itr in 0..#nIterations {
		if itr % reportFrequency == 0 && reportFrequency <= nIterations {
			writef("step %{####}...\n", itr);
		}
		
		//each atom adds up extent values from neighbors (members of its liz sphere)
		var totals: [GridDom] AtomMatrix;
		for (liz, total, i) in zip(lizDoms, totals, GridDom) {
			for ac in liz {
				total += atoms[ac];
			}
		}
		for a in GridDom {
			for param e in 1..nExtent {
				atoms[a][e] += totals[a][e] / reductionFactor;
			}
		}
		
		if itr == 0 || itr == nIterations-1 {
			checkExpected(itr, atoms);
		}
	}

	t.stop();

	writeln("Success!");

	if perfTest then writeln("Chapel serial time = ", t.elapsed(), " s");
}

proc checkExpected(itr: int, atoms: [GridDom] AtomMatrix) {
	if span != 5 || nIterations != 1024 {
		writeln("Expected value check disabled...");
		return;
	}
	const first = GridDom.low,
		  last  = GridDom.high;

	var ExpectedDom: sparse subdomain(GridDom);
	ExpectedDom += first; ExpectedDom += last;
	
	var expectedValues: [ExpectedDom] real;
	if itr == 0 {
		expectedValues[first] = 0.775;
		expectedValues[last]  = 125.5;
	} else {
		expectedValues[first] = 5320018409.9;
		expectedValues[last]  = 5324998555.3;
	}

	for i in ExpectedDom {
		var tolerance = max(expectedValues[i] * 1e-9, 0.1);
		var pass = true;
		for param e in 1..nExtent {
			if abs(atoms[i][e] - expectedValues[i]) > tolerance then pass = false;
		}
                if debug { writef("itr %4n @ %t: %12.9r ~=~ %12.9r\n",
                                  itr, i, atoms[i][1], expectedValues[i]); }
		if !pass {
			writeln("Computed values do not match expected values: itr = ", itr, 
					" @ ", i, ": ", atoms[i][1], " != ", expectedValues[i]);
			exit(0);
		} 
	}
}

proc circularDistance(a, b, size: int) {
	if a < b {
		return min(a+size-b, b-a);
	} else {
		return min(a-b, b-a+size);
	}
}

