// Testing the order of deinitialization of globals
// and invoking module deinit functions.
// Good to test with and without --printModuleDeinitOrder.

module UM1 {
  use UUtil;
  writeln("UM1.init {");
  var uv1a = new UR("uv1a");
  use UM2;
  var uv1b = new UR("uv1b");
  writeln("UM1.init  }");
  proc main {
    writeln("UM1.main ---");
  }
  use Time;
  use UM3;
}

module UM3 {
  use UUtil;
  writeln("UM3.init");
  var uv3a = new UR("uv3a");
  use UM1;
  use UM4implicitDeinit;
  use UM4explicitDeinit;
  use UM4noDeinit;
}

module UM2 {
  use UUtil;
  writeln("UM2.init");
  var uv2a = new UR("uv2a");
  use UM4explicitDeinit;
}

module UM4noDeinit {
  use UUtil;
  writeln("UM4noDeinit.init");
}

module UM4explicitDeinit {
  use UUtil;
  writeln("UM4explicitDeinit.init");
  var uv4ae = new unmanaged UC("uv4ae");
  proc deinit() {
    writeln("UM4explicitDeinit.deinit user");
    delete uv4ae;
  }
}

module UM4implicitDeinit {
  use UUtil;
  writeln("UM4implicitDeinit init");
  var uv4bi = new UR("uv4bi");
}

module UUtil {
  record UR {
    var name: string;
    proc init(arg: string) {
      name = arg;
      writeln(name, ".init");
    }
    proc deinit() {
      writeln(name, ".deinit");
    }
  }
  class UC {
    var name: string;
    proc init(arg: string) {
      name = arg;
      writeln(name, ".init UC");
    }
    proc deinit() {
      writeln(name, ".deinit UC");
    }
  }
}
