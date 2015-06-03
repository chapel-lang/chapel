use CyclicDist;
/*use CyclicZipOpt;*/
use BlockDist;
use Time;
use CommDiagnostics;

/****************************
	printMatrices: Set to false if you don't want to see the matrices printed
		Default = false
	N: size (square) of the matrix 
		Default = 128
    dist: the distribution of the domain which the matrices are based on. 
        Default: cyclical with modulo unrolling
*****************************/
config var correct = false;
config var timeit = false;
config var messages = false;
config var printMatrices: bool = false;
config var dist: string = "C";

config var N: int = 128;

/* Initializes a matrix based on a distribution */
proc initialize_matrix(distribution, n_dim: int) {
    var matrix: [distribution] real = 0.0;
    forall (i,j) in distribution {
        matrix[i,j] = (i * j) / (n_dim + 0.0);
    }
    return matrix;
}

/* Prints out the matrix passed in */
proc print_matrix(A: [], n_dim: int) {
    for i in 1..n_dim {
        for j in 1..n_dim {
            write(A[i,j], " ");
        }
        writeln();
    }
}

proc within_epsilon(a: real, b: real)
{
  return fabs(a-b) < 0.00001;
}

/* The process which runs the benchmark */
proc kernel_lu(dist_square, n_dim: int) {
	var still_correct = true;
    var t:Timer;
	
	if messages {
		resetCommDiagnostics();
		startCommDiagnostics();
	}
	
    /******* Start the timer: this is where we do work *******/
	if timeit {
		t = new Timer();
		t.start();
	}
	
    var A = initialize_matrix(dist_square, n_dim);
    var s: real;
    
    for i in 1..n_dim {
        forall j in (i+1)..n_dim {
                A[i,j] = A[i,j] / A[i,i];
        }
        for j in (i+1)..n_dim {
            forall (a, b) in zip(A[j, (i+1)..n_dim], A[i, (i+1)..n_dim]) {
                a -= A[j,i] * b;
            }
        }
    } 
	
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
	
    var ATest = initialize_matrix({1..n_dim, 1..n_dim}, n_dim);
    var sTest: real;
	
	//will say correctness is false but it is actually true
	if correct {
	    for i in 1..n_dim {
	        forall j in (i+1)..n_dim {
	                ATest[i,j] = ATest[i,j] / ATest[i,i];
	        }
	        for j in (i+1)..n_dim {
	            forall (a, b) in zip(ATest[j, (i+1)..n_dim], ATest[i, (i+1)..n_dim]) {
	                a -= ATest[j,i] * b;
	            }
	        }
	    }
		
		for ii in 1..n_dim {
			for jj in 1..n_dim {
				still_correct &&=
                                  (within_epsilon(A[ii,jj],ATest[ii,jj]));
			}
		}
		writeln("Is the calculation correct? ", still_correct);
		writeln("lu computation complete");
	}
    
    if (printMatrices) {
        writeln("A:");
        print_matrix(A, n_dim);
        writeln();
		writeln("ATest:");
		print_matrix(ATest, n_dim);
		writeln();
    }
}

proc main() {
    /* Initialize the domains */
    var dom_square = {1..N, 1..N};
    
    if dist == "NONE" {
        var user_dist_square = dom_square;
        /* Run the benchmark */
        kernel_lu(user_dist_square, N); 
    /*} else if dist == "CM" {
        var user_dist_square = dom_square dmapped CyclicZipOpt(startIdx=dom_square.low);
        kernel_lu(user_dist_square, N);   */
    } else if dist == "C" {
        var user_dist_square = dom_square dmapped Cyclic(startIdx=dom_square.low);
        kernel_lu(user_dist_square, N); 
    } else if dist == "B" {
        var user_dist_square = dom_square dmapped Block(boundingBox=dom_square);
        kernel_lu(user_dist_square, N);  
    } 
}
