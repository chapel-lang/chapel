// Test that the relational operators are folded over enums
enum color { red=2, orange=5, yellow, green=5, blue=6, indigo=1, violet };
enum nums { one=1, two, three, four, five, six };
proc main() {
  param result1 = testeq(color.red, color.violet);    // true  (2 == 2)
  param result2 = testeq(color.red, color.yellow);    // false (2 == 6)
  param result4 = testneq(nums.two, nums.four);       // true  (2 != 4)

  param result = result1  && !result2  && result4;

  if (!result) {
    writeln(result1);
    writeln(!result2);
    writeln(result4);
  }
}

proc testeq(param a: enumerated, param b: enumerated) param return a==b;
proc testneq(param a: enumerated, param b: enumerated) param return a!=b;
proc testgt(param a: enumerated, param b: enumerated) param return a>b;
proc testgte(param a: enumerated, param b: enumerated) param return a>=b;
proc testlt(param a: enumerated, param b: enumerated) param return a<b;
proc testlte(param a: enumerated, param b: enumerated) param return a<=b;
