// This tests mimicks the coforall+on in the range leader iterator 
// when compiling under numa. Which caused compiler assertion failure on:
//   parallel/forall/in-intents/both-arr-dom-const-const.chpl
//   parallel/forall/in-intents/both-arr-dom-var-const.chpl

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
  forall myiter() with (const in ARR, const in DOM) {
    update();
    sy$.readFE();
    writeln(ARR, " ", DOM);
  }
  writeln("done ", ARR, " ", DOM);
}

iter myiter() { yield 555; }

iter myiter(param tag) {
  coforall loc in Locales do
    on loc do
      for 1..3 do yield 777;
}
