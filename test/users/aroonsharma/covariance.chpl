use CyclicZipOpt;
use BlockDist;
use Time;
use CommDiagnostics;

/****************************
	printMatrices: Set to false if you don't want to see the matrices printed
		Default = false
	M and N: size (square, M x N) of the matrices
		Default = 128
    dist: the distribution of the domain which the matrices are based on. 
        Default: cyclical with modulo unrolling
*****************************/
config var correct = false;
config var timeit = false;
config var messages = false;
config var printMatrices: bool = false;
config var dist: string = "CM";
config var M: int = 128;
config var N: int = 128;

const float_n: real = 1.2;

/* Initializes a matrix based on a distribution */
proc initialize_matrix(distribution, m_dim: int) {
    var matrix: [distribution] real = 0.0;
    forall (i,j) in distribution {
        matrix[i,j] = ((i - 1) * (j - 1)):real / m_dim;
    }
    return matrix;
}

/* Prints out the matrix passed in */
proc print_matrix(A: [], m_dim: int, n_dim: int) {
    for i in 1..m_dim {
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
proc kernel_covariance(dist_square, dist_linear, m_dim: int, n_dim: int) {
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
	
	var data = initialize_matrix(dist_square, m_dim);
    var symmat: [dist_square] real = 0.0;
    var mean: [dist_linear] real = 0.0;
    
    /* Determine mean of column vectors of input data matrix */
    forall (m, i) in zip(mean, 1..) {
        m = (+ reduce(data[1..n_dim, i])):real / float_n;
    }
    
    /* Center the column vectors */
    forall (i,j) in dist_square {
        data[i,j] -= mean[j];
    }

    /* Calculate the m * m correlation matrix. Computes only upper triangle */
    forall (i, j) in dist_square {
        if (i <= j) {
            var tempArray: [1..m_dim] real;
            forall(d1, d2, k) in zip(data[1..m_dim, i], data[1..m_dim, j], 1..) {
                tempArray[k] = d1 * d2;
            }
            symmat[i,j] = + reduce(tempArray);
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
	
	var dataTest = initialize_matrix({1..m_dim,1..n_dim}, m_dim);
    var symmatTest: [{1..m_dim,1..n_dim}] real = 0.0;
    var meanTest: [1..m_dim] real = 0.0;
	
	//confirm correctness of calculation
	if correct {
	    /* Determine mean of column vectors of input data matrix */
	    forall (m, i) in zip(meanTest, 1..) {
	        m = (+ reduce(dataTest[1..n_dim, i])):real / float_n;
	    }
    
	    /* Center the column vectors */
	    forall (i,j) in {1..m_dim,1..n_dim} {
	        dataTest[i,j] -= meanTest[j];
	    }

	    /* Calculate the m * m correlation matrix. Computes only upper triangle */
	    forall (i, j) in {1..m_dim,1..n_dim} {
	        if (i <= j) {
	            var tempArray: [1..m_dim] real;
	            forall(d1, d2, k) in zip(dataTest[1..m_dim, i], dataTest[1..m_dim, j], 1..) {
	                tempArray[k] = d1 * d2;
	            }
	            symmatTest[i,j] = + reduce(tempArray);
	        }
	    }
		
		for ii in 1..m_dim {
			for jj in 1..m_dim {
				still_correct &&= symmat[ii,jj] == symmatTest[ii,jj];
			}
		}
		writeln("Is the calculation correct? ", still_correct);
		writeln("covariance computation complete.");
	}
	
    if (printMatrices) {
        writeln("data:");
        print_matrix(data, m_dim, n_dim);
        writeln();
        writeln("mean:");
        writeln(mean);
        writeln();
        writeln("symmat:");
        print_matrix(symmat, m_dim, m_dim);
        writeln();
		
        writeln("dataTest:");
        print_matrix(dataTest, m_dim, n_dim);
        writeln();
        writeln("meanTest:");
        writeln(meanTest);
        writeln();
        writeln("symmatTest:");
        print_matrix(symmatTest, m_dim, m_dim);
        writeln();
    }
}

proc main() {
    if (M != N) {
        return;
    }
    
    /* Initialize the domains */
    var dom_square = {1..M, 1..N};
    var dom_linear = dom_square[1, 1..M];
    
    if dist == "NONE" {
        var user_dist_square = dom_square;
        var user_dist_linear = dom_linear;
        /* Run the benchmark */
        kernel_covariance(user_dist_square, user_dist_linear, M, N); 
    } else if dist == "CM" {
        var user_dist_square = dom_square dmapped CyclicZipOpt(startIdx=dom_square.low);
        var user_dist_linear = dom_linear dmapped CyclicZipOpt(startIdx=dom_linear.low);
        kernel_covariance(user_dist_square, user_dist_linear, M, N);   
    } else if dist == "C" {
        var user_dist_square = dom_square dmapped Cyclic(startIdx=dom_square.low);
        var user_dist_linear = dom_linear dmapped Cyclic(startIdx=dom_linear.low);
        kernel_covariance(user_dist_square, user_dist_linear, M, N); 
    } else if dist == "B" {
        var user_dist_square = dom_square dmapped Block(boundingBox=dom_square);
        var user_dist_linear = dom_linear dmapped Block(boundingBox=dom_linear);
        kernel_covariance(user_dist_square, user_dist_linear, M, N);  
    } 
}