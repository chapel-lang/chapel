use BlockDist;
use CyclicDist;
//use CyclicZipOpt;
//useless comment 

use Time;
use CommDiagnostics;

config var n=16;
config var epsilon = 0.1;
config var verbose = false;
config var dist='C';
config var correct=false;
config var messages=false;
config var timeit=false;
config var volume=false;

const mydom = {0..n,0..n};
if dist=='NONE' {
	const mydist = mydom;
	dobench(mydist, mydom);
} /*else if dist=='CM' {
	const mydist = mydom dmapped CyclicZipOpt(startIdx=mydom.low);
	//totalcomm2=volume;
	dobench(mydist, mydom);	
} */else if dist=='C' {
	const mydist = mydom dmapped Cyclic(startIdx=mydom.low);
	dobench(mydist, mydom);	
} else if dist=='B' {
	const mydist = mydom dmapped Block(boundingBox=mydom);
	dobench(mydist, mydom);
} 

proc dobench(mydist, mydom) {
	var still_correct = true;

	var timer:Timer;

	if messages {
		resetCommDiagnostics();
		startCommDiagnostics();
	}

	if timeit {
		timer = new Timer();
		timer.start();
	}

/*
 *  Jacobi Method Example
 *
 *  This program computes the solution of a Laplace equation using 
 *  the Jacobi method.
 *
 */

	const ProblemSpace = {1..n-1, 1..n-1},    // domain for grid points
		BigDomain = mydom;   // domain including boundary points

	var X, XNew: [mydist] real = 0.0;  // declare arrays: 
										//   X stores approximate solution
										//   XNew stores the next solution
	var Xt, XNewt: [mydom] real = 0.0;

	X[n, 1..n-1] = 1.0;            // Set south boundary values to 1.0
	Xt[n, 1..n-1] = 1.0;

	if (verbose) {
	writeln("Initial configuration:");
	writeln(X, "\n");
	}

	var iteration = 0,                    // iteration counter
	  delta: real;                      // measure of convergence 

	//const north = (-1,0), south = (1,0), east = (0,1), west = (0,-1);
	const north={0..n-2, 1..n-1}, south={2..n, 1..n-1}, east={1..n-1, 0..n-2}, west={1..n-1, 2..n};

	do {
		// compute next approximation using Jacobi method and store in XNew
		//forall ij in ProblemSpace do
		  //XNew(ij) = (X(ij+north) + X(ij+south) + X(ij+east) + X(ij+west)) / 4.0;
		XNew(ProblemSpace) = (X(north) + X(south) + X(east) + X(west))/4.0;

		// compute difference between next and current approximations
		delta = max reduce abs(XNew[ProblemSpace] - X[ProblemSpace]);

		// update X with next approximation
		X[ProblemSpace] = XNew[ProblemSpace];

	    if correct {
	    	XNewt(ProblemSpace) = (Xt(north) + Xt(south) + Xt(east) + Xt(west))/4.0;
	    	Xt[ProblemSpace] = XNewt[ProblemSpace];
	    	for ii in mydom {
		    	still_correct &&= Xt[ii]==X[ii] && XNewt[ii]==XNew[ii];
		    }
	    }


		// advance iteration counter
		iteration += 1;

		if (verbose) {
		  writeln("iteration: ", iteration);
		  writeln(X);
		  writeln("delta: ", delta, "\n");
		}
	} while (delta > epsilon);
	
	
	
	if timeit {
		timer.stop();
	}

	if messages {
		stopCommDiagnostics();
	}

	if correct {
		writeln("it is correct? ", still_correct);
		writeln("Jacobi computation complete.");
		writeln("Delta is ", delta, " (< epsilon = ", epsilon, ")");
		writeln("# of iterations: ", iteration);
	}

	//var seconds=floor(stopTime-startTime);
	if timeit then writeln("took ", timer.elapsed(), " (s)");
	//writeln(stopTime:real(64));
	//writeln(startTime);
	//writeln(typeToString(startTime.type));

	// write comms
	if messages {
		var messages=0;
		var coms=getCommDiagnostics();
		for i in 0..numLocales-1 {
			//writeln(coms(i));
			messages+=coms(i).get:int;
			messages+=coms(i).put:int;
		}
		writeln('message count=', messages);
	}

	/*if volume {
		var total=0;
		for i in 0..numLocales-1 {
			total+=total_communication_counts2(i+1);
		}
		//writeln('totals count=', total_communication_counts+total_communication_counts2);
		writeln('message volume=', total);
	}*/
}
