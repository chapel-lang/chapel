use IO;
use JSON;

// Part 1: make sure we can specify locking and (de)serializerType
// positionally.

proc foo(writer: fileWriter(?)) {
  writeln("foo: fully generic: ", writer.type:string);
}

proc foo(writer: fileWriter(false, defaultSerializer)) {
  writeln("foo: fileWriter(false): ", writer.type:string);
}

proc foo(reader: fileReader(?)) {
  writeln("foo: fully generic: ", reader.type:string);
}

proc foo(reader: fileReader(false, defaultDeserializer)) {
  writeln("foo: fileReader(false): ", reader.type:string);
}

// Part 2: make sure we can specify kind with a named arg

proc bar(writer: fileWriter(?)) {
  writeln("bar: fileWriter(?): ", writer.type:string);
}

proc bar(writer: fileWriter(kind=_iokind.native, ?)) {
  writeln("bar: fileWriter(kind=iokind.native, ?): ", writer.type:string);
}

proc bar(reader: fileReader(?)) {
  writeln("bar: fileReader(?): ", reader.type:string);
}

proc bar(reader: fileReader(kind=_iokind.native, ?)) {
  writeln("bar: fileReader(kind=iokind.native, ?): ", reader.type:string);
}

// Part 3: various actual queries of generic fields

proc query(writer: fileWriter(?L, ?ST)) {
  writeln("L = ", L:string, " :: ST = ", ST:string);
}

proc query(reader: fileReader(?L, ?DT)) {
  writeln("L = ", L:string, " :: DT = ", DT:string);
}

proc queryNamed(writer: fileWriter(locking=?L, serializerType=?ST)) {
  writeln("L = ", L:string, " :: ST = ", ST:string);
}

proc queryNamed(reader: fileReader(locking=?L, deserializerType=?DT)) {
  writeln("L = ", L:string, " :: DT = ", DT:string);
}

proc queryKind(writer: fileWriter(kind=?K, ?L, ?ST)) {
  writeln("L = ", L:string, " :: ST = ", ST:string, " :: K = ", K:string);
}

proc queryKind(reader: fileReader(kind=?K, ?L, ?DT)) {
  writeln("L = ", L:string, " :: DT = ", DT:string, " :: K = ", K:string);
}

proc queryKindLast(writer: fileWriter(?L, ?ST, kind=?K)) {
  writeln("L = ", L:string, " :: ST = ", ST:string, " :: K = ", K:string);
}

proc queryKindLast(reader: fileReader(?L, ?DT, kind=?K)) {
  writeln("L = ", L:string, " :: DT = ", DT:string, " :: K = ", K:string);
}

proc helper(type channel, param locking: bool, type deserType) {
  type T = if channel == fileWriter(?) then deserType
           else if deserType == defaultSerializer then defaultDeserializer
           else if deserType == jsonSerializer then jsonDeserializer
           else nothing;
  var w : channel(locking, T);
  writeln("----- ", w.type:string, " -----");
  foo(w);
  bar(w);
  query(w);
  queryNamed(w);
  queryKind(w);
  queryKindLast(w);
  {
    var nw : channel(_iokind.native, locking, T);
    bar(nw);
    queryKind(nw);
  }
  writeln();
}

proc helper(param locking: bool, type deserType) {
  helper(fileWriter(?), locking, deserType);
  helper(fileReader(?), locking, deserType);
}

proc main() {
  helper(true, defaultSerializer);
  helper(false, defaultSerializer);
  helper(true, jsonSerializer);
  helper(false, jsonSerializer);
}
