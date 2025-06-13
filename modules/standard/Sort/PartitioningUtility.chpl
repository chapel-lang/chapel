/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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


/* This module contains utilities used by the sort implementation
   in Partitioning */
@chpldoc.nodoc
module PartitioningUtility {

import BlockDist.blockDist;
import CTypes.{c_int, c_sizeof, c_uintptr, c_ptr, c_ptrConst, c_size_t};
import ChplConfig.CHPL_COMM;
import Communication;
import CopyAggregation.{SrcAggregator,DstAggregator};
import OS.POSIX.memcpy;
import RangeChunk;
import Time;

private config param BULK_COPY_EXTRA_CHECKS = false;
private config param SUBTIMER_EXTRA_CHECKS = false;
private config param DISTRIBUTE_EVEN_WITH_COMM_NONE = false;
private config param BULK_COPY_WARNINGS = false;

/* Bulk copy "page" size */
@unstable("'bulkCopyPageSz' is unstable and may change in the future")
private config const bulkCopyPageSz:uint = 8*1024;
const BULK_COPY_PAGE_SIZE = bulkCopyPageSz;

/* Yield after this many iterations */
@unstable("'yieldPeriod' is unstable and may change in the future")
private config const yieldPeriod = 2048;
const YIELD_PERIOD = yieldPeriod;


/* Compute the number of tasks to be used for a data parallel operation */
proc computeNumTasks(ignoreRunning: bool = dataParIgnoreRunningTasks) {
  if __primitive("task_get_serial") {
    return 1;
  }

  const tasksPerLocale = dataParTasksPerLocale;
  const ignoreRunning = dataParIgnoreRunningTasks;
  var nTasks = if tasksPerLocale > 0 then tasksPerLocale else here.maxTaskPar;
  if !ignoreRunning {
    const otherTasks = here.runningTasks() - 1; // don't include self
    nTasks = if otherTasks < nTasks then (nTasks-otherTasks):int else 1;
  }

  return nTasks;
}

/* check to see if a domain is of a type that can be distributed */
proc isDistributedDomain(dom) param {
  // this uses unstable / undocumented features. a better way is preferred.
  return !chpl_domainDistIsLayout(dom);
}

/* are we running distributed according to CHPL_COMM ? */
proc maybeDistributed() param {
  return CHPL_COMM!="none" || DISTRIBUTE_EVEN_WITH_COMM_NONE;
}

/*
   Make a BlockDist domain usually, but just return the local 'dom' unmodified
   in some cases:

    * if 'targetLocales' is 'none'
    * if CHPL_COMM=none.
*/
proc makeBlockDomain(dom: domain(?), targetLocales) {
  if maybeDistributed() && targetLocales.type != nothing {
    return blockDist.createDomain(dom, targetLocales=targetLocales);
  } else {
    return dom;
  }
}
/* Helper for the above to accept a range */
proc makeBlockDomain(rng: range(?), targetLocales) {
  return makeBlockDomain({rng}, targetLocales);
}

proc makeBlockArray(region: range(?), targetLocales, type eltType) {
  const Dom = makeBlockDomain(region, targetLocales);
  var Ret: [Dom] eltType;
  return Ret;
}

/* Given a Block distributed domain and a range to slice it with,
   computes the locales that have a local subdomain that contains
   region.

   This is done in a communication-free manner.
 */
proc computeActiveLocales(const Dom: domain(?), const region: range) {
  if Dom.rank != 1 then compilerError("activeLocales only supports 1-D");

  //writeln("computeActiveLocales ", Dom, " ", region);

  // if the range is empty, return an empty array
  if region.size == 0 {
    const empty: [1..0] locale;
    //writeln("returning ", empty);
    return empty;
  }

  // if it's the full region or there is only one locale,
  // there isn't much to do here.
  if Dom.dim(0) == region || Dom.targetLocales().size == 1 {
    //writeln("returning ", Dom.targetLocales());
    return Dom.targetLocales();
  }

  // TODO: this could implemented more simply with an assumption
  // that Dom is Block distributed.

  var minIdV = max(int);
  var maxIdV = min(int);
  forall loc in Dom.targetLocales()
  with (min reduce minIdV, max reduce maxIdV) {
    minIdV = min(minIdV, loc.id);
    maxIdV = max(maxIdV, loc.id);
  }
  const minId = minIdV;
  const maxId = maxIdV;

  // count 1 for each locale that is active
  var CountPerLocale:[minId..maxId] int;
  local {
    forall loc in Dom.targetLocales() {
      // note: this should *not* move execution with 'on loc'
      const locRange = Dom.localSubdomain(loc).dim(0);
      const intersect = locRange[region];
      if intersect.size > 0 {
        CountPerLocale[loc.id] = 1;
      }
    }
  }
  //writeln("CountPerLocale ", CountPerLocale);
  // scan to compute packed offsets (to leave out zeros)
  var Ends = + scan CountPerLocale;
  var n = Ends.last;
  var ActiveLocales:[0..<n] locale;
  // store into the packed array
  local {
    forall (locId, count, end) in zip(minId..maxId, CountPerLocale, Ends) {
      if count > 0 {
        var start = end - count;
        ActiveLocales[start] = Locales[locId];
      }
    }
  }
  //writeln("returning ", ActiveLocales);
  return ActiveLocales;
}


/* Given a Block distributed domain or non-distributed domain,
   this iterator divides it into nLocales*nTasksPerLocale chunks
   (where nLocales=Dom.targetLocales().size) to be processed by a
   different task. Each task will only process local elements.

   A forall loop running this iterator will be distributed according to Dom
   and parallel according to nTasksPerLocale. The iteration will traverse
   only those elements in the range 'region' and create work only on
   those locales with elements in 'region'.

   This is different from a regular forall loop because it always divides
   Dom among tasks in the same way, assuming the same 'Dom', 'region', and
   'nTasksPerLocale' arguments. It does not make a different number of tasks
   depending on the number of running tasks.

   Yields (activeLocIdx, taskIdInLoc, chunk) for each chunk.

   activeLocIdx is the index among the active locales 0..

   taskIdInLoc is the task index within the locale

   chunk is a non-strided range that the task should handle

   Calling code that needs a unique task identifier can use
     activeLocIdx*nTasksPerLocale + taskIdInLoc
     (if the locale indices can be packed)
   or
     here.id*nTasksPerLocale + taskIdInLoc
     (if the locale indices need to fit into a global structure)

   to form a global task number in  0..<nLocales*nTasksPerLocale.
 */
iter divideIntoTasks(const Dom: domain(?),
                     const region: range,
                     nTasksPerLocale: int,
                     const ref activeLocales=computeActiveLocales(Dom, region))
{
  if Dom.rank != 1 then compilerError("divideIntoTasks only supports 1-D");
  if Dom.dim(0).strides != strideKind.one then
    compilerError("divideIntoTasks only supports non-strided domains");

  warning("serial divideIntoTasks called");

  for (loc, activeLocIdx) in zip(activeLocales, 0..) {
    on loc {
      const ref locDom = Dom.localSubdomain();
      const locRegion = locDom.dim(0)[region];
      for (chunk, taskIdInLoc) in
           zip(RangeChunk.chunks(locRegion, nTasksPerLocale), 0..) {
        //yield (nTasksPerLocale*locId + taskId, chunk);
        yield (activeLocIdx, taskIdInLoc, chunk);
      }
    }
  }
}
iter divideIntoTasks(param tag: iterKind,
                     const Dom: domain(?),
                     const region: range,
                     nTasksPerLocale: int,
                     const ref activeLocales=computeActiveLocales(Dom, region))
 where tag == iterKind.standalone {

  if Dom.rank != 1 then compilerError("divideIntoTasks only supports 1-D");
  if Dom.dim(0).strides != strideKind.one then
    compilerError("divideIntoTasks only supports non-strided domains");
  if !Dom.hasSingleLocalSubdomain() {
    compilerError("divideIntoTasks only supports dists " +
                  "with single local subdomain");
    // note: it'd be possible to support; would just need to be written
    // differently, and consider both
    //  # local subdomains < nTasksPerLocale and the inverse.
  }

  coforall (loc, activeLocIdx) in zip(activeLocales, 0..) {
    on loc {
      const ref locDom = Dom.localSubdomain();
      const locRegion = locDom.dim(0)[region];
      coforall (chunk, taskIdInLoc) in
               zip(RangeChunk.chunks(locRegion, nTasksPerLocale), 0..) {
        //yield (nTasksPerLocale*locId + taskId, chunk);
        yield (activeLocIdx, taskIdInLoc, chunk);
      }
    }
  }
}

/* Given a Block distributed domain or non-distributed domain,
   this iterator divides it into per-locale chunks and processes
   each on its owning locale.

   yields (activeLocIdx, chunk)
*/
iter divideByLocales(const Dom: domain(?),
                     const region: range,
                     const ref activeLocales=computeActiveLocales(Dom, region))
{
  if Dom.rank != 1 then compilerError("divideByLocales only supports 1-D");
  if Dom.dim(0).strides != strideKind.one then
    compilerError("divideByLocales only supports non-strided domains");
  yield (0, Dom.dim(0));
  halt("serial divideByLocales should not be called");
}
iter divideByLocales(param tag: iterKind,
                     const Dom: domain(?),
                     const region: range,
                     const ref activeLocales=computeActiveLocales(Dom, region))
 where tag == iterKind.standalone {

  if Dom.rank != 1 then compilerError("divideByLocales only supports 1-D");
  if Dom.dim(0).strides != strideKind.one then
    compilerError("divideByLocales only supports non-strided domains");
  if !Dom.hasSingleLocalSubdomain() {
    compilerError("divideByLocales only supports dists " +
                  "with single local subdomain");
    // note: it'd be possible to support; would just need to be written
    // differently, and consider both
    //  # local subdomains < nTasksPerLocale and the inverse.
  }

  coforall (loc, activeLocIdx) in zip(activeLocales, 0..) {
    on loc {
      const ref locDom = Dom.localSubdomain();
      const locRegion = locDom.dim(0)[region];
      yield (activeLocIdx, locRegion);
    }
  }
}

/* Divide up a range into "pages" -- that is, regions that
   have start indices that are aligned (that is, startidx % alignment == 0).
   The first region won't be aligned.

   Parallel standalone or serial, but not distributed.

   Yields non-empty ranges to be processed independently.
 */
iter divideIntoPages(const region: range(?),
                     alignment: region.idxType,
                     nTasksPerLocale: region.idxType = computeNumTasks()) {
  if region.bounds != boundKind.both {
    compilerError("divideIntoPages only supports bounded ranges");
  }
  if region.strides != strideKind.one {
    compilerError("divideIntoPages only supports non-strided ranges");
  }

  if region.size > 0 {
    yield region;
  }
}
iter divideIntoPages(param tag: iterKind,
                     const region: range(?),
                     alignment: region.idxType,
                     nTasksPerLocale: region.idxType = computeNumTasks())
 where tag == iterKind.standalone {
  if region.bounds != boundKind.both {
    compilerError("divideIntoPages only supports bounded ranges");
  }
  if region.strides != strideKind.one {
    compilerError("divideIntoPages only supports non-strided ranges");
  }

  const firstPage = region.low / alignment;
  const lastPage = region.high / alignment;

  if lastPage - firstPage < nTasksPerLocale {
    // just yield the whole range (serially) if the range doesn't
    // have enough "pages" for nTasksPerLocale.
    if region.size > 0 {
      yield region;
    }
    return;
  } else {
    coforall pages in RangeChunk.chunks(firstPage..lastPage, nTasksPerLocale) {
      for whichPage in pages {
        const pageRange = whichPage*alignment..#alignment;
        const toYield = region[pageRange]; // intersect page with input
        if toYield.size > 0 {
          yield toYield;
        }
      }
    }
  }
}

/* Copy a region between a default (local) array and a Block array.
   This code is optimized for the case that the region is relatively
   small and most or all of it is local.
   It assumes that the arrays are 1-D and the ranges are non-strided
   and bounded.
 */
proc bulkCopy(ref dst: [], dstRegion: range,
              const ref src: [], srcRegion: range) : void {
  if BULK_COPY_EXTRA_CHECKS { // or boundsChecking
    assert(dst.domain.dim(0).contains(dstRegion));
    assert(src.domain.dim(0).contains(srcRegion));
    assert(dstRegion.size == srcRegion.size);
  }

  if dst.eltType != src.eltType {
    compilerError("bulkCopy array element types need to match");
  }

  if isDistributedDomain(dst.domain) &&
     !isSubtype(dst.domain.distribution.type, blockDist) {
    compilerError("bulkCopy only works for blockDist as non-local array");
    // could work for anything with contiguous elements
  }

  if isDistributedDomain(src.domain) &&
     !isSubtype(src.domain.distribution.type, blockDist) {
    compilerError("bulkCopy only works for blockDist as non-local array");
  }

  const eltSize = c_sizeof(dst.eltType);

  // TODO: these are workarounds to avoid
  // error: references to remote data cannot be passed to external routines like 'c_pointer_return_const'
  proc addrOf(const ref p): c_ptr(p.type) {
    return __primitive("_wide_get_addr", p): c_ptr(p.type);
  }
  proc addrOfConst(const ref p): c_ptrConst(p.type) {
    return __primitive("_wide_get_addr", p): c_ptrConst(void) : c_ptrConst(p.type);
  }


  // helper for PUTs
  proc helpPut(dstStart: int, srcStart: int, size: int) {
    if size <= 0 {
      return;
    }

    const startLocale = dst[dstStart].locale.id;
    const endLocale = dst[dstStart+size-1].locale.id;
    if startLocale == endLocale {
      const nBytes = (size * eltSize):uint;
      const dstPtr = addrOf(dst[dstStart]):c_uintptr:uint;
      const srcPtr = addrOf(src[srcStart]):c_uintptr:uint;
      if startLocale == here.id {
        if BULK_COPY_EXTRA_CHECKS {
          for i in 0..<size {
            assert(dst[dstStart+i].locale == here);
            assert(src[srcStart+i].locale == here);
          }
        }

        forall dstPg in divideIntoPages(dstPtr..#nBytes, BULK_COPY_PAGE_SIZE) {
          const dstPartPtr = dstPg.low:c_uintptr:c_ptr(void);
          const srcPartPtr =
            (srcPtr + (dstPg.low - dstPtr)):c_uintptr:c_ptr(void);
          if BULK_COPY_EXTRA_CHECKS {
            const dstPartPtrU = dstPartPtr:c_uintptr:uint;
            const srcPartPtrU = srcPartPtr:c_uintptr:uint;
            const dstSize = dstPg.size:uint;
            assert((dstPtr..#nBytes).contains(dstPartPtrU..#dstSize));
            assert((srcPtr..#nBytes).contains(srcPartPtrU..#dstSize));
          }
          memcpy(dstPartPtr, srcPartPtr, dstPg.size:c_size_t);
        }
      } else {
        if BULK_COPY_EXTRA_CHECKS {
          for i in 0..<size {
            assert(dst[dstStart+i].locale.id == startLocale);
            assert(src[srcStart+i].locale == here);
          }
        }
        forall dstPg in divideIntoPages(dstPtr..#nBytes, BULK_COPY_PAGE_SIZE) {
          const dstPartPtr = dstPg.low:c_uintptr:c_ptr(void);
          const srcPartPtr =
            (srcPtr + (dstPg.low - dstPtr)):c_uintptr:c_ptr(void);
          if BULK_COPY_EXTRA_CHECKS {
            const dstPartPtrU = dstPartPtr:c_uintptr:uint;
            const srcPartPtrU = srcPartPtr:c_uintptr:uint;
            const dstSize = dstPg.size:uint;
            assert((dstPtr..#nBytes).contains(dstPartPtrU..#dstSize));
            assert((srcPtr..#nBytes).contains(srcPartPtrU..#dstSize));
          }
          Communication.put(dstPartPtr, srcPartPtr, startLocale, dstPg.size);
        }
      }
    } else {
      // do it with bulk transfer since many locales are involved
      if BULK_COPY_WARNINGS {
        writeln("warning: unopt bulkCopy PUT");
      }
      dst[dstStart..#size] = src[srcStart..#size];
    }
  }

  // helper for GETs
  proc helpGet(dstStart: int, srcStart: int, size: int) {
    if size <= 0 {
      return;
    }

    const startLocale = src[srcStart].locale.id;
    const endLocale = src[srcStart+size-1].locale.id;
    if startLocale == endLocale {
      const nBytes = (size * eltSize):uint;
      const dstPtr = addrOf(dst[dstStart]):c_uintptr:uint;
      const srcPtr = addrOf(src[srcStart]):c_uintptr:uint;
      if startLocale == here.id {
        if BULK_COPY_EXTRA_CHECKS {
          for i in 0..<size {
            assert(dst[dstStart+i].locale == here);
            assert(src[srcStart+i].locale == here);
          }
        }
        forall dstPg in divideIntoPages(dstPtr..#nBytes, BULK_COPY_PAGE_SIZE) {
          const dstPartPtr = dstPg.low:c_uintptr:c_ptr(void);
          const srcPartPtr =
            (srcPtr + (dstPg.low - dstPtr)):c_uintptr:c_ptr(void);
          if BULK_COPY_EXTRA_CHECKS {
            const dstPartPtrU = dstPartPtr:c_uintptr:uint;
            const srcPartPtrU = srcPartPtr:c_uintptr:uint;
            const dstSize = dstPg.size:uint;
            assert((dstPtr..#nBytes).contains(dstPartPtrU..#dstSize));
            assert((srcPtr..#nBytes).contains(srcPartPtrU..#dstSize));
          }
          memcpy(dstPartPtr, srcPartPtr, dstPg.size:c_size_t);
        }
      } else {
        if BULK_COPY_EXTRA_CHECKS {
          for i in 0..<size {
            assert(dst[dstStart+i].locale == here);
            assert(src[srcStart+i].locale.id == startLocale);
          }
        }
        forall dstPg in divideIntoPages(dstPtr..#nBytes, BULK_COPY_PAGE_SIZE) {
          const dstPartPtr = dstPg.low:c_uintptr:c_ptr(void);
          const srcPartPtr =
            (srcPtr + (dstPg.low - dstPtr)):c_uintptr:c_ptr(void);
          if BULK_COPY_EXTRA_CHECKS {
            const dstPartPtrU = dstPartPtr:c_uintptr:uint;
            const srcPartPtrU = srcPartPtr:c_uintptr:uint;
            const dstSize = dstPg.size:uint;
            assert((dstPtr..#nBytes).contains(dstPartPtrU..#dstSize));
            assert((srcPtr..#nBytes).contains(srcPartPtrU..#dstSize));
          }
          Communication.get(dstPartPtr, srcPartPtr, startLocale, dstPg.size);
        }
      }
    } else {
      // do it with bulk transfer since many locales are involved
      if BULK_COPY_WARNINGS {
        writeln("warning: unopt bulkCopy GET");
      }
      dst[dstStart..#size] = src[srcStart..#size];
    }
  }

  const dstLocal = dst.localSubdomain().dim(0)[dstRegion] == dstRegion;
  const srcLocal = src.localSubdomain().dim(0)[srcRegion] == srcRegion;

  if dstLocal && srcLocal {
    // neither are distributed, so do a memcpy
    helpPut(dstRegion.low, srcRegion.low, dstRegion.size);
    return;
  }

  if !dstLocal && srcLocal {
    // dst is distributed, src is not
    var middlePart = dst.localSubdomain().dim(0)[dstRegion];
    if middlePart.size == 0 {
      // just use the subdomain containing the first dst element
      // not expecting this to come up much
      middlePart =
        dst.localSubdomain(dst[dstRegion.low].locale).dim(0)[dstRegion];
    }
    const nonLocalBefore = dstRegion.low..<middlePart.low;
    const nonLocalAfter = middlePart.high+1..dstRegion.high;
    // now there are 3 regions:
    //  * nonLocalBefore is the dst region before the local part
    //  * localDstPart is the region before the local part
    //  * nonLocalAfter is the dst region after the local part

    helpPut(nonLocalBefore.low,
            srcRegion.low + (nonLocalBefore.low - dstRegion.low),
            nonLocalBefore.size);

    helpPut(middlePart.low,
            srcRegion.low + (middlePart.low - dstRegion.low),
            middlePart.size);

    helpPut(nonLocalAfter.low,
            srcRegion.low + (nonLocalAfter.low - dstRegion.low),
            nonLocalAfter.size);
    return;
  }

  if !srcLocal && dstLocal {
    // src is distributed, dst is not
    var middlePart = src.localSubdomain().dim(0)[srcRegion];
    if middlePart.size == 0 {
      middlePart =
        src.localSubdomain(src[srcRegion.low].locale).dim(0)[srcRegion];
    }
    const nonLocalBefore = srcRegion.low..<middlePart.low;
    const nonLocalAfter = middlePart.high+1..srcRegion.high;

    helpGet(dstRegion.low + (nonLocalBefore.low - srcRegion.low),
            nonLocalBefore.low,
            nonLocalBefore.size);

    helpGet(dstRegion.low + (middlePart.low - srcRegion.low),
            middlePart.low,
            middlePart.size);

    helpGet(dstRegion.low + (nonLocalAfter.low - srcRegion.low),
            nonLocalAfter.low,
            nonLocalAfter.size);
    return;
  }

  // Otherwise, they both have remote elements.
  // Find an element on the source locale and use an 'on' statement
  // to PUT back from there.
  // Use bulk transfer
  if BULK_COPY_WARNINGS {
    writeln("warning: unopt bulkCopy (both remote)");
  }
  dst[dstRegion] = src[srcRegion];
}

/*
   Help with timing regions of code within a parallel region.
   To use this type:

    * start timing with startTime (which returns this type)
    * stop timing with stopTime
    * accumulate with + reduce in parallel regions
    * report it with reportTime
 */
record subtimer : writeSerializable {
  // skip computations / timer start/stop if disabled
  param enabled: bool;

  var timer: Time.stopwatch;
  var running: bool = false;

  // the below represent data from combining times
  var count: int; // aka how many tasks summarized here
  var totalTime: real;
  var minTime: real;
  var maxTime: real;
};
proc ref subtimer.start() {
  if enabled {
    running = true;
    timer.reset();
    timer.start();
  }
}
proc ref subtimer.stop() {
  if enabled && running {
    timer.stop();
    running = false;

    const t = timer.elapsed();
    if SUBTIMER_EXTRA_CHECKS {
      assert(!running);
      assert(count == 0 || count == 1);
    }
    if count == 0 {
      count = 1;
      totalTime = t;
      minTime = t;
      maxTime = t;
    } else {
      count = 1;
      totalTime += t;
      minTime += t;
      maxTime += t;
    }
  }
}

// add times within a task
proc ref subtimer.accumulate(ref x: subtimer(?)) {
  // accumulate the timing within a single task
  // (vs + which adds across tasks)
  if enabled {
    x.stop();
    if SUBTIMER_EXTRA_CHECKS && x.enabled {
      assert(!x.running);
      assert(x.count == 0 || x.count == 1);
      assert(!running);
      assert(count == 0 || count == 1);
    }
    if x.enabled && x.count == 1 {
      count = 1;
      totalTime += x.totalTime;
      minTime += x.minTime;
      maxTime += x.maxTime;
    }
  }
}

// add times from different tasks (for + reduce)
operator subtimer.+(x: subtimer(?), y: subtimer(?)) {
  var ret: subtimer(enabled=(x.enabled || y.enabled));
  if ret.enabled {
    if x.count == 0 && y.count == 0 {
      // leave ret default initialized
    } else if y.count == 0 {
      // use only x
      ret = x;
    } else if x.count == 0 {
      // use only y
      ret = y;
    } else {
      // add them
      ret.count = x.count + y.count;
      ret.totalTime = x.totalTime + y.totalTime;
      ret.minTime = min(x.minTime, y.minTime);
      ret.maxTime = max(x.maxTime, y.maxTime);
    }
  }
  return ret;
}

proc subtimer.serialize(writer, ref serializer) throws {
  writer.write("(count=", count, " totalTime=", totalTime,
               " minTime=", minTime, " maxTime=", maxTime, ")");
}

/* start timing if doTiming, returning something to be used by reportTime */
proc startTime(param doTiming:bool) {
  if doTiming {
    var ret: subtimer(enabled=true);
    ret.start();
    return ret;
  } else {
    var ret: subtimer(enabled=false);
    return ret;
  }
}

/* report time started by startTime */
proc reportTime(ref x:subtimer(?), desc:string, n: int = 0, bytesPer: int = 0) {
  if x.enabled {
    x.stop();
    const avgTime = x.totalTime / x.count;
    if x.count <= 1 {
      // in that case, avgTime == minTime == maxTime
      if n == 0 {
        writeln(desc ," in ", avgTime, " s");
      } else if bytesPer == 0 {
        writeln(desc ," in ", avgTime, " s for ",
                n/avgTime/1000.0/1000.0, " M elements/s");
      } else {
        writeln(desc ," in ", avgTime, " s for ",
                n/avgTime/1000.0/1000.0, " M elements/s and ",
                bytesPer*n/avgTime/1024.0/1024.0, " MiB/s");
      }
    } else {
      writeln(desc, " in avg ", avgTime,
                    " min ", x.minTime,
                    " max ", x.maxTime, " s");
    }
  }
}

/* Similar to subtimer; counts something per-task and summarizes
   the min/max/average number per task */
record substat : writeSerializable {
  // skip computations / timer start/stop if disabled
  param enabled: bool;
  type statType;

  // the below represent data from combining times
  var count: int; // aka how many tasks summarized here
  var total: statType;
  var min_: statType;
  var max_: statType;
};

// add stats within a task
proc ref substat.accumulate(v: statType) {
  // accumulate the timing within a single task
  // (vs + which adds across tasks)
  if enabled {
    if SUBTIMER_EXTRA_CHECKS {
      assert(count == 0 || count == 1);
    }
    count = 1;
    total += v;
    min_ += v;
    max_ += v;
  }
}

// add stats from different tasks (for + reduce)
operator substat.+(x: substat(?), y: substat(?))
where x.statType == y.statType {
  var ret: substat(enabled=(x.enabled || y.enabled), x.statType);
  if ret.enabled {
    if x.count == 0 && y.count == 0 {
      // leave ret default initialized
    } else if y.count == 0 {
      // use only x
      ret = x;
    } else if x.count == 0 {
      // use only y
      ret = y;
    } else {
      // add them
      ret.count = x.count + y.count;
      ret.total = x.total + y.total;
      ret.min_ = min(x.min_, y.min_);
      ret.max_ = max(x.max_, y.max_);
    }
  }
  return ret;
}

proc substat.serialize(writer, ref serializer) throws {
  writer.write("(count=", count, " total=", total,
               " min=", min_, " max=", max_, ")");
}

proc reportStat(const ref x:substat(?), desc:string) {
  if x.enabled {
    const avg = x.total: real / x.count;
    if x.count <= 1 {
      // in that case, avgTime == minTime == maxTime
      writeln(desc ," : ", avg);
    } else {
      writeln(desc, " : avg ", avg,
                    " min ", x.min_,
                    " max ", x.max_);
    }
  }
}

// help to yield occasionally. AFAIK this is not necessary for performance,
// but it's here to cover the bases
record yieldHelper {
  var itersSinceLastYield: int;
}
proc ref yieldHelper.maybeYield(iters: int = 1) {
  itersSinceLastYield += iters;
  if itersSinceLastYield >= YIELD_PERIOD {
    itersSinceLastYield = 0;
    currentTask.yieldExecution();
  }
}


}
