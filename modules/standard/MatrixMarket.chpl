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
This code is based on the gensim implementation of the matrix market file format for matrices - it can currently store real/general matrixmarket data sets to disk. code was unit tested against the gensim implementation
*/

module MatrixMarket {

   use FileSystem;
   use Math;
   use IO;
   use Sys;
   use List;

/* extern var PROT_READ:c_int;
extern var MAP_SHARED:c_int;
extern var MAP_FILE:c_int; */

   class MMWriter {
      var HEADER_LINE = "%%MatrixMarket matrix coordinate real general\n"; // currently the only supported MM format in this module

      var fd:file;
      var fout:channel(true, iokind.dynamic, true);

      var headers_written:bool;
      var last_rowno:int;

      proc MMWriter(const fname:string) {
         fd = open(fname, iomode.cw, iokind.native);
         fout = fd.writer(start=0);
         headers_written=false;
      }

      proc write_headers(nrows, ncols, nnz=-1) {
         fout.write(HEADER_LINE);
         fout.write("%\n");

         if nnz < 0 {
            var blank = "                                                  \n";
            fout.write(blank);
         }
         else {
            fout.writef("%i %i %i\n", nrows, ncols, nnz);
         }

         last_rowno=-1;
         headers_written=true;
      }

      proc fake_headers(nrows, ncols, nnz) {
         var tfout = fd.writer(start=HEADER_LINE.size);
         tfout.writef("%i %i %i", nrows, ncols, nnz);
         tfout.close();
      }

      proc write_vector(i:int, jvec:[?Djvec] ?T) where Djvec.rank == 1 {
         assert(last_rowno < i, "rows %i and %i not in sequential order!", last_rowno, i);
         for (j,w) in zip(Djvec, jvec) {
            if abs(w) > 1e-12 { fout.writef("%i %i %s\n", i, j, w); }
         }

         last_rowno = i;
         var ret:(int,int);
         if jvec.size < 1 { ret = (-1, 0); } else { ret = (Djvec.size, jvec.size); }
         return ret;
      }

      proc close() { fout.close(); fd.close(); }
      proc ~MMWriter() { this.close(); }
   }

proc mmwrite(const fname:string, mat:[?Dmat] real, _num_cols=-1) where mat.domain.rank == 2 {
   var mw = new MMWriter(fname);
   mw.write_headers(-1,-1,-1);
   var (ncols, nnz) = (0,0);
   var (nrows, poslast) = (-1,-1);
   var n_cols = _num_cols;
   for r in 1..Dmat.high(1) {
      var row = mat(r,..);
      var (max_id, veclen) = mw.write_vector(r, row);
      n_cols = max(n_cols, max_id);
      nnz += veclen;
      ncols = r;
   }

   nrows = mw.last_rowno;
   ncols = n_cols; //if ncols != 0 then ncols else n_cols;

   mw.fake_headers(nrows, ncols, nnz);
   mw.close();
   delete mw;
}

class MMReader {
   var fd:file;
   var fin:channel(false, iokind.dynamic, true);

   proc MMReader(const fname:string) {
      fd = open(fname, iomode.r, hints=IOHINT_SEQUENTIAL|IOHINT_CACHED);
      fin = fd.reader(start=0, hints=IOHINT_SEQUENTIAL|IOHINT_CACHED);
   }

   proc read_header() {
      var header:string;
      assert(fin.readline(header) == true, "MMReader I/O error!");
      assert(header == "%%MatrixMarket matrix coordinate real general\n", "attempted to load an unsupported file");

      // test for files that have a % beneath the matrix market format header
      var percentfound:string;
      var offset = fin._offset();
      fin.readline(percentfound);

      // didn't find a precentage, rewind channel by length of read string...
      if percentfound != "%\n" {
         fin.close();
         fin = fd.reader(start=offset, hints=IOHINT_SEQUENTIAL|IOHINT_CACHED);
      }
   }

   proc read_matrix_info() {
      var nrows, ncols, nnz:int;
      var done = fin.readf("%i %i %i", nrows, ncols, nnz);
      assert(done == true, "error reading matrix market file's information");
      return (nrows, ncols, nnz);
   }

   iter read_data() {
      var done:bool = true;
      while done {
         var i, j:int;
         var w:real;
         done = fin.readf("%i %i %r\n", i, j, w);
         if done { yield (i,j,w); }
      }
   }

   proc read_file() {
      read_header();
      var (nrows, ncols, nnz) = read_matrix_info();

      var Dtoret = {1..nrows, 1..ncols}; // maybe look for sparse matrix support here?
      var toret:[Dtoret] real;

      // TODO: parallelizing this read would be nice...
      for (i,j,w) in read_data() {
         toret(i,j) = w;
      }

      return toret;
   }

   proc close() { 
      fin.close(); 
      fd.close(); 
   }

   proc ~MMReader() { this.close(); }
}

class LargeMMReader : MMReader {

   //var fdescptr, mmfdesc:c_void_ptr;
   //var fdesc:fd_t;
   //var mmap_sz:size_t;
   var file_sz:int;

   proc LargeMMReader(const fname:string, const mmapsz=(1024*25)) {
      file_sz = getFileSize(fname);
      //mmap_sz=mmapsz:size_t;
      //sys_open(fname.c_str(), 0:c_int, O_RDONLY:mode_t, fdesc);
      //sys_mmap(fdescptr, mmap_sz, PROT_READ:c_int, MAP_SHARED|MAP_FILE:c_int, fdesc, 0:off_t, mmfdesc);
      //fd = openfd(fdesc, mode=iomode.r, hints=IOHINT_CACHED|IOHINT_PARALLEL|IOHINT_RANDOM);
      fd = open(fname, mode=iomode.r, hints=IOHINT_CACHED|IOHINT_PARALLEL|IOHINT_RANDOM); 
      fin = fd.reader(start=0, hints=IOHINT_SEQUENTIAL|IOHINT_CACHED);
   }

   proc read_file() {
      read_header();
      var (nrows, ncols, nnz) = read_matrix_info();

      // start orchestration of parallel reads; preprocess
      // file to figure out where end-of-line markers are 
      // in order to expedite reads 
      //
      const numBytesPerThread = (file_sz - fin._offset())/here.numCores;
      const threadDom = {1..here.numCores};
      var eolMarkers:[threadDom] list(int);

      // lock per thread, control accessto eolMarker list for a thread
      var lock$ : [threadDom] sync bool; 

      forall i in fin._offset()..file_sz by numBytesPerThread {
         const threadid = i % here.numCores;
         lock$(threadid) = true; // lock grab
         if eolMarkers[threadid].length < 1 { eolMarkers[threadid] = new list(int); }

         var done:bool = true;
         var line:string;
         var eolReader = fd.reader(start=i, end=i+numBytesPerThread, hints=IOHINT_SEQUENTIAL|IOHINT_CACHED);

         // that ugly while loop in a parallel thread of execution... 
         while done { 
            eolMarkers(threadid).append(eolReader._offset());
            done = eolReader.readline(line); 
         }

         eolReader.close();
         const lock_release = lock$(threadid); // lock release
      }

      var eolMarkersList = new list(int);
      for i in threadDom { eolMarkersList.concat(eolMarkers(i)); eolMarkers.destroy(); }

      var eolMarkersArr : [1..eolMarkersList.length] int;
      for (i,j) in zip(1..eolMarkersList.length, eolMarkersList) { eolMarkersArr(i) = j; }

      const Dtoret = {1..nrows, 1..ncols}; // maybe look for sparse matrix support here?
      var toret:[Dtoret] real;

      forall mk in eolMarkersArr {
         var i, j:int; var w:real;
         var blkreader = fd.reader(start=mk, hints=IOHINT_SEQUENTIAL|IOHINT_CACHED);
         blkreader.readf("%i %i %r\n", i, j, w);
         toret(i, j) = w;
         blkreader.close();
      }

      eolMarkersList.destroy();
      return toret;
   }

   proc close() {
      //sys_munmap(mmfdesc, mmap_sz);
      //sys_close(fdesc);
      fin.close();
      fd.close();
   }

   proc ~LargeMMReader() { this.close(); }

}

proc mmread(const fname:string, const mmap_sz:int=-1) {
   //var mr = if mmap_sz < 0 then new MMReader(fname) else new LargeMMReader(fname, mmapsz=mmap_sz);
   var mr = new MMReader(fname);
   var toret = mr.read_file();
   delete mr;
   return toret;
}

}
