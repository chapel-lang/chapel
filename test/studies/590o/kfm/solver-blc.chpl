//------------------  Sudoku Solver -----------------------------------
//------------------ Step 1, read in the puzzle from a file ------------
// the module defining file operations
use IO;

// the name of a file holding a 9x9 soduku puzzle
config var infilename = "sudoku.txt";

// set up a table for the puzzle
const tableSize = 1..9;
const TableSpace: domain(2) = {tableSize, tableSize};
const Boxes: domain(2) = {1..3, 1..3};
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

var rowRemainders: [tableSize] domain(int) = All;
var colRemainders: [tableSize] domain(int) = All;
var boxRemainders: [Boxes] domain(int) = All;

// ----- Initialize the values we still need for each row ----
forall (i,j) in TableSpace {
  const v = Table(i,j);
  if (v != 0) {
    rowRemainders[i] -= v;
    colRemainders[j] -= v;
    boxRemainders[getBoxIndex((i,j))] -= v;
  }
}

// ----- Initialize the values we still need for each box ----
// NOTE that this is no longer used; still, it's cool
const BoxDomains: [Boxes] subdomain(TableSpace) 
                = [(br, bc) in Boxes] {(br-1)*3 + 1 .. br*3, (bc-1)*3+1..bc*3};
// DEBUG: writeln("BoxDomains = ", BoxDomains);

proc getBoxIndex(ij) {
  return (ij - (1,1)) / (3,3) + (1,1);
}

// Termination variable
var numZeroes = + reduce (Table == 0);

while (numZeroes > 0) {
  
  //  coforall (i,j) in TableSpace {
  forall (i,j) in TableSpace {
    if (Table[i, j] == 0) {
        // initialize the current set of available numbers
    	var currSet: domain(int);


        var boxIndex = ((i, j) - (1, 1)) / (3, 3) + (1, 1);
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
