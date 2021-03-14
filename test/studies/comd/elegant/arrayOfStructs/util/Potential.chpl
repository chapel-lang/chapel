class BasePotential {
  var cutoff      : real;
  var mass        : real;
  var lat         : real;
  var latticeType : string;
  var name        : string;
  var atomicNo    : int;

  proc init() {
  }

  proc init(info : unmanaged BasePotential) {
    this.cutoff      = info.cutoff;
    this.mass        = info.mass;
    this.lat         = info.lat;
    this.latticeType = info.latticeType;
    this.name        = info.name;
    this.atomicNo    = info.atomicNo;
  }

  proc force() { }
  proc print() { }
}
