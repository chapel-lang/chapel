class myClass {
  var x: int;
}

var default0: myClass;
var large1: myClass;
var random2: myClass;
var streaming3: myClass;
var default4: myClass;
var large5: myClass;
var random6: myClass;
var streaming7: myClass;
var default8: myClass;
var streaming9: myClass;
var defaultA: myClass;

on here.defaultMemory() {
  default0 = new myClass();
  default0.x = 0;
}

on here.largeMemory() {
  large1 = new myClass();
  large1.x = 1;
}

on here.randomMemory() {
  random2 = new myClass();
  random2.x = 2;
}

writeln("here = ", here);

on here.streamingMemory() {
  streaming3 = new myClass();
  streaming3.x = 3;
  on streaming3.locale.defaultMemory() {
    writeln("streaming3.locale.defaultMemory() = ", here);
    default4 = new myClass();
    default4.x = 4;
  }
  on Locales(0).largeMemory() {
    large5 = new myClass();
    large5.x = 5;
  }
}

on here.streamingMemory().defaultMemory() {
  random6 = new myClass();
  random6.x = 6;
}

writeln("here.streamingMemory().defaultMemory() = ",
        here.streamingMemory().defaultMemory());

on Locales(0).largeMemory() {
  on here.streamingMemory() {
    streaming7 = new myClass();
    streaming7.x = 7;
  }
}

on (here:LocaleModel).hbm.defaultMemory() {
  default8 = new myClass();
  default8.x = 8;
}

on (here:LocaleModel).getChild(0).streamingMemory() {
  streaming9 = new myClass();
  streaming9.x = 9;
  on here.defaultMemory() {
    defaultA = new myClass();
    defaultA.x = 10;
  }
}

writeln("default0: ", default0.x, ", large1: ", large1.x,
	", random2: ", random2.x, ", streaming3: ", streaming3.x,
	", default4: ", default4.x, ", large5: ", large5.x, ",");
writeln("  random6: ", random6.x, ", streaming7: ", streaming7.x,
        ", default8: ", default8.x, ", streaming9: ", streaming9.x,
        ", defaultA: ", defaultA.x);

writeln("default0 in hbm = ", chpl_addrIsInHbm(c_ptrTo(default0.x)));
writeln("large1 in hbm = ", chpl_addrIsInHbm(c_ptrTo(large1.x)));
writeln("random2 in hbm = ", chpl_addrIsInHbm(c_ptrTo(random2.x)));
writeln("streaming3 in hbm = ", chpl_addrIsInHbm(c_ptrTo(streaming3.x)));
writeln("default4 in hbm = ", chpl_addrIsInHbm(c_ptrTo(default4.x)));
writeln("large5 in hbm = ", chpl_addrIsInHbm(c_ptrTo(large5.x)));
writeln("random6 in hbm = ", chpl_addrIsInHbm(c_ptrTo(random6.x)));
writeln("streaming7 in hbm = ", chpl_addrIsInHbm(c_ptrTo(streaming7.x)));
writeln("default8 in hbm = ", chpl_addrIsInHbm(c_ptrTo(default8.x)));
writeln("streaming9 in hbm = ", chpl_addrIsInHbm(c_ptrTo(streaming9.x)));
writeln("defaultA in hbm = ", chpl_addrIsInHbm(c_ptrTo(defaultA.x)));
