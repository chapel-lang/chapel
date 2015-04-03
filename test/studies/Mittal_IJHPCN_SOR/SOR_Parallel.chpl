//Author Sparsh Mittal sparsh0mittal@gmail.com
//Research paper reference: Sparsh Mittal, "A Study of Successive Over-relaxation Method
//Parallelization Over Modern HPC Languages", International Journal of High
//Performance Computing and Networking, vol. 7, number  4, pp. 292-298, 2014.

 
 
 use Time;

config const gridSize: int = 4096;
config const INITIAL_GUESS: real = 0.0;
config const MAXSTEPS: int  = 50000;  
config const TOL_VAL: real =0.00001;
config const NUM_CHECK: int =4000;

config const omega: real =  0.376;
config const one_minus_omega: real =  1-omega;

config const numberOfSlaves: int = 2;
config const factorValue: int = gridSize / numberOfSlaves; 

var gridInfo: [1..(gridSize+2),1..(gridSize+2)] real ;
var gridInfoOld: [1..(gridSize+2),1..(gridSize+2)] real ;


proc MAX_FUNC(a: real, b: real): real {

	if (a > b) {
		return a;
}
	return b;


}

proc ABS_VAL(a: real): real {

	if (a > 0) {
		return a;
}
	return -a;
}


proc sorSolverSlaveRed(myNumberOne: int) {

	
	var sum: real = 0;

        var myNumber: int = myNumberOne-1;

	var iStart: int = (myNumber * factorValue) + 2; 
	var iEnd: int = ((myNumber + 1) * factorValue)+1; 

        //writeln(" factorValue ", factorValue);
        //writeln(" For slave ", myNumber , " iStart ", iStart, " iEnd ", iEnd);
	        

       //RED i even, j even
       for i in (iStart)..(iEnd) by 2 {
           for j in 2..(gridSize+1) by 2 {
           
              
              //writeln("RedEven ", i, " ", j);           
               
           
               sum = (gridInfo(i,j+1) + gridInfo(i+1,j) + gridInfo(i-1,j) + gridInfo(i,j-1)) * 0.25;       	  
	       gridInfo(i,j) = one_minus_omega*gridInfo(i,j) + omega*sum;
         }
        }
    
         //RED i odd, j odd
       for i in (iStart+1)..(iEnd) by 2 {
           
           for j in 3..(gridSize+1) by 2 {
           
               
              // writeln("RedOdd ", i, " ", j);
               
               sum = (gridInfo(i,j+1) + gridInfo(i+1,j) + gridInfo(i-1,j) + gridInfo(i,j-1)) * 0.25;       	       
	       gridInfo(i,j) = one_minus_omega*gridInfo(i,j) + omega*sum;
          }
         }



}






proc sorSolverSlaveBlack(myNumberOne: int) {

       var myNumber: int = myNumberOne-1;
   var sum: real = 0;

  var iStart: int = (myNumber * factorValue) + 2;
  var iEnd: int = ((myNumber + 1) * factorValue)+1;
 
       //Black  i even, j odd
       for i in (iStart)..(iEnd) by 2 {
           for j in 3..(gridSize+1) by 2 {
           
               
               //writeln("BlackEven ", i, " ", j);
               
               sum = (gridInfo(i,j+1) + gridInfo(i+1,j) + gridInfo(i-1,j) + gridInfo(i,j-1)) * 0.25;       	       
	       gridInfo(i,j) = one_minus_omega*gridInfo(i,j) + omega*sum;
}
}
    
         //Black i odd, j even
       for i in (iStart+1)..(iEnd) by 2 {
           for j in 2..(gridSize+1) by 2 {
           
               
               //writeln("BlackOdd ", i, " ", j);
               
               sum = (gridInfo(i,j+1) + gridInfo(i+1,j) + gridInfo(i-1,j) + gridInfo(i,j-1)) * 0.25;       	       
	       gridInfo(i,j) = one_minus_omega*gridInfo(i,j) + omega*sum;
}
}

	
         

}




//main starts here
writeln("GridSize ",gridSize, " MaxStep ", MAXSTEPS, " Accuracy ", TOL_VAL );

var maxError: real = 0.0; 
var hasConverged: bool = false;

for i in 1..(gridSize+2) {
  for j in 1..(gridSize+2) {
              if (i == 1) {
				gridInfo(i,j) = 1.0;
              } else {
				gridInfo(i,j) = INITIAL_GUESS;
              }
  }
}

var sum: real = 0.0;

var timeMeasure:Timer;

timeMeasure.start();
for k in 1..(MAXSTEPS) {


 maxError = 0.0;
        if (k%NUM_CHECK == 0) {
          for i in 2..(gridSize+1)  {
           for j in 2..(gridSize+1) {
             
	       gridInfoOld(i,j) = gridInfo(i,j);
              }
             }        
       }
 
       sync {
            for p in 1..numberOfSlaves {
             begin sorSolverSlaveRed(p);
             }   
      }
       
       sync {
            for p in 1..numberOfSlaves {
             begin sorSolverSlaveBlack(p);
          }   
         }

      if (k%NUM_CHECK == 0) {
      
      for i in 2..(gridSize+1)  {
           for j in 2..(gridSize+1) {
             
	  maxError = MAX_FUNC (ABS_VAL (gridInfoOld(i,j) - gridInfo(i,j)), maxError);   
         }
         }
			writeln("Iter ", k, " Error ", maxError);
			if (maxError < TOL_VAL) {
				hasConverged = true;
				break;
   } 
   }

}

timeMeasure.stop();
writeln(" Time elapsed ", timeMeasure.elapsed());


if( hasConverged) {
writeln("It converged");
} else {
writeln("It did not converge");
}

