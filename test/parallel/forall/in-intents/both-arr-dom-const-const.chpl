
var ARR = [555, 666];
var DOM = {555..666};
var sy$: sync int;

proc update() {
  ARR = [777, 888];
  DOM = {777..888};
  sy$.writeEF(1);
}

proc main {  test(ARR, DOM);  }

proc test(const ref ARR, const ref DOM) {
  forall 1..3 with (const in ARR, const in DOM) {
    update();
    sy$.readFE();
    writeln(ARR, " ", DOM);
  }
  writeln("done ", ARR, " ", DOM);
}
