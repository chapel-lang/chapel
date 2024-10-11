/*
 * Copyright 2023-2024 Hewlett Packard Enterprise Development LP
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

/* Support for reading and writing of Zarr stores.

  Support is limited to v2 Zarr arrays stored on local filesystems. NFS
  is not supported. The module uses c-blosc to compress and decompress chunks.
  Zarr specification: https://zarr-specs.readthedocs.io/en/latest/v2/v2.0.html
*/
module Zarr {
  use IO;
  use FileSystem;
  use JSON;
  use Map;
  use List;
  use Path;
  use CTypes;
  use BlockDist;
  use Time;

  require "blosc.h";
  require "-lblosc";

  private module Blosc {
    use CTypes;
    extern proc blosc_init();
    extern proc blosc_compress_ctx(clevel: c_int, doshuffle: c_int, typesize: c_size_t,
                              nbytes: c_size_t, src: c_ptrConst(void),
                              dest: c_ptr(void), destsize: c_size_t,
                              compressor: c_ptrConst(c_char), blocksize: c_size_t, numinternalthreads: c_int): int;
    extern proc blosc_decompress_ctx(src: c_ptrConst(void), dest: c_ptr(void), destsize: c_size_t, numinternalthreads: c_int): int;
    extern proc blosc_destroy();
    extern proc blosc_set_nthreads(nthreads_new: c_int) : c_int;
    extern proc blosc_get_nthreads() : c_int;
  }
  private use Blosc;

  /* Turns on/off profiling of Zarr IO */
  config param zarrProfiling = false;

  private var timerDomain: domain(string,parSafe=false) = {"Compression",
    "Decompression", "Opening File, Read", "Opening File, Write",
    "Creating Reader", "Reading File", "Creating Writer", "Writing File",
    "Reading to Update", "Copying In", "Creating Compressed Buffer", "Initializing copyIn"};
  private var times: [timerDomain] atomic real;

  /*
    Returns a map of profiling results for Zarr IO operations. The keys are
    the names of the operations and the values are the total time spent in
    each operation across all threads. Requires that zarrProfiling be set to
    true.
  */
  iter zarrProfilingResults() throws {
    for key in timerDomain do if times[key].read() != 0 then yield (key, times[key].read());
  }

  /*
    Resets the profiling timer for Zarr IO operations. Should only be
    used when compiled with zarrProfiling set to true.
  */
  proc resetZarrProfiling() {
    for key in timerDomain do times[key].write(0);
  }

  record zarrMetadataV2 {
    var zarr_format: int;
    var chunks: list(int);
    var dtype: string;
    var shape: list(int);
    var compressor: string;
  }

  record zarrMetadataV2Required {
    var zarr_format: int;
    var chunks: list(int);
    var dtype: string;
    var shape: list(int);
  }

  record zarrMetadataV2Optional {
    var compressor: string;
  }

  /* Unused until support is added for v3.0 stores */
  record zarrMetadataV3 {
    var zarr_format: int;
    var node_type: string;
    var shape: list(int);
    var data_type: string;
    var dimension_names: list(string);
  }

  private proc dtypeString(type dtype) throws {
    select dtype {
      when real(32) do return "f4";
      when real(64) do return "f8";
      when int(32) do return "i4";
      when int(64) do return "i8";
    }
    throw Error("Unexpected data type, only real and int types are supported.");
  }

  private proc getMetadata(directoryPath: string) throws {
    var metadataPath = joinPath(directoryPath, ".zarray");
    var r = openReader(metadataPath, deserializer = new jsonDeserializer(), locking=false);
    var mdRequired: zarrMetadataV2Required;
    r.readf("%?", mdRequired);

    r.seek(0..);
    var mdOptional: zarrMetadataV2Optional;
    try {
      r.readf("%?", mdOptional);
    } catch {
      mdOptional.compressor = "blosclz";
    }

    var md = new zarrMetadataV2(mdRequired.zarr_format, mdRequired.chunks, mdRequired.dtype, mdRequired.shape, mdOptional.compressor);
    return md;
  }

  private proc validateMetadata(metadata: zarrMetadataV2, type dtype, param dimCount) throws {
    //dimensionality matches
    if dimCount != metadata.shape.size then
      throw new Error("Expected metadata shape field to have %i dimensions: %?".format(dimCount, metadata.shape));
    if dimCount != metadata.chunks.size then
      throw new Error("Expected metadata chunks field to have %i dimensions: %?".format(dimCount, metadata.chunks));
    //positive, integer sizes
    for i in 0..<dimCount {
      if metadata.shape[i] <= 0 then
        throw new Error("Metadata shape field must have positive side lengths: %?".format(metadata.shape));
      if metadata.chunks[i] <= 0 then
        throw new Error("Metadata chunks field must have positive side lengths: %?".format(metadata.chunks));
    }

    var chplType: string;
    select metadata.dtype {
      when "i4", "<i4" do chplType = "int(32)";
      when "i8", "<i8" do chplType = "int(64)";
      when "f4", "<f4" do chplType = "real(32)";
      when "f8", "<f8" do chplType = "real(64)";
      otherwise {
        throw new Error("Only integer and floating point data types currently supported: %s".format(metadata.dtype));
      }
    }

    if chplType != dtype:string then
      throw new Error("Expected entries of type %s. Found %s".format(dtype:string, chplType));
  }

  private proc validateCompressor(compressor) throws {
    const supportedCompressors = ["blosclz", "lz4", "lz4hc", "zlib", "zstd"];
    if supportedCompressors.find(compressor) == -1 {
      throw new IllegalArgumentError("Unsupported compressor: %s.".format(compressor) +
                                     " Supported compressors are: blosclz, lz4, lz4hc, zlib, and zstd.");
    }
  }

  private proc buildChunkPath(directoryPath: string, delimiter: string, const chunkIndex: ?dimCount * int) {
    var indexStrings: dimCount*string;
    for i in 0..<dimCount do indexStrings[i] = chunkIndex[i] : string;
    return joinPath(directoryPath, delimiter.join(indexStrings));
  }
  private proc buildChunkPath(directoryPath: string, delimiter: string, chunkIndex: int) {
    return joinPath(directoryPath, chunkIndex:string);
  }

  /* Returns the indices of the chunks that contain elements present in a subdomain of the array. */
  proc getLocalChunks(D: domain(?), localD: domain(?), chunkShape: ?dimCount*int): domain(dimCount) {

    const totalShape = D.shape;
    var chunkCounts: dimCount*int;
    for i in 0..<dimCount {
      chunkCounts[i] = ceil(totalShape[i]:real / chunkShape[i]: real) : int;
    }

    var localChunks: dimCount*range(int);
    for i in 0..<dimCount {
      var l = if dimCount != 1 then localD.low[i] else localD.low;
      var h = if dimCount != 1 then localD.high[i] else localD.high;
      var low = floor(l:real / chunkShape[i]:real):int;
      var high = floor(h / chunkShape[i]:real):int;
      localChunks[i] = max(low,0)..min(high,chunkCounts[i]-1);
    }
    const localChunkDomain: domain(dimCount) = localChunks;
    return localChunkDomain;
  }


  /* Returns the domain of a chunk for a store with a given chunk shape.

     :arg chunkShape: A tuple of the extents of the dimensions of each chunk in the store.

     :arg chunkIndex: A tuple of the indices of the chunk to get the domain for.

     :returns: The domain of the chunk.
  */
  proc getChunkDomain(chunkShape: ?dimCount*int, chunkIndex: dimCount*int) {
    var thisChunkRange: dimCount*range(int);
    for i in 0..<dimCount {
      const start = chunkIndex[i] * chunkShape[i];
      thisChunkRange[i] = start..<start+chunkShape[i];
    }
    const thisChunkDomain: domain(dimCount) = thisChunkRange;
    return thisChunkDomain;
  }
  @chpldoc.nodoc
  proc getChunkDomain(chunkShape: ?dimCount*int, chunkIndex: int) {
    return getChunkDomain(chunkShape, (chunkIndex,));
  }


  /*
    Reads a chunk from storage and fills `arraySlice` with its corresponding
    values.

    :arg dimCount: Dimensionality of the array being read.

    :arg chunkPath: Relative or absolute path to the chunk being read.

    :arg chunkDomain: Domain of the chunk being read. Because boundary chunks
      are padded with zeros, the chunk's domain may be larger in some
      dimensions than the array's.

    :arg arraySlice: Reference to the portion of the calling locale's section
      of the array that this chunk will update. The domain of this slice
      should be a subset of the chunk's.

    :throws Error: If the decompression fails
  */
  proc readChunk(param dimCount: int, chunkPath: string, chunkDomain: domain(dimCount), ref arraySlice: [] ?t) throws {
    var s: stopwatch;
    const f: file;
    // if the file does not exist, the chunk is empty
    if zarrProfiling then s.restart();
    try {
      f = open(chunkPath, ioMode.r);
    } catch {
      arraySlice = 0;
      return;
    }
    if zarrProfiling then times["Opening File, Read"].add(s.elapsed());

    if zarrProfiling then s.restart();
    const r = f.reader(deserializer = new binaryDeserializer(), locking=false);
    if zarrProfiling then times["Creating Reader"].add(s.elapsed());

    if zarrProfiling then s.restart();
    const compressedChunk = r.readAll(bytes); // TODO: stream straight through to blosc
    var readBytes = compressedChunk.size;
    if zarrProfiling then times["Reading File"].add(s.elapsed());

    if zarrProfiling then s.restart();
    var copyIn: [chunkDomain] t = noinit;
    if zarrProfiling then times["Initializing copyIn"].add(s.elapsed());

    if zarrProfiling then s.restart();
    var numRead = blosc_decompress_ctx(compressedChunk.c_str(), c_ptrTo(copyIn), (copyIn.size*c_sizeof(t)) : c_size_t, 1: c_int);
    if numRead <= 0 {
      throw new Error("Failed to decompress data from %?. Blosc error code: %?".format(chunkPath, numRead));
    }
    if zarrProfiling then times["Decompression"].add(s.elapsed());

    if zarrProfiling then s.restart();
    arraySlice = copyIn[arraySlice.domain];
    if zarrProfiling then times["Copying In"].add(s.elapsed());
  }

  /*
    Updates a chunk in storage with a locale's contribution to that chunk.
    The calling function is expected to manage synchronization among locales.
    If the locale contributes the entire chunk, it will immediately compress
    and write the chunk's data. If the contribution is partial, it decompresses
    the chunk, updates the necessary values, then compresses and writes the
    chunk to storage.

    :arg dimCount: Dimensionality of the array being written.

    :arg chunkPath: Relative or absolute path to the chunk being written.

    :arg chunkDomain: Domain of the chunk being updated. Because boundary
      chunks are padded with zeros, the chunk's domain may be larger in
      some dimensions than the array's.

    :arg arraySlice: The portion of the array that the calling locale
      contributes to this chunk.

    :arg bloscLevel: Compression level to use. 0 indicates no compression,
      9 (default) indicates maximum compression. Values outside of this range
      will be clipped to a value between 0 and 9.

    :throws Error: If the compression fails
  */
  proc writeChunk(param dimCount, chunkPath: string, chunkDomain: domain(dimCount), ref arraySlice: [] ?t, bloscLevel: int(32) = 9, compressor: string="blosclz") throws {
    var s: stopwatch;

    // bloscLevel must be between 0 and 9
    var _bloscLevel = min(9,max(0,bloscLevel));

    // If this chunk is entirely contained in the array slice, we can write
    // it out immediately. Otherwise, we need to read in the chunk and update
    // it with the partial data before writing
    if zarrProfiling then s.restart();
    var copyOut: [chunkDomain] t;
    if chunkDomain != arraySlice.domain {
      readChunk(dimCount, chunkPath, chunkDomain, copyOut);
    }
    copyOut[arraySlice.domain] = arraySlice;
    if zarrProfiling then times["Reading to Update"].add(s.elapsed());


    // Create buffer for compressed bytes
    if zarrProfiling then s.restart();
    var compressedBuffer = allocate(t, (copyOut.size + 16): c_size_t);
    if zarrProfiling then times["Creating Compressed Buffer"].add(s.elapsed());

    // Compress the chunk's data
    if zarrProfiling then s.restart();
    var bytesCompressed = blosc_compress_ctx(_bloscLevel, 0, c_sizeof(t),
                                             (copyOut.size*c_sizeof(t)) : c_size_t, c_ptrTo(copyOut),
                                             compressedBuffer, ((copyOut.size + 16) * c_sizeof(t)) : c_size_t,
                                             compressor.c_str(), 0 : c_size_t, 1 : c_size_t);
    if bytesCompressed == 0 then
      throw new Error("Failed to compress bytes");
    if zarrProfiling then times["Compression"].add(s.elapsed());

    // Write it to storage
    if zarrProfiling then s.restart();
    const f = open(chunkPath, ioMode.cw);
    if zarrProfiling then times["Opening File, Write"].add(s.elapsed());
    if zarrProfiling then s.restart();
    const w = f.writer(serializer = new binarySerializer(),locking=false);
    if zarrProfiling then times["Creating Writer"].add(s.elapsed());

    if zarrProfiling then s.restart();
    w.writeBinary(compressedBuffer: c_ptr(void),bytesCompressed);
    if zarrProfiling then times["Writing File"].add(s.elapsed());

    deallocate(compressedBuffer);
  }

  /*
    Reads a v2.0 zarr store from storage using all locales, returning a
    block distributed array. Each locale reads and decompresses the chunks
    with elements in its subdomain. This method assumes a shared filesystem
    where all nodes can access the store directory.

    :arg directoryPath: Relative or absolute path to the root of the zarr
      store. The store is expected to contain a '.zarray' metadata file

    :arg dtype: Chapel type of the store's data

    :arg dimCount: Dimensionality of the zarr array

  */
  proc readZarrArray(directoryPath: string, type dtype, param dimCount: int, bloscThreads: int(32) = 1, targetLocales: [] locale = Locales) throws {
    var md = getMetadata(directoryPath);
    validateMetadata(md, dtype, dimCount);
    var totalRanges: dimCount*range(int);
    for i in 0..<dimCount {
      totalRanges[i] = 0..<md.shape[i];
    }
    const fullDomain: domain(dimCount) = totalRanges;

    return readZarrArrayPartial(directoryPath, dtype, dimCount, fullDomain, bloscThreads=bloscThreads, targetLocales=targetLocales);
  }



  /*
    Writes an array to storage as a v2.0 zarr store. The array metadata and
    chunks will be stored within the `directoryPath` directory, which is created
    if it does not yet exist. The chunks will have the dimensions given in the
    `chunkShape` argument. This function writes chunks in parallel, and supports
    distributed execution. It assumes a shared filesystem where all nodes can
    access the store directory.

    :arg directoryPath: Relative or absolute path to the root of the zarr store.
      The directory and all necessary parent directories will be created if it
      does not exist.

    :arg A: The array to write to storage.

    :arg chunkShape: The dimension extents to use when breaking A into chunks.

    :arg bloscLevel: Compression level to use. 0 indicates no compression,
      9 (default) indicates maximum compression.

    :arg compressor: Compression algorithm to use. Supported values are "blosclz" (default),
      "lz4", "lz4hc", "zlib", and "zstd".
  */
  proc writeZarrArray(directoryPath: string, const ref A: [?domainType] ?dtype, chunkShape: ?dimCount*int, bloscLevel: int(32) = 9, compressor="blosclz") throws {

    // Create the metadata record that is written before the chunks
    var shape, chunks: list(int);
    for size in A.shape do shape.pushBack(size);
    for size in chunkShape do chunks.pushBack(size);
    validateCompressor(compressor);
    const md: zarrMetadataV2 = new zarrMetadataV2(2, chunks, dtypeString(dtype), shape, compressor);

    // Clear the directory before writing
    if exists(directoryPath) then rmTree(directoryPath);
    mkdir(directoryPath, parents=true);

    // Write the metadata
    const metadataPath = joinPath(directoryPath, ".zarray");
    const w = openWriter(metadataPath, serializer = new jsonSerializer(),locking=true);
    w.writef("%?\n", md);

    // Normalize the array's domain to be zero-indexed
    var normalizedRanges: dimCount*range(int);
    for i in 0..<dimCount do
      normalizedRanges[i] = 0..<shape[i];
    const undistD: domain(dimCount) = normalizedRanges;
    const dist = new blockDist(boundingBox=undistD);
    const D = dist.createDomain(undistD);
    ref normA = A.reindex(D);

    // Locks to synchronize locales writing to the same chunks
    const allChunks = getLocalChunks(normA.domain, normA.domain, chunkShape);
    var locks: [allChunks] sync bool;

    // Write the chunks
    coforall loc in Locales do on loc {
      // Initialize blosc on each locale
      blosc_init();

      // Get the part of the array that belongs to this locale
      const hereD = normA.localSubdomain();
      ref hereA = normA[hereD];

      // Identify the range of chunks this locale will contribute to
      const localChunks = getLocalChunks(normA.domain, hereD, chunkShape);

      forall chunkIndex in localChunks {
        // Get the part of the array that contributes to this chunk
        const thisChunkDomain = getChunkDomain(chunkShape, chunkIndex);
        const thisChunkHere = hereD[thisChunkDomain];
        ref thisChunkSlice = hereA.localSlice(thisChunkHere);
        const chunkPath = buildChunkPath(directoryPath, ".", chunkIndex);
        locks[chunkIndex].writeEF(true);
        writeChunk(dimCount, chunkPath, thisChunkDomain, thisChunkSlice, bloscLevel=bloscLevel, compressor=compressor);
        locks[chunkIndex].readFE();
      }
    }
  }


  /*


  */ 
  proc readZarrArrayPartial(directoryPath: string, type dtype, param dimCount: int, partialDomain, 
                            bloscThreads: int(32) = 1, targetLocales: [] locale = Locales) throws {
    var md = getMetadata(directoryPath);
    validateMetadata(md, dtype, dimCount);
    // Size and shape tuples
    var totalShape, chunkShape : dimCount*int;
    var chunkCounts: dimCount*int;
    var totalRanges,chunkRanges: dimCount*range(int);
    for i in 0..<dimCount {
      totalShape[i] = md.shape[i];
      chunkShape[i] = md.chunks[i];
      chunkCounts[i] = ceil(totalShape[i]:real / chunkShape[i]:real) : int;
      totalRanges[i] = 0..<totalShape[i];
      chunkRanges[i] = 0..<chunkCounts[i];
    }
    const fullChunkDomain: domain(dimCount) = chunkRanges;

    // Initialize the distributed domain and array
    const undistD : domain(dimCount) = totalRanges;
    if ! undistD.contains(partialDomain) then
      throw new IllegalArgumentError("Partial domain is out of bounds of the array domain.");
    const Dist = new blockDist(boundingBox=undistD, targetLocales=targetLocales);
    const D = Dist.createDomain(partialDomain);
    var A: [D] dtype;

    coforall loc in Locales do on loc {
      blosc_init();
      blosc_set_nthreads(bloscThreads);
      const hereD = A.localSubdomain();
      ref hereA = A[hereD];

      const localChunks = getLocalChunks(D, hereD, chunkShape);
      forall chunkIndex in localChunks {

        const chunkPath = buildChunkPath(directoryPath, ".", chunkIndex);

        const thisChunkDomain = getChunkDomain(chunkShape, chunkIndex);
        const thisChunkHere = hereD[thisChunkDomain];

        ref thisChunkSlice = hereA.localSlice(thisChunkHere);
        readChunk(dimCount, chunkPath, thisChunkDomain, thisChunkSlice);
      }
      blosc_destroy();
    }
    return A;
  }

  /*
    Reads a v2.0 zarr store from storage using a single locale, returning a
    locally allocated array. This method assumes a shared filesystem
    where the current locale can access the store directory.

    :arg directoryPath: Relative or absolute path to the root of the zarr
      store. The store is expected to contain a '.zarray' metadata file

    :arg dtype: Chapel type of the store's data

    :arg dimCount: Dimensionality of the zarr array

  */
  proc readZarrArrayLocal(directoryPath: string, type dtype, param dimCount: int) throws {
    var md = getMetadata(directoryPath);
    validateMetadata(md, dtype, dimCount);
    var totalShape, chunkShape : dimCount*int;
    var chunkCounts: dimCount*int;
    var totalRanges,chunkRanges: dimCount*range(int);
    for i in 0..<dimCount {
      totalShape[i] = md.shape[i];
      chunkShape[i] = md.chunks[i];
      chunkCounts[i] = ceil(totalShape[i]:real / chunkShape[i]:real) : int;
      totalRanges[i] = 0..<totalShape[i];
      chunkRanges[i] = 0..<chunkCounts[i];
    }
    const fullChunkDomain: domain(dimCount) = chunkRanges;

    // Initialize the domain and array
    const D : domain(dimCount) = totalRanges;
    var A: [D] dtype;

    blosc_init();
    forall chunkIndex in fullChunkDomain {
      const chunkPath = buildChunkPath(directoryPath, ".", chunkIndex);
      const fullChunkDomain = getChunkDomain(chunkShape, chunkIndex);
      const chunkDomainWithinArray = D[fullChunkDomain];
      ref thisChunkSlice = A[chunkDomainWithinArray];
      readChunk(dimCount, chunkPath, fullChunkDomain, thisChunkSlice);
    }
    blosc_destroy();
    return A;
  }

  /*
    Writes an array to storage as a v2.0 zarr store using a single locale. The
    array metadata and chunks will be stored within the `directoryPath`
    directory, which is created if it does not yet exist. The chunks will
    have the dimensions given in the`chunkShape` argument.

    :arg directoryPath: Relative or absolute path to the root of the zarr store.
      The directory and all necessary parent directories will be created if it
      does not exist.

    :arg A: The array to write to storage.

    :arg chunkShape: The dimension extents to use when breaking A into chunks.

    :arg bloscLevel: Compression level to use. 0 indicates no compression,
      9 (default) indicates maximum compression.

    :arg compressor: Compression algorithm to use. Supported values are "blosclz" (default),
      "lz4", "lz4hc", "zlib", and "zstd".
  */
  proc writeZarrArrayLocal(directoryPath: string, ref A: [?domainType] ?dtype, chunkShape: ?dimCount*int, bloscLevel: int(32) = 9, compressor="blosclz") throws {

    // Create the metadata record that is written before the chunks
    var shape, chunks: list(int);
    for size in A.shape do shape.pushBack(size);
    for size in chunkShape do chunks.pushBack(size);
    validateCompressor(compressor);
    const md: zarrMetadataV2 = new zarrMetadataV2(2, chunks, dtypeString(dtype), shape, compressor);

    // Clear the directory before writing
    if exists(directoryPath) then rmTree(directoryPath);
    mkdir(directoryPath, parents=true);

    // Write the metadata
    const metadataPath = joinPath(directoryPath, ".zarray");
    const w = openWriter(metadataPath, serializer = new jsonSerializer(),locking=true);
    w.writef("%?\n", md);

    // Normalize the array's domain to be zero-indexed
    var normalizedRanges: dimCount*range(int);
    for i in 0..<dimCount do
      normalizedRanges[i] = 0..<shape[i];
    const D: domain(dimCount) = normalizedRanges;
    ref normA = A.reindex(D);

    blosc_init();

    const localChunks = getLocalChunks(D, D, chunkShape);
    forall chunkIndex in localChunks {
      // Get the part of the array that contributes to this chunk
      const chunkBounds = getChunkDomain(chunkShape, chunkIndex);
      const chunkForDomain = D[chunkBounds];
      ref chunkData = normA[chunkForDomain];
      const chunkPath = buildChunkPath(directoryPath, ".", chunkIndex);
      writeChunk(dimCount, chunkPath, chunkBounds, chunkData, bloscLevel=bloscLevel, compressor=compressor);
    }

    blosc_destroy();
  }

  /*
    Updates a single chunk within a Zarr store with the data in `A`. The
    Zarr store and the associated metadata file must already exist.

    :arg directoryPath: Relative or absolute path to the root of the zarr
      store. This directory should exist and contain a '.zarray' metadata file.

    :arg A: The array to update the chunk with.

    :arg chunkIndex: The index of the chunk to update.

    :arg bloscThreads: The number of threads to use during compression (default=1)
  */
  proc updateZarrChunk(directoryPath: string, ref A: [?domainType] ?dtype, chunkIndex: ?dimCount*int) throws {
    var md = getMetadata(directoryPath);
    validateMetadata(md, dtype, dimCount);
    var chunkShape: dimCount*int;
    for i in 0..<dimCount {
      chunkShape[i] = md.chunks[i];
    }

    // Normalize the array's domain to be zero-indexed
    var normalizedRanges: dimCount*range(int);
    for i in 0..<dimCount do
      normalizedRanges[i] = 0..<md.shape[i];
    const D: domain(dimCount) = normalizedRanges;
    ref normA = A.reindex(D);

    ref chunkData = normA[D[getChunkDomain(chunkShape, chunkIndex)]];
    const chunkPath = buildChunkPath(directoryPath, ".", chunkIndex);

    blosc_init();
    writeChunk(dimCount, chunkPath, chunkData.domain, chunkData, compressor=md.compressor);
    blosc_destroy();
  }

  proc updateZarrChunk(directoryPath: string, ref A: [?domainType] ?dtype, chunkIndex: int) throws {
    updateZarrChunk(directoryPath, A, (chunkIndex,));
  }
}
