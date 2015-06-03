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
/* Matrix will be n_dim * identity matrix to make sure 
   that Cholesky decomposition can take place */
proc initialize_matrix(distribution, n_dim: int) {
    var matrix: [distribution] real = 0.0;
    forall (i,j) in distribution {
		if i == j {
			matrix[i,j] = n_dim;
		}
		else {
			matrix[i,j] = 0.0;
		}
        //matrix[i,j] = 1.0 / n_dim;
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

/* The process which runs the benchmark */
proc kernel_cholesky(dist_square, n_dim: int) {
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
    var C: [dist_square] real = 0.0;
    var s: real;
    
  /*  A[1,1] = 18;
    A[1,2] = 22;
    A[1,3] = 54;
    A[1,4] = 42;
    A[2,1] = 22;
    A[2,2] = 70;
    A[2,3] = 86;
    A[2,4] = 62;
    A[3,1] = 54;
    A[3,2] = 86;
    A[3,3] = 174;
    A[3,4] = 134;
    A[4,1] = 42;
    A[4,2] = 62;
    A[4,3] = 134;
    A[4,4] = 106;*/
  
    for i in dist_square[1, 1..n_dim] {
        for j in dist_square[i, 1..i] {
            s = 0.0;
            forall (a,b) in zip(C[i,1..j], C[j,1..j]) {
                s += a * b;
            }                
            if (i == j) {
                C[i,j] = sqrt(A[i,i] - s);
            } else {
                C[i,j] = 1.0/(C[j,j]) * (A[i,j] - s);
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
	
	//Matrices to test correctness of calculation
	var Atest = initialize_matrix({1..n_dim, 1..n_dim}, n_dim);
	var Ctest: [1..n_dim, 1..n_dim] real = 0.0;
	var sTest: real;
	
	//confirm correctness of calculation
	if correct {
	    for i in 1..n_dim {
	        for j in 1..i {
	            sTest = 0.0;
	            forall (a,b) in zip(Ctest[i,1..j], Ctest[j,1..j]) {
	                sTest += a * b;
	            }                
	            if (i == j) {
	                Ctest[i,j] = sqrt(Atest[i,i] - sTest);
	            } else {
	                Ctest[i,j] = 1.0/(Ctest[j,j]) * (Atest[i,j] - sTest);
	            }
	        }
	    } 
		
		for ii in 1..n_dim {
			for jj in 1..n_dim {
				still_correct &&= C[ii,jj] == Ctest[ii,jj];
			}
		}
		still_correct &&= s == sTest;
		writeln("Is the calculation correct? ", still_correct);
		writeln("cholesky computation complete.");
	}
	
    if (printMatrices) {
        writeln("A:");
        print_matrix(A, n_dim);
        writeln();
        writeln("C:");
        print_matrix(C, n_dim);
        writeln();
        writeln("Atest:");
        print_matrix(Atest, n_dim);
        writeln();
        writeln("Ctest:");
        print_matrix(Ctest, n_dim);
        writeln();
    }
}

proc main() {
    /* Initialize the domains */
    var dom_square = {1..N, 1..N};
    
    if dist == "NONE" {
        var user_dist_square = dom_square;
        /* Run the benchmark */
        kernel_cholesky(user_dist_square, N); 
    /*} else if dist == "CM" {
        var user_dist_square = dom_square dmapped CyclicZipOpt(startIdx=dom_square.low);
        kernel_cholesky(user_dist_square, N);   */
    } else if dist == "C" {
        var user_dist_square = dom_square dmapped Cyclic(startIdx=dom_square.low);
        kernel_cholesky(user_dist_square, N); 
    } else if dist == "B" {
        var user_dist_square = dom_square dmapped Block(boundingBox=dom_square);
        kernel_cholesky(user_dist_square, N);  
    } 
}
