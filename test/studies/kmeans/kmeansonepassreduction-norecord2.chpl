use Time;

/*One pass Reduction version of K-means from the former version*/

/*Definition of variables*/

config const n: int = 100;
config const k: int = 10;
config param m: int = 3;
config const t: real = 1e-4;

config const printTiming = true;

var data1: [1..n] m*int;

for ii in 1..n do
{
    for param jj in 1..m do
    {
        if (jj == 1 ) 
        { 
            data1[ii][jj] = ii;
        }
        else 
        {
            data1[ii][jj] = 1;
        }
    }
}

var c:[1..k] m*int;
var old_error, t1: real;


/*initialization*/

var hh:int = 1;
for ii in 1..k
{
    for param jj in 1..m
    {
        c[ii][jj] = data1[hh][jj];
    }

    hh = hh + n/k;
}


/*Reduction class*/

class kmeansReduction : ReduceScanOp{

type eltType;
var error : real = 0;
var counts: [1..k] int = 0;
var c1:[1..k] m*int;

proc accumulate (da: eltType)
{
    //find nearest Cluster for this point
    
    var min_distance: real = max(real);
    var min_disposition: int = 1;

    for i in 1..k
    {
        var distance: real = 0;
        for param j in 1..m
        {
            distance = distance + (da[j] - c[i][j])*(da[j] - c[i][j]);
        }

        if (distance < min_distance)
        {
            min_disposition = i;
            min_distance = distance;
        }
    }

    //Add the result into reduction object

    error = error + min_distance;

    counts[min_disposition] = counts[min_disposition] + 1;
    
    for param j in 1..m
    {
        c1[min_disposition][j] = c1[min_disposition][j] + da[j];
    }
}

proc combine(km: borrowed kmeansReduction(eltType))
{
    counts = counts + km.counts;
    error = error + km.error;
    
    for i in 1..k
    {
        for param j in 1..m
        {
            c1[i][j] = c1[i][j] + km.c1[i][j];
        }
    }
}

proc generate()
{
  return (error, counts, c1);
}
}


/*outside loop and update centroid*/
//Main function, in order to make it easier, comment out the outside loop firstly.

//do{

    //used to identify where should we insert a timer.
    writeln("start reduce");
    const startTime = getCurrentTime();
    var (error, counts, c1)  = kmeansReduction reduce data1;
    const endTime = getCurrentTime() - startTime;
    
    write("finish reduce");
    if printTiming then
      write(": ", endTime);
     writeln();


    for ii in 1..k
    {
        for param jj in 1..m
        {
            if(counts[ii] != 0)
            {
                c[ii][jj] = c1[ii][jj]/counts[ii];
            }
            else
            {
                c[ii][jj] = c1[ii][jj];
            }
        }
    }

    if(error > old_error)
    {
        t1 = error - old_error;
    }
    else
    {
        t1 = old_error - error;
    }
    old_error = error;
//}
//while (t1 > t);

writeln(c);
writeln(counts);
writeln(c1);
writeln(error);
