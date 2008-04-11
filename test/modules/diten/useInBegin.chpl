module M1 {
  var a = init_a_and_side_effect();
  var b: single bool = true;
  def init_a_and_side_effect() {
    writeln("side effect!");
    return 1;
  }
}

module M2 {
  def main {
    use Time;
    for i in 1..100 {
      begin {
        sleep(1);
        { use M1; }
      }
    }
  }
}
