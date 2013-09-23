config param debugAdvancedIters:bool=false;

//************************* Dynamic iterator

// Serial iterator 

iter dynamic(c:range(?), chunkSize:int, numTasks:int=0) {

  if debugAdvancedIters then 
    writeln("Serial dynamic Iterator. Working with range ", c);
  
  for i in c do yield i;    
}             



// Parallel iterator

// Leader iterator

iter dynamic(param tag:iterKind, c:range(?), chunkSize:int, numTasks:int=0) 
where tag == iterKind.leader
{
  assert(chunkSize > 0); // caller's responsibility

  use UtilMath;
  // # of tasks the range can fill. (fast) ceil so all work is represented
  const chunkTasks = divceilpos(c.length, chunkSize): int;
  
  // Check if the number of tasks is 0, in that case it returns a default value
  const nTasks = min(chunkTasks, defaultNumTasks(numTasks));
  type rType=c.type;
  // Check the size and do it serial if not enough work
  if c.length == 0 then halt("The range is empty");
  if nTasks == 1 then {
    if debugAdvancedIters then
      writeln("Dynamic Iterator: serial execution because there is not enough work");
    const totalRange:rType= densify(c,c);
    yield (totalRange,);

  }
  else {
    var remain:rType=c;
    var moreWork=true;
    var splitLock$:sync bool=true;   // sync variables to control the splitting 

    coforall tid in 0..#nTasks ref(remain, moreWork) do { 
     while moreWork do {
      // There is local work in remain
      splitLock$; // Perform the splitting in a critical section
      const current:rType=splitChunk(remain, chunkSize, moreWork); 
      splitLock$=true;
      if current.length !=0 then {
        const zeroBasedIters:rType= densify(current,c);
        if debugAdvancedIters then 
          writeln("Parallel dynamic Iterator. Working at tid ", tid, " with range ", current, " yielded as ", zeroBasedIters);
        yield (zeroBasedIters,);
      }
     }
    }
  }
}

// Follower
iter dynamic(param tag:iterKind, c:range(?), chunkSize:int, numTasks:int, followThis) 
where tag == iterKind.follower
{
  type rType=c.type;
  const current:rType=unDensify(followThis(1),c);
  if debugAdvancedIters then
    writeln("Follower received range ", followThis, " ; shifting to ", current);
  for i in current do {
    yield i;
  }
}


//************************* Guided iterator
//Serial iterator 
iter guided(c:range(?), numTasks:int=0) {

  if debugAdvancedIters then 
    writeln("Serial guided Iterator. Working with range ", c);
  
  for i in c do yield i;                  
}


// Parallel iterator

// Leader iterator
iter guided(param tag:iterKind, c:range(?), numTasks:int=0)
where tag == iterKind.leader 
{   
  // Check if the number of tasks is 0, in that case it returns a default value  
  const nTasks=min(c.length, defaultNumTasks(numTasks));
  type rType=c.type;
  // Check the size and do it serial if not enough work
  if c.length == 0 then halt("The range is empty");
  if nTasks == 1 then {
    if debugAdvancedIters then
      writeln("Guided Iterator: serial execution because there is not enoguh work");
    const totalRange:rType= densify(c,c);
    yield (totalRange,); 
  }

  else {
    var remain:rType=c;
    var undone=true;
    const factor=nTasks;
    var splitLock$:sync bool=true;  // synnc variables to control the splitting

    coforall tid in 0..#nTasks ref(remain,undone) do { 
     while undone do {
      // There is local work in remain(tid)
      splitLock$; // Perform the splitting in a critical section
      const current:rType=adaptSplit(remain, factor, undone); 
      splitLock$=true;
      if current.length !=0 then {
        const zeroBasedIters:rType = densify(current,c);
        if debugAdvancedIters then 
          writeln("Parallel guided Iterator. Working at tid ", tid, " with range ", current, " yielded as ", zeroBasedIters);
        yield (zeroBasedIters,);
      }
     }            
    }
  }
}

// Follower
iter guided(param tag:iterKind, c:range(?), numTasks:int, followThis)
where tag == iterKind.follower

{
  type rType=c.type;
  const current:rType=unDensify(followThis(1),c);
  if debugAdvancedIters then
    writeln("Follower received range ", followThis, " ; shifting to ", current);
  for i in current do {
    yield i;
  }
}

//************************* Adaptive work-stealing iterator
//Serial iterator 
iter adaptive(c:range(?), numTasks:int=0) {  

  if debugAdvancedIters then 
    writeln("Serial adaptive work-stealing Iterator. Working with range ", c);
  
  for i in c do yield i;
              
}


// Parallel iterator

config param methodStealing:int=0;


// Leader iterator
iter adaptive(param tag:iterKind, c:range(?), numTasks:int=0)
where tag == iterKind.leader
  
{   
  if (methodStealing > 2 || methodStealing < 0) then
    compilerError("methodStealing value must be between 0 and 2");

  // Check if the number of tasks is 0, in that case it returns a default value  
  const nTasks=min(c.length, defaultNumTasks(numTasks));
  type rType=c.type;
  // Check the size and do it serial if not enough work
  if c.length == 0 then halt("The range is empty");
  if nTasks == 1 then {
    if debugAdvancedIters then
      writeln("Adaptive work-stealing Iterator: serial execution because there is not enough work");
    const totalRange:rType = densify(c,c);
    yield (totalRange,);
    
  }
  else {
    const r:rType=densify(c,c);
    const SpaceThreads:domain(1)=0..#nTasks;
    var localWork:[SpaceThreads] rType; // The remaining range to split on each Thread
    var moreLocalWork:[SpaceThreads] bool=true; // bool var to signal there is still work on each local range    
    var splitLock$:[SpaceThreads] sync bool=true; // sync var to control the splitting on each Thread
    const factor:int=2;
  
    const factorSteal:int=2;
    var moreWork:bool=true; // A global var to control the termination

 
    // Variables to put a barrier to ensure the initial range is computed on each Thread
    var barrierCount$:sync int=0;
    var wait$:single bool;


    // Start the parallel work
    coforall tid in 0..#nTasks ref(moreWork) do { 

      // Step 1: Initial range per Thread/Task

      localWork[tid]=splitRange(r,tid);  // Initial Local range in localWork[tid]
      if debugAdvancedIters then 
        writeln("Parallel adaptive work-stealing Iterator. Working at tid ", tid, " with initial range ", localWork[tid]);

      // A barrier waiting for each thread to finish the initial assignment
      barrier;

      // Step 2: While there is work at tid, do splitting
      
      while moreLocalWork[tid] do { 
        // There is local work 
        const zeroBasedIters:rType=splitWork(tid); // The current range we get after splitting locally
        if zeroBasedIters.length !=0 then {
          if debugAdvancedIters then 
            writeln("Parallel adaptive Iterator. Working locally at tid ", tid, " with range yielded as ", zeroBasedIters);
          yield (zeroBasedIters,);
        }
      }

      // Step3: Task tid finished its work, so it will try to steal from a neighbor

      var nVisitedVictims:int=0; 
      var victim=(tid+1) % nTasks; 
      var stealFailed:bool=false;

      while moreWork do {
      if debugAdvancedIters then 
        writeln("Entering at Stealing phase in tid ", tid," with victim ", victim, " using method of Stealing ", methodStealing);

      // Perform the spliting at the victim remaining range

      select methodStealing {
        when 0 do { // Steals from the victim until it exhausts its range

         while moreLocalWork[victim] do {
          // There is work in victim
          const zeroBasedIters2:rType=splitWork(victim); // The current range we get 
                                                //after splitting from a victim range
          if zeroBasedIters2.length !=0 then {
            if debugAdvancedIters then 
              writeln("Range stealed at victim ", victim," yielded as ", zeroBasedIters2," by tid ", tid);
            yield (zeroBasedIters2,);
          }
         }
        }       
     
        when 1 do { // Steals once from victim. If it has sucess on the stealing, 
                    // it chooses another victim and tries to steal again.
        
          if moreLocalWork[victim] then {
            // There is work in victim
            const zeroBasedIters2:rType=splitWork(victim); // The current range we get
                                                 //after splitting from a victim range
            if zeroBasedIters2.length !=0 then {
              if debugAdvancedIters then 
                writeln("Range stealed at victim ", victim," yielded as ", zeroBasedIters2," by tid ", tid);
              yield (zeroBasedIters2,);
            }
          } else {
            stealFailed=true;
          } 
        }
        otherwise { // Tt steals from the victim, but now splitting 
                    //from the tail of the victim's range 
          
         while moreLocalWork[victim] do {
          // There is work in victim
          const zeroBasedIters2:rType=splitWork(victim, true); // The current range we get 
                                // after splitting from a victim range: 
                               // now we set splitTatil to true (second parameter in the call)
          if zeroBasedIters2.length !=0 then {
            if debugAdvancedIters then 
              writeln("Range stealed at victim ", victim," yielded as ", zeroBasedIters2," by tid ", tid);
            yield (zeroBasedIters2,);
          }
         }
        }
      }
      // If here, then it can have failed the stealing intent at the victim (method 1), 
      // or we have exhausted the victim range (methods 0, 2)

      if (methodStealing==0 || methodStealing==2 || (methodStealing==1 && stealFailed)) then { 
        nVisitedVictims += 1; // Signal that there is no more work in victim
        if debugAdvancedIters then 
          writeln("Failed Stealing intent at tid ", tid," with victim ", victim, " and total no. of visited victims ", nVisitedVictims);
        stealFailed=false; 
      }
      // Check if there is no more work
      if nVisitedVictims >= nTasks-1 then
        moreWork=false; // Signal that there is no more work in any victim
      else {  // There can be still work in other victim
        victim=(victim+1) % nTasks; // New victim to steal
        if methodStealing==1 then
          if victim==tid then victim=(victim+1) % nTasks;
      } 
    }
    }  

    proc splitRange(c:range(?),tid:int)
  {
    const size= c.length/nTasks;  
    var rLocal:rType=(c+tid*size)#size;
    if tid==nTasks-1 then 
      rLocal=c#(size*(nTasks-1)-c.length); 
    return rLocal;
  }    

  proc barrier 
  {
    var tmp=barrierCount$;
    barrierCount$=tmp+1;
    if tmp+1==nTasks then wait$=true;
    wait$; 
  }
 
    proc splitWork(taskid:int, splitTail:bool=false)
  {
    if splitTail then { // Splitting from the tail of the remaining range (method 2)
      splitLock$[taskid]; // Perform the splitting in a critical section
      const current:rType=adaptSplit(localWork[taskid], factorSteal, moreLocalWork[taskid], splitTail);
      splitLock$[taskid]=true;
      const zeroBasedCurrent3:rType=current;
      return zeroBasedCurrent3;
    }
    else { // Splitting from the head of the remaining range (methods 0 and 1)
      splitLock$[taskid]; // Perform the splitting in a critical section
      const current:rType=adaptSplit(localWork[taskid], factorSteal, moreLocalWork[taskid]);
      splitLock$[taskid]=true;
      const zeroBasedCurrent:rType=current;
      return zeroBasedCurrent;  
      }
          
  }
  }
}

// Follower
iter adaptive(param tag:iterKind, c:range(?), numTasks:int, followThis)
where tag == iterKind.follower
{
  type rType=c.type;
  var current:rType=unDensify(followThis(1),c);
  if debugAdvancedIters then
    writeln("Follower received range ", followThis, " ; shifting to ", current);
  for i in current do {
    yield i;
  }
}

//************************* Helper functions
proc defaultNumTasks(nTasks:int)
{
  var dnTasks=nTasks;
  if nTasks==0 then {
    if dataParTasksPerLocale==0 then dnTasks=here.numCores; 
      else dnTasks=dataParTasksPerLocale;
  } else if nTasks<0 then {
    halt("'numTasks' is negative");
  }
  return dnTasks;
}

proc splitChunk(inout rangeToSplit:range(?), chunkSize:int, inout itLeft:bool)
{
  type rType=rangeToSplit.type;
  type lenType=rangeToSplit.length.type;
  var totLen, size:lenType;

  totLen=rangeToSplit.length;
  size=chunkSize;
  if totLen <= size then
    { 
    size=totLen;
    itLeft=false;
    }
  const firstRange:rType=rangeToSplit#size;
  rangeToSplit=rangeToSplit#(size-totLen);    
  return firstRange;
}

proc adaptSplit(inout rangeToSplit:range(?), splitFactor:int, inout itLeft:bool, splitTail:bool=false)
{
  type rType=rangeToSplit.type;
  type lenType=rangeToSplit.length.type;
  var totLen, size:lenType; 
  var direction: int;   
  const profThreshold=1; 

  totLen=rangeToSplit.length;
  if totLen > profThreshold then 
    size=max(totLen/splitFactor, profThreshold);
  else {
    size=totLen;
    itLeft=false;}
  if splitTail then 
    direction=-1;
  else direction=1; 
  const firstRange:rType=rangeToSplit#(direction*size);
  rangeToSplit=rangeToSplit#(direction*(size-totLen));    
  return firstRange;
}


