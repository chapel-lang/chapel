use DateTime;

proc test_basic_attributes() {
  var t = new time(12, 0);
  assert(t.hour == 12);
  assert(t.minute == 0);
  assert(t.second == 0);
  assert(t.microsecond == 0);
}

proc test_basic_attributes_nonzero() {
  var t = new time(12, 59, 59, 8000);
  assert(t.hour == 12);
  assert(t.minute == 59);
  assert(t.second == 59);
  assert(t.microsecond == 8000);
}

proc test_comparing() {
  var args = (1, 2, 3, 4);
  var t1 = new time((...args));
  var t2 = new time((...args));
  assert(t1 == t2);
  assert(t1 <= t2);
  assert(t1 >= t2);
  assert(!(t1 != t2));
  assert(!(t1 < t2));
  assert(!(t1 > t2));
  //self.assertEqual(cmp(t1, t2), 0);
  //self.assertEqual(cmp(t2, t1), 0);

  for i in 0..#args.size {
    var newargs = args;
    newargs[i] = args[i] + 1;
    t2 = new time((...newargs));   // this is larger than t1
    assert(t1 < t2);
    assert(t2 > t1);
    assert(t1 <= t2);
    assert(t2 >= t1);
    assert(t1 != t2);
    assert(t2 != t1);
    assert(!(t1 == t2));
    assert(!(t2 == t1));
    assert(!(t1 > t2));
    assert(!(t2 < t1));
    assert(!(t1 >= t2));
    assert(!(t2 <= t1));
    //self.assertEqual(cmp(t1, t2), -1);
    //self.assertEqual(cmp(t2, t1), 1);
  }
}

proc test_isoformat() {
  var t = new time(4, 5, 1, 123);
  assert(t.isoformat() == "04:05:01.000123");

  t = new time();
  assert(t.isoformat() == "00:00:00");

  t = new time(microsecond=1);
  assert(t.isoformat() == "00:00:00.000001");

  t = new time(microsecond=10);
  assert(t.isoformat() == "00:00:00.000010");

  t = new time(microsecond=100);
  assert(t.isoformat() == "00:00:00.000100");

  t = new time(microsecond=1000);
  assert(t.isoformat() == "00:00:00.001000");

  t = new time(microsecond=10000);
  assert(t.isoformat() == "00:00:00.010000");

  t = new time(microsecond=100000);
  assert(t.isoformat() == "00:00:00.100000");
}

proc test_strftime() {
  var t = new time(1, 2, 3, 4);
  assert(t.strftime('%H %M %S') == "01 02 03");
  // A naive object replaces %z and %Z with empty strings.
  // except that the system strftime doesn't seem to do that
  //assert(t.strftime("'%z' '%Z'") == "'' ''");
}

proc test_resolution_info() {
  proc valIsType(type t, val: t) param return true;
  proc valIsType(type t, val) param return false;

  assert(valIsType(time, time.min));
  assert(valIsType(time, time.max));
  assert(valIsType(timedelta, time.resolution));
  assert(time.max > time.min);
}

proc test_replace() {
  var args = (1, 2, 3, 4);
  var base = new time((...args));
  assert(base == base.replace(tzinfo=base.tzinfo));

  var i = 0;
  for (name, newval) in (("hour", 5),
                         ("minute", 6),
                         ("second", 7),
                         ("microsecond", 8)) {
    var newargs = args;
    newargs[i] = newval;
    var expected = new time((...newargs));
    var got: time;
    if name == "hour" then
      got = base.replace(hour=newval, tzinfo=base.tzinfo);
    else if name == "minute" then
      got = base.replace(minute=newval, tzinfo=base.tzinfo);
    else if name == "second" then
      got = base.replace(second=newval, tzinfo=base.tzinfo);
    else if name == "microsecond" then
      got = base.replace(microsecond=newval, tzinfo=base.tzinfo);
    assert(expected == got);
    i += 1;
  }
}

test_basic_attributes();
test_basic_attributes_nonzero();
test_comparing();
test_isoformat();
test_strftime();
test_resolution_info();
test_replace();
