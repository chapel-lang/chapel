record myR {
  var x = 10;
  proc init() do writeln("init");
  proc init=(other) {
    this.x = other.x;
    writeln("init=");
  }

}
var rInstance: myR;
var sum = 0;
writeln("Is this 10? ", rInstance.x);  // yes

foreach i in 1..10 with (ref sum, in rInstance) {  // ignore racy `ref sum`
  rInstance.x = 5;
  sum += rInstance.x;
}

writeln("Is this 10? ", rInstance.x);  // yes
writeln("Is this 50? ", sum);  // yes
