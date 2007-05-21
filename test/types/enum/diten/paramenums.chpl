// Test that the relational operators are folded over enums
enum color { red=2, orange=5, yellow, green=5, blue=6, indigo=1, violet };
enum nums { one=1, two, three, four, five, six };
def main() {
  param result1 = testeq(red, violet);    // true  (2 == 2)
  param result2 = testeq(red, yellow);    // false (2 == 6)
  param result3 = testneq(red, two);      // false (2 != 2)
  param result4 = testneq(two, four);     // true  (2 != 4)
  param result5 = testgt(blue, six);      // false (6 > 6)
  param result6 = testgt(red, three);     // false (2 > 3)
  param result7 = testgt(blue, five);     // true  (6 > 5)
  param result8 = testgte(yellow, blue);  // true  (6 >= 6)
  param result9 = testgte(indigo, four);  // false (1 >= 4)
  param result10= testgte(green, indigo); // true  (5 >= 1)
  param result11= testlt(one, red);       // true  (1 < 2)
  param result12= testlt(one, indigo);    // false (1 < 1)
  param result13= testlt(blue, yellow);   // false (6 < 6)
  param result14= testlte(orange, six);   // true  (5 <= 6)
  param result15= testlte(violet, two);   // true  (2 <= 2)
  param result16= testlte(orange, one);   // false (5 <= 1)

  param result = result1  && !result2  && !result3  && result4  && !result5 &&
                 !result6 && result7   && result8   && !result9 && result10 &&
                 result11 && !result12 && !result13 && result14 && result15 &&
                 !result16;

  if (!result) {
    writeln(result1);
    writeln(!result2);
    writeln(!result3);
    writeln(result4);
    writeln(!result5);
    writeln(!result6);
    writeln(result7);
    writeln(result8);
    writeln(!result9);
    writeln(result10);
    writeln(result11);
    writeln(!result12);
    writeln(!result13);
    writeln(result14);
    writeln(result15);
    writeln(!result16);
  }
}

def testeq(param a: enumerated, param b: enumerated) param return a==b;
def testneq(param a: enumerated, param b: enumerated) param return a!=b;
def testgt(param a: enumerated, param b: enumerated) param return a>b;
def testgte(param a: enumerated, param b: enumerated) param return a>=b;
def testlt(param a: enumerated, param b: enumerated) param return a<b;
def testlte(param a: enumerated, param b: enumerated) param return a<=b;
