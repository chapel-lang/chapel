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
        matrix[i,j] = (i * j):real / n_dim;
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

/* Print out locale distribution */
/*proc print_locale_data(A:[], n_dim: int) {
    var count: int = 0;
    for a in A {
        count += 1;
        write(a.locale.id);
        if (count == n_dim) {
            count = 0;
            writeln();
        }
    }
}*/

/* The process which runs the benchmark */
proc kernel_fw(dist_square, n_dim: int) {
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
	
    var path = initialize_matrix(dist_square, n_dim);

    for k in 1..n_dim {
        forall (i, j) in dist_square {
            var temp = path[k, j];
            forall (a, b) in zip(path[1..i, k], path[1..i, j]) {
                var tempSum = a + temp;
                if (tempSum < b) {
                    b = tempSum;
                }  
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
    
    if (printMatrices) {
        writeln("path:");
        print_matrix(path, n_dim);
        writeln();
    }
	
	//confirm correctness of calculation
	if correct {
		//Matrices and vectors to test correctness of calculation
	    var pathTest = initialize_matrix({1..n_dim,1..n_dim}, n_dim);
		
	    for k in 1..n_dim {
	        forall (i, j) in {1..n_dim,1..n_dim} {
	            var temp = pathTest[k, j];
	            forall (a, b) in zip(pathTest[1..i, k], pathTest[1..i, j]) {
	                var tempSum = a + temp;
	                if (tempSum < b) {
	                    b = tempSum;
	                }  
	            }
	        }
	    }
		
		for ii in 1..n_dim {
			for jj in 1..n_dim {
				still_correct &&= path[ii,jj] == pathTest[ii,jj];
			}
		}
		writeln("Is the calculation correct? ", still_correct);
		writeln("fw computation complete.");
	}
	
}

proc main() {
    /* Initialize the domains */
    var dom_square = {1..N, 1..N};
    
    if dist == "NONE" {
        var user_dist_square = dom_square;
        /* Run the benchmark */
        kernel_fw(user_dist_square, N); 
    /*} else if dist == "CM" {
        var user_dist_square = dom_square dmapped CyclicZipOpt(startIdx=dom_square.low);
        kernel_fw(user_dist_square, N);   */
    } else if dist == "C" {
        var user_dist_square = dom_square dmapped Cyclic(startIdx=dom_square.low);
        kernel_fw(user_dist_square, N); 
    } else if dist == "B" {
        var user_dist_square = dom_square dmapped Block(boundingBox=dom_square);
        kernel_fw(user_dist_square, N);  
    } 
}
