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
  test.assertEqual(myStrArg.getValue(),"twenty");
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
  test.assertEqual(myStrArg.getValue(),"twenty");
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
                                  numArgs=1..10);
  
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.getValues()),
                   new list(["twenty","thirty"]));
}


// attempt to specify no argument
proc testTryMakeEmptyOpts(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();
  var optList:[0..0]string;
  try {
    var myStrArg = parser.addOption(name="StringOpt",
                                    opts=optList,            
                                    numArgs=1..10);
  }catch ex: ArgumentError {
    test.assertTrue(true);
    stderr.writeln(ex.message());
    return;
  }
  test.assertTrue(false);  
}

// attempt to specify invalid argument opt
proc testTryMakeBadOptID(test: borrowed Test) throws {
  var argList = ["progName","-n=twenty","thirty"];
  var parser = new argumentParser();  
  try {
    var myStrArg = parser.addOption(name="StringOpt",
                                    opts=["name","-n"],            
                                    numArgs=1..10);
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
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.getValue(),"forty");
}

// a short string opt with range value, expressed multiple times
proc testRangeStringShortOptRepeated(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","two","hundred","-n",
                 "thirty","five","-n","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],            
                                  numArgs=1..3);
  
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.getValue(),"forty");
  test.assertEqual(new list(myStrArg.getValues()),new list(argList[9..9]));
}

// a short string opt with range value, expressed multiple times, with
// too many values supplied in first instance
proc testRangeStringShortOptRepeatedTooManyFirst(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","two","hundred","fifty","-n",
                 "thirty","five","-n","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],            
                                  numArgs=1..3);
  
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.getValue(),"forty");
  test.assertEqual(new list(myStrArg.getValues()),new list(argList[10..10]));
}

// a short string opt with single value and no values supplied
proc testOptSingleStringShortOptNoOpts(test: borrowed Test) throws {
  var argList = ["progName"];
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
                                  numArgs=0..1);
  
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
                                  numArgs=0..1);
  
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
                                  numArgs=1..2);
  
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
                                  numArgs=0..1);
  
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
                                  numArgs=0..1);
  
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
                                  numArgs=0..1);
  
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(myStrArg.getValue(),"twenty");
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
  test.assertEqual(myStrArg.getValue(),"twenty");
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
  test.assertEqual(new list(myStrArg.getValues()), new list(argList[2..]));
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
  test.assertEqual(new list(myStrArg.getValues()), new list(argList[2..]));
}

// a short string opt with fixed range numArgs, max # supplied
proc testMultiStringShortOptVar(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],            
                                  numArgs=1..3);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.getValues()), new list(argList[2..]));
}

// test a long string option with fixed range of numArgs, max # supplied
proc testMultiStringLongOptVar(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty","thirty","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],            
                                  numArgs=1..3);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.getValues()), new list(argList[2..]));
}

// test a short string option with fixed range of numArgs, less than
// max # supplied
proc testMultiStringShortOptVarLess(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],            
                                  numArgs=1..3);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.getValues()), new list(argList[2..]));
}

// test a short string option with fixed range of numArgs more than 1,
// fewer than min # supplied
proc testMultiStringShortOptVarLessThanMin(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],            
                                  numArgs=3..5);
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
                                  numArgs=1..3);
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
                                  numArgs=1..3);
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

// test a long string option with unlimited range of numArgs, inbound # supplied
proc testUnboundStringLongOptVar(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty","thirty","forty","fifty"];
  var parser = new argumentParser();
  // TODO: WHY IS UNBOUNDED RANGE A COMPILATION ERROR?
  var myStrArg = parser.addOption(name="StringOpt",
                                  opts=["-n","--stringVal"],
                                  numArgs=1..1000000);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  test.assertEqual(new list(myStrArg.getValues()), new list(argList[2..]));
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
  test.assertEqual(myStrArg1.getValue(),"twenty");
  test.assertEqual(myStrArg2.getValue(),"thirty");
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
  test.assertEqual(myStrArg1.getValue(),"twenty");
  test.assertEqual(myStrArg2.getValue(),"thirty");
  test.assertEqual(myStrArg3.getValue(),"forty");
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
                                   numArgs=1..2);

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
  test.assertEqual(myStrArg1.getValue(),"twenty");
  test.assertEqual(new list(myStrArg2.getValues()), new list(argList[4..5]));
  test.assertEqual(new list(myStrArg3.getValues()), new list(argList[7..8]));
}

// three short string opts with fixed range multi value opts,
// min, mid, and max # values supplied respectively
proc testThreeMultiStringShortOptPartialValues(test: borrowed Test) throws {
  var argList=["progName","-n","twenty","-p","thirty","five","-t",
               "forty","two"];
  var parser = new argumentParser();
  var myStrArg1 = parser.addOption(name="StringOpt1",
                                   opts=["-n","--stringVal1"],            
                                   numArgs=1..3);
  var myStrArg2 = parser.addOption(name="StringOpt2",
                                   opts=["-p","--stringVal2"],            
                                   numArgs=1..4);
  var myStrArg3 = parser.addOption(name="StringOpt3",
                                   opts=["-t","--stringVal3"],            
                                   numArgs=1..2);

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
  test.assertEqual(myStrArg1.getValue(),"twenty");
  test.assertEqual(new list(myStrArg2.getValues()), new list(argList[4..5]));
  test.assertEqual(new list(myStrArg3.getValues()), new list(argList[7..8]));
}

UnitTest.main();