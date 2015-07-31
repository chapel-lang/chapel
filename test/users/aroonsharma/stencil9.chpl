use CyclicDist;
/*use CyclicZipOpt;*/
use BlockDist;
use Time;
use CommDiagnostics;

config const n = 10;
config const epsilon = 0.01;
config const printArrays = false;
config var dist: string = "C";
config var timeit = false;
config var messages = false;
config var correct = false;

proc kernel_stencil9(dist_little, dist_big, dom_little, dom_big) {
	var still_correct = true;
	var t: Timer;
	
    const northWest = {0..n-1, 0..n-1}, north = {0..n-1, 1..n}, northEast = {0..n-1, 2..n+1};
    const west = {1..n, 0..n-1}, center = {1..n, 1..n}, east = {1..n, 2..n+1};
    const southWest = {2..n+1, 0..n-1}, south = {2..n+1, 1..n}, southEast = {2..n+1, 2..n+1};

    var A, B: [dist_big] real;
	var Atest, Btest: [dom_big] real;

    A[  n/4+1,   n/4+1] =  1.0;
    A[3*n/4+1, 3*n/4+1] =  1.0;
    A[  n/4+1, 3*n/4+1] = -1.0;
    A[3*n/4+1,   n/4+1] = -1.0;
	
    Atest[  n/4+1,   n/4+1] =  1.0;
    Atest[3*n/4+1, 3*n/4+1] =  1.0;
    Atest[  n/4+1, 3*n/4+1] = -1.0;
    Atest[3*n/4+1,   n/4+1] = -1.0;

    if printArrays then
      writeln("Initial A:\n", A[dist_little], "\n");

	if messages {
		resetCommDiagnostics();
		startCommDiagnostics();
	}

	if timeit {
		t = new Timer();
		t.start();
	}
	
    var numIters = 0;
	
    do {
      numIters += 1;

      forall (a,b,c,d,e) in zip (B(dist_little), A(northWest), A(northEast), A(southWest), A(southEast)) {
          a = ((.0625)*(b+c+d+e));
      }
  
      forall(a,b,c,d,e,f) in zip(B(dist_little), A(north), A(west), A(south), A(east), A(center)){
          a += ((.125)*(b+c+d+e) + (.25)*(f));
      }

      const delta = max reduce abs(B - A);

      B <=> A;

     } while (delta > epsilon);
	 
    /******* End the timer *******/
	if timeit {
		t.stop();
 		writeln("took ", t.elapsed(), " seconds");
	}
	
	//Print out communication counts (gets and puts)
	if messages {
		stopCommDiagnostics();	
		var messages=0;
		var coms=getCommDiagnostics();
		for i in 0..numLocales-1 {
			messages+=coms(i).get:int;
			messages+=coms(i).put:int;
		}
		writeln('message count=', messages);	
	}

    if printArrays then
      writeln("Final A:\n", A[dist_little]);

    writeln("# iterations: ", numIters);
	
	//confirm correctness of calculation
	if correct {
	    var numIters = 0;
	
	    do {
	      numIters += 1;

	      forall (a,b,c,d,e) in zip (Btest(dom_little), Atest(northWest), Atest(northEast), Atest(southWest), Atest(southEast)) {
	          a = ((.0625)*(b+c+d+e));
	      }
  
	      forall(a,b,c,d,e,f) in zip(Btest(dom_little), Atest(north), Atest(west), Atest(south), Atest(east), Atest(center)){
	          a += ((.125)*(b+c+d+e) + (.25)*(f));
	      }

	      const delta = max reduce abs(Btest - Atest);

	      Btest <=> Atest;

	     } while (delta > epsilon);
		 
    	for ii in dom_big {
	    	still_correct &&= A[ii]==Atest[ii] && B[ii]==Btest[ii];
	    }
		writeln("it is correct? ", still_correct);
		writeln("stencil9 computation complete.");
	}
}

proc main() {
    
    const    Dom = {1..n  , 1..n},
          BigDom = {0..n+1, 0..n+1};
    
    if dist == "NONE" {
        const dist_little = Dom;
        const dist_big = BigDom;
        kernel_stencil9(dist_little, dist_big, Dom, BigDom); 
    /*} else if dist == "CM" {
        const dist_little = Dom dmapped CyclicZipOpt(startIdx=Dom.low);
        const dist_big = BigDom dmapped CyclicZipOpt(startIdx=BigDom.low);
        kernel_stencil9(dist_little, dist_big, Dom, BigDom);*/
    } else if dist == "C" {
        const dist_little = Dom dmapped Cyclic(startIdx=Dom.low);
        const dist_big = BigDom dmapped Cyclic(startIdx=BigDom.low);
        kernel_stencil9(dist_little, dist_big, Dom, BigDom); 
    } else if dist == "B" {
        const dist_little = Dom dmapped Block(boundingBox=Dom);
        const dist_big = BigDom dmapped Block(boundingBox=BigDom);
        kernel_stencil9(dist_little, dist_big, Dom, BigDom); 
    }      
}
