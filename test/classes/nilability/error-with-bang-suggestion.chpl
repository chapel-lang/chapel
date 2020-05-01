module OuterModule {
  use M;
  var test = new Test();

  class Test {
    var ffout : shared H5File?;

    proc init() {
      this.complete();
      ffout = createH5File();
      writeScalarAttribute(ffout);
    }
  }

  module M {

    // Basic file operations
    class H5File {
      var fid : int;
      proc mymethod(arg: int) {
        writeln("in mymethod");
      }
    }

    proc createH5File() {
      var ret = new shared H5File();
      return ret;
    }

    proc myfun(arg:H5File) {
      writeln("in myfun");
    }    

    proc writeScalarAttribute(loc) {
      var gg = loc.fid;
      loc.mymethod(5);
      const locc = loc;
      myfun(locc);
    }

  }
}
