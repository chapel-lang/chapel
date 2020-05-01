// Test that the relational operators are folded over enums
enum color { red=2, orange=5, yellow, green=5, blue=6, indigo=1, violet };
enum nums { one=1, two, three, four, five, six };
proc main() {
  param result1 = testeq(color.red, color.violet);    // false
  param result2 = testeq(color.red, color.yellow);    // false
  param result3 = testeq(color.red, color.red);       // true
  param result4 = testneq(nums.two, nums.four);       // true
  param result8 = testgte(color.yellow, color.blue);  // false
  param result10 = testgte(color.green, color.indigo);// false
  param result13 = testlt(color.blue, color.yellow);  // false

  param result = !result1  && !result2  && result3 && result4 && !result8 && !result10 && !result13;

  if (!result) {
    writeln(!result1);
    writeln(!result2);
    writeln(result3);
    writeln(result4);
    writeln(!result8);
    writeln(!result10);
    writeln(!result13);
  }
}

proc testeq(param a: enum, param b: enum) param return a==b;
proc testneq(param a: enum, param b: enum) param return a!=b;
proc testgt(param a: enum, param b: enum) param return a>b;
proc testgte(param a: enum, param b: enum) param return a>=b;
proc testlt(param a: enum, param b: enum) param return a<b;
proc testlte(param a: enum, param b: enum) param return a<=b;
