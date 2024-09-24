/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

  This module provides a few procedures for reading a file's contents into
  a distributed array in parallel. The procedures are designed to be used for
  cases where a large file contains a header followed by a continuous stream
  of delimited values of the same type. The procedures are:

  * :proc:`readLinesAsBlockArray`: read each of the lines of a file as a
    ``string`` or ``bytes`` value
  * :proc:`readDelimitedAsBlockArray`: read a file where each value is strictly
    separated by a delimiter, and the delimiter cannot be found in the
    value (e.g., CSV)
  * :proc:`readItemsAsBlockArray`: read a file where values are separated by a
    delimiter, but the delimiter can be found in the value

  There are also non-distributed versions of these procedures that return a
  default rectangular array instead of a block-distributed array. These tend
  to be a faster option if the file is small enough to fit in memory on a
  single locale.

  Two parallel iterators are also provided:

  * :proc:`readLines`: iterate over a file's lines in parallel
  * :proc:`readDelimited`: iterate over the values of a delimited file in parallel

  Both iterators only work in a standalone context (i.e., they cannot be used
  for zippered iteration). Adding leader/follower support is a future goal.

  This module also exposes some helper procedures used to break files into chunks.
  These could be used as building blocks to implement other parallel I/O routines:

  * :proc:`findDelimChunks`: find a set of byte offsets that divide a file into
    roughly equal chunks where each chunk begins with a delimiter
  * :proc:`findItemOffsets`: get a prefix sum of the number of items in each
    chunk of a file, where the chunks are defined by the ``byteOffsets`` array,
    and each item is strictly separated by the given delimiter
  * :proc:`findDelimChunksChecked`: find a set of byte offsets that divide a file
    into roughly equal chunks where each chunk begins with a delimiter and each
    chunk starts with a deserializable value of the given type
*/
@unstable("the 'parallelIO' module is unstable and subject to change in a future release")
module ParallelIO {
  private use IO, BlockDist, List, CTypes, OS;

  /*
    Iterate over a file's lines in parallel.

    This routine is similar to :proc:`readLinesAsArray`, except that it yields
    each line as it is read instead of returning an array of lines.

    **Example**:

    .. code-block:: chapel

      use ParallelIO;

      var sum = 0;
      forall line in readLines("ints.txt") with (+ reduce sum) do
        sum += line:int;

    .. warning::

      This routine will halt if the file cannot be opened or if an
      I/O error occurs while reading the file. This limitation is
      expected to be removed in a future release.

    .. note::

      Only serial and standalone-parallel iteration is supported. This
      iterator cannot yet be used in a zippered context.

    :arg filePath: the file to read
    :arg lineType: which type to return for each line: either ``string`` or
                   ``bytes`` — defaults to ``string``
    :arg header: how to handle the file header (see :record:`headerPolicy`)
    :arg nTasks: the number of tasks used to read the file — defaults to
                 ``here.maxTaskPar``
    :arg targetLocales: the locales to use for reading the file — by default,
                        only the calling locale is used
  */
  iter readLines(filePath: string, type lineType = string,
                 header = headerPolicy.noHeader, nTasks: int = here.maxTaskPar,
                 targetLocales: [] locale = [here,]
  ): lineType
    where lineType == string || lineType == bytes
  {
    param delim = b"\n";

    const f = try! open(filePath, ioMode.r),
          (findStart, startOffset) = try! header.apply(f, 0..<f.size),
          r = try! f.reader(locking=false, region=startOffset..);

    if findStart then try! r.advanceThrough(delim);

    var line: lineType;
    while (try! r.readLine(line, stripNewline=true)) do
      yield line;
  }

  @chpldoc.nodoc
  iter readLines(param tag: iterKind, filePath: string, type lineType = string,
                 header = headerPolicy.noHeader, nTasks: int = here.maxTaskPar,
                 targetLocales: [?tld] locale = [here,]
  ): lineType
    where tag == iterKind.standalone && (lineType == string || lineType == bytes)
  {
    param delim = b"\n";

    const fMeta = try! open(filePath, ioMode.r),
          fileBounds = 0..(try! fMeta.size);

    if tld.size == 0 || (tld.size == 1 && targetLocales.first == here) {
      const byteOffsets = try! findDelimChunks(fMeta, delim, nTasks, fileBounds, header);
      coforall tid in 0..<nTasks {
        const taskBounds = byteOffsets[tid]..<byteOffsets[tid+1],
              r = try! fMeta.reader(locking=false, region=taskBounds);

        var line: lineType;
        while (try! r.readLine(line, stripNewline=true)) do
          yield line;
      }
    } else {
      const byteOffsets = try! findDelimChunks(fMeta, delim, tld.size, fileBounds, header);
      coforall (loc, id) in zip(targetLocales, 0..) do on loc {
        const locBounds = byteOffsets[id]..byteOffsets[id+1];

        // if byteOffsets looks like [0, 10, 10, 14, 21], then don't try to read 10..10 (locale 1)
        if locBounds.size > 1 {
          const locFile = try! open(filePath, ioMode.r),
                locByteOffsets = try! findDelimChunks(locFile, delim, nTasks, locBounds, headerPolicy.noHeader);

          coforall tid in 0..<nTasks {
            const taskBounds = locByteOffsets[tid]..<locByteOffsets[tid+1],
                  r = try! locFile.reader(locking=false, region=taskBounds);

            var line: lineType;
            while (try! r.readLine(line, stripNewline=true)) do
              yield line;
          }
        }
      }
    }
  }

  /*
    Iterate over the values of a delimited file in parallel.

    This routine is similar to :proc:`readDelimitedAsArray`, except that it
    yields each value as it is read instead of returning an array of values.

    **Example**:

    .. code-block:: chapel

      use IO, ParallelIO;

      record color {
        var r, g, b: uint(8);

        proc ref deserialize(reader: fileReader(?), ref deserializer) throws {
          reader.read(this.r);
          reader.readLiteral(b",");
          reader.read(this.g);
          reader.readLiteral(b",");
          reader.read(this.b);
        }
      }

      forall c in readDelimited("colors.csv", color, header=headerPolicy.skipLines(1)) do
        processColor(c);

    .. warning::

      This routine will halt if the file cannot be opened or if an
      I/O error occurs while reading the file. This limitation is
      expected to be removed in a future release.

    .. note::

      Only serial and standalone-parallel iteration is supported. This
      iterator cannot yet be used in a zippered context.

    :arg filePath: the file to read
    :arg t: the type of value to read from the file
    :arg delim: the delimiter to use to separate ``t`` values in the file —
                defaults to the newline character
    :arg header: how to handle the file header (see :record:`headerPolicy`)
    :arg nTasks: the number of tasks used to read the file — defaults to
                 ``here.maxTaskPar``
    :arg deserializerType: The type of deserializer to use when reading values —
                           defaults to the I/O module's default deserializer
    :arg targetLocales: the locales to use for reading the file — by default,
                        only the calling locale is used
  */
  iter readDelimited(filePath: string, type t, in delim: ?dt = b"\n",
                     header = headerPolicy.noHeader, nTasks: int = here.maxTaskPar,
                     type deserializerType = defaultDeserializer,
                     targetLocales: [] locale = [here,]
  ): t
    where dt == string || dt == bytes
  {
    var des: deserializerType;
    const f = try! open(filePath, ioMode.r),
          (findStart, startOffset) = try! header.apply(f, 0..<f.size),
          r = try! f.reader(locking=false, region=startOffset.., deserializer=des);

    if findStart then try! r.advanceThrough(delim);

    var item: t, first = true;
    while (try! r.read(item)) {
      if first then first = false; else try! r.advanceThrough(delim);
      yield item;
    }
  }

  @chpldoc.nodoc
  iter readDelimited(param tag: iterKind,
                     filePath: string, type t, in delim: ?dt = b"\n",
                     header = headerPolicy.noHeader, nTasks: int = here.maxTaskPar,
                     type deserializerType = defaultDeserializer,
                     targetLocales: [?tld] locale = [here,]
  ): t
    where tag == iterKind.standalone && (dt == string || dt == bytes)
  {
    const fMeta = try! open(filePath, ioMode.r),
          fileBounds = 0..(try! fMeta.size);

    if tld.size == 0 || (tld.size == 1 && targetLocales.first == here) {
      const byteOffsets = try! findDelimChunks(fMeta, delim, nTasks, fileBounds, header);
      coforall tid in 0..<nTasks {
        var des: deserializerType;
        const taskBounds = byteOffsets[tid]..byteOffsets[tid+1],
              r = try! fMeta.reader(locking=false, region=taskBounds, deserializer=des);

        var item: t;
        while (try! r.read(item)) {
          yield item;
          try! r.advanceThrough(delim);
        }
      }
    } else {
      const byteOffsets = try! findDelimChunks(fMeta, delim, tld.size, fileBounds, header);
      coforall (loc, id) in zip(targetLocales, 0..) do on loc {
        const locBounds = byteOffsets[id]..<byteOffsets[id+1];

        // if byteOffsets looks like [0, 10, 10, 14, 21], then don't try to read 10..10 (locale 1)
        if locBounds.size > 1 {
          const locFile = try! open(filePath, ioMode.r),
                locByteOffsets = try! findDelimChunks(locFile, delim, nTasks, locBounds, headerPolicy.noHeader);

          coforall tid in 0..<nTasks {
            var des: deserializerType;
            const taskBounds = locByteOffsets[tid]..locByteOffsets[tid+1],
                  r = try! locFile.reader(locking=false, region=taskBounds, deserializer=des);

            var item: t;
            while (try! r.read(item)) {
              yield item;
              try! r.advanceThrough(delim);
            }
          }
        }
      }
    }
  }

  /*
    Read a file's lines in parallel into a block-distributed array.

    This routine is similar to :proc:`readDelimitedAsBlockArray`, except that it
    reads each line as a :type:`~String.string` or :type:`~Bytes.bytes` value.

    :arg filePath: the file to read
    :arg lineType: the element type of the returned array: either ``string`` or
                   ``bytes`` — defaults to ``string``
    :arg nTasks: the number of tasks to use per locale — defaults to ``-1``, meaning
                 each locale should query ``here.maxTaskPar``
    :arg header: how to handle the file header (see :record:`headerPolicy`)
    :arg targetLocales: the locales to read the file on and the target locales for
                        the returned block-distributed array

    :returns: a block-distributed array of ``lineType`` values

    :throws: :class:`OffsetNotFoundError` if a starting offset cannot be found
             in any of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readLinesAsBlockArray(filePath: string, type lineType = string, header = headerPolicy.noHeader,
                             nTasks: int = -1, targetLocales: [?d] locale = Locales
  ): [] lineType throws
    where lineType == string || lineType == bytes
  {
    param delim = b"\n";

    const fMeta = open(filePath, ioMode.r),
          fileBounds = 0..<fMeta.size,
          byteOffsets = findDelimChunks(fMeta, delim, d.size, fileBounds, header),
          itemOffsets = findItemOffsets(fMeta, delim, byteOffsets);

    var results = blockDist.createArray({0..<itemOffsets.last}, lineType, targetLocales=targetLocales);

    coforall (loc, id) in zip(targetLocales, 0..) with (ref results) do on loc {
      const tasksPerLoc = if nTasks < 0 then here.maxTaskPar else nTasks,
            locBounds = byteOffsets[id]..byteOffsets[id+1],
            locFile = open(filePath, ioMode.r),
            tByteOffsets = findDelimChunks(locFile, delim, tasksPerLoc, locBounds, headerPolicy.noHeader),
            tItemOffsets = findItemOffsets(locFile, delim, tByteOffsets) + itemOffsets[id];

      coforall tid in 0..<tasksPerLoc with (ref results) {
        const taskBounds = tByteOffsets[tid]..tByteOffsets[tid+1],
              r = locFile.reader(locking=false, region=taskBounds);

        for i in tItemOffsets[tid]..<tItemOffsets[tid+1] do
          results[i] = r.readLine(lineType, stripNewline=true);
      }
    }

    return results;
  }

  /*
    Read a file's lines in parallel into an array.

    This routine is essentially the same as :proc:`readLinesAsBlockArray`, except
    that it only executes on the calling locale. As such, it does not accept a
    ``targetLocales`` argument and returns a non-distributed array.

    :arg filePath: the file to read
    :arg lineType: the element type of the returned array: either ``string`` or
                   ``bytes`` — defaults to ``string``
    :arg nTasks: the number of tasks to use — defaults to ``here.maxTaskPar``
    :arg header: how to handle the file header (see :record:`headerPolicy`)

    :returns: a default rectangular array of ``lineType`` values

    :throws: :class:`OffsetNotFoundError` if a starting offset cannot be found
             in one or more of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
  */
  proc readLinesAsArray(filePath: string, type lineType = string,
                        header = headerPolicy.noHeader, nTasks: int = here.maxTaskPar
  ): [] lineType throws
    where lineType == string || lineType == bytes
  {
    param delim = b"\n";

    const f = open(filePath, ioMode.r),
          fileBounds = 0..<f.size,
          byteOffsets = findDelimChunks(f, delim, nTasks, fileBounds, header),
          itemOffsets = findItemOffsets(f, delim, byteOffsets);

    var results: [0..<itemOffsets.last] lineType;

    coforall tid in 0..<nTasks with (ref results) {
      const taskBounds = byteOffsets[tid]..byteOffsets[tid+1],
            r = f.reader(locking=false, region=taskBounds);

      for i in itemOffsets[tid]..<itemOffsets[tid+1] do
        results[i] = r.readLine(lineType, stripNewline=true);
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
          * not contain the delimiter in its serialized form (if it does,
            consider using :proc:`readItemsAsBlockArray` instead)

    This procedure can be used for a variety of purposes, such as reading a CSV
    file. To do so, the delimiter should keep its default value of ``b"\n"``.
    The file will then be split by lines, where each line will be parsed as a
    ``t`` value. For CSV, the commas between ``t``'s fields must be parsed by
    it's ``deserialize`` method. This can be accomplished in one of two ways:
    (1) by using a custom deserialize method that parses the comma values
    manually (like in the example below), or (2) by using a deserializer that
    will handle commas appropriately with ``t``'s default ``deserialize`` method.

    **Example:**

    .. code-block:: chapel

      use IO, ParallelIO;

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

      var colors = readDelimitedAsBlockArray(
        "colors.csv",
        t=color,
        header=headerPolicy.skipLines(1)
      );

    :arg filePath: the file to read
    :arg t: the type of value to read from the file
    :arg delim: the delimiter to use to separate ``t`` values in the file —
                defaults to the newline character
    :arg nTasks: the number of tasks to use per locale — defaults to ``-1``, meaning
                 each locale should query ``here.maxTaskPar``
    :arg header: how to handle the file header (see :record:`headerPolicy`)
    :arg deserializerType: the type of deserializer to use — defaults to the I/O
                           module's default deserializer
    :arg targetLocales: the locales to read the file on and the target locales for
                        the returned block-distributed array

    :returns: a block-distributed array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a starting offset cannot be found
             in one or more of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readDelimitedAsBlockArray(filePath: string, type t, in delim: ?dt = b"\n", header = headerPolicy.noHeader,
                                 nTasks: int = -1, type deserializerType = defaultDeserializer,
                                 targetLocales: [?d] locale = Locales
  ): [] t throws
    where d.rank == 1 && (dt == bytes || dt == string)
  {
    const fMeta = open(filePath, ioMode.r),
          fileBounds = 0..<fMeta.size,
          byteOffsets = findDelimChunks(fMeta, delim, d.size, fileBounds, header),
          itemOffsets = findItemOffsets(fMeta, delim, byteOffsets);

    var results = blockDist.createArray({0..<itemOffsets.last}, t, targetLocales=targetLocales);

    coforall (loc, id) in zip(targetLocales, 0..) with (ref results) do on loc {
      const tasksPerLoc = if nTasks < 0 then here.maxTaskPar else nTasks,
            locBounds = byteOffsets[id]..byteOffsets[id+1],
            locFile = open(filePath, ioMode.r),
            tByteOffsets = findDelimChunks(locFile, delim, tasksPerLoc, locBounds, headerPolicy.noHeader),
            tItemOffsets = findItemOffsets(locFile, delim, tByteOffsets) + itemOffsets[id];

      coforall tid in 0..<tasksPerLoc with (ref results) {
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

    This procedure is essentially the same as :proc:`readDelimitedAsBlockArray`,
    except that it only executes on the calling locale. As such, it does not
    accept a ``targetLocales`` argument and returns a non-distributed array.

    :arg filePath: the file to read
    :arg t: the type of value to read from the file
    :arg delim: the delimiter to use to separate ``t`` values in the file —
                defaults to the newline character
    :arg nTasks: the number of tasks to use — defaults to ``here.maxTaskPar``
    :arg header: how to handle the file header (see :record:`headerPolicy`)
    :arg deserializerType: the type of deserializer to use — defaults to the I/O
                           module's default deserializer

    :returns: a default rectangular array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a starting offset cannot be found in
              one or more of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readDelimitedAsArray(filePath: string, type t, in delim: ?dt = b"\n",
                            header = headerPolicy.noHeader, nTasks: int = here.maxTaskPar,
                            type deserializerType = defaultDeserializer
  ): [] t throws
    where dt == bytes || dt == string
  {
    const f = open(filePath, ioMode.r),
          fileBounds = 0..<f.size,
          byteOffsets = findDelimChunks(f, delim, nTasks, fileBounds, header),
          itemOffsets = findItemOffsets(f, delim, byteOffsets);

    const resDom = {0..<itemOffsets.last};
    var results: [resDom] t;

    coforall tid in 0..<nTasks with (ref results) {
      var des: deserializerType;
      const taskBounds = byteOffsets[tid]..<byteOffsets[tid+1],
            r = f.reader(locking=false, region=taskBounds, deserializer=des);

      var item: t, first = true;
      for i in itemOffsets[tid]..<itemOffsets[tid+1] {
        if first then first = false; else r.advanceThrough(delim);
        r.read(item);
        results[i] = item;
      }
    }

    return results;
  }

  /*
    Read items from a file in parallel into a block-distributed array.

    This routine assumes that the file is composed of a series of deserializable
    values of type ``t`` (optionally with a header at the beginning of the file).
    Each ``t`` must be separated by a delimiter which can either be provided as
    a ``string`` or ``bytes`` value. Unlike :proc:`readDelimitedAsBlockArray`
    the delimiter can also be found in the serialized form of ``t``.

    This routine uses the following heuristic to split the file into chunks, which
    may not be accurate in all cases:

    A given byte offset is a valid offset for a task to start deserializing
    values of type ``t`` if:

    * it is preceded by, or begins with the delimiter
    * a ``t`` can be deserialized at that offset (i.e., calling ``t.deserialize``
      on the bytes starting at that offset does not throw an error)

    The heuristic, implemented in :proc:`findDelimChunksChecked`, will be used to
    split the file in ``d.size`` chunks with a roughly equal number of items per
    chunk. If multiple tasks per locale are used, each locale will further decompose
    its chunk into smaller chunks and read each of those in parallel.

    .. note:: ``t`` must:

          * have a 'deserialize' method that throws when a valid ``t`` cannot be read
          * have a default (zero argument) initializer

    :arg filePath: the file to read
    :arg t: the type of value to read from the file
    :arg delim: the delimiter used to guide file chunking - defaults to the
                newline character
    :arg nTasks: the number of tasks to use per locale — defaults to ``-1``,
                 meaning each locale should query ``here.maxTaskPar``
    :arg header: how to handle the file header (see :record:`headerPolicy`)
    :arg deserializerType: the type of deserializer to use — defaults to the I/O
                           module's default deserializer
    :arg targetLocales: the locales to read the file on and the target locales for
                        the returned block-distributed array

    :returns: a block-distributed array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
             in one or more of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readItemsAsBlockArray(filePath: string, type t, in delim: ?dt = b"\n", header = headerPolicy.noHeader,
                             nTasks: int = -1, type deserializerType = defaultDeserializer,
                             targetLocales: [?d] locale = Locales
  ): [] t throws
    where d.rank == 1 && (dt == bytes || dt == string)
  {
    const OnePerLoc = blockDist.createDomain({0..<d.size}, targetLocales=targetLocales);
    var results: [OnePerLoc] list(t);

    // find the starting offsets for each locale
    const fMeta = open(filePath, ioMode.r),
          fileBounds = 0..<fMeta.size,
          byteOffsets = findDelimChunksChecked(fMeta, delim, d.size, t, fileBounds,
                                               header, deserializerType);

    coforall (loc, id) in zip(targetLocales, 0..) with (ref results) do on loc {
      const tasksPerLoc = if nTasks < 0 then here.maxTaskPar else nTasks,
            locBounds = byteOffsets[id]..byteOffsets[id+1],
            locFile = open(filePath, ioMode.r),
            tByteOffsets = findDelimChunksChecked(locFile, delim, tasksPerLoc, t, locBounds,
                                                  headerPolicy.noHeader, deserializerType);

      var tResults: [0..tasksPerLoc] list(t);
      coforall tid in 0..<tasksPerLoc with (ref tResults) {
        const taskBounds = tByteOffsets[tid]..tByteOffsets[tid+1];
        var des: deserializerType,
            r = locFile.reader(locking=false, region=taskBounds, deserializer=des),
            s = new t();

        // read all the 't' values from the chunk into a list
        while r.read(s) do
          tResults[tid].pushBack(s);
      }

      // gather the results from each task into a single list
      for tid in 0..<tasksPerLoc do
        results[id].pushBack(tResults[tid]);
    }

    // gather results from each locale into an array
    const nPerLoc = [i in 0..d.size] if i < d.size then results[i].size else 0,
          itemOffsets = (+ scan nPerLoc) - nPerLoc;

    var result = blockDist.createArray({0..<itemOffsets.last}, t, targetLocales=targetLocales);
    coforall (loc, id) in zip(targetLocales, 0..) with (ref result) do on loc {
      // TODO: aggregation
      forall i in itemOffsets[id]..<itemOffsets[id+1] do
        result[i] = results[id][i-itemOffsets[id]];
    }

    return result;
  }

  /*
    Read items from a file in parallel into an array.

    This procedure is essentially the same as :proc:`readItemsAsBlockArray`,
    except that it only executes on the calling locale. As such, it does not
    accept a ``targetLocales`` argument and returns a non-distributed array.

    :arg filePath: the file to read
    :arg t: the type of value to read from the file
    :arg delim: the delimiter used to guide file chunking - defaults to the
                newline character
    :arg nTasks: the number of tasks to use — defaults to ``here.maxTaskPar``
    :arg header: how to handle the file header (see :record:`headerPolicy`)
    :arg deserializerType: the type of deserializer to use — defaults to the I/O
                           module's default deserializer

    :returns: a default rectangular array of ``t`` values

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
             in one or more of the chunks

    See :proc:`~IO.open` for other errors that could be thrown when attempting
    to open the file
  */
  proc readItemsAsArray(filePath: string, type t, in delim: ?dt = b"\n",
                        header = headerPolicy.noHeader, nTasks: int = here.maxTaskPar,
                        type deserializerType = defaultDeserializer
  ): [] t throws
    where dt == bytes || dt == string
  {
    var results: [0..nTasks] list(t);
    const f = open(filePath, ioMode.r),
          fileBounds = 0..<f.size,
          byteOffsets = findDelimChunksChecked(f, delim, nTasks, t, fileBounds,
                                               header, deserializerType);

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
    Get an array of ``n+1`` byte offsets that divide the file ``f`` into ``n``
    roughly equally sized chunks, where each byte offset lines up with a
    delimiter.

    :arg f: the file to search
    :arg delim: the delimiter to use to separate the file into chunks
    :arg n: the number of chunks to find
    :arg bounds: a range of byte offsets to break into chunks
    :arg header: a header policy to use when searching for the first byte offset

    :returns: a length ``n+1`` array of byte offsets (the last offset is
              ``bounds.high``)

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
              in any of the chunks
  */
  proc findDelimChunks(const ref f: file, in delim: ?dt, n: int, bounds: range,
                       header = headerPolicy.noHeader
  ): [] int throws
    where dt == bytes || dt == string
  {
    const (findStart, startOffset) = header.apply(f, bounds),
          chunkIndices = if findStart then 0..<n else 1..<n;

    const nDataBytes = bounds.high - startOffset,
          approxBytesPerChunk = nDataBytes / n;

    var chunkOffsets: [0..n] int;
    chunkOffsets[0] = startOffset;
    chunkOffsets[n] = bounds.high;

    forall i in chunkIndices with (ref chunkOffsets) {
      const estOffset = startOffset + i * approxBytesPerChunk,
            r = f.reader(locking=false, region=estOffset..);

      try {
        r.advanceThrough(delim); // advance past the next delimiter
      } catch {
        // there wasn't an offset in this chunk
        throw new OffsetNotFoundError(
          "Failed to find byte offset in the range (" + bounds.low:string + ".." + bounds.high:string +
          ") for chunk " + i:string + " of " + n:string + ". Try using fewer tasks."
        );
      }
      chunkOffsets[i] = r.offset(); // record the offset for this chunk
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
    :arg header: a header policy to use when searching for the first byte offset
    :arg deserializerType: the type of deserializer to use

    :returns: a length ``n+1`` array of byte offsets (the last offset is
              ``bounds.high``)

    :throws: :class:`OffsetNotFoundError` if a valid byte offset cannot be found
              in any of the chunks
  */
  proc findDelimChunksChecked(const ref f: file, in delim: ?dt, n: int, type t, bounds: range,
                              header = headerPolicy.noHeader, type deserializerType = defaultDeserializer
  ): [] int throws
    where dt == bytes || dt == string
  {
    const (findStart, startOffset) = header.apply(f, bounds),
          chunkIndices = if findStart then 0..<n else 1..<n;

    const nDataBytes = bounds.high - startOffset,
          approxBytesPerChunk = nDataBytes / n,
          maxAdvanceTrials = 100,
          maxDeserTrials = delim.size + 1;

    var chunkOffsets: [0..n] int;
    chunkOffsets[0] = startOffset;
    chunkOffsets[n] = bounds.high;

    forall i in chunkIndices with (ref chunkOffsets) {
      const estOffset = bounds.low + i * approxBytesPerChunk,
            r = f.reader(locking=false, region=estOffset..);
      var tTest: t;

      label advancing for 1..maxAdvanceTrials {
        try {
            r.advanceTo(delim); // advance to the next delimiter
        } catch {
          throw new OffsetNotFoundError(
            "Failed to find byte offset in the range (" + bounds.low:string + ".." + bounds.high:string +
            ") for chunk " + i:string + " of " + n:string + ". Try using fewer tasks."
          );
        }

        // try to deserialize a 't' at the offset (and the next few offsets)
        label deserializing for 1..maxDeserTrials {
          r.mark();
          try {
            const didRead = r.withDeserializer(deserializerType).read(tTest);
            if !didRead then throw new Error();
          } catch {
            // couldn't deserialize a 't' at the offset
            r.revert();   // move back to the marked position
            r.readByte(); // advance to the next byte
            continue deserializing; // try again
          }

          // deserialized successfully
          r.revert();
          chunkOffsets[i] = r.offset(); // record the starting offset
          break advancing;
        }
        continue advancing;
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

    forall c in 0..<d.high with (ref nPerChunk) {
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
    An error thrown when a starting offset cannot be found in a chunk of a file.
  */
  class OffsetNotFoundError: Error {
    proc init() do super.init();
    proc init(msg: string) do super.init(msg);
  }

  @chpldoc.nodoc
  enum headerKind {
    find,
    nbytes,
    nlines,
    noHeader
  }

  /*
    A type describing how to handle the file header when reading a file
    in parallel.
  */
  record headerPolicy {
    @chpldoc.nodoc
    var n: int = 0;
    @chpldoc.nodoc
    var kind: headerKind;
  }

  /*
    Skip the first ``n`` lines of the file
  */
  proc type headerPolicy.skipLines(n: int) {
    var h = new headerPolicy();
    h.kind = headerKind.nlines;
    h.n = n;
    return h;
  }

  /*
    Skip the first ``n`` bytes of the file
  */
  proc type headerPolicy.skipBytes(n: int) {
    var h = new headerPolicy();
    h.kind = headerKind.nbytes;
    h.n = n;
    return h;
  }

  /*
    Find the first byte offset in the file that can be used to start reading
  */
  proc type headerPolicy.findStart {
    var h = new headerPolicy();
    h.kind = headerKind.find;
    return h;
  }

  /*
    Don't expect a header in the file
  */
  proc type headerPolicy.noHeader {
    var h = new headerPolicy();
    h.kind = headerKind.noHeader;
    return h;
  }

  @chpldoc.nodoc
  proc headerPolicy.apply(f: file, bounds: range): (bool, int) throws {
    const r = f.reader(locking=false, region=bounds);
    select this.kind {
      when headerKind.nlines {
        for 0..#this.n do r.readLine();
        return (false, r.offset());
      }
      when headerKind.nbytes {
        r.advance(this.n);
        return (false, r.offset());
      }
      when headerKind.find do return (true, r.offset());
      otherwise do return (false, r.offset());
    }
  }
}
