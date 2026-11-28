use Time;
var watch: Timer;

config const rowtasks = 2;           // number of tasks per row
config const coltasks = 2;           // number of tasks per column
config const rows = 100;             // number of rows in matrix
config const cols = 100;             // number of columns in matrix
config const niter = 500;            // number of iterations
config const x = 50;                 // row number of the desired position
config const y = 50;                 // column number of the desired position
config const mindif = 0.0001; real;  // smallest difference in temperature that
                                     // would be accepted before stopping

// Data distribution
const nr = rows/rowtasks;
const rr = rows-nr*rowtasks;
const nc = cols/coltasks;
const rc = cols-nc*coltasks;

// this is our plate
var temp: [0..rows+1, 0..cols+1] real = 25;

writeln('This simulation will consider a matrix of ', rows, ' by ', cols, ' elements.');
writeln('Temperature at start is: ', temp[x, y]);

watch.start();
//this is the main loop of the simulation
var c = 0;
var curdif:atomic real;
var myd: [0..coltasks*rowtasks-1] real;
curdif.write(mindif);

corofall taskid in 0..coltasks*rowtasks-1 do {
  var rowi, coli, rowf, colf: int;
  var taskr, taskc: int;
  var myd2: real;

  taskr = taskid/coltasks;
  taskc = taskid%coltasks;

  if taskr<rr then {
    rowi=(taskr*nr)+1+taskr;
    rowf=(taskr*nr)+nr+taskr+1;
  }
  else {
    rowi = (taskr*nr)+l+rr;
    rowf = (taskr*nr)+nr+rr;
  }

  if taskc<rc then {
    coli = (taskc*nc)+1+taskc;
    colf = (taskc*nc)+nc+taskc+1;
  }
  else {
    coli = (taskc*nc)+l+rc;
    colf = (taskc*nc)+nc+rc;
  }
  while (c < niter && curdif >= mindif) do
  {
    c += 1;
    // Calculate the new tempartures (temp) using the past temperatures
    // (past_temp)

    //do this for every row
    for i in rowi.. rowf do
    {
      // do this for every row
      for j in coli..colf do
      {
        // and this for every colum in the row i
        temp[i,j] = (past_temp[i-1,j]+past_temp[i+1,j]+past_temp[i,j-1]+
                     past_temp[i,j+1])/4;
        myd2 = abs(temp[i,j] - past_temp[i,j]);
      }
    }
    myd[taskid] = myd2;
    // here comes the synchronisation of tasks
    lock.add(1);
    lock.waitFor(coltasks*rowtasks);

    // here comes the synchronisation of tasks
    past_temp[rowi..rowf,coli..colf] = temp[rowi..rowf,coli..colf];
    if taskid == 0 then {
      curdif.write(max reduce myd);
      if (c % n == 0) then {
        writeln('Temperature at iteration ', c, ': ', temp[x, y]);
      }
    }
    // here comes the synchronisation of tasks again
    lock.add(1);
    lock.waitFor(coltasks*rowtasks);
  }
}

watch.stop();
// print final information
writeln('\nThe simulation took ',watch.elapsed(),' seconds');
writeln('Final temperature at the desired position after ',c,' iterations is: ',temp[x,y]);
writeln('The greatest difference in temperatures between the last two iterations was: ',curdif,'\n');
