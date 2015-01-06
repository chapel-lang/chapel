/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
For more information, see "User-Defined Parallel Zippered iterators
in Chapel". Bradford L. Chamberlain, Sung-Eun Choi, Steven J. Deitz,
Angeles Navarro. PGAS 2011: Fifth Conference on Partitioned Global
Address Space Programming Models, October 2011.
*/

record vlock {
  var l: atomic bool;
  proc lock() {
    on this do while l.testAndSet() != false do chpl_task_yield();
  }
  proc unlock() {
    l.write(false);
  }
}

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

  // We're going to have to densify at some point, might as well
  // do it early and make range slicing easier.
  const remain:rType=densify(c,c);

  // If the number of tasks is insufficient, yield in serial
  if c.length == 0 then halt("The range is empty");
  if nTasks == 1 then {
    if debugAdvancedIters then
      writeln("Dynamic Iterator: serial execution because there is not enough work");
    yield (remain,);
  } else {
    var moreWork : bool =true;
    var curIndex : atomic remain.low.type;
    curIndex.write(remain.low);

    coforall tid in 0..#nTasks with (ref remain, ref moreWork, ref curIndex) do {
      while moreWork do {
        // There is local work in remain
        const low = curIndex.fetchAdd(chunkSize);
        var high = low + chunkSize-1;
        
        if low > remain.high {
          break;
        } else if high > remain.high {
          high = remain.high;
          moreWork = false;
        }

        const current:rType = remain(low .. high);
        
        if high >= low then {
          if debugAdvancedIters then 
            writeln("Parallel dynamic Iterator. Working at tid ", tid, " with range ", unDensify(current,c), " yielded as ", current);
          yield (current,);
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
  var remain:rType = densify(c,c);
  // If the number of tasks is insufficient, yield in serial
  if c.length == 0 then halt("The range is empty");
  if nTasks == 1 then {
    if debugAdvancedIters then
      writeln("Guided Iterator: serial execution because there is not enoguh work");
    yield (remain,); 
  }

  else {
    var undone=true;
    const factor=nTasks;
    var lock : vlock;

    coforall tid in 0..#nTasks with (ref remain, ref undone, ref lock) do {
      while undone do {
        // There is local work in remain(tid)
        const current:rType=adaptSplit(remain, factor, undone, lock); 
        if current.length !=0 then {
          if debugAdvancedIters then 
            writeln("Parallel guided Iterator. Working at tid ", tid, " with range ", unDensify(current,c), " yielded as ", current);
          yield (current,);
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

enum Method {
  Whole = 0,            // Steal until range is exhausted
  RoundRobin = 1,       // Steal from a different victim every time
  WholeTail = 2         // Steal from the tail until range is exhausted
};

config param methodStealing = Method.Whole;

// Leader iterator
iter adaptive(param tag:iterKind, c:range(?), numTasks:int=0)
where tag == iterKind.leader
  
{   
  /*if (methodStealing != Method.Whole || methodStealing != Method.WholeTail) then
    compilerError("methodStealing value must be between 0 and 2");*/

  // Check if the number of tasks is 0, in that case it returns a default value  
  const nTasks=min(c.length, defaultNumTasks(numTasks));
  type rType=c.type;

  // If the number of tasks is insufficient, yield in serial
  if c.length == 0 then halt("The range is empty");
  if nTasks == 1 then {
    if debugAdvancedIters then
      writeln("Adaptive work-stealing Iterator: serial execution because there is not enough work");
    yield (densify(c,c),);
    
  }
  else {
    const r:rType=densify(c,c);
    const SpaceThreads:domain(1)=0..#nTasks;
    var localWork:[SpaceThreads] rType; // The remaining range to split on each Thread
    var moreLocalWork:[SpaceThreads] bool=true; // bool var to signal there is still work on each local range    
    var locks:[SpaceThreads] vlock; // sync var to control the splitting on each Thread
    const factor:int=2;
  
    const factorSteal:int=2;
    var moreWork:bool=true; // A global var to control the termination

 
    // Variables to put a barrier to ensure the initial range is computed on each Thread
    var barrier : atomic int;

    // Start the parallel work
    coforall tid in 0..#nTasks with (ref localWork, ref moreLocalWork, ref moreWork, ref r) do { 

      // Step 1: Initial range per Thread/Task

      // Initial Local range in localWork[tid]
      const chunkSize = c.length/nTasks;
      localWork[tid]= 
      if tid==nTasks-1 then 
        r#(chunkSize*(nTasks-1)-r.length)
      else 
        (r+tid*chunkSize)#chunkSize;
      barrier.add(1);

      if debugAdvancedIters then 
        writeln("Parallel adaptive work-stealing Iterator. Working at tid ", tid, " with initial range ", localWork[tid]);

      // A barrier waiting for each thread to finish the initial assignment
      barrier.waitFor(nTasks);

      // Step 2: While there is work at tid, do splitting
      
      while moreLocalWork[tid] do { 
        // There is local work 
        // The current range we get after splitting locally
        const zeroBasedIters:rType=adaptSplit(localWork[tid], factorSteal, moreLocalWork[tid], locks[tid]);
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

        if methodStealing == Method.RoundRobin {
          if moreLocalWork[victim] then {
            // There is work in victim
            const zeroBasedIters2:rType=adaptSplit(localWork[victim], factorSteal, moreLocalWork[victim], locks[victim]);
            if zeroBasedIters2.length !=0 then {
              if debugAdvancedIters then 
                writeln("Range stealed at victim ", victim," yielded as ", zeroBasedIters2," by tid ", tid);
              yield (zeroBasedIters2,);
            }
          } else {
            stealFailed=true;
          } 
        } else {
          while moreLocalWork[victim] do {
            // There is work in victim
            const zeroBasedIters2:rType=adaptSplit(localWork[victim], factorSteal, moreLocalWork[victim], locks[victim], methodStealing==Method.WholeTail);
                                          //after splitting from a victim range
            if zeroBasedIters2.length !=0 then {
              if debugAdvancedIters then 
                writeln("Range stealed at victim ", victim," yielded as ", zeroBasedIters2," by tid ", tid);
              yield (zeroBasedIters2,);
            }
          }
        }
        
        // If here, then it can have failed the stealing intent at the victim (method 1), 
        // or we have exhausted the victim range (methods 0, 2)

        if (methodStealing==Method.Whole || methodStealing==Method.WholeTail || (methodStealing==Method.RoundRobin && stealFailed)) then { 
          nVisitedVictims += 1; // Signal that there is no more work in victim
          stealFailed=false; 
          if debugAdvancedIters then 
            writeln("Failed Stealing intent at tid ", tid," with victim ", victim, " and total no. of visited victims ", nVisitedVictims);
        }
        // Check if there is no more work
        if nVisitedVictims >= nTasks-1 then
          moreWork=false; // Signal that there is no more work in any victim
        else {  // There can be still work in other victim
          victim=(victim+1) % nTasks; // New victim to steal
          if methodStealing==Method.RoundRobin && victim==tid then
            victim=(victim+1) % nTasks;
        } 
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
    if dataParTasksPerLocale==0 then dnTasks=here.maxTaskPar;
      else dnTasks=dataParTasksPerLocale;
  } else if nTasks<0 then {
    halt("'numTasks' is negative");
  }
  return dnTasks;
}

proc adaptSplit(ref rangeToSplit:range(?), splitFactor:int, ref itLeft:bool, ref lock : vlock, splitTail:bool=false)
{
  type rType=rangeToSplit.type;
  type lenType=rangeToSplit.length.type;
  var totLen, size:lenType; 
  const profThreshold=1; 

  lock.lock();
  totLen=rangeToSplit.length;
  if totLen > profThreshold then 
    size=max(totLen/splitFactor, profThreshold);
  else {
    size=totLen;
    itLeft=false;}
  const direction = if splitTail then -1 else 1;
  const firstRange:rType=rangeToSplit#(direction*size);
  rangeToSplit=rangeToSplit#(direction*(size-totLen));
  lock.unlock();
  return firstRange;
}
