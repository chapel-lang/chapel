use UnitTest;
use MasonArgParse;
use List;
use IO;

// a short string opt with single value and expected # values supplied
proc testSingleStringShortOpt(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"twenty");
}

// a short string opt with single value with = and expected # values supplied
proc testSingleStringShortOptEquals(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"twenty");
}

// a short string opt with single value with = and extra # values supplied
proc testSingleStringShortOptEqualsExtra(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short string opt with range of value with = and OK # values supplied
proc testMultiStringShortOptEqualsOK(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..10,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.values()),
                   new list(["twenty","thirty"]));
}


// attempt to specify no option flag
proc testTryMakeEmptyOpts(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  var optList:[0..0]string;
  try {
    var myStrArg = parser.addOption(name="StringOpt",
                                    opts=optList,
                                    numArgs=1..10,
                                    required=false,
                                    defaultValue=none);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// three short string opts with fixed range multi value opts,
// min, mid, and max # values supplied respectively
proc testThreeMultiStringShortOptPartValEquals(test: borrowed Test) throws {
  var argList=["progName","-n=twenty","-p","thirty","five","-t=forty","two"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1..3,
                                   required=false,
                                   defaultValue=none);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1..4,
                                   required=false,
                                   defaultValue=none);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1..2,
                                   required=false,
                                   defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg1.hasValue());
  test.assertTrue(myStrArg2.hasValue());
  test.assertTrue(myStrArg3.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg1.value(),"twenty");
  test.assertEqual(new list(myStrArg2.values()), new list(argList[3..4]));
  test.assertEqual(new list(myStrArg3.values()), new list(["forty","two"]));
}

// attempt to specify invalid argument option flag
proc testTryMakeBadOptID(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  try {
    var myStrArg = parser.addOption(name="StringOpt",
                                    opts=["name","-n"],
                                    numArgs=1..10,
                                    required=false,
                                    defaultValue=none);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short string opt with single value, expressed multiple times
proc testSingleStringShortOptRepeated(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-n","thirty","-n","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  } catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}


// a short string opt with range value, expressed multiple times
proc testRangeStringShortOptRepeated(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","two","hundred","-n",
                 "thirty","five","-n","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  } catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short string opt with range value, expressed multiple times, with
// too many values supplied in first instance
proc testRangeStringShortOptRepeatedTooManyFirst(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","two","hundred","fifty","-n",
                 "thirty","five","-n","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  } catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short string opt with single value and no values supplied
// optional value not supplied
proc testOptSingleStringShortOptNoOpts(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure no value was captured
  test.assertFalse(myStrArg.hasValue());
}

// a short string opt with single required value and no values supplied
// required value not supplied
proc testOptSingleStringShortReqOptNoOpts(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1,
                                  required=true);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short string opt with single required value and 1 value supplied
// required value supplied
proc testOptSingleStringShortReqOptReqVal(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1,
                                  required=true);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"twenty");
}

// a parser with no arguments defined, gets no arguments
proc testNoOptsDefined(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  parser.parseArgs(argList[1..]);
  test.assertTrue(true);
}

// a parser with no arguments defined, gets 1 argument
proc testNoOptsDefinedUnknownOptGiven(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty"];
  var parser = new argumentParser();
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}


// a short string opt with 0..1 range and no values supplied
proc testOptRangeStringShortOpt1Val(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=0..1,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  test.assertFalse(myStrArg.hasValue());
}

// a short string opt with 0..1 range and opt, but no value supplied
proc testOptRangeStringShortOptOptNoVal(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=0..1,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  test.assertFalse(myStrArg.hasValue());
}

// a short string opt with 1..2 range and opt, but no value supplied
proc testOptRangeStringShortOptPresentNoVal(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..2,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short string opt with 0..1 range and extra values supplied
proc testOptRangeStringShortOptExtraVal(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","two"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=0..1,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}


// a short string opt with 0..1 range and accepted values supplied
// with an additional, undefined argument
proc testOptRangeStringShortOptOkValBadArg(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-p","thirty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=0..1,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short string opt with 0..1 value and 1 value supplied
proc testSingleStringShortOptNoOpts(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=0..1,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"twenty");
}

// a long string opt with single value and expected # values supplied
proc testSingleStringLongOpt(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"twenty");
}

// a short string opt with exact numArgs and expected # values supplied
proc testMultiStringShortOpt(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.values()), new list(argList[2..]));
}

// a long string option with exact numArgs and expected # values supplied
proc testMultiStringLongOpt(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty","thirty","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.values()), new list(argList[2..]));
}

// a short string opt with fixed range numArgs, max # supplied
proc testMultiStringShortOptVar(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.values()), new list(argList[2..]));
}

// test a long string option with fixed range of numArgs, max # supplied
proc testMultiStringLongOptVar(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty","thirty","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.values()), new list(argList[2..]));
}

// test a short string option with fixed range of numArgs, less than
// max # supplied
proc testMultiStringShortOptVarLess(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.values()), new list(argList[2..]));
}

// test a short string option with fixed range of numArgs more than 1,
// fewer than min # supplied
proc testMultiStringShortOptVarLessThanMin(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3..5,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  } catch ex : ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  //expect failure
  test.assertTrue(false);
}

// test a short string option with fixed range of numArgs, more than
// max # supplied
proc testMultiStringShortOptVarMore(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty","forty","fifty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  } catch ex : ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  //expect failure
  test.assertTrue(false);
}


// test a short string option with fixed range of numArgs, less than
// min # supplied
proc testMultiStringShortOptVarNoVal(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short string opt with single value and no values supplied
proc testSingleStringShortOptNoVal(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// test a long string option with unbound range of numArgs, inbound # supplied
proc testUnboundStringLongOptVar(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty","thirty","forty","fifty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.values()), new list(argList[2..]));
}

// two short string opts with single value and expected # values supplied
proc testTwoStringShortOpt(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-p","thirty"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg1.hasValue());
  test.assertTrue(myStrArg2.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg1.value(),"twenty");
  test.assertEqual(myStrArg2.value(),"thirty");
}

// three short string opts with single value and expected # values supplied
proc testThreeStringShortOpt(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-p","thirty","-t","forty"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg1.hasValue());
  test.assertTrue(myStrArg2.hasValue());
  test.assertTrue(myStrArg3.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg1.value(),"twenty");
  test.assertEqual(myStrArg2.value(),"thirty");
  test.assertEqual(myStrArg3.value(),"forty");
}

// three short string opts with single value and missing value first var
proc testThreeStringShortOptMissingFirst(test: borrowed Test) throws {
  var argList = ["progName","-n","-p","thirty","-t","forty"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// three short string opts with single value and missing value middle var
proc testThreeStringShortOptMissingMiddle(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-p","-t","forty"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// three short string opts with single value and missing value last var
proc testThreeStringShortOptMissingLast(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-p","thirty","-t"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// three short string opts with single value and extra value first var
proc testThreeStringShortOptExtraFirst(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","five","-p","thirty","-t","forty"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// three short string opts with single value and extra value middle var
proc testThreeStringShortOptExtraMiddle(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-p","thirty","five","-t","forty"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// three short string opts with single value and extra value last var
proc testThreeStringShortOptExtraLast(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-p","thirty","-t","forty","five"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// three short string opts with single value and extra value middle var,
// and missing last value
proc testThreeStringShortOptExtraMidMissingLast(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-p","thirty","five","-t"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// three short string opts with mixed single and multi value opts,
// expected # supplied
proc testThreeMixedStringShortOptAllValues(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-p","thirty","five","-t",
                 "forty","two"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=2);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1..2,
                                   required=false,
                                   defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg1.hasValue());
  test.assertTrue(myStrArg2.hasValue());
  test.assertTrue(myStrArg3.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg1.value(),"twenty");
  test.assertEqual(new list(myStrArg2.values()), new list(argList[4..5]));
  test.assertEqual(new list(myStrArg3.values()), new list(argList[7..8]));
}

// three short string opts with fixed range multi value opts,
// min, mid, and max # values supplied respectively
proc testThreeMultiStringShortOptPartialValues(test: borrowed Test) throws {
  var argList=["progName","-n","twenty","-p","thirty","five","-t",
               "forty","two"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1..3,
                                   required=false,
                                   defaultValue=none);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1..4,
                                   required=false,
                                   defaultValue=none);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1..2,
                                   required=false,
                                   defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg1.hasValue());
  test.assertTrue(myStrArg2.hasValue());
  test.assertTrue(myStrArg3.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg1.value(),"twenty");
  test.assertEqual(new list(myStrArg2.values()), new list(argList[4..5]));
  test.assertEqual(new list(myStrArg3.values()), new list(argList[7..8]));
}

// a short string opt with single value and default value specified,
// no value supplied by the user
// optional value not supplied, default assigned
proc testSingleStringShortOptDefNoVal(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1,
                                  defaultValue="twenty");

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"twenty");
}

// a short string opt with single value and default value specified,
// with a value supplied by the user
// optional value supplied
proc testSingleStringShortOptDefOneVal(test: borrowed Test) throws {
  var argList = ["progName","-n","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1,
                                  defaultValue="twenty");

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"forty");
}

// a short string opt with multiple values and default value specified,
// with a value supplied by the user
// multiple optional values defined and supplied by user
proc testMultStringShortOptDefMultiVal(test: borrowed Test) throws {
  var argList=["progName","-n","twenty","-p","thirty","five","-t",
               "forty","two"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   defaultValue=new list(["one","two"]),
                                   numArgs=1..3,
                                   required=false);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1..4,
                                   required=false,
                                   defaultValue=none);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   defaultValue=new list(["1","2"]),
                                   numArgs=1..2,
                                   required=false);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg1.hasValue());
  test.assertTrue(myStrArg2.hasValue());
  test.assertTrue(myStrArg3.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg1.value(),"twenty");
  test.assertEqual(new list(myStrArg2.values()), new list(argList[4..5]));
  test.assertEqual(new list(myStrArg3.values()), new list(argList[7..8]));
}

// a short string opt with multiple values and default value specified,
// with no value supplied by the user
proc testMultStringShortOptDefMultiValNoVal(test: borrowed Test) throws {
  var argList=["progName"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   defaultValue=new list(["one","two"]),
                                   numArgs=1..3,
                                   required=false);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1..4,
                                   required=false,
                                   defaultValue=none);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   defaultValue=new list(["1","2"]),
                                   numArgs=1..2,
                                   required=false);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertTrue(myStrArg3.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg1.values()), new list(["one","two"]));
  //test.assertEqual(new list(myStrArg2.values()), new list(argList[4..5]));
  test.assertEqual(new list(myStrArg3.values()), new list(["1","2"]));
}

// a short string opt with multiple values and default values specified,
// with a required option and required value supplied by the user
// required value supplied, optional values get defaults
proc testMultStringShortOptDefMultiValReqVal(test: borrowed Test) throws {
  var argList=["progName","-p","forty","two","thirty","five"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   defaultValue=new list(["one","two"]),
                                   numArgs=1..3,
                                   required=false);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   required=true,
                                   numArgs=1..4,
                                   defaultValue=none);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   defaultValue=new list(["1","2"]),
                                   numArgs=1..2,
                                   required=false);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg1.hasValue());
  test.assertTrue(myStrArg2.hasValue());
  test.assertTrue(myStrArg3.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg1.values()), new list(["one","two"]));
  test.assertEqual(new list(myStrArg2.values()), new list(argList[2..5]));
  test.assertEqual(new list(myStrArg3.values()), new list(["1","2"]));
}

// a short string opt with multiple values and default values specified,
// with a required option and no required value supplied by the user
// required value not supplied, optional values not supplied
proc testMultStringShortOptDefMultiValReqNoVal(test: borrowed Test) throws {
  var argList=["progName"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   defaultValue=new list(["one","two"]),
                                   numArgs=1..3,
                                   required=false);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   required=true,
                                   numArgs=1..4,
                                   defaultValue=none);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   defaultValue=new list(["1","2"]),
                                   numArgs=1..2,
                                   required=false);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);

}

// attempt to specify unsupported default value type
proc testTryMakeIntListOpts(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  try {
    var myStrArg = parser.addOption(name="StringOpt",
                                    opts=["-n","--strArg"],
                                    numArgs=1,
                                    required=false,
                                    defaultValue=new list([1,2,3]));
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// attempt to specify unsupported default value type
proc testTryMakeIntOpt(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  try {
    var myStrArg = parser.addOption(name="StringOpt",
                                    opts=["-n","--strArg"],
                                    numArgs=1,
                                    required=false,
                                    defaultValue=1);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// attempt to specify unsupported default value type
proc testTryMakeBoolListOpts(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  try {
    var myStrArg = parser.addOption(name="StringOpt",
                                    opts=["-n","--strArg"],
                                    numArgs=1,
                                    required=false,
                                    defaultValue=new list([true,false]));
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// attempt to specify unsupported default value type
proc testTryMakeBoolOpt(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  try {
    var myStrArg = parser.addOption(name="StringOpt",
                                    opts=["-n","--strArg"],
                                    numArgs=1,
                                    required=false,
                                    defaultValue=true);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// optional value not supplied, default empty list assigned
proc testEmptyListDefaultVal(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..,
                                  defaultValue=new list(string));

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //shouldn't have a value as none was passed
  test.assertFalse(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.values()),new list(string));
}

// attempt to define a name twice
proc testTryDuplicateNameOpt(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt",
                                opts=["-n","--strArg"],
                                numArgs=1);
  try {
    var myStrArg2 = parser.addOption(name="StringOpt",
                                    opts=["-p","--print"],
                                    numArgs=1);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// attempt to define a short option twice
proc testTryDuplicateShortOpt(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt",
                                opts=["-n","--strArg"],
                                numArgs=1);
  try {
    var myStrArg2 = parser.addOption(name="PrintOpt",
                                    opts=["-n","--print"],
                                    numArgs=1);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// attempt to define a long option twice
proc testTryDuplicateLongOpt(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt",
                                opts=["-n","--strArg"],
                                numArgs=1);
  try {
    var myStrArg2 = parser.addOption(name="PrintOpt",
                                    opts=["-p","--strArg"],
                                    numArgs=1);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// add a subcommand
proc testAddSubCommand(test: borrowed Test) throws {
  var argList = ["progName","subCommand1"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var remain = parser.parseArgs(argList[1..]);
  test.assertEqual(remain.size,0);
  test.assertTrue(mySubCmd1.hasValue());
}

// use an option and then a subcommand
proc testOptionPlusSubCommand(test: borrowed Test) throws {
  var argList = ["progName","-n","20","subCommand1"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var myStrArg1 = parser.addOption(name="StringOpt",
                                   opts=["-n","--strArg"],
                                   numArgs=1);
  var remain = parser.parseArgs(argList[1..]);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(remain.size, 0);
  test.assertTrue(myStrArg1.hasValue());
  test.assertEqual(myStrArg1.value(), "20");
}


// add a subcommand with sub options
proc testAddSubCommandSubOptions(test: borrowed Test) throws {
  var argList = ["progName","subCommand1","-n","20"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var remain = parser.parseArgs(argList[1..]);
  test.assertEqual(remain.size,2);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(argList[2..]),remain);
}

// add a subcommand and argument, but don't use subcommand
proc testAddArgAndSubCommandOnlyArgUsed(test: borrowed Test) throws {
  var argList = ["progName","-n","20"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var myStrArg1 = parser.addOption(name="StringOpt",
                                   opts=["-n","--strArg"],
                                   numArgs=1);
  var remain = parser.parseArgs(argList[1..]);
  test.assertEqual(remain.size,0);
  test.assertFalse(mySubCmd1.hasValue());
  test.assertTrue(myStrArg1.hasValue());
  test.assertEqual(myStrArg1.value(),"20");
}

// add a subcommand and argument, but don't use either
proc testAddArgAndSubCommandNoUseEither(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var myStrArg1 = parser.addOption(name="StringOpt",
                                   opts=["-n","--strArg"],
                                   numArgs=1);
  var remain = parser.parseArgs(argList[1..]);
  test.assertEqual(remain.size,0);
  test.assertFalse(mySubCmd1.hasValue());
  test.assertFalse(myStrArg1.hasValue());
}

// add two subcommands and use first
proc testAddTwoSubCommandUseFirst(test: borrowed Test) throws {
  var argList = ["progName","subCommand1"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  var remain = parser.parseArgs(argList[1..]);
  test.assertEqual(remain.size,0);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertFalse(mySubCmd2.hasValue());
}

// add two subcommands and use second
proc testAddTwoSubCommandUseSecond(test: borrowed Test) throws {
  var argList = ["progName","subCommand2"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  var remain = parser.parseArgs(argList[1..]);
  test.assertEqual(remain.size,0);
  test.assertFalse(mySubCmd1.hasValue());
  test.assertTrue(mySubCmd2.hasValue());
}

// add two subcommands and use none
proc testAddTwoSubCommandUseNone(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  var remain = parser.parseArgs(argList[1..]);
  test.assertEqual(remain.size,0);
  test.assertFalse(mySubCmd1.hasValue());
  test.assertFalse(mySubCmd2.hasValue());
}

// add two subcommands and use an undefined command
proc testAddTwoSubCommandUseUndefined(test: borrowed Test) throws {
  var argList = ["progName","subCommandNone"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  try {
    var remain = parser.parseArgs(argList[1..]);
  } catch ex: ArgumentError {
      test.assertTrue(true);
      stderr.writeln(ex.message());
      return;
  }
  test.assertTrue(false);
}

// add two subcommands and use an undefined command with good command
proc testAddTwoSubCommandUseUndefinedWithGood(test: borrowed Test) throws {
  var argList = ["progName","subCommandNone", "subCommand1"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  try {
    var remain = parser.parseArgs(argList[1..]);
  } catch ex: ArgumentError {
      test.assertTrue(true);
      stderr.writeln(ex.message());
      return;
  }
  test.assertTrue(false);
}

// test a short string option with bad option first
proc testStringShortOptVarBadVarNoVal(test: borrowed Test) throws {
  var argList = ["progName","-x","-n","twenty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short string opt with range value, expressed multiple times, with
// allowable number of values supplied overall
proc testRangeStringShortOptRepeatedAllowableCount(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-n","thirty","-n","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  test.assertTrue(myStrArg.hasValue());
  test.assertEqual(new list(myStrArg.values()),
                   new list(["twenty","thirty","forty"]));
}

// add a subcommand with sub options, and options before
proc testOptionsWithSubCommandSubOptions(test: borrowed Test) throws {
  var argList = ["progName","-x","four","subCommand1","-n","20"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-x","--stringVal"],
                                  numArgs=1);
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var remain = parser.parseArgs(argList[1..]);
  test.assertTrue(myStrArg.hasValue());
  test.assertEqual(myStrArg.value(), "four");
  test.assertEqual(remain.size,2);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(argList[4..]),remain);
}

// add a subcommand with sub options, and options before
proc testOptionsRangeWithSubCommandSubOptions(test: borrowed Test) throws {
  var argList = ["progName","-x","four","subCommand1","-n","20"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-x","--stringVal"],
                                  numArgs=1..2,
                                  required=false,
                                  defaultValue=none);
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var remain = parser.parseArgs(argList[1..]);
  test.assertTrue(myStrArg.hasValue());
  test.assertEqual(myStrArg.value(), "four");
  test.assertEqual(remain.size,2);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(argList[4..]),remain);
}

// add a subcommand with sub options, and options before with same flags
proc testOptionsRangeWithSubCommandSameSubOptions(test: borrowed Test) throws {
  var argList = ["progName","-n","four","subCommand1","-n","20","-x","30"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..2,
                                  required=false,
                                  defaultValue=none);
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var remain = parser.parseArgs(argList[1..]);
  test.assertTrue(myStrArg.hasValue());
  test.assertEqual(myStrArg.value(), "four");
  test.assertEqual(remain.size,4);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(argList[4..]),remain);
}

// add a subcommand with sub options, and options before with same flags
// when main command has low bound range
proc testOptsRangeWithSubCommandSameSubOptionsLow(test: borrowed Test) throws {
  var argList = ["progName","-n","four","subCommand1","-n","20","-x","30"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..,
                                  required=false,
                                  defaultValue=none);
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var remain = parser.parseArgs(argList[1..]);
  test.assertTrue(myStrArg.hasValue());
  test.assertEqual(myStrArg.value(), "four");
  test.assertEqual(remain.size,4);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(argList[4..]),remain);
}


// add a subcommand with sub options, and options before with same flags
// when main command has low bound range
proc testFromArgParseExample(test: borrowed Test) throws {
  var argList = ["progName","-o","w","a","y","d","t","a?","-t=tea","time",
                 "--myConfigVar=@10","subCmd1","-c","one","-t","two"];
  var parser = new argumentParser();
  var strArg = parser.addOption(name="strArg1",
                                opts=["-o","--option"],
                                numArgs=1..10,
                                required=false,
                                defaultValue=none);
  var typArg = parser.addOption(name="strArg2",
                                opts=["-t","--types"],
                                numArgs=1..);
  var confArg = parser.addOption(name="strArg3",
                                opts=["--myConfigVar"],
                                numArgs=1);

  var subCmd1 = parser.addSubCommand(cmd="subCmd1");
  var remain = parser.parseArgs(argList[1..]);
  test.assertTrue(strArg.hasValue());
  test.assertEqual(new list(strArg.values()),
                   new list(["w","a","y","d","t","a?"]));
  test.assertEqual(new list(typArg.values()), new list(["tea","time"]));
  test.assertEqual(confArg.value(),"@10");
  test.assertEqual(remain.size,4);
  test.assertTrue(subCmd1.hasValue());
  test.assertEqual(new list(argList[12..]),remain);
}

// short option interrupted with bad flag value
proc testOptRangeInterruptBadFlag(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","-f","-p","thirty","-t","two"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],
                                   numArgs=1..);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],
                                   numArgs=1);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],
                                   numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short bool flag test
proc testBoolFlag(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"]);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}

// a long bool flag test
proc testBoolLongFlag(test: borrowed Test) throws {
  var argList = ["progName","--boolVal"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"]);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}

// a long bool flag test with default value and no-option
proc testBoolLongNoFlag(test: borrowed Test) throws {
  var argList = ["progName","--no-boolVal"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                defaultValue=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertFalse(myBoolArg.valueAsBool());
}

// combine a subcommand and boolean flag
proc testSubCommandAndBoolLongFlag(test: borrowed Test) throws {
  var argList = ["progName","subCmd","--no-boolVal"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCmd");
  //make sure no value currently exists
  test.assertFalse(mySubCmd1.hasValue());
  //parse the options
  var remain = parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(mySubCmd1.hasValue());
  //ensure the value passed is correct
  test.assertEqual(remain, new list(argList[2..]));

  var subParser = new argumentParser();
  var myBoolArg = subParser.addFlag(name="BoolFlag",
                              opts=["-n","--boolVal"],
                              defaultValue=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  subParser.parseArgs(remain.toArray());
  test.assertTrue(myBoolArg.hasValue());
  test.assertFalse(myBoolArg.valueAsBool());
}

// combine a parent argument, subcommand and boolean flag
proc testParentCommandSubCommandAndBoolLongFlag(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty", "subCmd","--no-boolVal"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                  opts=["-n","--stringVal1"],
                                  numArgs=1..);
  var mySubCmd1 = parser.addSubCommand(cmd="subCmd");
  //make sure no value currently exists
  test.assertFalse(mySubCmd1.hasValue());
  test.assertFalse(myStrArg1.hasValue());
  //parse the options
  var remain = parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(mySubCmd1.hasValue());
  test.assertTrue(myStrArg1.hasValue());
  //ensure the value passed is correct
  test.assertEqual(remain, new list(argList[4..]));
  test.assertEqual(myStrArg1.value(), "twenty");

  var subParser = new argumentParser();
  var myBoolArg = subParser.addFlag(name="BoolFlag",
                              opts=["-n","--boolVal"],
                              defaultValue=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  subParser.parseArgs(remain.toArray());
  test.assertTrue(myBoolArg.hasValue());
  test.assertFalse(myBoolArg.valueAsBool());
}

// a short bool flag test with =
proc testBoolEqFlag(test: borrowed Test) throws {
  var argList = ["progName","-n=true"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=0..1,
                                flagInversion=false,
                                defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}

// a short bool flag test with =, or value required
proc testBoolEqForceOneFlag(test: borrowed Test) throws {
  var argList = ["progName","-n=true"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=1,
                                flagInversion=false,
                                defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}

// a short bool flag test with value required
proc testBoolForceOneFlag(test: borrowed Test) throws {
  var argList = ["progName","-n","true"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=1,
                                flagInversion=false,
                                defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}


// a short bool flag test with optional value not provided
proc testBoolZeroToOneFlagNoVal(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=0..1,
                                flagInversion=false,
                                defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}

// a short bool flag test with value required but none provided
proc testBoolForceOneFlagNoVal(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=1,
                                flagInversion=false,
                                defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short bool flag test with too many values
proc testBoolForceOneFlagExtraGiven(test: borrowed Test) throws {
  var argList = ["progName","-n","true","false"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=1,
                                flagInversion=false,
                                defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short bool flag test with too many values
proc testBoolRangeFlagExtraGiven(test: borrowed Test) throws {
  var argList = ["progName","-n","true","false"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=0..1,
                                flagInversion=false,
                                defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList[1..]);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// attempt to specify too many values for a flag
proc testTryMakeBadFlagRange(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  try {
    var myNewArg = parser.addFlag(name="BoolOpt",
                                    opts=["--name","-n"],
                                    numArgs=0..2,
                                    required=false,
                                    defaultValue=none);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// attempt to specify nonsense flag
proc testTryMakeNonSenseFlag(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  try {
    var myNewArg = parser.addFlag(name="BoolOpt",
                                    opts=["--name","-n"],
                                    flagInversion=false,
                                    required=true,
                                    defaultValue=true);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// attempt to specify nonsense flag
proc testTryMakeNonSenseFlagRange(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  try {
    var myNewArg = parser.addFlag(name="BoolOpt",
                                    opts=["--name","-n"],
                                    numArgs=0..1,
                                    required=true,
                                    defaultValue=true);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// attempt to specify nonsense flag
proc testTryMakeNonSenseFlagFixed(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  try {
    var myNewArg = parser.addFlag(name="BoolOpt",
                                    opts=["--name","-n"],
                                    numArgs=1,
                                    required=true,
                                    defaultValue=true);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short bool flag test with default value assigned
proc testBoolZeroToOneFlagNoValDefaultTrue(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=0..1,
                                flagInversion=false,
                                defaultValue=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}

// a short bool flag test with default value assigned
proc testBoolZeroToOneFlagNoValDefaultFalse(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=0..1,
                                flagInversion=false,
                                defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertFalse(myBoolArg.valueAsBool());
}

// a short bool flag test with default value assigned
proc testBoolOneFlagNoValDefaultTrue(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=1,
                                flagInversion=false,
                                defaultValue=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}

// a short bool flag test with default value assigned
proc testBoolOneFlagNoValDefaultFalse(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-n","--boolVal"],
                                 numArgs=1,
                                 flagInversion=false,
                                 defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertFalse(myBoolArg.valueAsBool());
}

// a short bool flag test with default value assigned
proc testBoolZeroFlagNoValDefaultTrue(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=1,
                                flagInversion=false,
                                defaultValue=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}

// a short bool flag test with default value assigned
proc testBoolZeroFlagNoValDefaultFalse(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=1,
                                flagInversion=false,
                                defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertFalse(myBoolArg.valueAsBool());
}

// a short bool flag specified multiple times
proc testBoolMultipleEntryValues(test: borrowed Test) throws {
  var argList = ["progName","-n","true","-n=false"];
  var parser = new argumentParser();
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                opts=["-n","--boolVal"],
                                numArgs=1,
                                flagInversion=false);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertFalse(myBoolArg.valueAsBool());
  test.assertEqual((new list(myBoolArg.values())).size, 1);
}

// TODO: SPLIT THIS INTO MULTIPLE FILES BY FEATURE

UnitTest.main();
