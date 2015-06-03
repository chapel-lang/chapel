/*use CyclicZipOpt;*/
use BlockDist;
use BlockCycDist;
use Time;
use CommDiagnostics;

/****************************
	printMatrices: Set to false if you don't want to see the matrices printed
		Default = false
	alpha: constant by which to multiply the D matrix entries by
		Default = 1
	beta: contant by which to multiply the E matrix entries by
		Default = 1
	size: size (square) of the matrices
		Default = 128
    dist: the distribution of the domain which the matrices are based on. 
        Default: cyclical with modulo unrolling
*****************************/
config var correct = false;
config var timeit = false;
config var messages = false;
config var printMatrices: bool = false;
config var alpha: int = 1;
config var beta: int = 1;
config var size: int = 128;
config var dist: string = "C";

/* Map the distribution according to user input. Currently not used */
proc map_distribution(size:int): domain(2) {
    var dom: domain(2) = {1..size, 1..size};
    var user_dist: domain(2);
    if dist == "NONE" {
    	user_dist = dom;
    /*} else if dist == "CM" {
    	user_dist = dom dmapped CyclicZipOpt(startIdx=dom.low);*/
    } else if dist == "C" {
    	user_dist = dom dmapped Cyclic(startIdx=dom.low);
    } else if dist == "B" {
    	user_dist = dom dmapped Block(boundingBox=dom);
    } 
    return user_dist;
}

/* Initializes a matrix based on a domain distribution (might be mapped)*/
proc initialize_matrix(distribution, matrix_size: int, adder: int) {
    var matrix: [distribution] real = 0.0;
    forall (i,j) in distribution {
        matrix[i,j] = i * (j + adder) / matrix_size;
    }
    return matrix;
}

/* The process which runs the benchmark */
proc kernel_2mm(alpha: int, beta: int, distribution, matrix_size: int) {
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
	
    var A = initialize_matrix(distribution, matrix_size, 0);
    var B = initialize_matrix(distribution, matrix_size, 1);
    var C = initialize_matrix(distribution, matrix_size, 2);
    /* The matrices that will be the result of the operation */
    var D, E: [distribution] real = 0.0;

    forall (i,j) in distribution {
        var tempArray: [1..matrix_size] real;
        forall (b, a, k) in zip(B[1..matrix_size, j], A[i, 1..matrix_size], 1..) {
            var temp = a * b;
            tempArray[k] = temp;
        }
        D[i,j] = alpha * (+ reduce (tempArray));
    }
    
    forall (i,j) in distribution {
        var tempArray: [1..matrix_size] real;
        forall (d, c, k) in zip(D[1..matrix_size, j], C[i, 1..matrix_size], 1..) {
			var temp = c * d;
			tempArray[k] = temp;
        }
        E[i,j] = beta * (+ reduce (tempArray));
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
    
	//Print out results
    if (printMatrices) {
        writeln("A:");
        print_matrix(A, matrix_size);
        writeln();
        print_locale_data(A, matrix_size);
        writeln();
        writeln("B:");
        print_matrix(B, matrix_size);
        writeln();
        print_locale_data(B, matrix_size);
        writeln();
        writeln("C:");
        print_matrix(C, size);
        writeln();
        print_locale_data(C, matrix_size);
        writeln();
        writeln("D:");
        print_matrix(D, matrix_size);
        writeln();
        print_locale_data(D, matrix_size);
        writeln();
        writeln("E:");
        print_matrix(E, size);
        writeln();
        print_locale_data(E, matrix_size);
        writeln();
    }  
	
	//confirm correctness of calculation
	if correct {
		//Matrices to test correctness of calculation
		var Atest = initialize_matrix({1..size, 1..size}, matrix_size, 0);
		var Btest = initialize_matrix({1..size, 1..size}, matrix_size, 1);
		var Ctest = initialize_matrix({1..size, 1..size}, matrix_size, 2);
		var Dtest: [{1..size, 1..size}] real = 0.0;
		var Etest: [{1..size, 1..size}] real = 0.0;
		
	    forall (i,j) in {1..matrix_size, 1..matrix_size} {
	        var tempArray: [1..matrix_size] real;
	        forall (a, b, k) in zip(Atest[i, 1..matrix_size], Btest[1..matrix_size, j], 1..) {
	            var temp = a * b;
	            tempArray[k] = temp;
	        }
	        Dtest[i,j] = alpha * (+ reduce (tempArray));
	    }
    
	    forall (i,j) in {1..matrix_size, 1..matrix_size} {
	        var tempArray: [1..matrix_size] real;
	        forall (c, d, k) in zip(Ctest[i, 1..matrix_size], Dtest[1..matrix_size, j], 1..) {
				var temp = c * d;
				tempArray[k] = temp;
	        }
	        Etest[i,j] = beta * (+ reduce (tempArray));
	    }
		
		for ii in 1..matrix_size {
			for jj in 1..matrix_size {
				still_correct &&= E[ii,jj] == Etest[ii,jj];
			}
		}
		writeln("Is the calculation correct? ", still_correct);
		writeln("2mm computation complete.");
	}
}

/* Prints out the square matrix passed in */
proc print_matrix(A: [], matrix_size: int) {
    for i in 1..matrix_size {
        for j in 1..matrix_size {
            write(A[i,j], " ");
        }
        writeln();
    }
}

/* Print out locale distribution */
proc print_locale_data(A:[], matrix_size: int) {
    var count: int = 0;
    for a in A {
        count += 1;
        write(a.locale.id);
        if (count == matrix_size) {
            count = 0;
            writeln();
        }
    }
}

proc main() {
    /* Initialize the data */
    var dom = {1..size, 1..size};
	//var MyLocaleView = {1..2, 0..4};
	//var MyLocales: [MyLocaleView] locale = reshape(Locales, MyLocaleView);

    if dist == "NONE" {
        var user_dist = dom;
        /* Run the benchmark */
        kernel_2mm(alpha, beta, user_dist, size); 
    } else if dist == "CM" {
	    var user_dist = dom dmapped CyclicZipOpt(startIdx=dom.low);
	    kernel_2mm(alpha, beta, user_dist, size);   
    } else if dist == "C" {
        var user_dist = dom dmapped Cyclic(startIdx=dom.low);
        kernel_2mm(alpha, beta, user_dist, size);   
    } else if dist == "B" {
        var user_dist = dom dmapped Block(boundingBox=dom);
        kernel_2mm(alpha, beta, user_dist, size);   
    } /*else if dist == "BC" {
    	var user_dist = dom dmapped BlockCyclic(startIdx=dom.low, blocksize=(2,2));
		kernel_2mm(alpha, beta, user_dist, size);
    }*/
}
