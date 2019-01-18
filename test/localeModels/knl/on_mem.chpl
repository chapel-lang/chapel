class myClass {
  var x: int;
}

var default0: myClass;
var large1: myClass;
var lowlat2: myClass;
var highbw3: myClass;
var default4: myClass;
var large5: myClass;
var lowlat6: myClass;
var highbw7: myClass;
var default8: myClass;
var highbw9: myClass;
var defaultA: myClass;

on here.defaultMemory() {
  default0 = new myClass();
  default0.x = 0;
}

on here.largeMemory() {
  large1 = new myClass();
  large1.x = 1;
}

on here.lowLatencyMemory() {
  lowlat2 = new myClass();
  lowlat2.x = 2;
}

writeln("here = ", here);

on here.highBandwidthMemory() {
  highbw3 = new myClass();
  highbw3.x = 3;
  on highbw3.locale.defaultMemory() {
    writeln("highbw3.locale.defaultMemory() = ", here);
    default4 = new myClass();
    default4.x = 4;
  }
  on Locales(0).largeMemory() {
    large5 = new myClass();
    large5.x = 5;
  }
}

on here.highBandwidthMemory().lowLatencyMemory() {
  lowlat6 = new myClass();
  lowlat6.x = 6;
}

writeln("here.highBandwidthMemory().defaultMemory() = ",
        here.highBandwidthMemory().defaultMemory());

on Locales(0).largeMemory() {
  on here.highBandwidthMemory() {
    highbw7 = new myClass();
    highbw7.x = 7;
  }
}

on here.highBandwidthMemory().defaultMemory() {
  default8 = new myClass();
  default8.x = 8;
}

on here.getChild(0).highBandwidthMemory() {
  highbw9 = new myClass();
  highbw9.x = 9;
  on here.defaultMemory() {
    defaultA = new myClass();
    defaultA.x = 10;
  }
}

writeln("default0: ", default0.x, ", large1: ", large1.x,
	", lowlat2: ", lowlat2.x, ", highbw3: ", highbw3.x,
	", default4: ", default4.x, ", large5: ", large5.x, ",");
writeln("  lowlat6: ", lowlat6.x, ", highbw7: ", highbw7.x,
        ", default8: ", default8.x, ", highbw9: ", highbw9.x,
        ", defaultA: ", defaultA.x);

writeln("default0 in hbm = ", chpl_addrIsInHbm(c_ptrTo(default0.x)));
writeln("large1 in hbm = ", chpl_addrIsInHbm(c_ptrTo(large1.x)));
writeln("lowlat2 in hbm = ", chpl_addrIsInHbm(c_ptrTo(lowlat2.x)));
writeln("highbw3 in hbm = ", chpl_addrIsInHbm(c_ptrTo(highbw3.x)));
writeln("default4 in hbm = ", chpl_addrIsInHbm(c_ptrTo(default4.x)));
writeln("large5 in hbm = ", chpl_addrIsInHbm(c_ptrTo(large5.x)));
writeln("lowlat6 in hbm = ", chpl_addrIsInHbm(c_ptrTo(lowlat6.x)));
writeln("highbw7 in hbm = ", chpl_addrIsInHbm(c_ptrTo(highbw7.x)));
writeln("default8 in hbm = ", chpl_addrIsInHbm(c_ptrTo(default8.x)));
writeln("highbw9 in hbm = ", chpl_addrIsInHbm(c_ptrTo(highbw9.x)));
writeln("defaultA in hbm = ", chpl_addrIsInHbm(c_ptrTo(defaultA.x)));
