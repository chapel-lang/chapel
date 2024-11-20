use Time;
var watch: Timer;

config const rows = 100;             // number of rows in matrix
config const cols = 100;             // number of columns in matrix
config const niter = 500;            // number of iterations
config const x = 50;                 // row number of the desired position
config const y = 50;                 // column number of the desired position
config const mindif = 0.0001; real;  // smallest difference in temperature that
                                     // would be accepted before stopping

// this is our plate
var temp: [0..rows+1, 0..cols+1] real = 25;

writeln('This simulation will consider a matrix of ', rows, ' by ', cols, ' elements.');
writeln('Temperature at start is: ', temp[x, y]);

watch.start();
//this is the main loop of the simulation
var c = 0;
curdif = mindif;
while (c < niter && curdif >= mindif) do
{
  c += 1;
  // Calculate the new tempartures (temp) using the past temperatures
  // (past_temp)

  //do this for every row
  for i in 1.. rows do
  {
    // do this for every row
    for j in 1..cols do
    {
      // and this for every colum in the row i
      temp[i,j] = (past_temp[i-1,j]+past_temp[i+1,j]+past_temp[i,j-1]+
                   past_temp[i,j+1])/4;
      tt = abs(temp[i,j] - past_temp[i,j]);
      if tt>curdif then curdif=tt;
    }
  }
  past_temp = temp;

  if (c % 20 == 0)
  {
    writeln('Temperature at iteration ', c, ': ', temp[x, y]);
  }
}

watch.stop();
// print final information
writeln('\nThe simulation took ',watch.elapsed(),' seconds');
writeln('Final temperature at the desired position after ',c,' iterations is: ',temp[x,y]);
writeln('The greatest difference in temperatures between the last two iterations was: ',curdif,'\n');
