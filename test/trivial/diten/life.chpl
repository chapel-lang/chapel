
// Define constants for indexing purposes
const north = (-1, 0);
const east  = (0, 1);
const west  = (0, -1);
const south = (1, 0);
const nw = north + west;
const ne = north + east;
const sw = south + west;
const se = south + east;

config const size = 20: int; // size of the square life game to simulate
config const iters = 36: int;

var full_domain = {0..size, 0..size};
var life_domain = {1..size-1, 1..size-1};

proc main(){
  var life: [full_domain] int;
  var tmp: [life_domain] int;

  const base= (10, 10);
  // A simple diagonal glider
  life(base) = 1;
  life(base + north) = 1;
  life(base + east) = 1;
  life(base + se) = 1;
  life(base + sw) = 1;

  writeln(life);  
  writeln();
  var iter_count = 0;
  do {
    [ij in life_domain] {
      var neighbors: int;

      neighbors = life(ij+north) + life(ij+east)  + 
                  life(ij+west)  + life(ij+south) +
                  life(ij+nw)    + life(ij+ne)    +
                  life(ij+sw)    + life(ij+se); 

      if life(ij) == 1 then // cell ij is currently alive
        if neighbors == 2 || neighbors == 3 then
          tmp(ij) = 1; // cell remains alive
        else
          tmp(ij) = 0; // cell dies
      else
        if neighbors == 3 then 
          tmp(ij) = 1; // cell is born
        else
          tmp(ij) = 0; // cell is not born
    }
    [ij in life_domain] life(ij) = tmp(ij);
    writeln(life);
    writeln();
    iter_count += 1;
  } while iter_count < iters;
}

proc +(a: (int, int), b: (int, int)){
  return (a(0) + b(0), a(1) + b(1));
}
