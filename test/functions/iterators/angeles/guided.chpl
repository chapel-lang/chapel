// Centralized Guided Splitting iterator similar to guided OpenMP. 
// It splits chunks of decreasing size among the threads in a centralized way.
// When a thread finishes its assigned chunk, it gets another chunk, until none remain.
// The size of each chunk is approximately the number of unassigned iterations 
// divided by the number of threads. These sizes decrease approximately exponentially to 1

// Contributed by Angeles Navarro 

_extern proc usleep(val:uint);
config const MaxThreadsPerLocale:int=4; //here.numCores;
writeln("Working with ", MaxThreadsPerLocale, " Threads");


config const verbose:bool=false;



//Serial iterator 
iter guided(c:range) {
  var remain=c;
  var current: range;

  const splitFactor=MaxThreadsPerLocale;
  var undone=true;
  // While there are remaining iterations do spliting 
  while undone do {
      current=splitRange(remain, splitFactor, undone); 	
      if verbose then {
	writeRange(current);
      }
      for i in current do yield i;    
    }    	      
}



if verbose then {
  // Printing ranges for Serial Spliting
  writeln("Working with serial guided scheduling ");
  // The initial range
  config const lo:int=0;
  config const hi:int=63;
  config const chunksize=4;
  var r=lo..hi;
  writeln("Initial range ", r," and chunksize ", chunksize);

  for c in guided(r) do{
    //do work
  }
 }


// Parallel iterator

// Leader iterator
iter guided(param tag:iterator, c:range)
where tag == iterator.leader
{   

  var remain=c;
  

  // While there are remaining iterations do spliting 
  var splitFactor=MaxThreadsPerLocale;
  var undone=true;
  // synnc variables to control the splitting action
  var split_lock$:sync bool=true; 

  coforall tid in 0..#MaxThreadsPerLocale do { 
    var current: range;
    while undone do {
	// There is local work in remain(tid)
	split_lock$; // Perform the splitting in a critical section
	current=splitRange(remain, splitFactor, undone); 
	split_lock$=true;
	yield current;
	if verbose then {
	  writeln("Working at tid ", tid);
	  writeRange(current);
	}

      }    	      
  }


}

// Follower
iter guided(param tag:iterator, c:range, follower)
where tag == iterator.follower
{
  var current=follower;
  for i in current do {
    yield i;
  }
}


proc splitRange(inout rangeToSplit:range, splitFactor:int, inout itleft:bool):range
{
  var totlen: int;   
  var sizechunk: int;
  var firstRange:range;
  const profThreshold=1; 

  totlen=rangeToSplit.length;
  if totlen > profThreshold then 
    sizechunk=max(totlen/splitFactor, profThreshold);
  else {
    sizechunk=totlen;
    itleft=false;}
  firstRange=rangeToSplit#sizechunk;
  rangeToSplit=rangeToSplit#(sizechunk-totlen);	  
  return firstRange;
}




proc writeRange(r: range) 
{
  writeln("Range ", r);
}


//// Perform a correctness check for  this splitting method
// Adding timing
use Time;
config const quiet: bool = true;
var t: Timer;

var grainsize:string; // "fine", "coarse", "tri", "rand"
use Random; 

t.start();
forall i in 0..#MaxThreadsPerLocale do {
  //initialize pool of threads
 }
t.stop();
if !quiet then {
  writeln();
  writeln("Time to initialize pool ", t.elapsed(TimeUnits.microseconds), " microseconds");
  writeln();
 }

for k in 0..3 {
  select k {
    when 0 do grainsize="fine";  
    when 1 do grainsize="coarse"; 
    when 2 do grainsize="tri"; 
    otherwise grainsize="rand"; // 
    }

  CheckCorrectness(grainsize);
}


proc CheckCorrectness(grainsize:string)
{
 
  use Time;
  var t: Timer;
  var delay:uint;
  var n,m, chunk:int;
  var r:range;

 select grainsize {
    when "fine" do {
	n=1000000;
	delay=1;
	chunk=10000;
	r=1..n;  
	var A:[r] int=0;
	var TestA:[r] int=1;
	var checkA:bool=true;
 
	writeln();
	writeln("Grain size: ", grainsize,". Working with guided scheduling");
	writeln();
	t.start();
	forall c in guided(r) do {
	  usleep(delay);
	  A[c]=A[c]+1;
	}
	t.stop();
	if !quiet then {
	  writeln();
	  writeln("Total time ", t.elapsed(TimeUnits.milliseconds), " milliseconds");
	  writeln("Average time per it. ", t.elapsed(TimeUnits.milliseconds)/(n), " milliseconds");
	  writeln();
	}
	for i in r do {
	  if A[i] != TestA[i] then {
	    checkA=false;
	    writeln(" ");
	    writeln("Error in iteration ", i);
	    writeln(" ");
	  }
	}
	if checkA==true then 
	  writeln("Correct");
    } 
    when "coarse" do {
	n=100;
	delay=10000;
	chunk=2;
	r=1..n; 
	var B:[r] int=0;
	var TestB:[r] int=1;
	var checkB:bool=true;
   
	writeln();
	writeln("Grain size: ", grainsize,". Working with guided scheduling");
	writeln();
	t.start();
	forall c in guided(r) do {
	  usleep(delay);
	  B[c]=B[c]+1;
	}
	t.stop();
	if !quiet then {
	  writeln();
	  writeln("Total time ", t.elapsed(TimeUnits.milliseconds), " milliseconds");
	  writeln("Average time per it. ", t.elapsed(TimeUnits.milliseconds)/(n), " milliseconds");
	  writeln();
	}
	for i in r do {
	  if B[i] != TestB[i] then {
	    checkB=false;
	    writeln(" ");
	    writeln("Error in iteration ", i);
	    writeln(" ");
	  }
	}
	if checkB==true then 
	  writeln("Correct");
    }
      
    when "tri" do {
      n=1000;
      delay=100;
      chunk=20;
      r=1..n;
      m=(n+1)/2;  
      var C:[r,r] int=0;
      var TestC:[r,r] int=1;
      var checkC:bool=true;  
  
      writeln();
      writeln("Grain size: ", grainsize,". Working with guided scheduling");
      writeln();
  
      t.start();
      forall c in guided(r) do {
	for j in c..n do{
	  usleep(delay);
	  C[c,j]=C[c,j]+1;
	}
      }
      t.stop();
      if !quiet then {
	writeln();
	writeln("Total time ", t.elapsed(TimeUnits.milliseconds), " milliseconds");
	writeln("Average time per it. ", t.elapsed(TimeUnits.milliseconds)/(n*m), " milliseconds");
	writeln();
      }
      for i in r do {
	for j in 1..i-1 do{
	  if C[i,j] !=0 then {
	    checkC=false;
	    writeln(" ");
	    writeln("Error in iteration (", i,",",j,")");
	    writeln(" ");
	  }
	}
	for j in i..n do {
	  if C[i,j] != TestC[i,j] then {
	    checkC=false;
	    writeln(" ");
	    writeln("Error in iteration (", i,",",j,")");
	    writeln(" ");
	  }
	}
      }
      if checkC==true then 
	writeln("Correct");
    }

    otherwise {
      n=1000;
      delay=100000;
      chunk=20;
      r=1..n;    
      var ran:[r] real; // for "irregular computation" 
      fillRandom(ran);
      var delayran=(delay*ran):uint;
      var D:[r] int=0;
      var TestD:[r] int=1;
      var checkD:bool=true;


      writeln();
      writeln("Grain size: ", grainsize,". Working with guided scheduling");
      writeln();
  
      t.start();
      forall c in guided(r) do {
	usleep(delayran(c));
	D[c]=D[c]+1;
      }
      t.stop();
      if !quiet then {
	writeln();
	writeln("Total time ", t.elapsed(TimeUnits.milliseconds), " milliseconds");
	writeln("Average time per it. ", t.elapsed(TimeUnits.milliseconds)/(n), " milliseconds");
	writeln();
      }
	for i in r do {
	  if D[i] != TestD[i] then {
	    checkD=false;
	    writeln(" ");
	    writeln("Error in iteration ", i);
	    writeln(" ");
	  }
	}
	if checkD==true then 
	  writeln("Correct");
    }
   }   

}

