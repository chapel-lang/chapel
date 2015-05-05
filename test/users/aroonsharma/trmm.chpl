use CyclicZipOpt;
use BlockDist;
use Time;
use CommDiagnostics;

/****************************
	printData: Set to false if you don't want to see the arrays printed
		Default = false
	Dim: dimension (square) of the distribution 
		Default = 128
    dist: the distribution of the domain which the matrices are based on. 
        Default: cyclical with modulo unrolling
*****************************/
config var correct = false;
config var timeit = false;
config var messages = false;
config var printData: bool = false;
config var dist: string = "CM";

config var Dim: int = 128;

config var alpha: int = 323412;

/* Initializes a matrix based on a distribution */
proc initialize_matrix(distribution, dim: int) {
    var matrix: [distribution] real = 0.0;
    for (i,j) in distribution {
        matrix[i,j] = ((i - 1.00) * (j - 1.00)) / dim;
    }
    return matrix;
}

/* Prints out the matrix passed in */
proc print_matrix(A: [], dim: int) {
    for i in 1..dim {
        for j in 1..dim {
            write(A[i,j], " ");
        }
        writeln();
    }
}

/* The process which runs the benchmark */
proc kernel_trmm(dist, dim: int) {
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
	
	
    var A = initialize_matrix(dist, dim);
    var B = initialize_matrix(dist, dim);

    for (i,j) in dist {
        var temp: [1..i] real = 0;
        forall (a,b,c) in zip(A[i,1..i], B[j,1..i],1..) {
            temp[c] = alpha * a * b;
        }
        B[i,j] += (+ reduce temp);
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
	
	if correct {
	    var ATest = initialize_matrix({1..dim, 1..dim}, dim);
	    var BTest = initialize_matrix({1..dim, 1..dim}, dim);

	    for (i,j) in {1..dim, 1..dim} {
	        var tempTest: [1..i] real = 0;
	        forall (a,b,c) in zip(ATest[i,1..i], BTest[j,1..i],1..) {
	            tempTest[c] = alpha * a * b;
	        }
	        BTest[i,j] += (+ reduce tempTest);
	    }
		
		for ii in 1..dim {
			for jj in 1..dim {
				still_correct &&= (B[ii,jj] == BTest[ii,jj]);
			}
		}
		writeln("Is the calculation correct? ", still_correct);
		writeln("trmm computation complete.");
	}
    
    if (printData) {
        print_matrix(B, dim);
    }
}

proc main() {
    /* Initialize the domains */
    var dom = {1..Dim, 1..Dim};
    
    if dist == "NONE" {
        var user_dist = dom;
        /* Run the benchmark */
        kernel_trmm(user_dist, Dim); 
    } else if dist == "CM" {
        var user_dist = dom dmapped CyclicZipOpt(startIdx=dom.low);
        kernel_trmm(user_dist, Dim);   
    } else if dist == "C" {
        var user_dist = dom dmapped Cyclic(startIdx=dom.low);
        kernel_trmm(user_dist, Dim); 
    } else if dist == "B" {
        var user_dist = dom dmapped Block(boundingBox=dom);
        kernel_trmm(user_dist, Dim);  
    } 
}