/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

// This module is a stub module to provide a deprecation warning for IO symbols
// that were included by default and will no longer be included.
private use IO;
private use ChapelBase;
private use SysCTypes;

pragma "last resort"
proc iodynamic param {
  compilerWarning("iodynamic will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.iodynamic;
}

pragma "last resort"
proc ionative param {
  compilerWarning("ionative will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.ionative;
}

pragma "last resort"
proc iobig param {
  compilerWarning("iobig will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.iobig;
}

pragma "last resort"
proc iolittle param {
  compilerWarning("iolittle will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.iolittle;
}
/* Not yet tested
pragma "last resort"
proc stringStyleTerminated(terminator: uint(8)) {
  compilerWarning("stringStyleTerminated will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return stringStyleTerminated(terminator);
}
*/
pragma "last resort"
proc stringStyleNullTerminated() {
  compilerWarning("stringStyleNullTerminated will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return stringStyleNullTerminated();
}

pragma "last resort"
proc stringStyleExactLen(len:int(64)) {
  compilerWarning("stringStyleExactLen will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return stringStyleExactLen(len);
}

pragma "last resort"
proc stringStyleWithVariableLength() {
  compilerWarning("stringStyleWithVariableLength will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return stringStyleWithVariableLength();
}

pragma "last resort"
proc stringStyleWithLength(lengthBytes:int) throws {
  compilerWarning("stringStyleWithLength will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return stringStyleWithLength(lengthBytes);
}

pragma "last resort"
proc IOHINT_NONE {
  compilerWarning("IOHINT_NONE will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.IOHINT_NONE;
}

pragma "last resort"
proc IOHINT_RANDOM {
  compilerWarning("IOHINT_RANDOM will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.IOHINT_RANDOM;
}

pragma "last resort"
proc IOHINT_SEQUENTIAL {
  compilerWarning("IOHINT_SEQUENTIAL will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.IOHINT_SEQUENTIAL;
}

pragma "last resort"
proc IOHINT_CACHED {
  compilerWarning("IOHINT_CACHED will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.IOHINT_CACHED;
}

pragma "last resort"
proc IOHINT_PARALLEL {
  compilerWarning("IOHINT_PARALLEL will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.IOHINT_PARALLEL;
}

// TODO: iostyle record
/*pragma "last resort"
proc iostyle type {
  compilerWarning("iostyle will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.iostyle;
  }*/

pragma "last resort"
proc defaultIOStyle() {
  compilerWarning("defaultIOStyle will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return defaultIOStyle();
}

pragma "last resort"
proc iohints type {
  compilerWarning("iohints will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.iohints;
}

// TODO: file record methods
pragma "last resort"
proc file type {
  compilerWarning("file will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.file;
}

pragma "last resort"
proc (IO.file).home ref: locale {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.home;
}

pragma "last resort"
proc (IO.file).length():int(64) throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.length();
}

pragma "last resort"
proc (IO.file).writer(param kind=IO.iokind.dynamic, param locking=true, start:int(64) = 0,
                 end:int(64) = max(int(64)), hints:c_int = 0, style:IO.iostyle = this._style):
                 IO.channel(true,kind,locking) throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.writer(kind, locking, start, end, hints, style);
}

pragma "last resort"
proc (IO.file).reader(param kind=IO.iokind.dynamic, param locking=true, start:int(64) = 0,
                 end:int(64) = max(int(64)), hints:IO.iohints = IO.IOHINT_NONE,
                 style:IO.iostyle = this._style): IO.channel(false, kind, locking) throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.reader(kind, locking, start, end, hints, style);
}

pragma "last resort"
proc (IO.file).close() throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  this.close();
}

/* Relies on symbols that already generate an error to function.  Will not be
   possible to access without, thus not worth adding an error for
pragma "last resort"
proc open(path:string, mode:IO.iomode, hints:IO.iohints=IO.IOHINT_NONE,
          style:IO.iostyle = defaultIOStyle()): IO.file throws {
  compilerWarning("open will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return open(path, mode, hints, style);
}
*/
/* not yet tested
pragma "last resort"
proc openplugin(pluginFile: QioPluginFile, mode:IO.iomode,
                seekable:bool, style:IO.iostyle) throws {
  compilerWarning("openplugin will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return openplugin(pluginFile, mode, seekable, style);
}
*/
pragma "last resort"
proc openfd(fd: fd_t, hints:IO.iohints=IO.IOHINT_NONE, style:IO.iostyle = defaultIOStyle()):IO.file throws {
  compilerWarning("openfd will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return openfd(fd, hints, style);
}

pragma "last resort"
proc openfp(fp: _file, hints:IO.iohints=IO.IOHINT_NONE, style:IO.iostyle = defaultIOStyle()):IO.file throws {
  compilerWarning("openfp will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return openfp(fp, hints, style);
}

pragma "last resort"
proc opentmp(hints:IO.iohints=IO.IOHINT_NONE, style:IO.iostyle = defaultIOStyle()):IO.file throws {
  compilerWarning("opentmp will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return opentmp(hints, style);
}

pragma "last resort"
proc openmem(style:IO.iostyle = defaultIOStyle()):IO.file throws {
  compilerWarning("openmem will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return openmem(style);
}

// TODO: channel record
/* Currently causes channel methods to fail to compile, likely due to ChapelIO
needing the channel class and having the use of this module at the same scope
as the use of IO.
pragma "last resort"
proc channel type {
  compilerWarning("file will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.channel;
}
*/
pragma "last resort"
proc (IO.channel).write(const args ...?k, style:IO.iostyle):bool throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.write((... args), style);
}

pragma "last resort"
proc (IO.channel).write(const args ...?k):bool throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.write((... args));
}

pragma "last resort"
proc (IO.channel).writef(fmtStr: string, const args ...?k): bool throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.writef(fmtStr, (... args));
}

pragma "last resort"
proc (IO.channel).writef(fmtStr: string): bool throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.writef(fmtStr);
}

pragma "last resort"
proc (IO.channel).readline(arg: [] uint(8), out numRead : int, start = arg.domain.low,
                      amount = arg.domain.high - start + 1) : bool throws
                      where arg.rank == 1 && isRectangularArr(arg) {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.readline(arg, numRead, start, amount);
}

pragma "last resort"
proc (IO.channel).close() throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  this.close();
}

pragma "last resort"
proc (IO.channel).readf(fmtStr:string, ref args ...?k): bool throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.readf(fmtStr, (...args));
}

pragma "last resort"
proc (IO.channel).readf(fmtStr:string): bool throws {
  // No deprecation warnings for methods and fields, we'll get them when we get
  // the type itself.
  return this.readf(fmtStr);
}

pragma "last resort"
iter (IO.channel).lines() {
  // No deprecation warnings for iterator methods, we'll get them when we get
  // the type itself.
  for i in this.lines() do
    yield i;
}

// TODO: ioChar record
// TODO: ioNewline record
// TODO: ioLiteral record
// TODO: ioBits record

pragma "last resort"
proc openreader(path:string,
                param kind=IO.iokind.dynamic, param locking=true,
                start:int(64) = 0, end:int(64) = max(int(64)),
                hints:IO.iohints = IO.IOHINT_NONE,
                style:IO.iostyle = defaultIOStyle())
    : IO.channel(false, kind, locking) throws {
  compilerWarning("openreader will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return IO.openreader(path, kind, locking, start, end, hints, style);
}
/*
pragma "last resort"
proc openwriter(path:string,
                param kind=IO.iokind.dynamic, param locking=true,
                start:int(64) = 0, end:int(64) = max(int(64)),
                hints:IO.iohints = IO.IOHINT_NONE,
                style:IO.iostyle = defaultIOStyle())
    : IO.channel(true, kind, locking) throws {
  compilerWarning("openwriter will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return openwriter(path, kind, locking, start, end, hints, style);
}

pragma "last resort"
inline proc <~>(const ref ch: IO.channel, x) const ref
  where ch.writing {
  compilerWarning("<~> will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  ch <~> x;
  return ch;
}

pragma "last resort"
inline proc <~>(const ref ch: channel, ref x) const ref
  where !ch.writing {
  compilerWarning("<~> will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  ch <~> x;
  return ch;
}

pragma "last resort"
inline proc <~>(const ref r: channel, lit:ioLiteral) const ref
  where !r.writing {
  compilerWarning("This function will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  r <~> lit;
  return r;
}

pragma "last resort"
inline proc <~>(const ref r: channel, nl:ioNewline) const ref
  where !r.writing {
  compilerWarning("<~> will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  r <~> nl;
  return r;
}

pragma "last resort"
proc stringify(const args ...?k):string {
  compilerWarning("stringify will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return stringify((...args));
}

// TODO: ItemReader record
// TODO: ItemWriter record
/*
pragma "last resort"
proc stdin {
  compilerWarning("stdin will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.stdin;
}

pragma "last resort"
proc stdout {
  compilerWarning("stdout will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.stdout;
}

pragma "last resort"
proc stderr {
  compilerWarning("stderr will no longer be available by default, please\ninsert a use of the IO module to access it.");
  return IO.stderr;
}
*/
*/
pragma "last resort"
proc stdinInit() {
  compilerWarning("stdinInit will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return stdinInit();
}

pragma "last resort"
proc stdoutInit() {
  compilerWarning("stdoutInit will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return stdoutInit();
}

pragma "last resort"
proc stderrInit() {
  compilerWarning("stderrInit will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return stderrInit();
}


pragma "last resort"
proc read(ref args ...?n):bool throws {
  compilerWarning("read will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return read((...args));
}

pragma "last resort"
proc readln(ref args ...?n):bool throws {
  compilerWarning("readln will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return readln((...args));
}

pragma "last resort"
proc readln():bool throws {
  compilerWarning("readln will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return readln();
}

pragma "last resort"
proc readln(type t ...?numTypes) throws {
  compilerWarning("readln will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return readln((...t));
}

pragma "last resort"
proc read(type t ...?numTypes) throws {
  compilerWarning("read will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return read((...t));
}

pragma "last resort"
proc unlink(path:string) throws {
  compilerWarning("unlink will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  unlink(path);
}

pragma "last resort"
proc unicodeSupported():bool {
  compilerWarning("unicodeSupported will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return unicodeSupported();
}

pragma "last resort"
proc readf(fmt:string, ref args ...?k):bool throws {
  compilerWarning("readf will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return readf(fmt, (...args));
}

pragma "last resort"
proc readf(fmt:string):bool throws {
  compilerWarning("readf will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return readf(fmt);
}

pragma "last resort"
proc string.format(args ...?k): string throws {
  compilerWarning("string.format will no longer be available by default,\nplease insert a use of the IO module to continue calling it.");
  return format((...args));
}
