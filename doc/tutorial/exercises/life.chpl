//
// Life Exercise
//
// Simulate the Game of Life for a configurable number of generations
// or until a stable state has been reached.
//

// import standard modules to generate random number and use timers
use Random, Time;

// configuration constants
config const printGenerations: bool = true, // print grid at each iteration
             n:                int = 20,    // size of grid
             p:                int = 20,    // initial percentage of live cells
             k:                int = 10;    // maximum number of generations

// global constants and variables
const BigD = [0..n+1, 0..n+1], // domain of grid with border cells
      D = [1..n, 1..n];        // domain of grid without border cells
var Grid:     [BigD] bool, // grid of life
    NextGrid: [D]    bool; // grid for next iteration

// initialize grid
var rs = new RandomStream();
for i in D do
  Grid(i) = if rs.getNext() <= p:real / 100 then true else false;

writeln("Initial Grid");
          printGrid();

//
// Iterate according to the Game of Life until a stable state has been
// reached, max k generations.
//
for i in 1..k {
  // your code here
}

// print a generation
def printGrid() {
  write("+"); for i in 1..n do write("-"); writeln("+");
  for i in D.dim(1) {
    write("|");
    for j in D.dim(2) {
      write(if Grid(i,j) then "o" else " ");
    }
    writeln("|");
  }
  write("+"); for i in 1..n do write("-"); writeln("+");
}
