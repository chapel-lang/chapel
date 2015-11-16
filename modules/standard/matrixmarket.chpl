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

module matrixmarket {
   use Math;
   use IO;

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
         tfout.flush(); tfout.close();
      }

      proc write_vector(i:int, jvec:[?Djvec] ?T) where Djvec.rank == 1 {
         assert(last_rowno < i, "rows %i and %i not in sequential order!", last_rowno, i);
         for (j,w) in zip(1..Djvec.size, jvec) {
            if abs(w) > 1e-12 { fout.writef("%i %i %s\n", i, j, w); }
         }

         last_rowno = i;
         var ret:(int,int);
         if jvec.size < 1 { ret = (-1, 0); } else { ret = (Djvec.size, jvec.size); }
         return ret;
      }

      proc close() { fout.flush(); fout.close(); }
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
      n_cols = max(n_cols, 1+max_id);
      nnz += veclen;
      ncols = r;
   }

   nrows = mw.last_rowno;
   ncols = if ncols != 0 then ncols else n_cols;

   mw.fake_headers(nrows, ncols, nnz);
   mw.close();
   delete mw;
}

class MMReader {
   var fd:file;
   var fin:channel(false, iokind.dynamic, true);

   proc MMReader(const fname:string) {
      fd = open(fname, iomode.r, iokind.native);
      fin = fd.reader(start=0);
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
         fin = fd.reader(start=offset);
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
}

proc mmread(const fname:string) {
   var mr = new MMReader(fname);
   var toret = mr.read_file();
   return toret;
}

}
