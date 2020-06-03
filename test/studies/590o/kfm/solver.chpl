//------------------  Sudoku Solver -----------------------------------
//------------------ Step 1, read in the puzzle from a file ------------
// the module defining file operations
use IO;

// the name of a file holding a 9x9 soduku puzzle
config var infilename = "sudoku.txt";

// set up a table for the puzzle
var TableSpace: domain(2) = {1..9, 1..9};
var Table: [TableSpace] int;

//...then read the values from the puzzle into it
var infile = open(infilename, iomode.r);
var reader = infile.reader();

for ij in TableSpace {
   var str: string;
   reader.read(str);
   if (str == "*") {
     Table(ij) = 0;
   } else {
     Table(ij) = str:int;
   }
}

reader.close();
infile.close();


//---------------------- Print out the puzzle that was given as input -----

writeln("The Puzzle to Solve is:\n\n", Table);
writeln("------------------------------------------------------------------");
writeln("\nsolving...\n");

//-------- Define an important set function or two...  --------------------

proc setUnion(X, Y) {
   var Res: X.type;
   for i in X {
     Res += i;
   }
   for i in Y {
     Res += i;
   }
   return Res;
}


proc setIntersect(X, Y) {
   var Res: X.type;
   for i in X {
     if (Y.contains(i)) {
       Res += i;
     }
   }
   return Res;
}

//--------- Next, get the available values for each row, column, and box

const All: domain(int) = (1, 2, 3, 4, 5, 6, 7, 8, 9);
var A, B, C: domain(int);

var rowRemainders: [1..9] domain(int) = All;
var colRemainders: [1..9] domain(int) = All;
var boxRemainders: [1..9] domain(int) = All;
// ----- Initialize the values we still need for each row ----
for i in 1..9 { 
    for v in Table[i, ..] {
    	if (v != 0) {
     	  rowRemainders[i] -= v;
        }
   }
}

// ----- Initialize the values we still need for each column ----
for i in 1..9 { 
    for v in Table[.., i] {
    	if (v != 0) {
     	  colRemainders[i] -= v;
        }
   }
}

// ----- Initialize the values we still need for each box ----
var BoxDomains: [1..9] domain(2);
var Box1: domain(2) = {1..3, 1..3};
var Box2: domain(2) = {1..3, 4..6};
var Box3: domain(2) = {1..3, 7..9};
var Box4: domain(2) = {4..6, 1..3};
var Box5: domain(2) = {4..6, 4..6};
var Box6: domain(2) = {4..6, 7..9};
var Box7: domain(2) = {7..9, 1..3};
var Box8: domain(2) = {7..9, 4..6};
var Box9: domain(2) = {7..9, 7..9};
BoxDomains[1] = Box1;
BoxDomains[2] = Box2;
BoxDomains[3] = Box3;
BoxDomains[4] = Box4;
BoxDomains[5] = Box5;
BoxDomains[6] = Box6;
BoxDomains[7] = Box7;
BoxDomains[8] = Box8;
BoxDomains[9] = Box9;
for i in 1..9 {
    var currDom = Table[BoxDomains[i]];
    for j in currDom {
        if (j != 0) {
	   boxRemainders[i] -= j;
	 }
    }
    // DEBUG: writeln("Box #", i, " is:\n", Table[BoxDomains[i]]);
}

proc getBoxIndex(i, j) {
    var bI: int = 0;    
    if (i <= 3) {
      if (j <= 3) {
        bI = 1;
      }
      else if (6 < j) then
	bI = 3;
      else bI = 2;
    }    

    if (6 < i) {
      if (j <= 3) {
        bI = 7;
      }
      else if (6 < j) then
	bI = 9;
      else bI = 8;
    }    

    if ((3 < i) && (i < 7)) {
      if (j <= 3) {
        bI = 4;
      }
      else if (6 < j) then
	bI = 6;
      else bI = 5;
    }    
    return bI;
}

// Termination variable
var numZeroes = + reduce (Table == 0);

while (numZeroes > 0) {
  
  //  coforall (i,j) in TableSpace {
  forall (i,j) in TableSpace {
    if (Table[i, j] == 0) {
        // initialize the current set of available numbers
    	var currSet: domain(int);


      	var boxIndex: int = getBoxIndex(i, j);
    	// Then do an intersection for all three possible constraints
    	currSet =  setIntersect(rowRemainders[i], colRemainders[j]);
    	currSet = setIntersect(currSet, boxRemainders[boxIndex]);
    	if (currSet.size == 1) {
	   // TODO: we know there is only 1 element, 
 	   //       is there another way to get v?

           for v in currSet { 

	      if (v != 0) {
	            
		Table[i, j]= v;
              	rowRemainders[i] -= v;
              	colRemainders[j] -= v;
	      	boxRemainders[boxIndex] -= v;
	     }
       	   }
         }
      }
   }
  numZeroes = + reduce (Table == 0);
}

// YAY!  Output the final result!
writeln("Solved Puzzle is:\n", Table);
