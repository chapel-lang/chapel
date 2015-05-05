//use CyclicZipOpt;
use CyclicDist;
use BlockDist;
use BlockCycDist;
//use MyBlockCyclic;
use Time;
use CommDiagnostics;

/****************************
    Dimensions are set up to be M.
    M: 1st dimension
        Default = 10000

    TSTEPS: The number of iterations to perform
        Default = 100

	printData: Set to false if you don't want to see the data printed
		Default = false
    dist: the distribution of the domain which the matrices are based on. 
        Default: cyclical with modulo unrolling
*****************************/
config var correct = false;
config var timeit = false;
config var messages = false;
config var M: int = 10000;
config var bsize = 4;

config var TSTEPS: int = 10;

config var printData: bool = false;
config var dist: string = "C";

/* Initializes a 1D structure */
proc initialize_1D(distribution, adder: int, divider: int) {
    var array: [distribution] real = 0.0;
    forall i in distribution {
        array[i] = (i - 1.0 + adder) / divider;
    }
    return array;
}

/* Prints out the 1D structure passed in */
proc print_1D(A: []) {
    writeln(A);
}

/* The process which runs the benchmark */
proc kernel_jacobi1d(dist_1D, m_dim: int) {
	var still_correct = true;
	var t:Timer;
	
	if messages {
	    resetCommDiagnostics();
	    startCommDiagnostics();
	}
	
	if timeit {
		t = new Timer();
		t.start();
	}
    var A = initialize_1D(dist_1D, 2, m_dim);
    var B = initialize_1D(dist_1D, 3, m_dim);
    
    const left_stencil = {1..m_dim - 2};
    const curr_stencil = {2..m_dim - 1};
    const right_stencil = {3..m_dim};

    for t in 1..TSTEPS {
        forall (a, b, c, d) in zip(B(curr_stencil), A(left_stencil), A(curr_stencil), A(right_stencil)) {
            a = (b + c + d) * (0.33333);
        }
        A(curr_stencil) = B(curr_stencil);
    }
	
	if timeit {
		t.stop();
		writeln("took ", t.elapsed(), " seconds");
	}
	
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
    
    if (printData) {
        writeln("A:");
        print_1D(A);
    }
	
	if correct {
	    var Atest = initialize_1D({1..m_dim}, 2, m_dim);
	    var Btest = initialize_1D({1..m_dim}, 3, m_dim);
		
	    for t in 1..TSTEPS {
	        forall (a, b, c, d) in zip(Btest(curr_stencil), Atest(left_stencil), Atest(curr_stencil), Atest(right_stencil)) {
	            a = (b + c + d) * (0.33333);
	        }
	        Atest(curr_stencil) = Btest(curr_stencil);
	    }
		
		for ii in 1..m_dim {
			still_correct &&= Atest[ii] == A[ii];
		}
		writeln("Is the calculation correct? ", still_correct);
		writeln("jacobi-1d computation complete");
	}
}

proc main() {
    /* Initialize the domains */
    var dom_1D = {1..M};
    
    if dist == "NONE" {
        var dist_1D = dom_1D;
        /* Run the benchmark */
        kernel_jacobi1d(dist_1D, M); 
    } /*else if dist == "CM" {
        var dist_1D = dom_1D dmapped CyclicZipOpt(startIdx=dom_1D.low);
        kernel_jacobi1d(dist_1D, M);
    } */else if dist == "C" {
        var dist_1D = dom_1D dmapped Cyclic(startIdx=dom_1D.low);
        kernel_jacobi1d(dist_1D, M); 
    } else if dist == "B" {
        var dist_1D = dom_1D dmapped Block(boundingBox=dom_1D);
        kernel_jacobi1d(dist_1D, M);
    } else if dist == "BC" {
        var dist_1D = dom_1D dmapped BlockCyclic(startIdx = dom_1D.low, blocksize=bsize);
        kernel_jacobi1d(dist_1D, M);
    } /*else if dist == "BCM" {
        var dist_1D = dom_1D dmapped MyBlockCyclic(startIdx = dom_1D.low, blocksize=bsize);
        kernel_jacobi1d(dist_1D, M);
    }*/
}