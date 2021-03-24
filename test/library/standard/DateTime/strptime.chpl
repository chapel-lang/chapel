use DateTime;

proc test_microsecond() {
  var dt = datetime.strptime("%T.%f", "12:59:59.008000");
  assert(dt.microsecond == 8000);
}

test_microsecond();
