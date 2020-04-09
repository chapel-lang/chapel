use DateTime;
use List;

var today = date.today(); // workaround for problem with unused records

proc test_date_delta_non_days_ignored() {
  var dt = new date(2000, 1, 2);
  var delta = new timedelta(days=1, hours=2, minutes=3, seconds=4,
                            microseconds=5);

  var days = new timedelta(delta.days);
  assert(days == new timedelta(1));

  var dt2 = dt + delta;
  assert(dt2 == dt+days);

  dt2 = delta + dt;
  assert(dt2 == dt + days);

  dt2 = dt - delta;
  assert(dt2 == dt - days);

  delta = -delta;
  days = new timedelta(delta.days);
  assert(days == new timedelta(-2));

  dt2 = dt + delta;
  assert(dt2 == dt + days);

  dt2 = delta + dt;
  assert(dt2 == dt + days);

  dt2 = dt - delta;
  assert(dt2 == dt - days);
}

proc test_date_ordinal_conversions() {
  for (y,m,d,n) in [(1,1,1,1),
                    (1,12,31,365),
                    (2,1,1,366),
                    (1945,11,12,710347)] {
    var d1 = new date(y, m, d);
    assert(n == d1.toordinal());
    var fromord = date.fromordinal(n);
    assert(d1 == fromord);
  }

  for year in MINYEAR..MAXYEAR by 7 {
    var d = new date(year, 1, 1);
    var n = d.toordinal();
    var d2 = date.fromordinal(n);
    assert(d == d2);

    if year > 1 {
      d = date.fromordinal(n-1);
      d2 = new date(year-1, 12, 31);
      assert(d == d2);
      assert(d2.toordinal() == n-1);
    }
  }

  var dim = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
  for (year, isleap) in [(2000, true), (2002, false)] {
    var n = (new date(year, 1, 1)).toordinal();
    for (month, maxday) in zip (1..12, dim) {
      var md = maxday;
      if month == 2 && isleap then
        md += 1;
      for day in 1..md {
        var d = new date(year, month, day);
        assert(d.toordinal() == n);
        assert(d == date.fromordinal(n));
        n += 1;
      }
    }
  }
}

proc test_date_extreme_ordinals() {
  var a = date.min;
  var aord = a.toordinal();
  var b = date.fromordinal(aord);
  assert(a == b);

  b = a + new timedelta(days=1);
  assert(b.toordinal() == aord + 1);
  assert(b == date.fromordinal(aord+1));

  a = date.max;
  aord = a.toordinal();
  b = date.fromordinal(aord);
  assert(a == b);

  b = a - new timedelta(days=1);
  assert(b.toordinal() == aord - 1);
  assert(b == date.fromordinal(aord - 1));
}

proc test_date_computations() {
  var a = new date(2002, 1, 31);
  var b = new date(1956, 1, 31);

  var diff = a - b;
  assert(diff.days == 46*365 + (1956..2002 by 4).size);
  assert(diff.seconds == 0);
  assert(diff.microseconds == 0);

  var day = new timedelta(1);
  var week = new timedelta(7);
  a = new date(2002, 3, 2);

  assert(a + day == new date(2002, 3, 3));
  assert(day + a == new date(2002, 3, 3));
  assert(a - day == new date(2002, 3, 1));
  assert(-day + a == new date(2002, 3, 1));
  assert(a + week == new date(2002, 3, 9));
  assert(a - week == new date(2002, 2, 23));
  assert(a + 52*week == new date(2003, 3, 1));
  assert(a - 52*week == new date(2001, 3, 3));
  assert((a + week) - a == week);
  assert((a + day) - a == day);
  assert((a - week) - a == -week);
  assert((a - day) - a == -day);
  assert(a - (a + week) == -week);
  assert(a - (a + day) == -day);
  assert(a - (a - week) == week);
  assert(a - (a - day) == day);
}

proc test_date_fromtimestamp() {
  var (year, month, day) = (1999, 9, 19);
  var d1 = new date(year, month, day);
  var delta = d1 - unixEpoch.getdate();
  var ts = delta.days * 60 * 60 * 24 + delta.seconds;

  var d = date.fromtimestamp(ts);
  assert(d.year == year);
  assert(d.month == month);
  assert(d.day == day);
}

proc test_date_today() {
  var tday, tdayAgain: date;
  for 1..3 { // give it a few tries in case we cross midnight
    tday = date.today();
    var delta = tday - unixEpoch.getdate();
    var ts = delta.days * 60 * 60 * 24 + delta.seconds;
    tdayAgain = date.fromtimestamp(ts);
    if tday == tdayAgain then
      break;
  }
  assert(tday == tdayAgain);
}

proc test_date_weekday() {
  for i in 0..#7 {
    assert((new date(2002, 3, 4+i)).weekday(): int == i);
    assert((new date(2002, 3, 4+i)).isoweekday(): int == i+1);
    assert((new date(1956, 1, 2+i)).weekday(): int == i);
    assert((new date(1956, 1, 2+i)).isoweekday(): int == i+1);
  }
}

proc test_date_isocalendar() {
  for i in 0..#7 {
    var d = new date(2003, 12, 22+i);
    assert(d.isocalendar() == (2003, 52, i+1));
    d = new date(2003, 12, 29) + new timedelta(i);
    assert(d.isocalendar() == (2004, 1, i+1));
    d = new date(2004, 1, 5+i);
    assert(d.isocalendar() == (2004, 2, i+1));
    d = new date(2009, 12, 21+i);
    assert(d.isocalendar() == (2009, 52, i+1));
    d = new date(2009, 12, 28) + new timedelta(i);
    assert(d.isocalendar() == (2009, 53, i+1));
    d = new date(2010, 1, 4+i);
    assert(d.isocalendar() == (2010, 1, i+1));
  }
}

proc test_date_iso_long_years() {
  use Sort;
  var ISO_LONG_YEARS_TABLE = [4, 32, 60, 88, 9, 37, 65, 93, 15, 43, 71, 99,
                              20, 48, 76, 26, 54, 82, 105, 133, 161, 189,
                              111, 139, 167, 195, 116, 144, 172, 122, 150,
                              178, 128, 156, 184, 201, 229, 257, 285, 207,
                              235, 263, 291, 212, 240, 268, 296, 218, 246,
                              274, 224, 252, 280, 303, 331, 359, 387, 308,
                              336, 364, 392, 314, 342, 370, 398, 320, 348,
                              376, 325, 353, 381];
  sort(ISO_LONG_YEARS_TABLE);
  var L: list(int);

  for i in 0..#400 {
    var d = new date(2000+i, 12, 31);
    var d1 = new date(1600+i, 12, 31);
    assert(d.isocalendar()(1) == d1.isocalendar()(1) &&
           d.isocalendar()(2) == d1.isocalendar()(2));
    if d.isocalendar()(1) == 53 then
      L.append(i);
  }

  assert(L.size == ISO_LONG_YEARS_TABLE.size);
  for (a, b) in zip(ISO_LONG_YEARS_TABLE, L) {
    assert(a == b);
  }
}

proc test_date_isoformat() {
  var t = new date(2, 3, 2);
  assert(t.isoformat() == "0002-03-02");
}

proc test_date_ctime() {
  var t = new date(2002, 3, 2);
  assert(t.ctime() == "Sat Mar  2 00:00:00 2002");
}

proc test_date_strftime() {
  var t = new date(2005, 3, 2);
  assert(t.strftime("m:%m d:%d y:%y") == "m:03 d:02 y:05");

  // strftime is showing timezone info for these
  //assert(t.strftime("'%z' '%Z'") == "'' ''");
}

proc test_date_resolution_info() {
  proc istype(type t, d: t) param return true;
  proc isdate(type t, d) param return false;
  assert(istype(date, date.min));
  assert(istype(date, date.max));
  assert(istype(timedelta, date.resolution));
  assert(date.max > date.min);
}

proc test_date_extreme_timedelta() {
  var big = date.max - date.min;
  var n = (big.days*24*3600 + big.seconds)*1000000 + big.microseconds;
  var justasbig = new timedelta(0, 0, n);

  assert(big == justasbig);
  assert(date.min + big == date.max);
  assert(date.max - big == date.min);
}

proc test_date_timetuple() {
  proc checkTimeTuple(t, year, month, day, hour, minute,
                      second, wday, yday, isdst) {
    return t.tm_year == year && t.tm_mon == month && t.tm_mday == day &&
           t.tm_hour == hour && t.tm_min == minute && t.tm_sec == second &&
           t.tm_wday == wday && t.tm_yday == yday && t.tm_isdst == isdst;
  }

  for i in 0..#7 {
    var d = new date(1956, 1, 2+i);
    var t = d.timetuple();
    assert(checkTimeTuple(t, 1956, 1, 2+i, 0, 0, 0, i, 2+i, -1));

    d = new date(1956, 2, 1+i);
    t = d.timetuple();
    assert(checkTimeTuple(t, 1956, 2, 1+i, 0, 0, 0, (2+i)%7, 32+i, -1));

    d = new date(1956, 3, 1+i);
    t = d.timetuple();
    assert(checkTimeTuple(t, 1956, 3, 1+i, 0, 0, 0, (3+i)%7, 61+i, -1));
  }
}

proc test_date_compare() {
  var t1 = new date(2, 3, 4);
  var t2 = new date(2, 3, 4);

  assert(t1 == t2);
  assert(t1 <= t2);
  assert(t1 >= t2);

  assert(!(t1 != t2));
  assert(!(t1 < t2));
  assert(!(t1 > t2));

  for args in ((3, 3, 3), (2, 4, 4), (2, 3, 5)) {
    t2 = new date((...args));
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
  }
}

proc test_date_replace() {
  var args = (1,2,3);
  var base = new date((...args));
  assert(base == base.replace());

  var i = 0;
  var newargs = args;
  newargs(i) = 2;
  var expected = new date((...newargs));
  var got = base.replace(year=2);
  i += 1;
  assert(expected == got);

  newargs = args;
  newargs(i) = 3;
  expected = new date((...newargs));
  got = base.replace(month=3);
  i += 1;
  assert(expected == got);

  newargs = args;
  newargs(i) = 4;
  expected = new date((...newargs));
  got = base.replace(day=4);
  i += 1;
  assert(expected == got);

  base = new date(2000, 2, 29);
  //base.replace(year=2001); // should (and currently does) halt
}

test_date_delta_non_days_ignored();
test_date_ordinal_conversions();
test_date_extreme_ordinals();
test_date_computations();
test_date_fromtimestamp();
test_date_today();
test_date_weekday();
test_date_isocalendar();
test_date_iso_long_years();
test_date_isoformat();
test_date_ctime();
test_date_strftime();
test_date_resolution_info();
test_date_extreme_timedelta();
test_date_timetuple();
test_date_compare();
test_date_replace();
