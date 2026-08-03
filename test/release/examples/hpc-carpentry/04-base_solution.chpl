const rows = 100;             // number of rows in matrix
const cols = 100;             // number of columns in matrix
const niter = 500;            // number of iterations
const x = 50;                 // row number of the desired position
const y = 50;                 // column number of the desired position
const mindif = 0.0001; real;  // smallest difference in temperature that would be 
                              // accepted before stopping

// this is our plate
var temp: [0..rows+1, 0..cols+1] real = 25;

writeln('This simulation will consider a matrix of ', rows, ' by ', cols, ' elements.');
writeln('Temperature at start is: ', temp[x, y]);

//this is the main loop of the simulation
var c = 0;
while (c < niter) do
{
  c += 1;
  if (c % 20 == 0)
  {
    writeln('Temperature at iteration ', c, ': ', temp[x, y]);
  }
}
