use DateTime;

proc test_microsecond() {
  var dt = datetime.strptime("12:59:59.008000", "%T.%f");
  assert(dt.microsecond == 8000);
}

test_microsecond();
