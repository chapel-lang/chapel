/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

/* Helper procedures for doing parallel I/O */
@unstable("the 'parallelIO' module is unstable and subject to change in a future release")
module ParallelIO {
  use IO, BlockDist, List, CTypes, OS;

  private extern const qbytes_iobuf_size: c_ssize_t;
  private extern proc qio_channel_seek_unsafe(ch: qio_channel_ptr_t, nbytes:int(64)): errorCode;

  /*
    Read a file in parallel into a distributed array of lists.

    This routine assumes that the file is composed of a series of deserializable
    values of type ``t`` (optionally with a header at the beginning of the file).
    It is intended to be used for situations where a ``t`` value can be of
    variable size and thus cannot be read in parallel by simply splitting the
    file into equally sized blocks.

    The algorithm will split the file into ``d.size`` blocks of roughly equal
    size and read each block in parallel on each locale. If multiple tasks are
    used per locale, each locale will further decompose its block into smaller
    blocks and read each of those in parallel.

    ..Note::
      ``t`` must:

          * have a 'deserialize method'
          * have a throwing deserializing initializer
          * have a default (zero argument) initializer

    :arg f: the file to read from
    :arg t: the type of value to read from the file
    :arg targetLocales: the locales to read the file on
    :arg tasksPerLoc: the number of tasks to use per locale
        (if ``-1``, query ``here.maxTaskPar``)
    :arg skipHeaderBytes: the number of bytes to skip at the beginning of the file
        (if ``-1``, search for the first ``t`` value in the file and start there)
    :arg deserializerType: the type of deserializer to use

    :returns: a block distributed array of lists of ``t`` values with one
      list per locale

    :throws: ``OffsetNotFoundError`` if a starting offset cannot be found
      in any of the blocks
  */
  proc readParallel(f: file, type t, targetLocales: [?d] locale = Locales,
                    tasksPerLoc: int = -1, skipHeaderBytes: int = 0,
                    type deserializerType = defaultDeserializer
  ): [] list(t) throws
    where d.rank == 1
  {
    const OnePerLoc = blockDist.createDomain(d);
    var results: [OnePerLoc] list(t),
        findStart = false,
        globalStartOffset = 0;

    if skipHeaderBytes < 0 {
      findStart = true;
    } else {
      globalStartOffset = skipHeaderBytes;
    }

    // find the starting offsets for each locale
    const startOffsets = getStartOffsets(f, d.size, globalStartOffset..<f.size, t, findStart, deserializerType);

    coforall loc in targetLocales with (ref results) do on loc {
      const nTasks = if tasksPerLoc < 0 then here.maxTaskPar else tasksPerLoc,
            indices = startOffsets[loc.id]..<startOffsets[loc.id+1],
            tStartOffsets = getStartOffsets(f, nTasks, indices, t, false, deserializerType);

      var tResults: [0..nTasks] list(t);
      coforall tid in 0..<nTasks with (ref tResults) {
        const tIndices = tStartOffsets[tid]..<tStartOffsets[tid+1];
        var des: deserializerType,
            r = f.reader(locking=false, region=tIndices, deserializer=des),
            s = new t();

        // read all the 't' values in the block into a list
        while r.read(s) do
          tResults[tid].pushBack(s);
      }

      // gather the results from each task into a single list
      for tid in 0..<nTasks {
        results[loc.id].pushBack(tResults[tid]);
      }
    }

    return results;
  }

  /*
    Read a file in parallel into a list.

    This routine assumes that the file is composed of a series of deserializable
    values of type ``t`` (optionally with a header at the beginning of the file).
    It is intended to be used for situations where a ``t`` value can be of
    variable size and thus cannot be read in parallel by simply splitting the
    file into equally sized blocks.

    The procedure will split the file into ``nTasks`` blocks of roughly equal
    size and read each block in parallel.

    ..Note::
      ``t`` must:

          * have a 'deserialize method'
          * have a throwing deserializing initializer
          * have a default (zero argument) initializer

    :arg f: the file to read from
    :arg t: the type of value to read from the file
    :arg nTasks: the number of tasks to use
    :arg skipHeaderBytes: the number of bytes to skip at the beginning of the file
        (if ``-1``, search for the first ``t`` value in the file and start there)
    :arg deserializerType: the type of deserializer to use

    :returns: a list of ``t`` values

    :throws: ``OffsetNotFoundError`` if a starting offset cannot be found in
      any of the blocks
  */
  proc readParallelLocal(f: file, type t, nTasks: int = here.maxTaskPar, skipHeaderBytes: int = 0,
                         type deserializerType = defaultDeserializer
  ): list(t) throws {
    var findStart = false,
        globalStartOffset = 0;

    if skipHeaderBytes < 0 {
      findStart = true;
    } else {
      globalStartOffset = skipHeaderBytes;
    }

    const startOffsets = getStartOffsets(f, nTasks, globalStartOffset..<f.size, t, findStart, deserializerType);
    var results: [0..nTasks] list(t);

    coforall tid in 0..<nTasks with (ref results) {
      const indices = startOffsets[tid]..<startOffsets[tid+1];
      var des: deserializerType,
          r = f.reader(locking=false, region=indices, deserializer=des),
          s = new t();

      // read all the 't' values in the block into a list
      while r.read(s) do
        results[tid].pushBack(s);
    }

    // gather the results from each task into a single list
    var result = new list(t);
    for tid in 0..<nTasks do
      result.pushBack(results[tid]);

    return result;
  }

  /*
    Find ``n`` file offsets in the file ``f`` where a deserializable value of
    type ``t`` begins.

    The procedure will split the file into ``bounds.size / n`` blocks and
    search for a starting offset as close to the start of each block as
    possible.

    :arg f: the file to search
    :arg n: the number of starting offsets to find
    :arg bounds: a range of byte offsets in the file to search
    :findStart: whether or not to search for the first offset (near the beginning of the file).
        if ``false``, the first offset will be ``bounds.low``
        if ``true``, the first offset will be the first offset in the file where a value of type ``t`` can be found
    :arg deserializerType: the type of deserializer to use

    :returns: a length ``n+1`` array of offsets (the last offset is ``bounds.high``)

    :throws: ``OffsetNotFoundError`` if a starting offset cannot be found in any of the blocks
  */
  proc getStartOffsets(f: file, n: int, bounds: range, type t, findStart: bool,
                       type deserializerType = defaultDeserializer
  ) : [] int throws {
    var startOffsets: [0..n] int;
    const numBytesPerBlock = bounds.size / n;

    startOffsets[0] = bounds.low;
    startOffsets[n] = bounds.high;

    const offsetIndices = if findStart then 0..<n else 1..<n;

    for i in offsetIndices do {
      const estOffset = bounds.low + i * numBytesPerBlock,
            startOffset = max(estOffset - (qbytes_iobuf_size / 2), bounds.low),
            stopOffset = min(startOffset + qbytes_iobuf_size, bounds.high);

      // TODO: if numBytesPerBlock > qbytes_iobuf_size and each 't' is very large,
      // this method could fail to find a starting offset in the first iobuf chunk.
      // Setup a mechanism to read a new buffer-chunk from the file if we step
      // outside the first?
      // Or is that a non issue since this method would be extremely slow in that
      // case anyway? (many many sequential try/throw/catch attempts)

      var des: deserializerType,
          r = f.reader(locking=false, region=startOffset.., deserializer=des);
      r.mark();

      // seek to the start offset and 'mark' to ensure the iobuf chunk stays in memory
      // r.seek(startOffset..); r.mark();
      // read a byte to force the buffer to be filled
      r.mark(); r.readByte(); r.revert();
      // start reading from the estimated offset
      qio_channel_seek_unsafe(r._channel_internal, estOffset - startOffset);

      var prevOffset = r.offset();
      for delta in seekIndices(rightOnly = (i == 0)) {
        const offset = estOffset + delta;

        // error out if a starting offset cannot be found in the iobuf chunk
        if offset < startOffset || offset > stopOffset {
          r.revert();
          throw new Error(
            "Failed to find starting file offset in the range (" + startOffset:string + " .. " + stopOffset:string +
            ") for block " + i:string + " of " + n:string + ". Try using fewer tasks."
          );
        }

        // seek to 'offset'
        qio_channel_seek_unsafe(r._channel_internal, offset - prevOffset);
        prevOffset = offset;

        // try to read a 't'
        r.mark();
        try {
          r.read(t);
        } catch e {
          r.revert();
          continue;
        }
        r.commit();

        // if successful, we found a starting offset
        // save it and move on to the next block
        startOffsets[i] = offset;
        break;
      }
    }
    return startOffsets;
  }

  private iter seekIndices(rightOnly: bool) {
    if rightOnly {
      for i in 0.. do yield i;
    } else {
      // 0, 1, -1, 2, -2, 3, -3, ...
      var step = 1,
          dir = 1,
          i = 0;

      while true {
        yield i;
        i += step * dir;
        step += 1;
        dir *= -1;
      }
    }
  }

  /*
    An error thrown when a starting offset cannot be found in a block of a file.
  */
  class OffsetNotFoundError: Error {}
}
