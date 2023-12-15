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

/*
  Helper procedures for doing parallel I/O

  This module provides the following procedures, each intended for a slightly
  different use case (but all focused on cases where a large file containing
  only a header, and then a continuous stream of deserializeable values of the
  same type).

  There are distributed and local versions of each procedure. The distributed
  procedures return a block distributed array over the provided locales, while
  the local procedures return a default rectangular array.

  * :proc:`readParallelLines`: read each of the lines of a file as a ``string``
                               or ``bytes`` value
  * :proc:`readParallelDelimitedStrict`: read a file where each value is strictly
                                         separated by a delimiter, and the delimiter
                                         cannot be found in the (e.g., CSV)
  * :proc:`readParallelDelimited`: read a file where each value is
                                   separated by delimiter
  * :proc:`readParallel`: read a file where no reliable delimiter is used to
                          separate values

  There are also some helper procedures that can be used to implement other
  parallel I/O procedures:

  * :proc:`findDelimChunks`: find a set of byte offsets that divide a file into
                             roughly equal chunks where each chunk begins with
                             a delimiter
  * :proc:`findDelimChunksChecked`: find a set of byte offsets that divide a file
                                    into roughly equal chunks where each chunk
                                    begins with a delimiter and each chunk
                                    starts with a deserializable value
  * :proc:`findItemOffsets`: find a prefix sum of the number of items in each
                             chunk of a file, where the chunks are defined by
                             the ``byteOffsets`` array, and each item is
                             separated by the given delimiter
  * :proc:`findDeserChunks`: find a set of byte offsets that divide a file into
                             roughly equal chunks where each chunk begins with
                             a deserializable value of type ``t``
*/
@unstable("the 'parallelIO' module is unstable and subject to change in a future release")
module ParallelIO {
  private use IO, BlockDist, List, CTypes, OS, CopyAggregation;

  private extern const qbytes_iobuf_size: c_ssize_t;

  /*
    Read a file's lines in parallel into a block-distributed array.

    This routine is similar to :proc:`readParallelDelimitedStrict`, except that it
    reads each line as a :type:`~String.string` or :type:`~Bytes.bytes` value.

    :arg filePath: a path to the file to read from
    :arg lineType: which type to represent a line: either ``string`` or ``bytes``
    :arg tasksPerLoc: the number of tasks to use per locale
        (if ``-1``, query ``here.maxTaskPar`` on each locale)
    :arg skipHeaderLines: the number of lines to skip at the beginning of the file
    :arg deserializerType: the type of deserializer to use
    :arg targetLocales: the locales to read the file on

    :returns: a block-distributed array of ``lineType`` values

    :throws: :class:`OffsetNotFoundError` if a starting offset cannot be found
             in any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
  */
  proc readParallelLines(filePath: string, type lineType = string, tasksPerLoc: int = -1,
                         skipHeaderLines: int = 0, type deserializerType = defaultDeserializer,
                         targetLocales: [?d] locale = Locales
  ): [] lineType throws
    where lineType == string || lineType == bytes
  {
    param delim = b"\n";

    const fMeta = open(filePath, ioMode.r),
          fileBounds = 0..<fMeta.size,
          byteOffsets = findDelimChunks(fMeta, delim, d.size, fileBounds, skipHeaderLines),
          itemOffsets = findItemOffsets(fMeta, delim, byteOffsets);

    var results = blockDist.createArray({0..<itemOffsets.last}, lineType, targetLocales=targetLocales);

    coforall (loc, id) in zip(targetLocales, 0..) with (ref results) do on loc {
      const nTasks = if tasksPerLoc < 0 then here.maxTaskPar else tasksPerLoc,
            locBounds = byteOffsets[id]..byteOffsets[id+1],
            locFile = open(filePath, ioMode.r),
            tByteOffsets = findDelimChunks(locFile, delim, nTasks, locBounds, 0),
            tItemOffsets = findItemOffsets(locFile, delim, tByteOffsets) + itemOffsets[id];

      coforall tid in 0..<nTasks with (ref results) {
        var des: deserializerType;
        const taskBounds = tByteOffsets[tid]..tByteOffsets[tid+1],
              r = locFile.reader(locking=false, region=taskBounds, deserializer=des);

        for i in tItemOffsets[tid]..<tItemOffsets[tid+1] do
          results[i] = r.readLine(lineType);
      }
    }

    return results;
  }

  /*
    Read a file's lines in parallel into an array.

    This routine is essentially the same as :proc:`readParallelLines`, except
    that it only executes on the calling locale. As such, it does not accept a
    ``targetLocales`` argument and returns a non-distributed array.

    :arg filePath: a path to the file to read from
    :arg lineType: which type to represent a line: either ``string`` or ``bytes``
    :arg nTasks: the number of tasks to use
    :arg skipHeaderLines: the number of lines to skip at the beginning of the file
    :arg deserializerType: the type of deserializer to use

    :returns: a default rectangular array of ``lineType`` values

    :throws: :class:`OffsetNotFoundError` if a starting offset cannot be found
             in any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
  */
  proc readParallelLinesLocal(filePath: string, type lineType = string, nTasks: int = here.maxTaskPar,
                              skipHeaderLines: int = 0, type deserializerType = defaultDeserializer
  ): [] lineType throws
    where lineType == string || lineType == bytes
  {
    param delim = b"\n";

    const f = open(filePath, ioMode.r),
          fileBounds = 0..<f.size,
          byteOffsets = findDelimChunks(f, delim, nTasks, fileBounds, skipHeaderLines),
          itemOffsets = findItemOffsets(f, delim, byteOffsets);

    var results: [0..<itemOffsets.last] lineType;

    coforall tid in 0..<nTasks with (ref results) {
      var des: deserializerType;
      const taskBounds = byteOffsets[tid]..byteOffsets[tid+1],
            r = f.reader(locking=false, region=taskBounds, deserializer=des);

      for i in itemOffsets[tid]..<itemOffsets[tid+1] do
        results[i] = r.readLine(lineType);
    }

    return results;
  }

  /*
    Read a delimited file in parallel into a block-distributed array.

    This routine assumes that the file is composed of a series of deserializable
    values of type ``t`` (optionally with a header at the beginning of the file).
    Each ``t`` must be separated by exactly one delimiter which can either be
    provided as a ``string`` or ``bytes`` value.

    This routine will use the delimiter to split the file into ``d.size`` chunks
    of roughly equal size and read each chunk concurrently across the target locales.
    If multiple tasks are used per locale, each locale will further decompose its
    chunk into smaller chunks and read each of those in parallel. The chunks and
    corresponding array indices are computed using :proc:`findDelimChunks` and
    :proc:`findItemOffsets` respectively.

    .. note:: ``t`` must:

          * have a 'deserialize method'
          * have a default (zero argument) initializer
          * not contain the delimiter in its serialized form (if it does, use
            :proc:`readParallelDelimited` instead)

    This procedure can be used for a variety of purposes, such as reading a CSV
    file. To do so, the delimiter should keep its default value of ``b"\n"``.
    The file will then be split by lines, where each line will be parsed as a
    ``t`` value. For CSV, the commas between ``t``'s fields must be parsed by
    it's ``deserialize`` method. This can be accomplished in one of two ways:
    (1) by using a custom deserialize method that parses the comma values
    manually, e.g.,

    .. code-block:: chapel

        record color {
          var r, g, b: uint(8);
        }

        proc ref color.deserialize(reader, ref deserializer) throws {
          reader.read(this.r);
          reader.readLiteral(b",");
          reader.read(this.g);
          reader.readLiteral(b",");
          reader.read(this.b);
        }

    or (2) by using a deserializer that will handle commas appropriately
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
  proc readParallelDelimitedStrict(filePath: string, in delim: ?dt = b"\n", type t, tasksPerLoc: int = -1,
                                   skipHeaderLines: int = 0, type deserializerType = defaultDeserializer,
                                   targetLocales: [?d] locale = Locales
  ): [] t throws
    where d.rank == 1 && (dt == bytes || dt == string)
  {
    const fMeta = open(filePath, ioMode.r),
          fileBounds = 0..<fMeta.size,
          byteOffsets = findDelimChunks(fMeta, delim, d.size, fileBounds, skipHeaderLines),
          itemOffsets = findItemOffsets(fMeta, delim, byteOffsets);

    var results = blockDist.createArray({0..<itemOffsets.last}, t, targetLocales=targetLocales);

    coforall (loc, id) in zip(targetLocales, 0..) with (ref results) do on loc {
      const nTasks = if tasksPerLoc < 0 then here.maxTaskPar else tasksPerLoc,
            locBounds = byteOffsets[id]..byteOffsets[id+1],
            locFile = open(filePath, ioMode.r),
            tByteOffsets = findDelimChunks(locFile, delim, nTasks, locBounds, 0),
            tItemOffsets = findItemOffsets(locFile, delim, tByteOffsets) + itemOffsets[id];

      coforall tid in 0..<nTasks with (ref results) {
        var des: deserializerType;
        const taskBounds = tByteOffsets[tid]..tByteOffsets[tid+1],
              r = locFile.reader(locking=false, region=taskBounds, deserializer=des);

        var item: t, first = true;
        for i in tItemOffsets[tid]..<tItemOffsets[tid+1] {
          if first then first = false; else r.advanceThrough(delim);
          r.read(item);
          results[i] = item;
        }
      }
    }

    return results;
  }

  /*
    Read a delimited file in parallel into an array.

    This procedure is essentially the same as :proc:`readParallelDelimitedStrict`,
    except that it only executes on the calling locale. As such, it does not
    accept a ``targetLocales`` argument and returns a non-distributed array.

    :arg filePath: a path to the file to read from
    :arg delim: the delimiter to use to separate ``t`` values in the file
    :arg t: the type of value to read from the file
    :arg nTasks: the number of tasks to use
    :arg skipHeaderLines: the number of lines to skip at the beginning of the file
    :arg deserializerType: the type of deserializer to use

    :returns: a default rectangular array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a starting offset cannot be found in
              any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readParallelDelimitedStrictLocal(filePath: string, in delim: ?dt = b"\n", type t,
                                        nTasks: int = here.maxTaskPar, skipHeaderLines: int = 0,
                                        type deserializerType = defaultDeserializer
  ): [] t throws
    where dt == bytes || dt == string
  {
    const f = open(filePath, ioMode.r),
          fileBounds = 0..<f.size,
          byteOffsets = findDelimChunks(f, delim, nTasks, fileBounds, skipHeaderLines),
          itemOffsets = findItemOffsets(f, delim, byteOffsets);

    const resDom = {0..<itemOffsets.last};
    var results: [resDom] t;

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
    Read a delimited file in parallel into a block-distributed array.

    This routine assumes that the file is composed of a series of deserializable
    values of type ``t`` (optionally with a header at the beginning of the file).
    Each ``t`` must be separated by a delimiter which can either be provided as
    a ``string`` or ``bytes`` value. Unlike :proc:`readParallelDelimitedStrict`
    the delimiter can also be found in the serialized form of ``t``.

    This routine will use the delimiter to split the file into ``d.size`` chunks
    of roughly equal size and read each chunk concurrently across the target locales.
    If multiple tasks are used per locale, each locale will further decompose its
    chunk into smaller chunks and read each of those in parallel. The chunks are
    computed using :proc:`findDelimChunksChecked`.

    .. note:: ``t`` must:

          * have a 'deserialize' method that throws when a ``t`` cannot be read
          * have a default (zero argument) initializer

    :arg filePath: a path to the file to read from
    :arg delim: the delimiter to use to separate ``t`` values in the file
    :arg t: the type of value to read from the file
    :arg delimInclusive: whether or not ``t`` includes the delimiter at the
                         beginning of its serialized form (i.e., is ``t``'s
                         deserialize method also expecting to consume the
                         delimiter?)
    :arg tasksPerLoc: the number of tasks to use per locale
        (if ``-1``, query ``here.maxTaskPar`` on each locale)
    :arg skipHeaderLines: the number of lines to skip at the beginning of the file
    :arg deserializerType: the type of deserializer to use
    :arg targetLocales: the locales to read the file on

    :returns: a block-distributed array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
             in any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readParallelDelimited(filePath: string, in delim: ?dt = b"\n", type t, delimInclusive = false,
                             tasksPerLoc: int = -1, skipHeaderLines: int = 0,
                             type deserializerType = defaultDeserializer,
                             targetLocales: [?d] locale = Locales
  ): [] t throws
    where d.rank == 1 && (dt == bytes || dt == string)
  {
    const OnePerLoc = blockDist.createDomain({0..<d.size}, targetLocales=targetLocales);
    var results: [OnePerLoc] list(t),
        globalStartOffset = 0;

    // find the starting offsets for each locale
    const fMeta = open(filePath, ioMode.r),
          fileBounds = globalStartOffset..<fMeta.size,
          byteOffsets = findDelimChunksChecked(fMeta, delim, d.size, t, fileBounds,
                                               skipHeaderLines, deserializerType, delimInclusive);

    // writeln(byteOffsets);

    coforall (loc, id) in zip(targetLocales, 0..) with (ref results) do on loc {
      const nTasks = if tasksPerLoc < 0 then here.maxTaskPar else tasksPerLoc,
            locBounds = byteOffsets[id]..byteOffsets[id+1],
            locFile = open(filePath, ioMode.r),
            tByteOffsets = findDelimChunksChecked(locFile, delim, nTasks, t, locBounds,
                                                  0, deserializerType, delimInclusive);

      var tResults: [0..nTasks] list(t);
      coforall tid in 0..<nTasks with (ref tResults) {
        const taskBounds = tByteOffsets[tid]..tByteOffsets[tid+1];
        var des: deserializerType,
            r = locFile.reader(locking=false, region=taskBounds, deserializer=des),
            s = new t();

        // read all the 't' values from the chunk into a list
        while r.read(s) do
          tResults[tid].pushBack(s);
      }

      // gather the results from each task into a single list
      for tid in 0..<nTasks do
        results[id].pushBack(tResults[tid]);
    }

    // gather results from each locale into an array
    const nPerLoc = [i in 0..d.size] if i < d.size then results[i].size else 0,
          itemOffsets = (+ scan nPerLoc) - nPerLoc;

    var result = blockDist.createArray({0..<itemOffsets.last}, t, targetLocales=targetLocales);
    coforall (loc, id) in zip(targetLocales, 0..) with (ref result) do on loc {
      forall i in itemOffsets[id]..<itemOffsets[id+1] with (var agg = new DstAggregator(t)) do
        agg.copy(result[i], results[id][i-itemOffsets[id]]);
    }

    return result;
  }

  /*
    Read a delimited file in parallel into an array.

    This procedure is essentially the same as :proc:`readParallelDelimited`,
    except that it only executes on the calling locale. As such, it does not
    accept a ``targetLocales`` argument and returns a non-distributed array.

    :arg filePath: a path to the file to read from
    :arg delim: the delimiter to use to separate ``t`` values in the file
    :arg t: the type of value to read from the file
    :arg delimInclusive: whether or not ``t`` includes the delimiter at the
                         beginning of its serialized form (i.e., is ``t``'s
                         deserialize method also expecting to consume the
                         delimiter?)
    :arg nTasks: the number of tasks to use
    :arg skipHeaderLines: the number of lines to skip at the beginning of the file
    :arg deserializerType: the type of deserializer to use

    :returns: a default rectangular array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
             in any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readParallelDelimitedLocal(filePath: string, in delim: ?dt = b"\n", type t,
                                  delimInclusive: bool = false, nTasks: int = here.maxTaskPar,
                                  skipHeaderLines: int = 0,
                                  type deserializerType = defaultDeserializer
  ): [] t throws
    where dt == bytes || dt == string
  {
    var results: [0..nTasks] list(t);
    const f = open(filePath, ioMode.r),
          fileBounds = 0..<f.size,
          byteOffsets = findDelimChunksChecked(f, delim, nTasks, t, fileBounds, skipHeaderLines,
                                               deserializerType, delimInclusive);

    coforall tid in 0..<nTasks with (ref results) {
      const taskBounds = byteOffsets[tid]..byteOffsets[tid+1];
      var des: deserializerType,
          r = f.reader(locking=false, region=taskBounds, deserializer=des),
          s = new t();

      // read all the 't' values in the chunk into a list
      while r.read(s) do
        results[tid].pushBack(s);
    }

    // gather results from each task into an array
    const nPerTask = [r in results] r.size,
          itemOffsets = (+ scan nPerTask) - nPerTask;

    var result: [0..<itemOffsets.last] t;
    coforall tid in 0..<nTasks do
      result[itemOffsets[tid]..<itemOffsets[tid+1]] = results[tid].toArray();

    return result;
  }

  /*
    Read a file in parallel into a block-distributed array.

    This routine assumes that the file is composed of a series of deserializable
    values of type ``t`` (optionally with a header at the beginning of the file).
    It is intended for use in situations where a ``t``'s serialized form is of
    variable length, such as when it contains a string field. If the file contains a
    reliable delimiter between each ``t``, consider using :proc:`readParallelDelimited`
    or :proc:`readParallelDelimitedStrict` instead for better performance.

    This procedure uses :proc:`findDeserChunks` to split the file into ``d.size``
    chunks of roughly equal size where each chunk is read concurrently across the
    target locales. If multiple tasks are used per locale, each will further decompose
    its chunk into smaller chunks and read each of those in parallel.

    .. note:: ``t`` must:

          * have a 'deserialize' method that throws when a ``t`` cannot be read
          * have a default (zero argument) initializer

    :arg filePath: a path to the file to read from
    :arg t: the type of value to read from the file
    :arg tasksPerLoc: the number of tasks to use per locale
        (if ``-1``, query ``here.maxTaskPar`` on each locale)
    :arg skipHeaderBytes: the number of bytes to skip at the beginning of the file
        (if ``-1``, search for the first ``t`` value in the file and start there)
    :arg deserializerType: the type of deserializer to use
    :arg targetLocales: the locales to read the file on

    :returns: a block-distributed array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
             in any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readParallel(filePath: string, type t, tasksPerLoc: int = -1, skipHeaderBytes: int = 0,
                    type deserializerType = defaultDeserializer,
                    targetLocales: [?d] locale = Locales
  ): [] t throws
    where d.rank == 1
  {
    const OnePerLoc = blockDist.createDomain({0..<d.size}, targetLocales=targetLocales);
    var results: [OnePerLoc] list(t),
        findStart = false,
        globalStartOffset = 0;

    if skipHeaderBytes < 0
      then findStart = true;
      else globalStartOffset = skipHeaderBytes;

    // find the starting offsets for each locale
    const fMeta = open(filePath, ioMode.r),
          fileBounds = globalStartOffset..<fMeta.size,
          byteOffsets = findDeserChunks(fMeta, d.size, fileBounds, t, findStart, deserializerType);

    coforall (loc, id) in zip(targetLocales, 0..) with (ref results) do on loc {
      const nTasks = if tasksPerLoc < 0 then here.maxTaskPar else tasksPerLoc,
            locBounds = byteOffsets[id]..<byteOffsets[id+1],
            locFile = open(filePath, ioMode.r),
            tByteOffsets = findDeserChunks(locFile, nTasks, locBounds, t, false, deserializerType);

      var tResults: [0..<nTasks] list(t);
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
        results[id].pushBack(tResults[tid]);
    }

    // gather results from each locale into an array
    const nPerLoc = [i in 0..d.size] if i > d.high then 0 else results[i].size,
          itemOffsets = (+ scan nPerLoc) - nPerLoc;

    var result = blockDist.createArray({0..<itemOffsets.last}, t, targetLocales=targetLocales);
    coforall (loc, id) in zip(targetLocales, 0..) with (ref result) do on loc do
      forall i in itemOffsets[id]..<itemOffsets[id+1] with (var agg = new DstAggregator(t)) do
        agg.copy(result[i], results[id][i-itemOffsets[id]]);

    return result;
  }

  /*
    Read a file in parallel into an array.

    This routine is essentially the same as :proc:`readParallel`, except that
    it only executes on the calling locale. As such, it does not accept a
    ``targetLocales`` argument and returns a non-distributed array.

    :arg filePath: a path to the file to read from
    :arg t: the type of value to read from the file
    :arg nTasks: the number of tasks to use
    :arg skipHeaderBytes: the number of bytes to skip at the beginning of the file
        (if ``-1``, search for the first ``t`` value in the file and start there)
    :arg deserializerType: the type of deserializer to use

    :returns: a default rectangular array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
             in any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readParallelLocal(filePath: string, type t, nTasks: int = here.maxTaskPar, skipHeaderBytes: int = 0,
                         type deserializerType = defaultDeserializer
  ): [] t throws {
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
          byteOffsets = findDeserChunks(f, nTasks, fileBounds, t, findStart, deserializerType);

    coforall tid in 0..<nTasks with (ref results) {
      const taskBounds = byteOffsets[tid]..<byteOffsets[tid+1];
      var des: deserializerType,
          r = f.reader(locking=false, region=taskBounds, deserializer=des),
          s = new t();

      // read all the 't' values in the chunk into a list
      while r.read(s) do
        results[tid].pushBack(s);
    }

    const nPerTask = [r in results] r.size,
          itemOffsets = (+ scan nPerTask) - nPerTask;

    var result: [0..<itemOffsets.last] t;
    coforall tid in 0..<nTasks do
      result[itemOffsets[tid]..<itemOffsets[tid+1]] = results[tid].toArray();

    return result;
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

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
              in any of the chunks
  */
  proc findDelimChunks(const ref f: file, in delim: ?dt, n: int, bounds: range,
                       skipHeaderLines: int
  ): [] int throws
    where dt == bytes || dt == string
  {
    const r = f.reader(locking=false, region=bounds);
    for 0..<skipHeaderLines do r.readLine();

    const nDataBytes = bounds.high - r.offset(),
          approxBytesPerChunk = nDataBytes / n;

    var chunkOffsets: [0..n] int;
    chunkOffsets[0] = r.offset();
    chunkOffsets[n] = bounds.high;

    for i in 1..<n {
      const estOffset = chunkOffsets[i-1] + approxBytesPerChunk;
      r.seek(estOffset..);              // seek to the estimated offset
      try {
        r.advanceThrough(delim);        // advance past the next delimiter
      } catch {
        // there wasn't an offset in this chunk
        throw new OffsetNotFoundError(
          "Failed to find byte offset in the range (" + bounds.low:string + ".." + bounds.high:string +
          ") for chunk " + i:string + " of " + n:string + ". Try using fewer tasks."
        );
      }
      chunkOffsets[i] = r.offset();     // record the offset for this chunk
    }

    return chunkOffsets;
  }

  /*
    Get an array of ``n+1`` byte offsets that divide the file ``f`` into ``n``
    roughly equally sized chunks, where each byte offset lines up with a
    delimiter and a ``t`` can be deserialized at that offset.

    This procedure is similar to :proc:`findDelimChunks`, except that when it
    finds a delimiter, it confirms that a ``t`` can be deserialized at that
    offset before recording it. This way, the serialized values can also
    contain the delimiter.

    :arg f: the file to search
    :arg delim: the delimiter to use to separate the file into chunks
    :arg n: the number of chunks to find
    :arg t: the type of value to read from the file
    :arg bounds: a range of byte offsets to break into chunks
    :arg skipHeaderLines: the number of lines to skip at the beginning of the range
    :arg deserializerType: the type of deserializer to use
    :arg delimInclusive: whether or not the serialized form of ``t`` includes
                         the delimiter itself at the beginning of its serialized
                         form

    :returns: a length ``n+1`` array of byte offsets (the last offset is
              ``bounds.high``)

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
              in any of the chunks
  */
  proc findDelimChunksChecked(const ref f: file, in delim: ?dt, n: int, type t, bounds: range,
                              skipHeaderLines: int, type deserializerType = defaultDeserializer,
                              delimInclusive: bool = false
  ): [] int throws
    where dt == bytes || dt == string
  {
    const r = f.reader(locking=false, region=bounds);
    for 0..<skipHeaderLines do r.readLine();

    const nDataBytes = bounds.high - r.offset(),
          approxBytesPerChunk = nDataBytes / n,
          max_deser_trials = qbytes_iobuf_size; // could be something smarter?

    var chunkOffsets: [0..n] int;
    chunkOffsets[0] = r.offset();
    chunkOffsets[n] = bounds.high;

    for i in 1..<n {
      const estOffset = i * approxBytesPerChunk;
      var r = f.reader(locking=false, region=estOffset..),
          tTest: t;

      for 1..max_deser_trials {
        try {
          if delimInclusive
            then r.advanceTo(delim);      // advance to the next delimiter
            else r.advanceThrough(delim); // advance past the next delimiter
        } catch {
          throw new OffsetNotFoundError(
            "Failed to find byte offset in the range (" + bounds.low:string + ".." + bounds.high:string +
            ") for chunk " + i:string + " of " + n:string + ". Try using fewer tasks."
          );
        }

        // try to deserialize a 't' at the offset
        r.mark();
        try {
          r.withDeserializer(deserializerType).read(tTest);
        } catch {
          r.revert();   // move back to the marked position
          r.readByte(); // advance to the next byte
          continue;     // try again
        }
        r.revert();
        chunkOffsets[i] = r.offset();
        break;
      }
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
              items in the file before the start of each chunk. The last entry
              contains the total number of items in the file.
  */
  proc findItemOffsets(const ref f: file, in delim: ?dt, const ref byteOffsets: [?d] int): [d] int throws
    where dt == bytes || dt == string
  {
    var nPerChunk: [d] int;

    coforall c in 0..<d.high with (ref nPerChunk) {
      const r = f.reader(locking=false, region=byteOffsets[c]..byteOffsets[c+1]);

      // count the number of items in the chunk
      var n = 0;
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
    Get an array of ``n+1`` roughly evenly spaced byte offsets in the file
    ``f`` where a deserializable value of type ``t`` begins.

    The procedure will split the file into approximately ``bounds.size / n``
    sized chunks and search for a starting offset near the boundary of each
    chunk where a ``t`` can be deserialized.

    This procedure uses ``t``'s throwing ``deserialize`` method to determine
    whether or not a ``t`` can be deserialized at a given offset. If the
    deserialize method throws, the procedure will move on to the next byte
    offset and try again. Note that this can be inefficient if ``t`` is large
    and/or has a complex deserialize method.

    :arg f: the file to search
    :arg n: the number of chunks to find
    :arg bounds: a range of byte offsets to break into chunks
    :arg findStart: whether or not to search for the first offset
                  * if ``false``, the first offset will be ``bounds.low``
                  * if ``true``, the first offset will be found by searching for a ``t`` after ``bounds.low``
    :arg deserializerType: the type of deserializer to use

    :returns: a length ``n+1`` array of byte offsets (the last offset is
              ``bounds.high``)

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
              in any of the chunks
  */
  proc findDeserChunks(const ref f: file, n: int, bounds: range, type t, findStart: bool,
                       type deserializerType = defaultDeserializer
  ) : [] int throws {
    const approxBytesPerChunk = bounds.size / n,
          offsetIndices = if findStart then 0..<n else 1..<n;

    var startOffsets: [0..n] int;
    startOffsets[0] = bounds.low;
    startOffsets[n] = bounds.high+1;

    for i in offsetIndices {
      const estOffset = bounds.low + i * approxBytesPerChunk,
            maxOffset = bounds.high;

      var r = f.reader(locking=false, region=estOffset..),
          tTest: t;

      for byteOffset in estOffset..maxOffset {
        r.mark();
        try {
          r.withDeserializer(deserializerType).read(tTest);
        } catch {
          r.revert();   // move back to the marked position
          r.readByte(); // advance to the next byte
          continue;     // try again
        }
        r.commit();
        startOffsets[i] = byteOffset;
        break;
      }

      if r.offset() == maxOffset - 1 {
        throw new OffsetNotFoundError(
          "Failed to find byte offset in the range (" + estOffset:string + ".." + maxOffset:string +
          ") for chunk " + i:string + " of " + n:string + ". Try using fewer tasks."
        );
      }
    }

    return startOffsets;
  }

  // /*
  //   Get an array of ``n+1`` roughly evenly spaced byte offsets in the file
  //   ``f`` where a deserializable value of type ``t`` begins.

  //   The procedure will split the file into approximately ``bounds.size / n``
  //   sized chunks and search for a starting offset near the boundary of each
  //   chunk where a ``t`` can be deserialized.

  //   :arg f: the file to search
  //   :arg n: the number of chunks to find
  //   :arg bounds: a range of byte offsets to break into chunks
  //   :arg findStart: whether or not to search for the first offset
  //                 * if ``false``, the first offset will be ``bounds.low``
  //                 * if ``true``, the first offset will be found by searching for a ``t`` after ``bounds.low``
  //   :arg deserializerType: the type of deserializer to use

  //   :returns: a length ``n+1`` array of byte offsets (the last offset is
  //             ``bounds.high``)

  //   :throws: ``OffsetNotFoundError`` if a starting offset cannot be found in
  //             any of the chunks
  // */
  // proc findByteOffsets(f: file, n: int, bounds: range, type t, findStart: bool,
  //                      type deserializerType = defaultDeserializer
  // ) : [] int throws {
  //   const approxBytesPerChunk = bounds.size / n,
  //         offsetIndices = if findStart then 0..<n else 1..<n;

  //   var startOffsets: [0..n] int;
  //   startOffsets[0] = bounds.low;
  //   startOffsets[n] = bounds.high;

  //   for i in offsetIndices {
  //     const estOffset = bounds.low + i * approxBytesPerChunk,
  //           startOffset = max(estOffset - (qbytes_iobuf_size / 2), bounds.low),
  //           stopOffset = min(startOffset + qbytes_iobuf_size, bounds.high);

  //     // TODO: if approxBytesPerChunk > qbytes_iobuf_size and each 't' is _very_ large,
  //     // this method could fail to find a starting offset in the first iobuf chunk.
  //     // Setup a mechanism to read a new buffer-chunk from the file if we step
  //     // outside the first?
  //     // Or is that a non issue since this method would be extremely slow in that
  //     // case anyway? (many many sequential try/throw/catch attempts)

  //     var des: deserializerType,
  //         r = f.reader(locking=false, region=startOffset.., deserializer=des);
  //     r.mark();

  //     // seek to the start offset and 'mark' to ensure the iobuf chunk stays in memory
  //     // r.seek(startOffset..); r.mark();
  //     // read a byte to force the buffer to be filled
  //     r.mark(); r.readByte(); r.revert();
  //     // start reading from the estimated offset
  //     qio_channel_seek_unsafe(r._channel_internal, estOffset - startOffset);

  //     var prevOffset = r.offset();
  //     for delta in seekIndices(rightOnly = (i == 0)) {
  //       const offset = estOffset + delta;

  //       // error out if a starting offset cannot be found in the iobuf chunk
  //       if offset < startOffset || offset > stopOffset {
  //         r.revert();
  //         throw new OffsetNotFoundError(
  //           "Failed to find starting file offset in the range (" + startOffset:string + " .. " + stopOffset:string +
  //           ") for chunk " + i:string + " of " + n:string + ". Try using fewer tasks."
  //         );
  //       }

  //       // seek to 'offset'
  //       qio_channel_seek_unsafe(r._channel_internal, offset - prevOffset);
  //       prevOffset = offset;

  //       // try to read a 't'
  //       r.mark();
  //       try {
  //         r.read(t);
  //       } catch e {
  //         r.revert();
  //         continue;
  //       }
  //       r.commit();

  //       // if successful, we found a starting offset
  //       // save it and move on to the next chunk
  //       startOffsets[i] = offset;
  //       break;
  //     }
  //   }
  //   return startOffsets;
  // }

  // private iter seekIndices(rightOnly: bool) {
  //   if rightOnly {
  //     for i in 0.. do yield i;
  //   } else {
  //     // 0, 1, -1, 2, -2, 3, -3, ...
  //     var step = 1,
  //         dir = 1,
  //         i = 0;

  //     while true {
  //       yield i;
  //       i += step * dir;
  //       step += 1;
  //       dir *= -1;
  //     }
  //   }
  // }

  /*
    An error thrown when a starting offset cannot be found in a chunk of a file.
  */
  class OffsetNotFoundError: Error {
    proc init() do super.init();
    proc init(msg: string) do super.init(msg);
  }
}
