// This is the counterpart to tuple-vararg-in-lt-clause.chpl
// that ensures that these constraints fire when they should.

var global = new borrowed Test();

class Test {
  /** a single constraint **/
  proc dependsOnN1(tests: object...)
    lifetime this < tests
  { }                                       // varargs not mentioned in body
  proc dependsOnX1(tests: object...)
    lifetime this < tests
  { writeln(tests(0)); }                    // access a single vararg
  proc dependsOnY1(tests: object...)
    lifetime this < tests
  { var quests = tests; writeln(quests); }  // access all varargs collectively

  /** two constraints **/
  proc dependsOnN2(tests: object...)
    lifetime tests > this, this < tests
  { }
  proc dependsOnX2(tests: object...)
    lifetime tests > this, this < tests
  { writeln(tests(0)); }
  proc dependsOnY2(tests: object...)
    lifetime tests > this, this < tests
  { var quests = tests; writeln(quests); }

  /** three constraints **/
  proc dependsOnN3(tests: object...)
    lifetime tests > global, tests > this, this < tests
  { }
  proc dependsOnX3(tests: object...)
    lifetime tests > global, tests > this, this < tests
  { writeln(tests(0)); }
  proc dependsOnY3(tests: object...)
    lifetime tests > global, tests > this, this < tests
  { var quests = tests; writeln(quests); }
}

proc main {
  const ttt = new borrowed Test();
  const jbo = new borrowed object();
  {
    const obj = new borrowed object();

    /* dependsOnN1 */
    ttt.dependsOnN1(obj);
    ttt.dependsOnN1(jbo);
    ttt.dependsOnN1(obj,obj);
    ttt.dependsOnN1(obj,jbo);
    ttt.dependsOnN1(jbo,obj);
    ttt.dependsOnN1(jbo,jbo);

    /* dependsOnX1 */
    ttt.dependsOnX1(obj);
    ttt.dependsOnX1(jbo);
    ttt.dependsOnX1(obj,obj);
    ttt.dependsOnX1(obj,jbo);
    ttt.dependsOnX1(jbo,obj);
    ttt.dependsOnX1(jbo,jbo);

    /* dependsOnY1 */
    ttt.dependsOnY1(obj);
    ttt.dependsOnY1(jbo);
    ttt.dependsOnY1(obj,obj);
    ttt.dependsOnY1(obj,jbo);
    ttt.dependsOnY1(jbo,obj);
    ttt.dependsOnY1(jbo,jbo);

    /* dependsOnN2 */
    ttt.dependsOnN2(obj);
    ttt.dependsOnN2(jbo);
    ttt.dependsOnN2(obj,obj);
    ttt.dependsOnN2(obj,jbo);
    ttt.dependsOnN2(jbo,obj);
    ttt.dependsOnN2(jbo,jbo);

    /* dependsOnX2 */
    ttt.dependsOnX2(obj);
    ttt.dependsOnX2(jbo);
    ttt.dependsOnX2(obj,obj);
    ttt.dependsOnX2(obj,jbo);
    ttt.dependsOnX2(jbo,obj);
    ttt.dependsOnX2(jbo,jbo);

    /* dependsOnY2 */
    ttt.dependsOnY2(obj);
    ttt.dependsOnY2(jbo);
    ttt.dependsOnY2(obj,obj);
    ttt.dependsOnY2(obj,jbo);
    ttt.dependsOnY2(jbo,obj);
    ttt.dependsOnY2(jbo,jbo);

    /* dependsOnN3 */
    ttt.dependsOnN3(obj);
    ttt.dependsOnN3(jbo);
    ttt.dependsOnN3(obj,obj);
    ttt.dependsOnN3(obj,jbo);
    ttt.dependsOnN3(jbo,obj);
    ttt.dependsOnN3(jbo,jbo);

    /* dependsOnX3 */
    ttt.dependsOnX3(obj);
    ttt.dependsOnX3(jbo);
    ttt.dependsOnX3(obj,obj);
    ttt.dependsOnX3(obj,jbo);
    ttt.dependsOnX3(jbo,obj);
    ttt.dependsOnX3(jbo,jbo);

    /* dependsOnY3 */
    ttt.dependsOnY3(obj);
    ttt.dependsOnY3(jbo);
    ttt.dependsOnY3(obj,obj);
    ttt.dependsOnY3(obj,jbo);
    ttt.dependsOnY3(jbo,obj);
    ttt.dependsOnY3(jbo,jbo);
  }
}
