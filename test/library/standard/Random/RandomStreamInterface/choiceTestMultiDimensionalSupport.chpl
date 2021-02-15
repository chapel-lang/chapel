use choiceTestUtils, Random;

proc main() {
  const vec = 1..7 by 2;
  const vec1 = 2..8 by 3;
  const vec2 = 1..3;
  const vec3 = 1..30;
  var D = {vec,vec,vec};
  var D1 = {vec,vec1,vec2};
  var D2 = {vec2,vec2};
  var D3 = {vec3};
  var testDom = {vec2,vec2};
  var A:[D] int;
  var B:[1..30] int;
  var C:[D1] int;
  var Z:[testDom] int;
  A = 3;
  C = 4;
  var Prob2:[D2] real;
  var Prob:[D1] real;

  var r = new owned PCGRandomStream(int);

  //ARRAY USECASES
  //Multi Dimensional Array
  writeln(r.choice(C, testDom, false, Prob));
  writeln("----------");
  writeln(r.choice(C, testDom, true, Prob));
  writeln("----------");
  writeln(r.choice(C, 2, false, Prob));
  writeln("----------");
  writeln(r.choice(C, 5, true, Prob));
  writeln("----------");

  writeln(r.choice(C, testDom, false));
  writeln("----------");
  writeln(r.choice(C, testDom));
  writeln("----------");

  writeln(r.choice(C, 3));
  writeln("----------");
  writeln(r.choice(C, 2, false));
  writeln("----------");
  writeln(r.choice(C, 8, false));

  //Single Dimensional Array
  writeln(r.choice(B, testDom, false));
  writeln("----------");
  writeln(r.choice(B, testDom));

  writeln("----------");
  writeln(r.choice(B, 3));
  writeln("----------");
  writeln(r.choice(B, 2, false));
  writeln("----------");
  writeln(r.choice(B, 8, false));




  //DOMAIN USECASES
  //Multi Dimensional Domain
  writeln(r.choice(D1, testDom, false, Prob));
  writeln("----------");
  writeln(r.choice(D1, testDom, true, Prob));
  writeln("----------");
  writeln(r.choice(D1, 2, false, Prob));
  writeln("----------");
  writeln(r.choice(D1, 5, true, Prob));
  writeln("----------");

  writeln(r.choice(D1, testDom, false));
  writeln("----------");
  writeln(r.choice(D1, testDom));
  writeln("----------");

  writeln(r.choice(D1, 3));
  writeln("----------");
  writeln(r.choice(D1, 2, false));
  writeln("----------");
  writeln(r.choice(D1, 8, false));

  //Single Dimensional Domain
  writeln(r.choice(D3, testDom, false));
  writeln("----------");
  writeln(r.choice(D3, testDom));

  writeln("----------");
  writeln(r.choice(D3, 3));
  writeln("----------");
  writeln(r.choice(D3, 2, false));
  writeln("----------");
  writeln(r.choice(D3, 8, false));
}
