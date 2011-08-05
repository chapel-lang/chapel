// Adaptative Spliting iterator 
// Initially it distributes the ranges to split among the threads (tasks)
// Then it performs Binary/Guided Splitting localy on each thread
// When a Threads exhausts its local range, it steals and splits from another thread (victim), but now from the tail of the range 
// It keeps stealing from the victim until it exhausts the victim's range
// The Splitting method on the stealed thread can be Binary/Guided
// The Termination strategy is still very naive

// Contributed by Angeles Navarro 
_extern proc usleep(val:uint);
config const MaxThreadsPerLocale:int=4; //here.numCores;
writeln("Working with ", MaxThreadsPerLocale, " Threads");

// config debugging flags
config const debugranges: bool=false;
var dFlag: [0..#MaxThreadsPerLocale] bool=debugranges;


// config vars choosing the method of splitting:
//       false: Binary Splitting (BS)
//       true: Guided Splitting (GS)
config var method, methodWorking, methodStealing:bool;



//Serial iterator 
iter adaptative(c:range) {
  var remain, current:range;
  var splitFactor:int;
  remain=c;
  select method {
    when false do splitFactor=2;
    otherwise splitFactor=MaxThreadsPerLocale;
}  

    // While there are remaining iterations do spliting 
  var undone=true;
  while undone do {
      current=splitRange(remain, splitFactor, undone);
      if debugranges then {
	writeRange(current);
      }
      for i in current do yield i;

    }    	      
}

if debugranges then {
  // Printing ranges for Serial Spliting
  writeln("Working with Serial Dist. Adaptative Splitting");
  // The initial range
  config const lo:int=0;
  config const hi:int=63;
  var r=lo..hi;
  writeln("Initial range ", r);

  // Printing ranges for Binary Spliting
  method=false;
  writeln("Ranges in Serial Dist Adaptative Splitting (Binary strategy)");

  for c in adaptative(lo..hi) do{
    //Array(c)=c;
  }


  // Printing ranges for Guided Spliting
  method=true;
  writeln("Ranges in Serial Adaptative Splitting (Guided strategy)");
  
  for c in adaptative(lo..hi) do{
    //Array(c)=c;
  }

 }

// Parallel iterator

// Leader iterator
iter adaptative(param tag:iterator, c:range)
where tag == iterator.leader
{   

  const SpaceThreads:domain(1)=0..#MaxThreadsPerLocale;
  var remain:[SpaceThreads] range; // The remaining range to split on each Thread
  // synnc variables to control the splitting action on each Thread
  var split_lock$:[SpaceThreads] sync bool;  
  // bool variables to signal there is still work on each range  
  var undone:[SpaceThreads] bool; 
  var morework:bool=true; // A global var to control the termination

  // Variables to put a barrier to ensure the first range is computed on each Thread
  var runningcount$:sync int=0;
  var barrier$:single bool;

  // Start the parallel work
  coforall tid in 0..#MaxThreadsPerLocale do { 

    // Initialization of signals and locks
    undone(tid)=true;   
    split_lock$(tid)=true; 
 
    // Initial range per Thread
    var totlen=c.length;
    var sizechunk=totlen/MaxThreadsPerLocale;  
    remain(tid)=(c+tid*sizechunk)#sizechunk;
    remain(MaxThreadsPerLocale-1)=c#(sizechunk*(MaxThreadsPerLocale-1)-totlen); 

    if dFlag(tid) then {
      writeln("Initial Range at tid ", tid);
      writeRange(remain(tid));}

    // A barrier waiting for each thread to finish the initial assignments
    var tmp=runningcount$;
    runningcount$=tmp+1;
    if tmp+1==MaxThreadsPerLocale then barrier$=true;
    barrier$;

    // Initialization of spliting factors 
    var splitFactor, splitFactorSteal:int; // The spliting factors
    select methodWorking {
      when false do splitFactor=2;    // BS = Binary Splitting
      otherwise splitFactor=MaxThreadsPerLocale; //GS= Guided Splitting
      }  
    select methodStealing {
      when false do splitFactorSteal=2;
      otherwise splitFactorSteal=MaxThreadsPerLocale;
      }

    // Initialization of stealing variables  
    var numberofStealintents: int=0;
    var victim=(tid+1) % MaxThreadsPerLocale; // Initial victim to steal (neighbor)

    // While there is work at tid, do splitting
    var current:range;    // The current range we get after splitting
    while undone(tid) do {
	// There is local work in remain(tid)
	split_lock$(tid); // Perform the splitting in a critical section
	current=splitRange(remain(tid), splitFactor, undone(tid)); 
	split_lock$(tid)=true;
	yield current;


	if dFlag(tid) then {
	  writeln("Working at tid ", tid);
	  writeRange(current);}
      } 

    //I finished my work. Now I try to steal
    while morework do {
	if dFlag(tid) then 
	  writeln("Entering Stealing phase at tid ", tid," with victim ", victim);

	// Perform the spliting at the victim remaining range
	while undone(victim) do {
	    // There is work in victim
	  split_lock$(victim); // Perform the splitting in a critical section
	  current=splitRangeWS(remain(victim), splitFactorSteal, undone(victim));
	  split_lock$(victim)=true;	  
	  yield current;


	  if dFlag(tid) then {
	    writeln("Range stealed by tid ", tid, " at victim ", victim);
	    writeRange(current);}
	}
	
	numberofStealintents += 1; // Signal that there is no more work in victim
	if dFlag(tid) then 
	  writeln("Failed Stealing intent at tid ", tid," with victim ", victim, " and total no. of failed steals ", numberofStealintents);

	// Check if there is no more work
	if numberofStealintents >= MaxThreadsPerLocale-1 then
	  morework=false; // Signal that there is no more work in any victim
	else{  // There can be still work in other victim
	  victim=(victim+1) % MaxThreadsPerLocale; // New victim to steal
	  // Update Splitting factor for Guided method	  
	  // if methodStealing==true then splitFactorSteal=MaxThreadsPerLocale; 
	  }
      }       
 }
}

// Follower
iter adaptative(param tag:iterator, c:range, follower)
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
  if totlen > profThreshold  then sizechunk=max(totlen/splitFactor,profThreshold);
  else {
    sizechunk=totlen;
    itleft=false;}
  firstRange=rangeToSplit#sizechunk;
  rangeToSplit=rangeToSplit#(sizechunk-totlen);	  
  return firstRange;
}

proc splitRangeWS(inout rangeToSplit:range, splitFactor:int, inout itleft:bool):range
{
  var totlen: int;   
  var sizechunk: int;
  var firstRange:range;

  totlen=rangeToSplit.length;
  if totlen >= splitFactor then sizechunk=totlen/splitFactor;
  else {
    sizechunk=totlen;
    itleft=false;}
  firstRange=rangeToSplit#(-sizechunk);
  rangeToSplit=rangeToSplit#(totlen-sizechunk);	  
  return firstRange;
}

/*proc ThereIsWork(c:range):bool
{
  if c.length > 1 then return true; 
  else return false;
}
*/

// Perform a correctness check for all the splitting methods
// Adding timing
use Time;
config const quiet: bool = true;
config const nIterTimesF, nIterTimesC, nIterTimesT, nIterTimesR:int=1;
config const chunkTimesF, chunkTimesC, chunkTimesT, chunkTimesR:int=1;
var t: Timer;

var grainsize:string; // "fine", "coarse", "tri", "ran"
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
    for i in 0..1 {
      select i {
	when 0 do methodWorking=false; // methodWorking = Binary Spliting (BS)
	otherwise methodWorking=true; // methodWorking = Guided Spliting (GS)
	}
      for j in 0..1 {
	select j {
	  when 0 do methodStealing=false; // methodStealing = Binary Spliting (BS)
	  otherwise methodStealing=true; // methodStealing = Guided Spliting (GS)
	  }
	CheckCorrectness(methodWorking,methodStealing, grainsize);   
      }      
    }  
}
 

proc CheckCorrectness(mWorking:bool, mStealing:bool, grainsize:string)
{
 
  use Time;
  var t: Timer;
  var delay:uint;
  var n,m,chunk:int;
  var r:range;
  var mW, mS:string;
  select mWorking {
    when false do mW="BS";
    otherwise mW="GS";
    }  

  select mStealing {
    when false do mS="BS";
    otherwise mS="GS";
    }  

 
  select grainsize {
    when "fine" do {
	n=100*nIterTimesF;
	delay=1;
	chunk=10*chunkTimesF;
	r=1..n;   
	var A:[r] int=0;
	var TestA:[r] int=1;
	var checkA:bool=true;
  
	writeln();
	writeln("Grain size: ", grainsize,". Working with Adaptative v2 ", mW," and Stealing with ", mS);
	writeln();
	t.start();
	forall c in adaptative(r) do {
	  usleep(delay);
	  A[c]=A[c]+1;
	}
	t.stop();
	if !quiet then {
	  writeln();
	  writeln("Total time ", grainsize, " (", mW, ",", mS,") ", t.elapsed(TimeUnits.milliseconds), " milliseconds");
//	  writeln("Average time per it. ", t.elapsed(TimeUnits.milliseconds)/(n), " milliseconds");
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
	n=10*nIterTimesC;
	delay=10000;
	chunk=2*chunkTimesC;
	r=1..n;  
	var B:[r] int=0;
	var TestB:[r] int=1;
	var checkB:bool=true;
  
	writeln();
	writeln("Grain size: ", grainsize,". Working with Adaptative v2 ", mW," and Stealing with ", mS);
	writeln();
	t.start();
	forall c in adaptative(r) do {
	  usleep(delay);
	  B[c]=B[c]+1;
	}
	t.stop();
	if !quiet then {
	  writeln();
	  writeln("Total time ", grainsize, " (", mW, ",", mS,") ", t.elapsed(TimeUnits.milliseconds), " milliseconds");
//	  writeln("Average time per it. ", t.elapsed(TimeUnits.milliseconds)/(n), " milliseconds");
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
      n=10*nIterTimesT;
      delay=100;
      chunk=2*chunkTimesT;
      r=1..n;
      m=(n+1)/2;    
      var C:[r,r] int=0;
      var TestC:[r,r] int=1;
      var checkC:bool=true;

      writeln();
      writeln("Grain size: ", grainsize,". Working with Adaptative v2 ", mW," and Stealing with ", mS);
      writeln();
  
      t.start();
      forall c in adaptative(r) do {
	for j in c..n do{
	  usleep(delay);
	  C[c,j]=C[c,j]+1;
	}
      }
      t.stop();
      if !quiet then {
	writeln();
	writeln("Total time ", grainsize, " (", mW, ",", mS,") ", t.elapsed(TimeUnits.milliseconds), " milliseconds");
//	writeln("Average time per it. ", t.elapsed(TimeUnits.milliseconds)/(n*m), " milliseconds");
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
      n=10*nIterTimesR;
      delay=100000;
      chunk=2*chunkTimesR;
      r=1..n;    
      var ran:[r] real; // for "irregular computation" 
      fillRandom(ran);
      var delayran=(delay*ran):uint;
      var D:[r] int=0;
      var TestD:[r] int=1;
      var checkD:bool=true;

      writeln();
      writeln("Grain size: ", grainsize,". Working with Adaptative v2 ", mW," and Stealing with ", mS);
      writeln();
  
      t.start();
      forall c in  adaptative(r) do {
	usleep(delayran(c));
	D[c]=D[c]+1;
      }
      t.stop();

      if !quiet then {      
	writeln();
	writeln("Total time ", grainsize, " (", mW, ",", mS,") ", t.elapsed(TimeUnits.milliseconds), " milliseconds");
//	writeln("Average time per it. ", t.elapsed(TimeUnits.milliseconds)/(n), " milliseconds");
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

//The writeRange proc (short version from primers/ranges.chpl)

proc writeRange(r: range) 
{
  writeln("Range ", r);
}
