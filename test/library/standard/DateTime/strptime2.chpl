use DateTime;
use UnitTest;

proc test_microsecond(test: borrowed Test) throws {
  var dt : datetime = new datetime();
  try {
    dt.strptime("12:59:59.008000", "%T.%f");
  } catch e {}
  test.assertEqual(dt.microsecond,8000);
}

proc test_out_of_range_microseconds(test: borrowed Test) throws {
  var dt : datetime = new datetime();
  try {
    dt.strptime("12:59:59.1000001", "%T.%f");
  } catch e {
    test.assertEqual(e.message(),"Invalid Arguments Provided");
  }
}

proc test_negative_microseconds(test: borrowed Test) throws {
  var dt : datetime = new datetime();
  try {
    dt.strptime("12:59:59.-10", "%T.%f");
  } catch e {
    test.assertEqual(e.message(),"Invalid Arguments Provided");
  }
}

proc test_no_microseconds(test: borrowed Test) throws {
  var dt : datetime = new datetime();
  try {
    dt.strptime("12:59:59", "%T.%f");
  } catch e {
    test.assertEqual(e.message(),"Invalid Arguments Provided");
  }
}

UnitTest.main();
