const rows = 100;             // number of rows in matrix
const cols = 100;             // number of columns in matrix
const niter = 500;            // number of iterations
const x = 50;                 // row number of the desired position
const y = 50;                 // column number of the desired position
var curdif: real;             // here we will store the greatest difference in
                              // temperature from one iteration to another
var tt: real;                 // for temporary results when computing the temperatures
const mindif = 0.0001; real;  // smallest difference in temperature that would be 
                              // accepted before stopping
const n = 20: int;            // the temperature at the desired position will be printed
                              // every n iterations

