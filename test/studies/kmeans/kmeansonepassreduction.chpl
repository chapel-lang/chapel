use Time;
/*One pass Reduction version of K-means from the former version*/

/*Definition of variables*/

config const n: int = 100;
config const k: int = 10;
config const m: int = 3;
config const t: real = 1e-4;

config const printTiming = true;

record Data{
var countNO: int = 0;
var dim: [1..m] int = 0;
}

var data1: [1..n] Data;

for ii in 1..n do
{
    data1[ii].countNO = ii;
    for jj in 1..m do
    {
        if (jj == 1 ) 
        { 
            data1[ii].dim[jj] = ii;
        }
        else 
        {
            data1[ii].dim[jj] = 1;
        }
    }
}

var c:[1..k] Data;
var error, old_error, t1: real;


/*Reduction Object*/

record redObj{
var error : real = 0;
var counts: [1..k] int = 0;
var c1:[1..k] Data;
}

//var RO: redObj;
var testRedObj: redObj;



/*initialization*/

var hh:int = 1;
for ii in 1..k
{
    c[ii].countNO = hh;

    for jj in 1..m
    {
        c[ii].dim[jj] = data1[hh].dim[jj];
    }

    hh = hh + n/k;
}


/*Reduction class*/

class kmeansReduction : ReduceScanOp{

type eltType;
var RO : redObj;

proc identity
{
    return new redObj();
}

proc accumulateOntoState (ref RO: redObj, da: eltType)
{
    //find nearest Cluster for this point
    
    var min_distance: real = max(real);
    var min_disposition: int = 1;

    for i in 1..k
    {
        var distance: real = 0;
        for j in 1..m
        {
            distance = distance + (da.dim[j] - c[i].dim[j])*(da.dim[j] - c[i].dim[j]);
        }

        if (distance < min_distance)
        {
            min_disposition = i;
            min_distance = distance;
        }
    }

    //Add the result into reduction object

    RO.error = RO.error + min_distance;

    //writeln(min_disposition);
    
    RO.counts[min_disposition] = RO.counts[min_disposition] + 1;
    
    for j in 1..m
    {
        RO.c1[min_disposition].dim[j] = RO.c1[min_disposition].dim[j] + da.dim[j];
    }
}

proc accumulate (da: eltType)
{
    accumulateOntoState(RO, da);
}

proc accumulate(other: RO)
{
    RO.counts += other.counts;
    RO.error += other.error;
    
    for i in 1..k
    {
        for j in 1..m
        {
            RO.c1[i].dim[j] += other.c1[i].dim[j];
        }
    }
}

proc combine(km: borrowed kmeansReduction(eltType))
{
    accumulate(km.RO);
}

proc generate()
{
    var RO1:redObj;
    RO1 = RO;
    return RO1;
}

proc clone()
{
    return new unmanaged kmeansReduction(eltType=eltType);
}
}


/*outside loop and update centroid*/
//Main function, in order to make it easier, comment out the outside loop firstly.

//do{
    old_error = error;
    error = 0;

    //used to identify where should we insert a timer.
    writeln("start reduce");
    var OnePassRedObj: redObj;
    const startTime = getCurrentTime();
    OnePassRedObj = kmeansReduction reduce data1;
    const endTime = getCurrentTime() - startTime;

    write("finish reduce");
    if printTiming then
      write(": ", endTime);
     writeln();

    testRedObj = OnePassRedObj;
    

    for ii in 1..k
    {
        for jj in 1..m
        {
            if(OnePassRedObj.counts[ii] != 0)
            {
                c[ii].dim[jj] = OnePassRedObj.c1[ii].dim[jj]/OnePassRedObj.counts[ii];
            }
            else
            {
                c[ii].dim[jj] = OnePassRedObj.c1[ii].dim[jj];
            }
        }
    }

    error = OnePassRedObj.error;

    
    if(error > old_error)
    {
        t1 = error - old_error;
    }
    else
    {
        t1 = old_error - error;
    }
//}
//while (t1 > t);

writeln(c);
writeln(testRedObj.counts);
writeln(testRedObj.c1);
writeln(testRedObj.error);
