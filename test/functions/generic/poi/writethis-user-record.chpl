/*
This code summarizes what happens upon:

  record Color {
    var red, green, blue: uint(8);
    proc serialize(writer, ref serializer) { }  // can be user-defined or compiler-generated
  }

  var taupe = new Color(179, 139, 109);
  writeln(taupe); // invokes taupe.writeThis(...) within writeln() in IO.chpl

where the module IO.chpl has:

  proc writeln(args...) {
    stdout.writeln((...args));
  }
  ..... more wrappers .....
  proc _write_one_internal(....) {
    arg.serialize(writer, ref serializer)
  }
*/


module Application {
  use ReproIO;

  record Color {
    var red, green, blue: uint(8);

    proc reproWriteThis(channel) {
      compilerError("success");
    }
  }

  proc main {
    var taupe = new Color(179, 139, 109);
    reproWriteln(taupe);
  }
}

module ReproIO {
  const reproChannel = new RootClass();

  // IO.chpl has ~5 levels of indirection, here we make just one level.
  proc reproWriteln(arg) {
    reproWriteln(reproChannel, arg);
  }

  proc reproWriteln(channel, arg) {
    // The target function is visible only in the scope
    // of the call to reproWriteln in the user code.
    arg.reproWriteThis(reproChannel);
  }
}
