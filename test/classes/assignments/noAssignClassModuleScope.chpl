module Main {
  module Mod {
    class Obj {
      var frominit, fromassign, count: int;

      proc init(x:int){
        this.frominit = x;
        this.fromassign = 0;
	this.count = 1;
      }
    }

    operator =(ref lhs: Obj, rhs: Obj) {
      halt("In my Obj.operator= -- should not be callable");
      lhs.fromassign = rhs.frominit;
      lhs.count += 1;
    }
  }

  use Mod;

  var oa: owned Obj? = new Obj(1);
  writeln(oa);
  var ob: owned Obj? = new Obj(2);
  writeln(ob);
  oa = ob;
  writeln(oa);
}
