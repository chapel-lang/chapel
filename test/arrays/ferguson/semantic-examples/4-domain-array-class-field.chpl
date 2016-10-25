use samples; 

class ClassStoringArray{
  var dom: domain(1);
  var arr:[dom] R;

  proc setup() {
    dom = {1..2};
    arr = ArrTwoThree;
  }
}

proc run() {
  var c = new ClassStoringArray();
  c.setup();
  writeln(c);
  delete c;
}

run();

