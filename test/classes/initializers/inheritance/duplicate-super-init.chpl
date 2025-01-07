class P { }
class C: P {
  proc init() {
    super.init();
    super.init();
  }
}

proc main() {
  var c = new C();
}
