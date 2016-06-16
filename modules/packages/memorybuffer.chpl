/*
 * Copyright 2004-2016 Cray Inc.
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
  code module to freeze/thaw data types into byte arrays
 */
module memorybuffer {

  record MemoryBuffer {
    type eltype;
    const memstyle = new iostyle(binary=1);
    param byteorder = iokind.native;
    var buffd:file;
    var bufsize:int;

    proc MemoryBuffer(type eltype, data_item:eltype) {
      buffd = openmem(style=memstyle);
      var buffd_writer = buffd.writer(start=0, kind=byteorder);

      buffd_writer.write(data_item);
      buffd_writer.close();

      bufsize = buffd.length();
    }

    proc ~MemoryBuffer() {
      buffd.close();
    }

    proc size() {
      return bufsize;
    }

    proc bytes() {
      var Dbytebuf = {1..bufsize};
      var bytebuf:[Dbytebuf] uint(8);

      var buffd_reader = buffd.reader(start=0, kind=byteorder);
      buffd_reader.read(bytebuf);
      buffd_reader.close();

      return bytebuf;
    }

    proc get() {
      var retobj:eltype;
      var buffd_reader = buffd.reader(start=0, kind=byteorder);
      buffd_reader.read(retobj);
      buffd_reader.close();
      return retobj;
    }
  }

  proc type MemoryBuffer.freeze(data_item:?T) {
    return new MemoryBuffer(T, data_item);
  }

  proc type MemoryBuffer.thaw(type eltype, data:[?Ddata_buffer] uint(8)) {
   param byteorder = iokind.native;
   const memstyle = new iostyle(binary=1);
   var fd = openmem(style=memstyle);

   var fd_writer = fd.writer(start=0, kind=byteorder);
   fd_writer.write(data);
   fd_writer.close();

   var retobj:eltype;
   var fd_reader = fd.reader(start=0, kind=byteorder);
   fd_reader.read(retobj);
   fd_reader.close();
   return retobj;
  }


} // module
