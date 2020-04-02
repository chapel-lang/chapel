use DateTime;

proc test_basic_attributes() {
  var dt = new datetime(2002, 3, 1, 12, 0);
  assert(dt.year == 2002);
  assert(dt.month == 3);
  assert(dt.day == 1);
  assert(dt.hour == 12);
  assert(dt.minute == 0);
  assert(dt.second == 0);
  assert(dt.microsecond == 0);
}

proc test_basic_attributes_nonzero() {
  // Make sure all attributes are non-zero so bugs in
  // bit-shifting access show up.
  var dt = new datetime(2002, 3, 1, 12, 59, 59, 8000);
  assert(dt.year == 2002);
  assert(dt.month == 3);
  assert(dt.day == 1);
  assert(dt.hour == 12);
  assert(dt.minute == 59);
  assert(dt.second == 59);
  assert(dt.microsecond == 8000);
}

proc test_isoformat() {
  var t = new datetime(2, 3, 2, 4, 5, 1, 123);
  assert(t.isoformat() == "0002-03-02T04:05:01.000123");
  assert(t.isoformat('T') == "0002-03-02T04:05:01.000123");
  assert(t.isoformat(' ') == "0002-03-02 04:05:01.000123");
  // str is ISO format with the separator forced to a blank.
  //assert(str(t) == "0002-03-02 04:05:01.000123");

  t = new datetime(2, 3, 2);
  assert(t.isoformat() == "0002-03-02T00:00:00");
  assert(t.isoformat('T') == "0002-03-02T00:00:00");
  assert(t.isoformat(' ') == "0002-03-02 00:00:00");
  // str is ISO format with the separator forced to a blank.
  //assert(str(t) == "0002-03-02 00:00:00");
}

proc test_more_ctime() {
  // Test fields that TestDate doesn't touch.
  var t = new datetime(2002, 12, 4, 20, 30, 40);
  assert(t.ctime() == "Wed Dec  4 20:30:40 2002");

  t = new datetime(2002, 3, 22, 18, 3, 5, 123);
  assert(t.ctime() == "Fri Mar 22 18:03:05 2002");
/*
  import time;

  var t = new datetime(2002, 3, 2, 18, 3, 5, 123);
  assert(t.ctime() == "Sat Mar  2 18:03:05 2002");
  // Oops!  The next line fails on Win2K under MSVC 6, so it's commented
  // out.  The difference is that t.ctime() produces " 2" for the day,
  // but platform ctime() produces "02" for the day.  According to
  // C99, t.ctime() is correct here.
  // self.assertEqual(t.ctime(), time.ctime(time.mktime(t.timetuple())))

  // So test a case where that difference doesn't matter.
  t = new datetime(2002, 3, 22, 18, 3, 5, 123);
  assert(t.ctime() == time.ctime(time.mktime(t.timetuple())));
*/
}

proc test_tz_independent_comparing() {
  var dt1 = new datetime(2002, 3, 1, 9, 0, 0);
  var dt2 = new datetime(2002, 3, 1, 10, 0, 0);
  var dt3 = new datetime(2002, 3, 1, 9, 0, 0);
  assert(dt1 == dt3);
  assert(dt2 > dt3);

  // Make sure comparison doesn't forget microseconds, and isn't done
  // via comparing a float timestamp (an IEEE double doesn't have enough
  // precision to span microsecond resolution across years 1 thru 9999,
  // so comparing via timestamp necessarily calls some distinct values
  // equal).
  dt1 = new datetime(MAXYEAR, 12, 31, 23, 59, 59, 999998);
  var us = new timedelta(microseconds=1);
  dt2 = dt1 + us;
  assert(dt2 - dt1 == us);
  assert(dt1 < dt2);
}

proc test_computations() {
  var a = new datetime(2002, 1, 31);
  var b = new datetime(1956, 1, 31);
  var diff = a-b;
  assert(diff.days == 46*365 + (1956..2001 by 4).size);
  assert(diff.seconds == 0);
  assert(diff.microseconds == 0);
  a = new datetime(2002, 3, 2, 17, 6);
  var millisec = new timedelta(0, 0, 1000);
  var hour = new timedelta(0, 3600);
  var day = new timedelta(1);
  var week = new timedelta(7);
  assert(a + hour == new datetime(2002, 3, 2, 18, 6));
  assert(hour + a == new datetime(2002, 3, 2, 18, 6));
  assert(a + 10*hour == new datetime(2002, 3, 3, 3, 6));
  assert(a - hour == new datetime(2002, 3, 2, 16, 6));
  assert(-hour + a == new datetime(2002, 3, 2, 16, 6));
  assert(a - hour == a + -hour);
  assert(a - 20*hour == new datetime(2002, 3, 1, 21, 6));
  assert(a + day == new datetime(2002, 3, 3, 17, 6));
  assert(a - day == new datetime(2002, 3, 1, 17, 6));
  assert(a + week == new datetime(2002, 3, 9, 17, 6));
  assert(a - week == new datetime(2002, 2, 23, 17, 6));
  assert(a + 52*week == new datetime(2003, 3, 1, 17, 6));
  assert(a - 52*week == new datetime(2001, 3, 3, 17, 6));
  assert((a + week) - a == week);
  assert((a + day) - a == day);
  assert((a + hour) - a == hour);
  assert((a + millisec) - a == millisec);
  assert((a - week) - a == -week);
  assert((a - day) - a == -day);
  assert((a - hour) - a == -hour);
  assert((a - millisec) - a == -millisec);
  assert(a - (a + week) == -week);
  assert(a - (a + day) == -day);
  assert(a - (a + hour) == -hour);
  assert(a - (a + millisec) == -millisec);
  assert(a - (a - week) == week);
  assert(a - (a - day) == day);
  assert(a - (a - hour) == hour);
  assert(a - (a - millisec) == millisec);
  assert(a + (week + day + hour + millisec) ==
                   new datetime(2002, 3, 10, 18, 6, 0, 1000));
  assert(a + (week + day + hour + millisec) ==
                   (((a + week) + day) + hour) + millisec);
  assert(a - (week + day + hour + millisec) ==
                   new datetime(2002, 2, 22, 16, 5, 59, 999000));
  assert(a - (week + day + hour + millisec) ==
                   (((a - week) - day) - hour) - millisec);
}

proc test_more_compare() {
  var args = (2000, 11, 29, 20, 58, 16, 999998);
  var t1 = new datetime((...args));
  var t2 = new datetime((...args));
  assert(t1 == t2);
  assert(t1 <= t2);
  assert(t1 >= t2);
  assert(!(t1 != t2));
  assert(!(t1 < t2));
  assert(!(t1 > t2));
  //assert(cmp(t1, t2), 0);
  //assert(cmp(t2, t1), 0);

  for i in 1..args.size {
    var newargs = args;
    newargs[i] = args[i] + 1;
    t2 = new datetime((...newargs));   // this is larger than t1
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
    //assert(cmp(t1, t2), -1);
    //assert(cmp(t2, t1), 1);
  }
}

proc test_more_timetuple() {
  var t = new datetime(2004, 12, 31, 6, 22, 33);
  var tt = t.timetuple();
  assert(tt.tm_year == t.year);
  assert(tt.tm_mon == t.month);
  assert(tt.tm_mday == t.day);
  assert(tt.tm_hour == t.hour);
  assert(tt.tm_min == t.minute);
  assert(tt.tm_sec == t.second);
  assert(tt.tm_wday == t.weekday(): int(32));
  assert(tt.tm_yday == t.toordinal() -
                       (new date(t.year, 1, 1)).toordinal() + 1);
  assert(tt.tm_isdst == -1);
}

proc test_more_strftime() {
  var t = new datetime(2004, 12, 31, 6, 22, 33);
  assert(t.strftime("%m %d %y %S %M %H %j") == "12 31 04 33 22 06 366");
}

proc test_extract() {
  var dt = new datetime(2002, 3, 4, 18, 45, 3, 1234);
  assert(dt.getdate() == new date(2002, 3, 4));
  assert(dt.gettime() == new time(18, 45, 3, 1234));
}

proc test_combine() {
  var d = new date(2002, 3, 4);
  var t = new time(18, 45, 3, 1234);
  var expected = new datetime(2002, 3, 4, 18, 45, 3, 1234);
  var dt = datetime.combine(d, t);
  assert(dt == expected);

  dt = datetime.combine(t=t, d=d);
  assert(dt == expected);

  assert(d == dt.getdate());
  assert(t == dt.gettime());
  assert(dt == datetime.combine(dt.getdate(), dt.gettime()));
}

proc test_replace() {
  var args = (1, 2, 3, 4, 5, 6, 7);
  var base = new datetime((...args));
  var nilTZ:shared TZInfo?;

  assert(base == base.replace(tzinfo=nilTZ));

  var i = 1;
  for (name, newval) in (("year", 2),
                         ("month", 3),
                         ("day", 4),
                         ("hour", 5),
                         ("minute", 6),
                         ("second", 7),
                         ("microsecond", 8)) {
    var newargs = args;
    var got: datetime;
    newargs[i] = newval;
    var expected = new datetime((...newargs));

    if name == "year" then
      got = base.replace(year = newval, tzinfo=nilTZ);
    else if name == "month" then
      got = base.replace(month = newval, tzinfo=nilTZ);
    else if name == "day" then
      got = base.replace(day = newval, tzinfo=nilTZ);
    else if name == "hour" then
      got = base.replace(hour = newval, tzinfo=nilTZ);
    else if name == "minute" then
      got = base.replace(minute = newval, tzinfo=nilTZ);
    else if name == "second" then
      got = base.replace(second = newval, tzinfo=nilTZ);
    else if name == "microsecond" then
      got = base.replace(microsecond = newval, tzinfo=nilTZ);

    assert(expected == got);
    i += 1;
  }
}

test_basic_attributes();
test_basic_attributes_nonzero();
test_isoformat();
test_more_ctime();
test_tz_independent_comparing();
test_computations();
test_more_compare();
//test_fromtimestamp();
//test_utcfromtimestamp();
//test_utcnow();
test_more_timetuple();
test_more_strftime();
test_extract();
test_combine();
test_replace();
