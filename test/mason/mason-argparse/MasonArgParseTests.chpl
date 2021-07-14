use UnitTest;
use MasonArgParse;
use List;

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
  var expect = new list(argList[2..]);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  var ret = new list(myStrArg.getValues());
  test.assertEqual(ret, expect);
}

// a long string option with exact numArgs and expected # values supplied
proc testMultiStringLongOpt(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty","thirty","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
				    opts=["-n","--stringVal"],				    
				    numArgs=3);
  var expect = new list(argList[2..]);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  var ret = new list(myStrArg.getValues());
  test.assertEqual(ret, expect);
}

// a short string opt with fixed range numArgs, max # supplied
proc testMultiStringShortOptVar(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
				    opts=["-n","--stringVal"],				    
				    numArgs=1..3);
  var expect = new list(argList[2..]);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  var ret = new list(myStrArg.getValues());
  test.assertEqual(ret, expect);
}

// test a long string option with fixed range of numArgs, max # supplied
proc testMultiStringLongOptVar(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty","thirty","forty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
				    opts=["-n","--stringVal"],				    
				    numArgs=1..3);
  var expect = new list(argList[2..]);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  var ret = new list(myStrArg.getValues());
  test.assertEqual(ret, expect);
}

// test a short string option with fixed range of numArgs, less than
// max # supplied
proc testMultiStringShortOptVarLess(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
				    opts=["-n","--stringVal"],				    
				    numArgs=1..3);
  var expect = new list(argList[2..]);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  var ret = new list(myStrArg.getValues());
  test.assertEqual(ret, expect);
}

// test a short string option with fixed range of numArgs, more than
// max # supplied
proc testMultiStringShortOptVarMore(test: borrowed Test) throws {
  var argList = ["progName","-n","twenty","thirty","forty","fifty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
				    opts=["-n","--stringVal"],				    
				    numArgs=1..3);
  var expect = new list(argList[2..]);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  //test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  //var ret = new list(myStrArg.getValues());
  //test.assertEqual(ret, expect);
}

// test a short string option with fixed range of numArgs, less than
// min # supplied
proc testMultiStringShortOptVarNoVal(test: borrowed Test) throws {
  var argList = ["progName","-n"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
				    opts=["-n","--stringVal"],				    
				    numArgs=1..3);
  //var expect = new list(argList[2..]);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  //test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  //var ret = new list(myStrArg.getValues());
  //test.assertEqual(ret, expect);
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
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  //test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  //test.assertEqual(myStrArg.getValue(),"twenty");
}

// test a long string option with unlimited range of numArgs, inbound # supplied
proc testUnboundStringLongOptVar(test: borrowed Test) throws {
  var argList = ["progName","--stringVal","twenty","thirty","forty","fifty"];
  var parser = new argumentParser();
  var myStrArg = parser.addOption(name="StringOpt",
				                          opts=["-n","--stringVal"],
				                          numArgs=1..);
  var expect = new list(argList[2..]);
  //make sure no value currently exists
  test.assertFalse(myStrArg.hasValue());
  //parse the options
  parser.parseArgs(argList[1..]);
  //make sure we now have a value
  test.assertTrue(myStrArg.hasValue());
  //ensure the value passed is correct
  var ret = new list(myStrArg.getValues());
  test.assertEqual(ret, expect);
}

UnitTest.main();