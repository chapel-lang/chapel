use ArgumentParser;
use IO;
use List;
use UnitTest;



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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"twenty");
}

// a short string opt with single value with = and extra # values supplied
proc testSingleStringShortOptEqualsExtra(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// test to catch undefined args entered at beginning of cmd string
proc testBadArgsAtFront(test: borrowed Test) throws {
  var argList = ["progName","badArg1","--BadFlag","-n=twenty"];
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  parser.parseArgs(argList);
  //make sure no value was captured
  test.assertFalse(myStrArg.hasValue());
}

// a short string opt with single required value and no values supplied
// required value not supplied
proc testOptSingleStringShortReqOptNoOpts(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1,
                                  required=true);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"twenty");
}

// a parser with no arguments defined, gets no arguments
proc testNoOptsDefined(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  parser.parseArgs(argList);
  test.assertTrue(true);
}

// a parser with no arguments defined, gets 1 argument
proc testNoOptsDefinedUnknownOptGiven(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty"];
  var parser = new argumentParser(exitOnError=false);
  //parse the options
  try {
    parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
  test.assertFalse(myStrArg.hasValue());
}

// a short string opt with 1..2 range and opt, but no value supplied
proc testOptRangeStringShortOptPresentNoVal(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..2,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=0..1,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=0..1,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.values()), new list(argList[2..]));
}

// test a short string option with fixed range of numArgs more than 1,
// fewer than min # supplied
proc testMultiStringShortOptVarLessThanMin(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty"];
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3..5,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
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
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
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
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
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
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
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
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
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
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
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
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
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
    parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
                                   defaultValue=["1","2"],
                                   numArgs=1..2,
                                   required=false);

  //make sure no value currently exists
  test.assertFalse(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertFalse(myStrArg3.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                   defaultValue=["one","two"],
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
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myStrArg1.hasValue());
  test.assertFalse(myStrArg2.hasValue());
  test.assertTrue(myStrArg3.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg1.values()), new list(["one","two"]));
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
  parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
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
    parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(mySubCmd1.value(), argList[1]);
}

// use an option and then a subcommand
proc testOptionPlusSubCommand(test: borrowed Test) throws {
  var argList = ["progName","-n","20","subCommand1"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var myStrArg1 = parser.addOption(name="StringOpt",
                                   opts=["-n","--strArg"],
                                   numArgs=1);
  parser.parseArgs(argList);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(mySubCmd1.value(), argList[3]);
  test.assertTrue(myStrArg1.hasValue());
  test.assertEqual(myStrArg1.value(), "20");
}


// add a subcommand with sub options
proc testAddSubCommandSubOptions(test: borrowed Test) throws {
  var argList = ["progName","subCommand1","-n","20"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  parser.parseArgs(argList);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(mySubCmd1.values()), new list(argList[1..]));
}

// add a subcommand and argument, but don't use subcommand
proc testAddArgAndSubCommandOnlyArgUsed(test: borrowed Test) throws {
  var argList = ["progName","-n","20"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var myStrArg1 = parser.addOption(name="StringOpt",
                                   opts=["-n","--strArg"],
                                   numArgs=1);
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
  test.assertFalse(mySubCmd1.hasValue());
  test.assertFalse(myStrArg1.hasValue());
}

// add two subcommands and use first
proc testAddTwoSubCommandUseFirst(test: borrowed Test) throws {
  var argList = ["progName","subCommand1"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  parser.parseArgs(argList);
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(mySubCmd1.value(), argList[1]);
  test.assertFalse(mySubCmd2.hasValue());
}

// add two subcommands and use second
proc testAddTwoSubCommandUseSecond(test: borrowed Test) throws {
  var argList = ["progName","subCommand2"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  parser.parseArgs(argList);
  test.assertFalse(mySubCmd1.hasValue());
  test.assertTrue(mySubCmd2.hasValue());
  test.assertEqual(mySubCmd2.value(), argList[1]);
}

// add two subcommands and use none
proc testAddTwoSubCommandUseNone(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  parser.parseArgs(argList);
  test.assertFalse(mySubCmd1.hasValue());
  test.assertFalse(mySubCmd2.hasValue());
}

// add two subcommands and use an undefined command
proc testAddTwoSubCommandUseUndefined(test: borrowed Test) throws {
  var argList = ["progName","subCommandNone"];
  var parser = new argumentParser(exitOnError=false);
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var mySubCmd1 = parser.addSubCommand(cmd="subCommand1");
  var mySubCmd2 = parser.addSubCommand(cmd="subCommand2");
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);

  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
  test.assertTrue(myStrArg.hasValue());
  test.assertEqual(myStrArg.value(), "four");
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(mySubCmd1.values()), new list(argList[3..]));
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
  parser.parseArgs(argList);
  test.assertTrue(myStrArg.hasValue());
  test.assertEqual(myStrArg.value(), "four");
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(mySubCmd1.values()), new list(argList[3..]));
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
  parser.parseArgs(argList);
  test.assertTrue(myStrArg.hasValue());
  test.assertEqual(myStrArg.value(), "four");
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(mySubCmd1.values()), new list(argList[3..]));
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
  parser.parseArgs(argList);
  test.assertTrue(myStrArg.hasValue());
  test.assertEqual(myStrArg.value(), "four");
  test.assertTrue(mySubCmd1.hasValue());
  test.assertEqual(new list(mySubCmd1.values()), new list(argList[3..]));
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
  parser.parseArgs(argList);
  test.assertTrue(strArg.hasValue());
  test.assertEqual(new list(strArg.values()),
                   new list(["w","a","y","d","t","a?"]));
  test.assertEqual(new list(typArg.values()), new list(["tea","time"]));
  test.assertEqual(confArg.value(),"@10");
  test.assertTrue(subCmd1.hasValue());
  test.assertEqual(new list(subCmd1.values()), new list(argList[11..]));
}

// short option interrupted with bad flag value
// upon review this shouldn't be an error, we would just consume the -f
// and it would be on the user/dev to sort out the bad value, if it truly
// is a bad value.
proc testOptRangeMixedWithLeadingDash(test: borrowed Test) throws {
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
  parser.parseArgs(argList);
  test.assertTrue(myStrArg1.hasValue());
  test.assertTrue(myStrArg2.hasValue());
  test.assertTrue(myStrArg3.hasValue());

  test.assertEqual(new list(myStrArg1.values()), new list(argList[2..3]));
  test.assertEqual(myStrArg2.value(), argList[5]);
  test.assertEqual(myStrArg3.value(), argList[7]);

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
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
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
                                 defaultValue=true,
                                 flagInversion=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(mySubCmd1.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(mySubCmd1.values()), new list(argList[1..]));

  var subParser = new argumentParser();
  var myBoolArg = subParser.addFlag(name="BoolFlag",
                                    opts=["-n","--boolVal"],
                                    defaultValue=true,
                                    flagInversion=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  subParser.parseArgs((new list(mySubCmd1.values())).toArray());
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
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(mySubCmd1.hasValue());
  test.assertTrue(myStrArg1.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(mySubCmd1.values()), new list(argList[3..]));
  test.assertEqual(myStrArg1.value(), "twenty");

  var subParser = new argumentParser();
  var myBoolArg = subParser.addFlag(name="BoolFlag",
                                    opts=["-n","--boolVal"],
                                    defaultValue=true,
                                    flagInversion=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  subParser.parseArgs((new list(mySubCmd1.values())).toArray());
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
                                 defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                 defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                 defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                 defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertTrue(myBoolArg.valueAsBool());
}

// a short bool flag test with value required but none provided
proc testBoolForceOneFlagNoVal(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser(exitOnError=false);
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-n","--boolVal"],
                                 numArgs=1,
                                 defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-n","--boolVal"],
                                 numArgs=1,
                                 defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
  var parser = new argumentParser(exitOnError=false);
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-n","--boolVal"],
                                 numArgs=0..1,
                                 defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
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
                                  flagInversion=true,
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
                                  defaultValue=true,
                                  flagInversion=true);
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
                                  defaultValue=true,
                                  flagInversion=true);
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
                                 defaultValue=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                 defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                 defaultValue=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                 defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                 defaultValue=true);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                 defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
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
                                 numArgs=1);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  //ensure the value passed is correct
  test.assertFalse(myBoolArg.valueAsBool());
  test.assertEqual((new list(myBoolArg.values())).size, 1);
}

// a short bool flag with an unrecognized value
proc testFlagBadBoolValue(test: borrowed Test) throws {
  var argList = ["progName","-n","tru"];
  var parser = new argumentParser(exitOnError=false);
  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-n","--boolVal"],
                                 numArgs=1);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
  } catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a short bool flag with too many values defined
proc testFlagBadBoolNumArgs(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser();
  try {
    var myBoolArg = parser.addFlag(name="BoolFlag",
                                   opts=["-n","--boolVal"],
                                   numArgs=2);
  } catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// check expected output from helper string to bool method
proc unitTestStringToBool(test: borrowed Test) throws {
  var trueStrVals = ["true","1","yes"," true ", " yes ", " 1 "];
  var falseStrVals =  ["no", "0", "false", " no ", " 0 ", " false "];
  var badStrVals = ["truf", "not", "10", "01", "2", "bad", "good"];

  var rtn:bool;

  for tVal in trueStrVals {
    test.assertTrue(_convertStringToBool(tVal, rtn));
    test.assertTrue(rtn);
  }

  for fVal in falseStrVals {
    test.assertTrue(_convertStringToBool(fVal, rtn));
    test.assertFalse(rtn);
  }
  rtn = false;
  for bVal in badStrVals {
    test.assertFalse(_convertStringToBool(bVal, rtn));
    test.assertFalse(rtn);
  }
}

// single positional argument test
proc testSinglePositionalArgument(test: borrowed Test) throws {
  var argList = ["progName","myFileName"];
  var parser = new argumentParser();
  var myPosArg = parser.addArgument(name="FileName");
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myPosArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myPosArg.value(),argList[1]);
  test.assertEqual((new list(myPosArg.values())).size, 1);
}

// single positional argument test with default value
proc testPositionalArgumentDefault(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myPosArg = parser.addArgument(name="FileName",
                                    defaultValue=".",
                                    numArgs=0..1);
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myPosArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myPosArg.value(),".");
  test.assertEqual((new list(myPosArg.values())).size, 1);
}

// single positional argument test with no default value
proc testPositionalArgumentOptionalMissing(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myPosArg = parser.addArgument(name="FileName",
                                    defaultValue=none,
                                    numArgs=0..1);
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we still don't have a value
  test.assertFalse(myPosArg.hasValue());
}

// multiple value positional argument test
proc testPositionalArgumentRangeOneOrMore(test: borrowed Test) throws {
  var argList = ["progName","file1","file2","file3","file4"];
  var parser = new argumentParser();
  var myPosArg = parser.addArgument(name="FileNames",
                                    numArgs=1..);
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myPosArg.hasValue());
  //ensure the value passed is correct
  var valList = new list(myPosArg.values());
  test.assertEqual(valList,new list(argList[1..]));
  test.assertEqual(valList.size, 4);
}

// too many values for positional argument
proc testPositionalArgumentTooManyVals(test: borrowed Test) throws {
  var argList = ["progName","file1","file2","file3","file4"];
  var parser = new argumentParser(exitOnError=false);
  var myPosArg = parser.addArgument(name="FileNames",
                                    numArgs=1..2,
                                    defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
  } catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// not enough values for positional argument
proc testPositionalArgumentNotEnoughVals(test: borrowed Test) throws {
  var argList = ["progName","file1","file2","file3","file4"];
  var parser = new argumentParser(exitOnError=false);
  var myPosArg = parser.addArgument(name="FileNames",
                                    numArgs=5..7,
                                    defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
  } catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// no values for positional argument
proc testPositionalArgumentNoVals(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser(exitOnError=false);
  var myPosArg = parser.addArgument(name="FileNames",
                                    numArgs=1..);
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  //parse the options
  try {
    parser.parseArgs(argList);
  } catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// multiple value positional arguments with range before other positional
proc testPositionalArgumentRangeBeforePos(test: borrowed Test) throws {
  var argList = ["progName","file1","file2","file3","file4"];
  var parser = new argumentParser();
  var myPosArg = parser.addArgument(name="FileNames",
                                    numArgs=1..);
  try {
    var myPosArg = parser.addArgument(name="otherArg");
  } catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);
}

// a mix of positionals, bools, and options
proc testMixPosBoolOptWithValues(test: borrowed Test) throws {
  var argList = ["progName","myFile1","myFile2","myFile3","-b","true", "-n",
                 "myStrOpt1", "myStrOpt2", "myStrOpt3"];
  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"],
                                 numArgs=1);
  var myPosArg = parser.addArgument(name="FileNames",
                                    numArgs=1..);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..3,
                                  required=false,
                                  defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  test.assertTrue(myPosArg.hasValue());
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  var posList = new list(myPosArg.values());
  var optList = new list(myStrArg.values());
  test.assertTrue(myBoolArg.valueAsBool());
  test.assertEqual(posList, new list(argList[1..3]));
  test.assertEqual(optList, new list(argList[7..9]));
}

// a mix of positionals, bools, and options with fixed # values expected
proc testMixPosBoolOptWithValuesFixed(test: borrowed Test) throws {
  var argList = ["progName","myFile1","myFile2","myFile3","-b","true", "-n",
                 "myStrOpt1", "myStrOpt2", "myStrOpt3"];
  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"],
                                 numArgs=1);
  var myPosArg = parser.addArgument(name="FileNames",
                                    numArgs=3);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3,
                                  required=false,
                                  defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  test.assertTrue(myPosArg.hasValue());
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  var posList = new list(myPosArg.values());
  var optList = new list(myStrArg.values());
  test.assertTrue(myBoolArg.valueAsBool());
  test.assertEqual(posList, new list(argList[1..3]));
  test.assertEqual(optList, new list(argList[7..9]));
}

// a mix of positionals, bools, and options with fixed # values expected
// and positional argument after the boolean flag
proc testMixPosBoolOptWithValuesFixedPosAfterBool(test: borrowed Test) throws {
  var argList = ["progName","-n","myStrOpt1","myStrOpt2","myStrOpt3",
                 "-b","myFile1","subCmd1"];
  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"]);
  var myPosArg = parser.addArgument(name="FileNames");
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3,
                                  required=false,
                                  defaultValue=none);
  var subCmd1 = parser.addSubCommand(cmd="subCmd1");

  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(myStrArg.hasValue());
  test.assertFalse(subCmd1.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  test.assertTrue(myPosArg.hasValue());
  test.assertTrue(myStrArg.hasValue());
  test.assertTrue(subCmd1.hasValue());
  //ensure the value passed is correct
  var optList = new list(myStrArg.values());
  test.assertTrue(myBoolArg.valueAsBool());
  test.assertEqual(myPosArg.value(), argList[6]);
  test.assertEqual(optList, new list(argList[2..4]));
  test.assertEqual(subCmd1.value(),"subCmd1");
}

// a mix of positionals, bools, and options with fixed # values expected
// and positional argument intermixed in the input
proc testMixPosBoolOptWithValuesFixedPosInterMixed(test: borrowed Test) throws {
  var argList = ["progName","p1","-n","myStrOpt1","myStrOpt2","myStrOpt3","p2",
                 "-b","myFile1","subCmd1"];
  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"]);
  var myPosArg0 = parser.addArgument(name="pos0");
  var myPosArg1 = parser.addArgument(name="pos1");
  var myPosArg2 = parser.addArgument(name="FileName");
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3,
                                  required=false,
                                  defaultValue=none);
  var subCmd1 = parser.addSubCommand(cmd="subCmd1");

  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg0.hasValue());
  test.assertFalse(myPosArg1.hasValue());
  test.assertFalse(myPosArg2.hasValue());
  test.assertFalse(myStrArg.hasValue());
  test.assertFalse(subCmd1.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  test.assertTrue(myPosArg0.hasValue());
  test.assertTrue(myPosArg1.hasValue());
  test.assertTrue(myPosArg2.hasValue());
  test.assertTrue(myStrArg.hasValue());
  test.assertTrue(subCmd1.hasValue());
  //ensure the value passed is correct
  var optList = new list(myStrArg.values());
  test.assertTrue(myBoolArg.valueAsBool());
  test.assertEqual(myPosArg0.value(), argList[1]);
  test.assertEqual(myPosArg1.value(), argList[6]);
  test.assertEqual(myPosArg2.value(), argList[8]);
  test.assertEqual(optList, new list(argList[3..5]));
  test.assertEqual(subCmd1.value(),"subCmd1");
}

// a mix of positionals, bools, and options with fixed # values expected
// and positional argument intermixed in the input
proc testMixPosBoolOptWithValuesPosInterMixedRange(test: borrowed Test) throws {
  var argList = ["progName","p1","-n","myStrOpt1","myStrOpt2","myStrOpt3","p2",
                 "-b","subCmd1"];
  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"]);
  var myPosArg0 = parser.addArgument(name="pos0");
  var myPosArg1 = parser.addArgument(name="pos1");
  var myPosArg2 = parser.addArgument(name="FileName",
                                     numArgs=0..1,
                                     defaultValue=none);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3,
                                  required=false,
                                  defaultValue=none);
  var subCmd1 = parser.addSubCommand(cmd="subCmd1");

  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg0.hasValue());
  test.assertFalse(myPosArg1.hasValue());
  test.assertFalse(myPosArg2.hasValue());
  test.assertFalse(myStrArg.hasValue());
  test.assertFalse(subCmd1.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  test.assertTrue(myPosArg0.hasValue());
  test.assertTrue(myPosArg1.hasValue());
  test.assertFalse(myPosArg2.hasValue());
  test.assertTrue(myStrArg.hasValue());
  test.assertTrue(subCmd1.hasValue());
  //ensure the value passed is correct
  var optList = new list(myStrArg.values());
  test.assertTrue(myBoolArg.valueAsBool());
  test.assertEqual(myPosArg0.value(), argList[1]);
  test.assertEqual(myPosArg1.value(), argList[6]);
  test.assertEqual(optList, new list(argList[3..5]));
  test.assertEqual(subCmd1.value(),"subCmd1");
}

// a mix of positionals, bools, and options with fixed # values expected
// and positional argument intermixed in the input, with several values
// expected for last positional argument
proc testMixPosBoolOptPosInterMixedRangeWithVals(test: borrowed Test) throws {
  var argList = ["progName","p1","-n","myStrOpt1","myStrOpt2","myStrOpt3","p2",
                 "myFile0","-b","myFile1","myFile2","subCmd1"];
  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"]);
  var myPosArg0 = parser.addArgument(name="pos0");
  var myPosArg1 = parser.addArgument(name="pos1");
  var myPosArg2 = parser.addArgument(name="FileName",
                                     numArgs=0..,
                                     defaultValue=none);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3,
                                  required=false,
                                  defaultValue=none);
  var subCmd1 = parser.addSubCommand(cmd="subCmd1");

  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg0.hasValue());
  test.assertFalse(myPosArg1.hasValue());
  test.assertFalse(myPosArg2.hasValue());
  test.assertFalse(myStrArg.hasValue());
  test.assertFalse(subCmd1.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  test.assertTrue(myPosArg0.hasValue());
  test.assertTrue(myPosArg1.hasValue());
  test.assertTrue(myPosArg2.hasValue());
  test.assertTrue(myStrArg.hasValue());
  test.assertTrue(subCmd1.hasValue());
  //ensure the value passed is correct
  var optList = new list(myStrArg.values());
  var posList = new list(myPosArg2.values());
  test.assertTrue(myBoolArg.valueAsBool());
  test.assertEqual(myPosArg0.value(), argList[1]);
  test.assertEqual(myPosArg1.value(), argList[6]);
  test.assertEqual(optList, new list(argList[3..5]));
  test.assertEqual(posList, new list(["myFile0","myFile1","myFile2"]));
  test.assertEqual(subCmd1.value(),"subCmd1");
}

// a mix of positionals, bools, and options some with fixed # values expected
// and positional argument intermixed in the input, with several values
// possible for last positional argument, but none provided
proc testMixPosBoolOptPosInterMixedRangeDefVals(test: borrowed Test) throws {
  var argList = ["progName","p1","-n","myStrOpt1","myStrOpt2","myStrOpt3","p2",
                "-b","subCmd1"];
  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"]);
  var myPosArg0 = parser.addArgument(name="pos0");
  var myPosArg1 = parser.addArgument(name="pos1");
  var myPosArg2 = parser.addArgument(name="FileName",
                                     numArgs=0..,
                                     defaultValue=["1","2","3"]);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3,
                                  required=false,
                                  defaultValue=none);
  var subCmd1 = parser.addSubCommand(cmd="subCmd1");

  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg0.hasValue());
  test.assertFalse(myPosArg1.hasValue());
  test.assertFalse(myPosArg2.hasValue());
  test.assertFalse(myStrArg.hasValue());
  test.assertFalse(subCmd1.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  test.assertTrue(myPosArg0.hasValue());
  test.assertTrue(myPosArg1.hasValue());
  test.assertTrue(myPosArg2.hasValue());
  test.assertTrue(myStrArg.hasValue());
  test.assertTrue(subCmd1.hasValue());
  //ensure the value passed is correct
  var optList = new list(myStrArg.values());
  var posList = new list(myPosArg2.values());
  test.assertTrue(myBoolArg.valueAsBool());
  test.assertEqual(myPosArg0.value(), argList[1]);
  test.assertEqual(myPosArg1.value(), argList[6]);
  test.assertEqual(posList, new list(["1","2","3"]));
  test.assertEqual(optList, new list(argList[3..5]));
  test.assertEqual(subCmd1.value(),"subCmd1");
}

// a mix of positionals, bools, and options with fixed # values expected
// and positional argument intermixed in the input, with several values
// expected for last positional argument
proc testMixPosBoolOptPosInterMixedRangeNoVals(test: borrowed Test) throws {
  var argList = ["progName","p1","-n","myStrOpt1","myStrOpt2","myStrOpt3","p2",
                "-b","subCmd1"];
  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"]);
  var myPosArg0 = parser.addArgument(name="pos0");
  var myPosArg1 = parser.addArgument(name="pos1");
  var myPosArg2 = parser.addArgument(name="FileName",
                                     numArgs=0..,
                                     defaultValue=none);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=3,
                                  required=false,
                                  defaultValue=none);
  var subCmd1 = parser.addSubCommand(cmd="subCmd1");

  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg0.hasValue());
  test.assertFalse(myPosArg1.hasValue());
  test.assertFalse(myPosArg2.hasValue());
  test.assertFalse(myStrArg.hasValue());
  test.assertFalse(subCmd1.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  test.assertTrue(myPosArg0.hasValue());
  test.assertTrue(myPosArg1.hasValue());
  test.assertFalse(myPosArg2.hasValue());
  test.assertTrue(myStrArg.hasValue());
  test.assertTrue(subCmd1.hasValue());
  //ensure the value passed is correct
  var optList = new list(myStrArg.values());
  test.assertTrue(myBoolArg.valueAsBool());
  test.assertEqual(myPosArg0.value(), argList[1]);
  test.assertEqual(myPosArg1.value(), argList[6]);
  test.assertEqual(optList, new list(argList[3..5]));
  test.assertEqual(subCmd1.value(),"subCmd1");
}

// a mix of positionals, bools, and options with ranges of values and subcommand
// with optional positional, bools, and options.
proc testMixPosBoolOptWithValuesSubCommand(test: borrowed Test) throws {
  var argList = ["progName","myFile1","myFile2","myFile3","-b","true", "-n",
                 "myStrOpt1", "myStrOpt2", "myStrOpt3","subCmd1","-b","false",
                 "--stringVal=s1","--stringVal=s2","-n","s3"];

  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"],
                                 numArgs=0..1,
                                 defaultValue=none);
  var myPosArg = parser.addArgument(name="FileNames",
                                    numArgs=1..);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..10,
                                  required=false,
                                  defaultValue=none);
  var subCmd1 = parser.addSubCommand("subCmd1");
  // setup the subcommand parser
  var subParser = new argumentParser();
  var subCmdBoolArg = subParser.addFlag(name="BoolFlag",
                                        opts=["-b","--boolVal"],
                                        numArgs=0..1,
                                        defaultValue=none);
  var subCmdPosArg = subParser.addArgument(name="FileNames",
                                           numArgs=0..,
                                           defaultValue=(["f1","f2"]));
  var subCmdStrArg = subParser.addOption(name="StringOpt",
                                         opts=["-n","--stringVal"],
                                         numArgs=1..10,
                                         required=false,
                                         defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(myStrArg.hasValue());
  test.assertFalse(subCmd1.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myBoolArg.hasValue());
  test.assertTrue(myPosArg.hasValue());
  test.assertTrue(myStrArg.hasValue());
  test.assertTrue(subCmd1.hasValue());
  //ensure the value passed is correct
  var posList = new list(myPosArg.values());
  var optList = new list(myStrArg.values());
  test.assertTrue(myBoolArg.valueAsBool());
  test.assertEqual(posList, new list(argList[1..3]));
  test.assertEqual(optList, new list(argList[7..9]));
  test.assertEqual(new list(subCmd1.values()), new list(argList[10..]));

  //make sure no sub-values currently exist
  test.assertFalse(subCmdBoolArg.hasValue());
  test.assertFalse(subCmdPosArg.hasValue());
  test.assertFalse(subCmdStrArg.hasValue());
  //parse the remaining args
  subParser.parseArgs((new list(subCmd1.values())).toArray());
  //make sure we now have a value
  test.assertTrue(subCmdBoolArg.hasValue());
  test.assertTrue(subCmdPosArg.hasValue());
  test.assertTrue(subCmdStrArg.hasValue());
  //ensure the value passed is correct
  var subCmdPosList = new list(subCmdPosArg.values());
  var subCmdOptList = new list(subCmdStrArg.values());
  test.assertFalse(subCmdBoolArg.valueAsBool());
  test.assertEqual(subCmdPosList, new list(["f1","f2"]));
  test.assertEqual(subCmdOptList, new list(["s1","s2","s3"]));
}

// a mix of positionals, bools, and options with ranges of values and subcommand
// with optional positional, bools, and options.
proc testMixPosBoolOptWithValuesSubCommandLight(test: borrowed Test) throws {
  var argList = ["progName","subCmd1","-b=true","--stringVal", "s1","s2","s3"];

  var parser = new argumentParser();

  var myBoolArg = parser.addFlag(name="BoolFlag",
                                 opts=["-b","--boolVal"],
                                 numArgs=0..1,
                                 defaultValue=none);
  var myPosArg = parser.addArgument(name="FileNames",
                                      numArgs=0..);
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..10,
                                  required=false,
                                  defaultValue=none);
  var subCmd1 = parser.addSubCommand("subCmd1");
  // setup the subcommand parser
  var subParser = new argumentParser();
  var subCmdBoolArg = subParser.addFlag(name="BoolFlag",
                                        opts=["-b","--boolVal"],
                                        numArgs=0..1,
                                        defaultValue=none);
  var subCmdPosArg = subParser.addArgument(name="FileNames",
                                           numArgs=0..,
                                           defaultValue=(["f1","f2"]));
  var subCmdStrArg = subParser.addOption(name="StringOpt",
                                         opts=["-n","--stringVal"],
                                         numArgs=1..10,
                                         required=false,
                                         defaultValue=none);
  //make sure no value currently exists
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(myStrArg.hasValue());
  test.assertFalse(subCmd1.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertFalse(myBoolArg.hasValue());
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(myStrArg.hasValue());
  test.assertTrue(subCmd1.hasValue());
  //make sure no sub-values currently exist
  test.assertFalse(subCmdBoolArg.hasValue());
  test.assertFalse(subCmdPosArg.hasValue());
  test.assertFalse(subCmdStrArg.hasValue());
  //parse the remaining args
  subParser.parseArgs((new list(subCmd1.values())).toArray());
  //make sure we now have a value
  test.assertTrue(subCmdBoolArg.hasValue());
  test.assertTrue(subCmdPosArg.hasValue());
  test.assertTrue(subCmdStrArg.hasValue());
  //ensure the value passed is correct
  var subCmdPosList = new list(subCmdPosArg.values());
  var subCmdOptList = new list(subCmdStrArg.values());
  test.assertTrue(subCmdBoolArg.valueAsBool());
  test.assertEqual(subCmdPosList, new list(["f1","f2"]));
  test.assertEqual(subCmdOptList, new list(["s1","s2","s3"]));
}

// mockup of a Mason interface testing "new" options
proc testMockMasonNew(test: borrowed Test) throws {
  var argList = ["mason","new","--no-vcs","myPackage"];

  var parser = new argumentParser();
  var newCmd = parser.addSubCommand("new");
  var initCmd = parser.addSubCommand("init");
  var addCmd = parser.addSubCommand("add");
  var rmCmd = parser.addSubCommand("rm");
  var upCmd = parser.addSubCommand("update");
  var testCmd = parser.addSubCommand("test");
  var buildCmd = parser.addSubCommand("build");
  var runCmd = parser.addSubCommand("run");
  var searchCmd = parser.addSubCommand("search");
  var envCmd = parser.addSubCommand("env");
  var cleanCmd = parser.addSubCommand("clean");
  var docCmd = parser.addSubCommand("doc");
  var sysCmd = parser.addSubCommand("system");
  var extCmd = parser.addSubCommand("external");
  var pubCmd = parser.addSubCommand("publish");
  // var helpFlag = parser.addFlag(name="help",
  //                               opts=["-h","--help"],
  //                               defaultValue=false);
  var verFlag = parser.addFlag(name="version",
                               opts=["-V","--version"],
                               defaultValue=false);
  // setup the subcommand parsers (normally done in each sub-module)
  var newParser = new argumentParser();
  var initParser = new argumentParser();
  var addParser = new argumentParser();
  var rmParser = new argumentParser();
  var upParser = new argumentParser();
  var testParser = new argumentParser();
  var buildParser = new argumentParser();
  var runParser = new argumentParser();
  var searchParser = new argumentParser();
  var envParser = new argumentParser();
  var cleanParser = new argumentParser();
  var docParser = new argumentParser();
  var sysParser = new argumentParser();
  var extParser = new argumentParser();
  var pubParser = new argumentParser();

  // setup arguments for subcommand 'new'
  var newVCS = newParser.addFlag(name="vcs",
                                 opts=["--vcs"],
                                 defaultValue=true,
                                 flagInversion=true);
  var newShow = newParser.addFlag(name="show",
                                  opts=["--show"],
                                  defaultValue=false);
  var newLegalName = newParser.addOption(name="legalName",
                                    opts=["--name"]);
  var newName = newParser.addArgument(name="name");

  // var newHelpFlag = newParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  // setup arguments for subcommand 'add'
  var addExt = addParser.addFlag(name="external",
                                 opts=["--external"],
                                 defaultValue=false);
  var addSys = addParser.addFlag(name="system",
                                 opts=["--system"],
                                 defaultValue=false);
  var addPkg = addParser.addArgument(name="package");

  // var addHelpFlag = addParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertFalse(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertFalse(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertFalse(helpFlag.hasValue());
  test.assertFalse(verFlag.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertFalse(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertFalse(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertTrue(helpFlag.hasValue());
  test.assertTrue(verFlag.hasValue());
  // test.assertFalse(helpFlag.valueAsBool());
  test.assertFalse(verFlag.valueAsBool());
  test.assertFalse(newLegalName.hasValue());
  //parse the remaining args
  newParser.parseArgs((new list(newCmd.values())).toArray());
  //make sure we now have a value
  test.assertTrue(newVCS.hasValue());
  test.assertTrue(newName.hasValue());
  // test.assertTrue(newHelpFlag.hasValue());
  // test.assertFalse(newHelpFlag.valueAsBool());
  //ensure the value passed is correct
  test.assertFalse(newVCS.valueAsBool());
  test.assertEqual(newName.value(), "myPackage");
}


// mockup of a Mason interface testing "new" options
proc testMockMasonNewDiffName(test: borrowed Test) throws {
  var argList = ["mason","new","--no-vcs","--name","notMyPackage","myPackage"];

  var parser = new argumentParser();
  var newCmd = parser.addSubCommand("new");
  var initCmd = parser.addSubCommand("init");
  var addCmd = parser.addSubCommand("add");
  var rmCmd = parser.addSubCommand("rm");
  var upCmd = parser.addSubCommand("update");
  var testCmd = parser.addSubCommand("test");
  var buildCmd = parser.addSubCommand("build");
  var runCmd = parser.addSubCommand("run");
  var searchCmd = parser.addSubCommand("search");
  var envCmd = parser.addSubCommand("env");
  var cleanCmd = parser.addSubCommand("clean");
  var docCmd = parser.addSubCommand("doc");
  var sysCmd = parser.addSubCommand("system");
  var extCmd = parser.addSubCommand("external");
  var pubCmd = parser.addSubCommand("publish");
  // var helpFlag = parser.addFlag(name="help",
  //                               opts=["-h","--help"],
  //                               defaultValue=false);
  var verFlag = parser.addFlag(name="version",
                               opts=["-V","--version"],
                               defaultValue=false);
  // setup the subcommand parsers (normally done in each sub-module)
  var newParser = new argumentParser();
  var initParser = new argumentParser();
  var addParser = new argumentParser();
  var rmParser = new argumentParser();
  var upParser = new argumentParser();
  var testParser = new argumentParser();
  var buildParser = new argumentParser();
  var runParser = new argumentParser();
  var searchParser = new argumentParser();
  var envParser = new argumentParser();
  var cleanParser = new argumentParser();
  var docParser = new argumentParser();
  var sysParser = new argumentParser();
  var extParser = new argumentParser();
  var pubParser = new argumentParser();

  // setup arguments for subcommand 'new'
  var newVCS = newParser.addFlag(name="vcs",
                                 opts=["--vcs"],
                                 defaultValue=true,
                                 flagInversion=true);
  var newShow = newParser.addFlag(name="show",
                                  opts=["--show"],
                                  defaultValue=false);
  var newLegalName = newParser.addOption(name="legalName",
                                    opts=["--name"]);
  var newName = newParser.addArgument(name="name");

  // var newHelpFlag = newParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  // setup arguments for subcommand 'add'
  var addExt = addParser.addFlag(name="external",
                                 opts=["--external"],
                                 defaultValue=false);
  var addSys = addParser.addFlag(name="system",
                                 opts=["--system"],
                                 defaultValue=false);
  var addPkg = addParser.addArgument(name="package");

  // var addHelpFlag = addParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertFalse(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertFalse(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertFalse(helpFlag.hasValue());
  test.assertFalse(verFlag.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertFalse(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertFalse(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertTrue(helpFlag.hasValue());
  test.assertTrue(verFlag.hasValue());
  // test.assertFalse(helpFlag.valueAsBool());
  test.assertFalse(verFlag.valueAsBool());
  test.assertFalse(newLegalName.hasValue());
  //parse the remaining args
  newParser.parseArgs((new list(newCmd.values())).toArray());
  //make sure we now have a value
  test.assertTrue(newVCS.hasValue());
  test.assertTrue(newName.hasValue());
  test.assertTrue(newLegalName.hasValue());
  // test.assertTrue(newHelpFlag.hasValue());
  // test.assertFalse(newHelpFlag.valueAsBool());
  //ensure the value passed is correct
  test.assertFalse(newVCS.valueAsBool());
  test.assertEqual(newLegalName.value(),"notMyPackage");
  test.assertEqual(newName.value(), "myPackage");
}

// mockup of a Mason interface testing "new" options
proc testMockMasonNewTypical(test: borrowed Test) throws {
  var argList = ["mason","new","myPackage"];

  var parser = new argumentParser();
  var newCmd = parser.addSubCommand("new");
  var initCmd = parser.addSubCommand("init");
  var addCmd = parser.addSubCommand("add");
  var rmCmd = parser.addSubCommand("rm");
  var upCmd = parser.addSubCommand("update");
  var testCmd = parser.addSubCommand("test");
  var buildCmd = parser.addSubCommand("build");
  var runCmd = parser.addSubCommand("run");
  var searchCmd = parser.addSubCommand("search");
  var envCmd = parser.addSubCommand("env");
  var cleanCmd = parser.addSubCommand("clean");
  var docCmd = parser.addSubCommand("doc");
  var sysCmd = parser.addSubCommand("system");
  var extCmd = parser.addSubCommand("external");
  var pubCmd = parser.addSubCommand("publish");
  // var helpFlag = parser.addFlag(name="help",
  //                               opts=["-h","--help"],
  //                               defaultValue=false);
  var verFlag = parser.addFlag(name="version",
                               opts=["-V","--version"],
                               defaultValue=false);
  // setup the subcommand parsers (normally done in each sub-module)
  var newParser = new argumentParser();
  var initParser = new argumentParser();
  var addParser = new argumentParser();
  var rmParser = new argumentParser();
  var upParser = new argumentParser();
  var testParser = new argumentParser();
  var buildParser = new argumentParser();
  var runParser = new argumentParser();
  var searchParser = new argumentParser();
  var envParser = new argumentParser();
  var cleanParser = new argumentParser();
  var docParser = new argumentParser();
  var sysParser = new argumentParser();
  var extParser = new argumentParser();
  var pubParser = new argumentParser();

  // setup arguments for subcommand 'new'
  var newVCS = newParser.addFlag(name="vcs",
                                 opts=["--vcs"],
                                 defaultValue=true,
                                 flagInversion=true);
  var newShow = newParser.addFlag(name="show",
                                  opts=["--show"],
                                  defaultValue=false);
  var newLegalName = newParser.addOption(name="legalName",
                                         opts=["--name"]);
  var newName = newParser.addArgument(name="name");

  // var newHelpFlag = newParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  // setup arguments for subcommand 'add'
  var addExt = addParser.addFlag(name="external",
                                 opts=["--external"],
                                 defaultValue=false);
  var addSys = addParser.addFlag(name="system",
                                 opts=["--system"],
                                 defaultValue=false);
  var addPkg = addParser.addArgument(name="package");

  // var addHelpFlag = addParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertFalse(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertFalse(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertFalse(helpFlag.hasValue());
  test.assertFalse(verFlag.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertFalse(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertFalse(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertTrue(helpFlag.hasValue());
  test.assertTrue(verFlag.hasValue());
  // test.assertFalse(helpFlag.valueAsBool());
  test.assertFalse(verFlag.valueAsBool());
  test.assertFalse(newLegalName.hasValue());
  //parse the remaining args
  newParser.parseArgs((new list(newCmd.values())).toArray());
  //make sure we now have a value
  test.assertTrue(newVCS.hasValue());
  test.assertTrue(newName.hasValue());
  test.assertFalse(newLegalName.hasValue());
  // test.assertTrue(newHelpFlag.hasValue());
  // test.assertFalse(newHelpFlag.valueAsBool());
  //ensure the value passed is correct
  test.assertTrue(newVCS.valueAsBool());
  test.assertEqual(newName.value(), "myPackage");
}

// mockup of a Mason interface testing "add" options
proc testMockMasonAddExternal(test: borrowed Test) throws {
  var argList = ["mason","add","--external","externalPackage@0.2.3"];

  var parser = new argumentParser();
  var newCmd = parser.addSubCommand("new");
  var initCmd = parser.addSubCommand("init");
  var addCmd = parser.addSubCommand("add");
  var rmCmd = parser.addSubCommand("rm");
  var upCmd = parser.addSubCommand("update");
  var testCmd = parser.addSubCommand("test");
  var buildCmd = parser.addSubCommand("build");
  var runCmd = parser.addSubCommand("run");
  var searchCmd = parser.addSubCommand("search");
  var envCmd = parser.addSubCommand("env");
  var cleanCmd = parser.addSubCommand("clean");
  var docCmd = parser.addSubCommand("doc");
  var sysCmd = parser.addSubCommand("system");
  var extCmd = parser.addSubCommand("external");
  var pubCmd = parser.addSubCommand("publish");
  // var helpFlag = parser.addFlag(name="help",
  //                               opts=["-h","--help"],
  //                               defaultValue=false);
  var verFlag = parser.addFlag(name="version",
                               opts=["-V","--version"],
                               defaultValue=false);
  // setup the subcommand parsers (normally done in each sub-module)
  var newParser = new argumentParser();
  var initParser = new argumentParser();
  var addParser = new argumentParser();
  var rmParser = new argumentParser();
  var upParser = new argumentParser();
  var testParser = new argumentParser();
  var buildParser = new argumentParser();
  var runParser = new argumentParser();
  var searchParser = new argumentParser();
  var envParser = new argumentParser();
  var cleanParser = new argumentParser();
  var docParser = new argumentParser();
  var sysParser = new argumentParser();
  var extParser = new argumentParser();
  var pubParser = new argumentParser();

  // setup arguments for subcommand 'new'
  var newVCS = newParser.addFlag(name="vcs",
                                 opts=["--vcs"],
                                 defaultValue=true,
                                 flagInversion=true);
  var newShow = newParser.addFlag(name="show",
                                  opts=["--show"],
                                  defaultValue=false);
  var newLegalName = newParser.addOption(name="legalName",
                                         opts=["--name"]);
  var newName = newParser.addArgument(name="name");

  // var newHelpFlag = newParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  // setup arguments for subcommand 'add'
  var addExt = addParser.addFlag(name="external",
                                 opts=["--external"],
                                 defaultValue=false);
  var addSys = addParser.addFlag(name="system",
                                 opts=["--system"],
                                 defaultValue=false);
  var addPkg = addParser.addArgument(name="package");

  // var addHelpFlag = addParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertFalse(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertFalse(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertFalse(helpFlag.hasValue());
  test.assertFalse(verFlag.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertFalse(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertTrue(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertFalse(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertTrue(helpFlag.hasValue());
  test.assertTrue(verFlag.hasValue());
  // test.assertFalse(helpFlag.valueAsBool());
  test.assertFalse(verFlag.valueAsBool());
  test.assertFalse(addPkg.hasValue());
  test.assertFalse(addExt.hasValue());
  test.assertFalse(addSys.hasValue());
  //parse the remaining args
  addParser.parseArgs((new list(addCmd.values())).toArray());
  //make sure we now have a value
  test.assertTrue(addExt.hasValue());
  test.assertTrue(addPkg.hasValue());
  test.assertTrue(addSys.hasValue());
  // test.assertTrue(addHelpFlag.hasValue());
  // test.assertFalse(addHelpFlag.valueAsBool());
  //ensure the value passed is correct
  test.assertFalse(addSys.valueAsBool());
  test.assertTrue(addExt.valueAsBool());
  test.assertEqual(addPkg.value(), "externalPackage@0.2.3");
}

// mockup of a Mason interface testing "run" options
proc testMockMasonRun(test: borrowed Test) throws {
  var argList = ["mason","run","--build","--","--myConfigVar=true"];

  var parser = new argumentParser();
  var newCmd = parser.addSubCommand("new");
  var initCmd = parser.addSubCommand("init");
  var addCmd = parser.addSubCommand("add");
  var rmCmd = parser.addSubCommand("rm");
  var upCmd = parser.addSubCommand("update");
  var testCmd = parser.addSubCommand("test");
  var buildCmd = parser.addSubCommand("build");
  var runCmd = parser.addSubCommand("run");
  var searchCmd = parser.addSubCommand("search");
  var envCmd = parser.addSubCommand("env");
  var cleanCmd = parser.addSubCommand("clean");
  var docCmd = parser.addSubCommand("doc");
  var sysCmd = parser.addSubCommand("system");
  var extCmd = parser.addSubCommand("external");
  var pubCmd = parser.addSubCommand("publish");
  // var helpFlag = parser.addFlag(name="help",
  //                               opts=["-h","--help"],
  //                               defaultValue=false);
  var verFlag = parser.addFlag(name="version",
                               opts=["-V","--version"],
                               defaultValue=false);
  // setup the subcommand parsers (normally done in each sub-module)
  var newParser = new argumentParser();
  var initParser = new argumentParser();
  var addParser = new argumentParser();
  var rmParser = new argumentParser();
  var upParser = new argumentParser();
  var testParser = new argumentParser();
  var buildParser = new argumentParser();
  var runParser = new argumentParser();
  var searchParser = new argumentParser();
  var envParser = new argumentParser();
  var cleanParser = new argumentParser();
  var docParser = new argumentParser();
  var sysParser = new argumentParser();
  var extParser = new argumentParser();
  var pubParser = new argumentParser();

  // setup arguments for subcommand 'run'
  var runBuild = runParser.addFlag(name="build",
                                 opts=["--build"],
                                 defaultValue=false);
  var runShow = runParser.addFlag(name="show",
                                  opts=["--show"],
                                  defaultValue=false);
  var runExample = runParser.addOption(name="example",
                                       opts=["--example"],
                                       numArgs=0..);
  var runPassThrough = runParser.addPassThrough();

  // var runHelpFlag = runParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  // setup arguments for subcommand 'add'
  var addExt = addParser.addFlag(name="external",
                                 opts=["--external"],
                                 defaultValue=false);
  var addSys = addParser.addFlag(name="system",
                                 opts=["--system"],
                                 defaultValue=false);
  var addPkg = addParser.addArgument(name="package");

  // var addHelpFlag = addParser.addFlag(name="help",
  //                                     opts=["-h","--help"],
  //                                     defaultValue=false);
  //make sure no value currently exists
  test.assertFalse(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertFalse(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertFalse(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertFalse(helpFlag.hasValue());
  test.assertFalse(verFlag.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertFalse(newCmd.hasValue());
  test.assertFalse(initCmd.hasValue());
  test.assertFalse(addCmd.hasValue());
  test.assertFalse(rmCmd.hasValue());
  test.assertFalse(upCmd.hasValue());
  test.assertFalse(testCmd.hasValue());
  test.assertFalse(buildCmd.hasValue());
  test.assertTrue(runCmd.hasValue());
  test.assertFalse(searchCmd.hasValue());
  test.assertFalse(envCmd.hasValue());
  test.assertFalse(cleanCmd.hasValue());
  test.assertFalse(docCmd.hasValue());
  test.assertFalse(sysCmd.hasValue());
  test.assertFalse(extCmd.hasValue());
  test.assertFalse(pubCmd.hasValue());
  // test.assertTrue(helpFlag.hasValue());
  test.assertTrue(verFlag.hasValue());
  // test.assertFalse(helpFlag.valueAsBool());
  test.assertFalse(verFlag.valueAsBool());
  test.assertFalse(runBuild.hasValue());
  test.assertFalse(runShow.hasValue());
  test.assertFalse(runExample.hasValue());
  test.assertFalse(runPassThrough.hasValue());
  //parse the remaining args
  runParser.parseArgs((new list(runCmd.values())).toArray());
  //make sure we now have a value
  test.assertTrue(runBuild.hasValue());
  test.assertTrue(runShow.hasValue());
  test.assertFalse(runExample.hasValue());
  test.assertTrue(runPassThrough.hasValue());
  // test.assertTrue(runHelpFlag.hasValue());
  // test.assertFalse(runHelpFlag.valueAsBool());
  //ensure the value passed is correct
  test.assertFalse(runShow.valueAsBool());
  test.assertTrue(runBuild.valueAsBool());
  test.assertEqual(new list(runPassThrough.values()), new list(argList[4..]));
}

// test passthrough arg with nothing else
proc testPassthrough(test: borrowed Test) throws {
  var argList = ["progName","--","--myVar","setting"];
  var parser = new argumentParser();
  var myPosArg = parser.addArgument(name="FileName",
                                    defaultValue=none,
                                    numArgs=0..1);
  var passThrough = parser.addPassThrough();
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(passThrough.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we have a value
  test.assertFalse(myPosArg.hasValue());
  test.assertTrue(passThrough.hasValue());
  test.assertEqual(new list(passThrough.values()), new list(argList[2..]));
}

// test passthrough arg with positional first
proc testPassthroughWithPositional(test: borrowed Test) throws {
  var argList = ["progName","myFile","--","--myVar","setting"];
  var parser = new argumentParser();
  var myPosArg = parser.addArgument(name="FileName",
                                    defaultValue=none,
                                    numArgs=0..1);
  var passThrough = parser.addPassThrough();
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(passThrough.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we have a value
  test.assertTrue(myPosArg.hasValue());
  test.assertTrue(passThrough.hasValue());
  test.assertEqual(myPosArg.value(),argList[1]);
  test.assertEqual(new list(passThrough.values()), new list(argList[3..]));
}

// test passthrough arg with positional first and no values
proc testPassthroughWithPositionalNoVals(test: borrowed Test) throws {
  var argList = ["progName"];
  var parser = new argumentParser();
  var myPosArg = parser.addArgument(name="FileName",
                                    defaultValue=none,
                                    numArgs=0..1);
  var passThrough = parser.addPassThrough();
  //make sure no value currently exists
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(passThrough.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we have no value
  test.assertFalse(myPosArg.hasValue());
  test.assertFalse(passThrough.hasValue());
  test.assertEqual(new list(passThrough.values()), new list(string));
}

// specify pass-through on main command with a subcommand
// note that the subCmd will not match because it's after the
// pass-thru indicator ++
proc testSubCmdAndPassThruOnMain(test: borrowed Test) throws {
  var argList = ["progName","-n","20","++","--strArg","passValue","subCmd"];
  var parser = new argumentParser();
  var passThru = parser.addPassThrough("++");
  var mySubCmd1 = parser.addSubCommand(cmd="subCmd");
  var myStrArg1 = parser.addOption(name="StringOpt",
                                   opts=["-n","--strArg"],
                                   numArgs=1);
  parser.parseArgs(argList);
  test.assertFalse(mySubCmd1.hasValue());
  test.assertTrue(myStrArg1.hasValue());
  test.assertEqual(myStrArg1.value(),"20");
  test.assertFalse(mySubCmd1.hasValue());
  test.assertTrue(passThru.hasValue());
  test.assertEqual(new list(passThru.values()), new list(argList[4..]));
}


// test using name as default long option flag
proc testDefaultNameOpts(test: borrowed Test) throws {
  var argList = ["progName","--myOpt1","twenty", "--myFlag"];
  var parser = new argumentParser();
  // TODO: Compilation is crashing with UnitTest when opts are not specified
  //       Find out why as part of cray/chapel-private#2300, regular use in
  //       MasonArgParseExample.chpl compiles as intended
  var myStrArg = parser.addOption(name="myOpt1",
                                  opts=["--myOpt1"]);
  var myBoolArg = parser.addFlag(name="myFlag",
                                 opts=["--myFlag"]);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  test.assertFalse(myBoolArg.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.value(),"twenty");
  test.assertTrue(myBoolArg.valueAsBool());
}

// unit test for helper _processNameToOpts
proc testDefaultNameToOpts(test: borrowed Test) throws {

  var opt1 = _processNameToOpts("show");
  test.assertEqual(opt1,["--show"]);

  var opt2 = _processNameToOpts("--show");
  test.assertEqual(opt2,["--show"]);

  var opt3 = _processNameToOpts("-show");
  test.assertEqual(opt3,["-show"]);

  var opt4 = _processNameToOpts("--show-");
  test.assertEqual(opt4,["--show-"]);

  var opt5 = _processNameToOpts("show-me");
  test.assertEqual(opt5,["--show-me"]);
}

// unit test for helper _prepareRange
proc testRangeConverter(test: borrowed Test) throws {
  var range1 = _prepareRange(1..);
  test.assertEqual(range1, 1..max(int));

  var range2 = _prepareRange(1..20);
  test.assertEqual(range2, 1..20);

  try {
    var range3 = _prepareRange(..20);
  } catch ex : ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());

    try {
      var range4 = _prepareRange(..);
    } catch ex : ArgumentError {
      test.assertTrue(true);
      stderr.writeln(ex.message());
      return;
    }
  }
  test.assertTrue(false);
}

// test for negative integers with arguments
proc testSimpleNegativeIntegerArguments(test: borrowed Test) throws {
  var argList = ["progName","-1","-100"];
  var parser = new argumentParser();
  var myIntArgs = parser.addArgument(name="StringOpt", numArgs=1..);

  //make sure no value currently exists
  test.assertFalse(myIntArgs.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myIntArgs.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myIntArgs.values()), new list(argList[1..]));
}

// test for negative integers with options
proc testSimpleNegativeIntegerOptions(test: borrowed Test) throws {
  var argList = ["progName","-n","-1","-100"];
  var parser = new argumentParser();
  var myIntArgs = parser.addOption(name="-n", opts=["-n"], numArgs=1..);

  //make sure no value currently exists
  test.assertFalse(myIntArgs.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myIntArgs.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myIntArgs.values()), new list(argList[2..]));
}

// test for negative integers mixed in arguments and options
proc testMixedNegativeIntegerArgsOptions(test: borrowed Test) throws {
  var argList = ["progName","-200","-999.98i","-n","-1","-100"];
  var parser = new argumentParser();
  var myIntOpts = parser.addOption(name="-n", opts=["-n"], numArgs=1..);
  var myIntArgs = parser.addArgument(name="StringArg", numArgs=1..);
  //make sure no value currently exists
  test.assertFalse(myIntArgs.hasValue());
  test.assertFalse(myIntOpts.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we now have a value
  test.assertTrue(myIntArgs.hasValue());
  test.assertTrue(myIntOpts.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myIntArgs.values()), new list(argList[1..2]));
  test.assertEqual(new list(myIntOpts.values()), new list(argList[4..5]));
}

// test for presence of option without values
proc testOptionZeroValueDetection(test: borrowed Test) throws {
  var argList = ["progName","--example"];
  var parser = new argumentParser();
  var myOption = parser.addOption(name="example",
                                  opts=["--example"],
                                  numArgs=0..);
  var myOtherOption = parser.addOption(name="other",
                                       opts=["--other"],
                                       numArgs=0..);

  //make sure no value currently exists
  test.assertFalse(myOption.hasValue());
  test.assertFalse(myOtherOption.hasValue());
  //parse the options
  parser.parseArgs(argList);
  //make sure we found the option
  test.assertTrue(myOption._present);
  test.assertFalse(myOtherOption._present);
  //ensure there is no value
  test.assertFalse(myOption.hasValue());
  test.assertFalse(myOtherOption.hasValue());
  test.assertEqual(new list(myOption.values()), new list(string));
}

// TODO: SPLIT THIS INTO MULTIPLE FILES BY FEATURE

UnitTest.main();
