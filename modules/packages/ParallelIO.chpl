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
  private use IO, BlockDist, List, CTypes, OS;

  private extern const qbytes_iobuf_size: c_ssize_t;
  private extern proc qio_channel_seek_unsafe(ch: qio_channel_ptr_t, nbytes:int(64)): errorCode;

  /*
    Read a file in parallel into a distributed array of lists.

    This routine assumes that the file is composed of a series of deserializable
    values of type ``t`` (optionally with a header at the beginning of the file).
    It is intended to be used for situations where a ``t`` value can be of
    variable size and thus cannot be read in parallel by simply splitting the
    file into equally sized chunks.

    The algorithm will split the file into ``d.size`` chunks of roughly equal
    size and read each chunk in parallel on each locale. If multiple tasks are
    used per locale, each locale will further decompose its chunk into smaller
    chunks and read each of those in parallel.

    .. note:: ``t`` must:

          * have a 'deserialize method'
          * have a throwing deserializing initializer
          * have a default (zero argument) initializer

    :arg filePath: a path to the file to read from
    :arg t: the type of value to read from the file
    :arg tasksPerLoc: the number of tasks to use per locale
        (if ``-1``, query ``here.maxTaskPar`` on each locale)
    :arg skipHeaderBytes: the number of bytes to skip at the beginning of the file
        (if ``-1``, search for the first ``t`` value in the file and start there)
    :arg deserializerType: the type of deserializer to use
    :arg targetLocales: the locales to read the file on

    :returns: a block-distributed array of lists of ``t`` values with one
      list per locale

    :throws: :class:`OffsetNotFoundError` if a starting offset cannot be found
             in any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readParallel(filePath: string, type t, tasksPerLoc: int = -1, skipHeaderBytes: int = 0,
                    type deserializerType = defaultDeserializer,
                    targetLocales: [?d] locale = Locales
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
    const fMeta = open(filePath, ioMode.r),
          fileBounds = globalStartOffset..<fMeta.size,
          byteOffsets = findByteOffsets(fMeta, d.size, fileBounds, t, findStart, deserializerType);

    coforall loc in targetLocales with (ref results) do on loc {
      const nTasks = if tasksPerLoc < 0 then here.maxTaskPar else tasksPerLoc,
            locBounds = byteOffsets[loc.id]..<byteOffsets[loc.id+1],
            locFile = open(filePath, ioMode.r),
            tByteOffsets = findByteOffsets(locFile, nTasks, locBounds, t, false, deserializerType);

      var tResults: [0..nTasks] list(t);
      coforall tid in 0..<nTasks with (ref tResults) {
        const taskBounds = tByteOffsets[tid]..<tByteOffsets[tid+1];
        var des: deserializerType,
            r = locFile.reader(locking=false, region=taskBounds, deserializer=des),
            s = new t();

        // read all the 't' values in the chunk into a list
        while r.read(s) do
          tResults[tid].pushBack(s);
      }

      // gather the results from each task into a single list
      for tid in 0..<nTasks do
        results[loc.id].pushBack(tResults[tid]);
    }

    return results;
  }

  /*
    Read a file in parallel into a list.

    This routine is essentially the same as :proc:`readParallel`, except that it
    only operates on a single locale. As such, it does not accept a ``targetLocales``
    argument and returns a single list of ``t`` values rather than a block-
    distributed array of lists.

    :arg filePath: a path to the file to read from
    :arg t: the type of value to read from the file
    :arg nTasks: the number of tasks to use
    :arg skipHeaderBytes: the number of bytes to skip at the beginning of the file
        (if ``-1``, search for the first ``t`` value in the file and start there)
    :arg deserializerType: the type of deserializer to use

    :returns: a list of ``t`` values

    :throws: ``OffsetNotFoundError`` if a starting offset cannot be found in
             any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readParallelLocal(filePath: string, type t, nTasks: int = here.maxTaskPar, skipHeaderBytes: int = 0,
                         type deserializerType = defaultDeserializer
  ): list(t) throws {
    var findStart = false,
        globalStartOffset = 0;

    if skipHeaderBytes < 0 {
      findStart = true;
    } else {
      globalStartOffset = skipHeaderBytes;
    }

    var results: [0..nTasks] list(t);
    const f = open(filePath, ioMode.r),
          fileBounds = globalStartOffset..<f.size,
          byteOffsets = findByteOffsets(f, nTasks, fileBounds, t, findStart, deserializerType);

    coforall tid in 0..<nTasks with (ref results) {
      const taskBounds = byteOffsets[tid]..<byteOffsets[tid+1];
      var des: deserializerType,
          r = f.reader(locking=false, region=taskBounds, deserializer=des),
          s = new t();

      // read all the 't' values in the chunk into a list
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
    Read a delimited file in parallel into a block-distributed array.

    This routine assumes that the file is composed of a series of deserializable
    values of type ``t`` (optionally with a header at the beginning of the file).
    Each ``t`` must be separated by exactly one delimiter which can either be
    provided as a ``string`` or ``bytes`` value.

    The algorithm will use the delimiter to split the file into ``d.size`` chunks
    of roughly equal size and read each chunk in parallel on each locale. If
    multiple tasks are used per locale, each locale will further decompose its
    chunk into smaller chunks and read each of those in parallel.

    This procedure can be used for a variety of purposes, such as reading a CSV
    file in parallel. To do so, the delimiter should keep its default value of
    ``b"\n"``. This will cause the file to be split by lines, where each line
    will be parsed as a ``t`` value.  The comma values between fields must be
    parsed by ``t``'s deserialize method. The two ways to accomplish this are
    (1) by using a custom ``deserialize`` method that parses the comma values, e.g.,

    .. code-block:: chapel

        record color {
          var r, g, b: int(8);
        }
        proc ref color.deserialize(ref reader, deserializer) throws {
          this.r = reader.read(int(8));
          reader.matchLiteral(b",");
          this.g = reader.read(int(8));
          reader.matchLiteral(b",");
          this.b = reader.read(int(8));
        }

    or (2) by providing a deserializer that will handle commas appropriately
    with ``t``'s default ``deserialize`` method.

    :arg filePath: a path to the file to read from
    :arg delim: the delimiter to use to separate ``t`` values in the file
    :arg t: the type of value to read from the file
    :arg tasksPerLoc: the number of tasks to use per locale
        (if ``-1``, query ``here.maxTaskPar`` on each locale)
    :arg skipHeaderLines: the number of lines to skip at the beginning of the file
    :arg deserializerType: the type of deserializer to use
    :arg targetLocales: the locales to read the file on

    :returns: a block-distributed array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a starting offset cannot be found
             in any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readParallelDelimited(filePath: string, in delim: ?dt = b"\n", type t, tasksPerLoc: int = -1,
                             skipHeaderLines: int = 0, type deserializerType = defaultDeserializer,
                             targetLocales: [?d] locale = Locales
  ): [] t throws
    where d.rank == 1 && (dt == bytes || dt == string)
  {
    const fMeta = open(filePath, ioMode.r),
          fileBounds = 0..<fMeta.size,
          byteOffsets = findFileChunks(fMeta, delim, d.size, fileBounds, skipHeaderLines),
          itemOffsets = findItemOffsets(fMeta, delim, byteOffsets);

    const resDom = blockDist.createDomain({0..<itemOffsets.last}, targetLocales=targetLocales),
          results: [resDom] t;

    coforall loc in targetLocales with (ref results) do on loc {
      const nTasks = if tasksPerLoc < 0 then here.maxTaskPar else tasksPerLoc,
            locBounds = byteOffsets[loc.id]..<byteOffsets[loc.id+1],
            locFile = open(filePath, ioMode.r),
            tByteOffsets = findFileChunks(locFile, delim, nTasks, locBounds, 0),
            tItemOffsets = findItemOffsets(locFile, delim, tByteOffsets) + itemOffsets[loc.id];

      coforall tid in 0..<nTasks {
        var des: deserializerType;
        const taskBounds = tByteOffsets[tid]..<tByteOffsets[tid+1],
              r = locFile.reader(locking=false, region=taskBounds, deserializer=des);

        var item: t,
            i = tItemOffsets[c];
        while r.read(item) {
          // TODO: maybe add some aggregation here for the few items that will end up on adjacent locales?
          results[i] = item;
          r.advanceThrough(delim);
          i += 1;
        }
      }
    }

    return results;
  }

  /*
    Read a delimited file in parallel into a block-distributed array.

    This procedure is essentially the same as :proc:`readParallelDelimited`,
    except that it only operates on a single locale. As such, it does not
    accept a ``targetLocales`` argument and it returns a non-distributed array.

    :arg filePath: a path to the file to read from
    :arg delim: the delimiter to use to separate ``t`` values in the file
    :arg t: the type of value to read from the file
    :arg nTasks: the number of tasks to use
    :arg skipHeaderLines: the number of lines to skip at the beginning of the file
    :arg deserializerType: the type of deserializer to use

    :returns: an array of ``t`` values

    :throws: ``OffsetNotFoundError`` if a starting offset cannot be found in
              any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readParallelDelimitedLocal(filePath: string, in delim: ?dt = b"\n", type t,
                                  nTasks: int = here.maxTaskPar, skipHeaderLines: int = 0,
                                  type deserializerType = defaultDeserializer
  ): [] t throws
    where dt == bytes || dt == string
  {
    const f = open(filePath, ioMode.r),
          fileBounds = 0..<f.size,
          byteOffsets = findFileChunks(f, delim, nTasks, fileBounds, skipHeaderLines),
          itemOffsets = findItemOffsets(f, delim, byteOffsets);

    const resDom = blockDist.createDomain({0..<itemOffsets.last}),
          results: [resDom] t;

    coforall tid in 0..<nTasks with (ref results) {
      var des: deserializerType;
      const taskBounds = byteOffsets[tid]..<byteOffsets[tid+1],
            r = f.reader(locking=false, region=taskBounds, deserializer=des);

      var item: t,
          i = itemOffsets[tid];
      while r.read(item) {
        results[i] = item;
        r.advanceThrough(delim);
        i += 1;
      }
    }

    return results;
  }

  /*
    Get an array of ``n+1`` roughly evenly spaced byte offsets in the file
    ``f`` where a deserializable value of type ``t`` begins.

    The procedure will split the file into approximately ``bounds.size / n``
    sized chunks and search for a starting offset near the boundary of each
    chunk where a ``t`` can be deserialized.

    :arg f: the file to search
    :arg n: the number of chunks to find
    :arg bounds: a range of byte offsets to break into chunks
    :arg findStart: whether or not to search for the first offset
                  * if ``false``, the first offset will be ``bounds.low``
                  * if ``true``, the first offset will be found by searching for a ``t`` after ``bounds.low``
    :arg deserializerType: the type of deserializer to use

    :returns: a length ``n+1`` array of byte offsets (the last offset is
              ``bounds.high``)

    :throws: ``OffsetNotFoundError`` if a starting offset cannot be found in
              any of the chunks
  */
  proc findByteOffsets(f: file, n: int, bounds: range, type t, findStart: bool,
                       type deserializerType = defaultDeserializer
  ) : [] int throws {
    const approxBytesPerChunk = bounds.size / n,
          offsetIndices = if findStart then 0..<n else 1..<n;

    var startOffsets: [0..n] int;
    startOffsets[0] = bounds.low;
    startOffsets[n] = bounds.high;

    for i in offsetIndices do {
      const estOffset = bounds.low + i * approxBytesPerChunk,
            startOffset = max(estOffset - (qbytes_iobuf_size / 2), bounds.low),
            stopOffset = min(startOffset + qbytes_iobuf_size, bounds.high);

      // TODO: if approxBytesPerChunk > qbytes_iobuf_size and each 't' is _very_ large,
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
          throw new OffsetNotFoundError(
            "Failed to find starting file offset in the range (" + startOffset:string + " .. " + stopOffset:string +
            ") for chunk " + i:string + " of " + n:string + ". Try using fewer tasks."
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
        // save it and move on to the next chunk
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
    Get an array of ``n+1`` byte offsets that divide the file ``f`` into ``n``
    roughly equally sized chunks, where each byte offset lines up with a
    delimiter.

    :arg f: the file to search
    :arg delim: the delimiter to use to separate the file into chunks
    :arg n: the number of chunks to find
    :arg bounds: a range of byte offsets to break into chunks
    :arg skipHeaderLines: the number of lines to skip at the beginning of the range

    :returns: a length ``n+1`` array of byte offsets (the last offset is
              ``bounds.high``)

    :throws: ``OffsetNotFoundError`` if a starting offset cannot be found in
              any of the chunks
  */
  proc findFileChunks(ref f: file, in delim: ?dt, n: int, bounds: range, skipHeaderLines: int): [] int throws
    where dt == bytes || dt == string
  {
    const r = f.reader(locking=false, region=bounds);
    for 0..<skipHeaderLines do r.readLine();

    const nDataBytes = bounds.high - r.offset(),
          approxBytesPerChunk = nDataBytes / n;

    var chunkOffsets: [0..n] int;
    chunkOffsets[0] = r.offset();
    chunkOffsets[n] = bounds.high;

    for i in 1..<n do {
      const estOffset = chunkOffsets[i-1] + approxBytesPerChunk;
      r.seek(estOffset..);              // seek to the estimated offset
      try {
        r.advanceThrough(delim);        // advance past the next delimiter
      } catch {
        // there wasn't an offset in this chunk
        throw new OffsetNotFoundError();
      }
      chunkOffsets[i] = r.offset();     // record the offset for this chunk
    }

    return chunkOffsets;
  }

  /*
    Get a prefix sum of the number of items in each chunk of the file ``f``,
    where the chunks are defined by the ``byteOffsets`` array, and each item
    is separated by the given delimiter.

    :arg f: the file to search
    :arg delim: the delimiter used to separate items in the file
    :arg byteOffsets: an array of byte offsets that divide the file into chunks

    :returns: an array of length ``byteOffsets.size`` containing the number of
              items in the file before the start of each chunk
  */
  proc findItemOffsets(ref f: file, delim: ?dt, byteOffsets: [?d] int): [d] int
    where dt == bytes || dt == string
  {
    var nPerChunk: [d] int;

    coforall c in 0..<d.high with (ref nPerChunk) {
      const r = f.reader(locking=false, region=byteOffsets[c]..<byteOffsets[c+1]);

      // count the number of items in the chunk
      var n = 0, line: bytes;
      while true {
        try {
          r.advanceThrough(delim); // throws if we advance to the end of the region
          n += 1;
        } catch {
          break;
        }
      }
      nPerChunk[c+1] = n; // (c+1) because we want an exclusive prefix-sum below
    }

    return (+ scan nPerChunk); // compute the number of items leading up to each chunk
  }

  /*
    An error thrown when a starting offset cannot be found in a chunk of a file.
  */
  class OffsetNotFoundError: Error {}
}
