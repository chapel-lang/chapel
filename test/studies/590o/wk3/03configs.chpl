/* 
 * config parameters can be set on the compiler command line using
 * -s<var>=<val>
 */

config param intSize = 32,
             debug = false;


/*
 * config constants and variables can be set on the executable
 * command-line using --<var>=<val> or -s<var>=<val>
 */

config const filename = "data.in",
             probSize = 100;

config var firstIteration: int;


/* 
 * set up a type alias using a configuration parameter to specify the
 * width 
 */
type intType = int(intSize);


proc main() {
  /*
   * eliminate conditional code at compile-time using configuration
   * parameters
   */
  if (debug) then
    writeln("Started exeuting main");

  /* 
   * A's size controlled by a configuration constant (could be a var
   * or param as well)
   */
  var A: [1..probSize] intType;

  writeln("If I wasn't so lazy, I'd read in A's values from ", filename);

  /* configuration variables can be changed like other variables */
  if (firstIteration < 1) then firstIteration = 1;

  /* control loop using our configuration var */
  for i in firstIteration..probSize do
    writeln("A(", i, ") = ", A(i));
  
  if (debug) then
    writeln("Exiting main");
}
