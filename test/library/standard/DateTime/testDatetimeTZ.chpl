use DateTime;

class FixedOffset: TZInfo {
  var offset: timedelta;
  var name: string;
  var dstoffset: timedelta;
  proc init(offset: timedelta, name: string, dstoffset: timedelta = new timedelta(minutes=42)) {
    this.offset = offset;
    this.name = name;
    this.dstoffset = dstoffset;
  }
  proc init(offset: int, name, dstoffset:int=42) {
    this.offset = new timedelta(minutes=offset);
    this.name = name;
    this.dstoffset = new timedelta(minutes=dstoffset);
  }

  override proc utcoffset(dt: datetime) {
    return offset;
  }
  override proc tzname(dt: datetime) {
    return name;
  }
  override proc dst(dt: datetime) {
    return dstoffset;
  }
  override proc fromutc(dt: datetime) {
    var dtoff = dt.utcoffset();
    var dtdst = dt.dst();
    var delta = dtoff - dtdst;
    var dt2 = dt + delta;
    dtdst = dt2.dst();
    return dt2 + dtdst;
  }
}

proc test_trivial() {
  var dt = new datetime(1, 2, 3, 4, 5, 6, 7);
  assert(dt.year == 1);
  assert(dt.month == 2);
  assert(dt.day == 3);
  assert(dt.hour == 4);
  assert(dt.minute == 5);
  assert(dt.second == 6);
  assert(dt.microsecond == 7);
  assert(dt.tzinfo.borrow() == nil);
}

proc test_even_more_compare() {
  // Smallest possible after UTC adjustment.
  var t1 = new datetime(1, 1, 1,
                        tzinfo=new shared FixedOffset(1439, ""));
  // Largest possible after UTC adjustment.
  var t2 = new datetime(MAXYEAR, 12, 31, 23, 59, 59, 999999,
                        tzinfo=new shared FixedOffset(-1439, ""));

  // Make sure those compare correctly, and w/o overflow.
  assert(t1 < t2);
  assert(t1 != t2);
  assert(t2 > t1);

  assert(t1 == t1);
  assert(t2 == t2);

  // Equal afer adjustment.
  t1 = new datetime(1, 12, 31, 23, 59,
                    tzinfo=new shared FixedOffset(1, ""));
  t2 = new datetime(2, 1, 1, 3, 13,
                    tzinfo=new shared FixedOffset(3*60+13+2, ""));
  assert(t1 == t2);

  // Change t1 not to subtract a minute, and t1 should be larger.
  t1 = new datetime(1, 12, 31, 23, 59,
                    tzinfo=new shared FixedOffset(0, ""));
  assert(t1 > t2);

  // Change t1 to subtract 2 minutes, and t1 should be smaller.
  t1 = new datetime(1, 12, 31, 23, 59,
                    tzinfo=new shared FixedOffset(2, ""));
  assert(t1 < t2);

  // Back to the original t1, but make seconds resolve it.
  t1 = new datetime(1, 12, 31, 23, 59,
                    tzinfo=new shared FixedOffset(1, ""),
                    second=1);
  assert(t1 > t2);

  // Likewise, but make microseconds resolve it.
  t1 = new datetime(1, 12, 31, 23, 59,
                    tzinfo=new shared FixedOffset(1, ""),
                    microsecond=1);
  assert(t1 > t2);
}

proc test_zones() {
  var est = new shared FixedOffset(-300, "EST");
  var utc = new shared FixedOffset(0, "UTC");
  var met = new shared FixedOffset(60, "MET");
  var t1 = new datetime(2002, 3, 19,  7, 47, tzinfo=est);
  var t2 = new datetime(2002, 3, 19, 12, 47, tzinfo=utc);
  var t3 = new datetime(2002, 3, 19, 13, 47, tzinfo=met);
  assert(t1.tzinfo == est);
  assert(t2.tzinfo == utc);
  assert(t3.tzinfo == met);
  assert(t1.utcoffset() == new timedelta(minutes=-300));
  assert(t2.utcoffset() == new timedelta(minutes=0));
  assert(t3.utcoffset() == new timedelta(minutes=60));
  assert(t1.tzname() == "EST");
  assert(t2.tzname() == "UTC");
  assert(t3.tzname() == "MET");
  assert(t1 == t2);
  assert(t1 == t3);
  assert(t2 == t3);
}

proc test_combine() {
  var met = new shared FixedOffset(60, "MET");
  var d = new date(2002, 3, 4);
  var tz = new time(18, 45, 3, 1234, tzinfo=met);
  var dt = datetime.combine(d, tz);
  assert(dt == new datetime(2002, 3, 4, 18, 45, 3, 1234,
                          tzinfo=met));
}

proc test_extract() {
  var met = new shared FixedOffset(60, "MET");
  var dt = new datetime(2002, 3, 4, 18, 45, 3, 1234, tzinfo=met);
  assert(dt.getdate() == new date(2002, 3, 4));
  assert(dt.gettime() == new time(18, 45, 3, 1234));
  assert(dt.timetz() == new time(18, 45, 3, 1234, tzinfo=met));
}

proc test_tz_aware_arithmetic() {
  use Random;
  var rng = createRandomStream(eltType=int);

  var now = datetime.now();
  var tz55 = new shared FixedOffset(-330, "west 5:30");
  var timeaware = now.gettime().replace(tzinfo=tz55);
  var nowaware = datetime.combine(now.getdate(), timeaware);
  assert(nowaware.tzinfo == tz55);
  assert(nowaware.timetz() == timeaware);

  // Subtracting should yield 0.
  assert(now - now == new timedelta(0));
  assert(nowaware - nowaware == new timedelta(0));

  // Adding a delta should preserve tzinfo.
  var delta = new timedelta(weeks=1, minutes=12, microseconds=5678);
  var nowawareplus = nowaware + delta;
  assert(nowaware.tzinfo == tz55);
  var nowawareplus2 = delta + nowaware;
  assert(nowawareplus2.tzinfo == tz55);
  assert(nowawareplus == nowawareplus2);

  // that - delta should be what we started with, and that - what we
  // started with should be delta.
  var diff = nowawareplus - delta;
  assert(diff.tzinfo == tz55);
  assert(nowaware == diff);
  //self.assertRaises(TypeError, lambda: delta - nowawareplus);
  assert(nowawareplus - nowaware == delta);

  // Make up a random timezone.
  var tzr = new shared FixedOffset(rng.getNext(-1439, 1439), "randomtimezone");
  // Attach it to nowawareplus.
  nowawareplus = nowawareplus.replace(tzinfo=tzr);
  assert(nowawareplus.tzinfo == tzr);
  // Make sure the difference takes the timezone adjustments into account.
  var got = nowaware - nowawareplus;
  // Expected:  (nowaware base - nowaware offset) -
  //            (nowawareplus base - nowawareplus offset) =
  //            (nowaware base - nowawareplus base) +
  //            (nowawareplus offset - nowaware offset) =
  //            -delta + nowawareplus offset - nowaware offset
  var expected = nowawareplus.utcoffset() - nowaware.utcoffset() - delta;
  assert(got == expected);

  // Try max possible difference.
  var min = new datetime(1, 1, 1,
                         tzinfo=new shared FixedOffset(1439, "min"));
  var max = new datetime(MAXYEAR, 12, 31, 23, 59, 59, 999999,
                      tzinfo=new shared FixedOffset(-1439, "max"));
  var maxdiff = max - min;
  assert(maxdiff == datetime.max - datetime.min +
                    new timedelta(minutes=2*1439));
}

proc test_tzinfo_now() {
  // Ensure it doesn't require tzinfo (i.e., that this doesn't blow up).
  var base = datetime.now();
  // Try with and without naming the keyword.
  var off42 = new shared FixedOffset(42, "42");
  var another = datetime.now(off42);
  var again = datetime.now(tz=off42);
  assert(another.tzinfo == again.tzinfo);
  assert(another.utcoffset() == new timedelta(minutes=42));

  // We don't know which time zone we're in, and don't have a tzinfo
  // class to represent it, so seeing whether a tz argument actually
  // does a conversion is tricky.
  var weirdtz = new shared FixedOffset(new timedelta(hours=15, minutes=58),
                                           "weirdtz", new timedelta(0));
  var utc = new shared FixedOffset(0, "utc", 0);
  for 0..2 {
    var now = datetime.now(weirdtz);
    assert(now.tzinfo == weirdtz);
    var utcnow = datetime.utcnow().replace(tzinfo=utc);
    var now2 = utcnow.astimezone(weirdtz);
    if abs(now - now2) < new timedelta(seconds=30) {
      break;
    // Else the code is broken, or more than 30 seconds passed between
    // calls; assuming the latter, just try again.
    } else {
      // Three strikes and we're out.
      halt("utcnow(), now(tz), or astimezone() may be broken");
    }
  }
}

proc test_tzinfo_fromtimestamp() {
  proc getTimeOfDay() {
    extern "struct timeval" record timeval {
      var tv_sec: int;
      var tv_usec: int;
    }
    extern proc gettimeofday(ref tv: timeval, tz): int;

    var tv: timeval;
    if gettimeofday(tv, nil) != 0 then
      halt("error in call to gettimeofday()");
    return (tv.tv_sec, tv.tv_usec);
  }

  var ts = getTimeOfDay()(1);
  // Ensure it doesn't require tzinfo (i.e., that this doesn't blow up).
  var base = datetime.fromtimestamp(ts);
  // Try with and without naming the keyword.
  var off42 = new shared FixedOffset(42, "42");
  var another = datetime.fromtimestamp(ts, off42);
  var again = datetime.fromtimestamp(ts, tz=off42);
  assert(another.tzinfo == again.tzinfo);
  assert(another.utcoffset() == new timedelta(minutes=42));

  // Try to make sure tz= actually does some conversion.
  var timestamp = 1000000000;
  var utcdatetime = datetime.utcfromtimestamp(timestamp);
  // In POSIX (epoch 1970), that's 2001-09-09 01:46:40 UTC, give or take.
  // But on some flavor of Mac, it's nowhere near that.  So we can't have
  // any idea here what time that actually is, we can only test that
  // relative changes match.
  var utcoffset = new timedelta(hours=-15, minutes=39); // arbitrary, but not zero
  var tz = new shared FixedOffset(utcoffset, "tz", new timedelta());
  var expected = utcdatetime + utcoffset;
  var got = datetime.fromtimestamp(timestamp, tz);
  assert(expected == got.replace(tzinfo=nil));
}

proc test_tzinfo_timetuple() {
  // TestDateTime tested most of this.  datetime adds a twist to the
  // DST flag.
  class DST: TZInfo {
    var dstvalue: timedelta;
    proc init(i) {
      dstvalue = new timedelta(minutes=i);
    }
    override proc dst(dt) {
      return dstvalue;
    }
  }

  for (dstvalue, flag) in ((-33, 1), (33, 1), (0, 0)) {
    var d = new datetime(1, 1, 1, 10, 20, 30, 40,
                         tzinfo=new shared DST(dstvalue));
    var t = d.timetuple();
    assert(1 == t.tm_year);
    assert(1 == t.tm_mon);
    assert(1 == t.tm_mday);
    assert(10 == t.tm_hour);
    assert(20 == t.tm_min);
    assert(30 == t.tm_sec);
    assert(0 == t.tm_wday);
    assert(1 == t.tm_yday);
    assert(flag == t.tm_isdst);
  }

  // dst() at the edge.
  assert((new datetime(1,1,1, tzinfo=new shared DST(1439))).timetuple().tm_isdst == 1);
  assert((new datetime(1,1,1, tzinfo=new shared DST(-1439))).timetuple().tm_isdst == 1);
}

proc test_utctimetuple() {
  class DST: TZInfo {
    var dstvalue: timedelta;
    proc init(dstvalue) {
      this.dstvalue = new timedelta(minutes=dstvalue);
    }
    override proc dst(dt) {
      return dstvalue;
    }
  }

  class UOFS: DST {
    var uofs: timedelta;
    proc init(uofs, dofs=0) {
      super.init(dofs);
      this.uofs = new timedelta(minutes=uofs);
    }
    override proc utcoffset(dt) {
      return uofs;
    }
  }

  // Ensure tm_isdst is 0 regardless of what dst() says:  DST is never
  // in effect for a UTC time.
  for dstvalue in (-33, 33, 0) {
    var d = new datetime(1, 2, 3, 10, 20, 30, 40, tzinfo=new shared UOFS(-53, dstvalue));
    var t = d.utctimetuple();
    assert(d.year == t.tm_year);
    assert(d.month == t.tm_mon);
    assert(d.day == t.tm_mday);
    assert(11 == t.tm_hour); // 20mm + 53mm = 1hn + 13mm
    assert(13 == t.tm_min);
    assert(d.second == t.tm_sec);
    assert(d.weekday():int == t.tm_wday);
    assert(d.toordinal() - (new date(1, 1, 1)).toordinal() + 1 == t.tm_yday);
    assert(0 == t.tm_isdst);
  }

  // At the edges, UTC adjustment can normalize into years out-of-range
  // for a datetime object.  Ensure that a correct timetuple is
  // created anyway.
  var tiny = new datetime(MINYEAR, 1, 1, 0, 0, 37, tzinfo=new shared UOFS(1439));
  // That goes back 1 minute less than a full day.
  var t = tiny.utctimetuple();
  assert(t.tm_year == MINYEAR-1);
  assert(t.tm_mon == 12);
  assert(t.tm_mday == 31);
  assert(t.tm_hour == 0);
  assert(t.tm_min == 1);
  assert(t.tm_sec == 37);
  assert(t.tm_yday == 366);    // "year 0" is a leap year
  assert(t.tm_isdst == 0);

  var huge = new datetime(MAXYEAR, 12, 31, 23, 59, 37, 999999, tzinfo=new shared UOFS(-1439));
  // That goes forward 1 minute less than a full day.
  t = huge.utctimetuple();
  assert(t.tm_year == MAXYEAR+1);
  assert(t.tm_mon == 1);
  assert(t.tm_mday == 1);
  assert(t.tm_hour == 23);
  assert(t.tm_min == 58);
  assert(t.tm_sec == 37);
  assert(t.tm_yday == 1);
  assert(t.tm_isdst == 0);
}

proc test_tzinfo_isoformat() {
  var zero = new shared FixedOffset(0, "+00:00");
  var plus = new shared FixedOffset(220, "+03:40");
  var minus = new shared FixedOffset(-231, "-03:51");

  var datestr = '0001-02-03';
  for ofs in (zero, plus, minus) {
    for us in (0, 987001) {
      var d = new datetime(1, 2, 3, 4, 5, 59, us, tzinfo=ofs);
      var timestr = '04:05:59' + if us != 0 then '.987001' else '';
      var ofsstr = d.tzname();
      var tailstr = timestr + ofsstr;
      var iso = d.isoformat();
      assert(iso == datestr + 'T' + tailstr);
      assert(iso == d.isoformat('T'));
      assert(d.isoformat('k') == datestr + 'k' + tailstr);
      //assert(str(d) == datestr + ' ' + tailstr);
    }
  }
}

proc test_replace() {
  var z100 = new shared FixedOffset(100, "+100");
  var zm200 = new shared FixedOffset(new timedelta(minutes=-200), "-200");
  var args = (1, 2, 3, 4, 5, 6, 7);
  var base = new datetime((...args), z100);
  assert(base == base.replace(tzinfo=base.tzinfo));

  var i = 0;
  for (name, newval) in (("year", 2),
                         ("month", 3),
                         ("day", 4),
                         ("hour", 5),
                         ("minute", 6),
                         ("second", 7),
                         ("microsecond", 8)) {
    var newargs = args;
    newargs[i] = newval;
    var expected = new datetime((...newargs), z100);
    var got: datetime;
    if name == "year" then
      got = base.replace(year=newval, tzinfo=z100);
    else if name == "month" then
      got = base.replace(month=newval, tzinfo=z100);
    else if name == "day" then
      got = base.replace(day=newval, tzinfo=z100);
    else if name == "hour" then
      got = base.replace(hour=newval, tzinfo=z100);
    else if name == "minute" then
      got = base.replace(minute=newval, tzinfo=z100);
    else if name == "second" then
      got = base.replace(second=newval, tzinfo=z100);
    else if name == "microsecond" then
      got = base.replace(microsecond=newval, tzinfo=z100);
    assert(expected == got);
    i += 1;
  }

  { // test replacing the timezone
    var newargs = args;
    var expected = new datetime((...newargs), zm200);
    var got = base.replace(tzinfo=zm200);
    assert(expected == got);
  }

  // Ensure we can get rid of a tzinfo.
  assert(base.tzname() == "+100");
  var base2 = base.replace(tzinfo=nil);
  assert(base2.tzinfo.borrow() == nil);

  // Ensure we can add one.
  var base3 = base2.replace(tzinfo=z100);
  assert(base == base3);
  assert(base.tzinfo == base3.tzinfo);
}

proc test_more_astimezone() {
  // The inherited test_astimezone covered some trivial and error cases.
  var f44m = new shared FixedOffset(44, "44");
  var fm5h = new shared FixedOffset(-(new timedelta(hours=5)), "m300");

  var dt = datetime.now(tz=f44m);
  assert(dt.tzinfo == f44m);

  // Replacing with same tzinfo makes no change.
  var x = dt.astimezone(dt.tzinfo:shared TZInfo);
  assert(x.tzinfo == f44m);
  assert(x.getdate() == dt.getdate());
  assert(x.gettime() == dt.gettime());

  // Replacing with different tzinfo does adjust.
  var got = dt.astimezone(fm5h);
  assert(got.tzinfo == fm5h);
  assert(got.utcoffset() == new timedelta(hours=-5));
  var expected = dt - dt.utcoffset();  // in effect, convert to UTC
  expected += fm5h.utcoffset(dt);  // and from there to local time
  expected = expected.replace(tzinfo=fm5h); // and attach new tzinfo
  assert(got.getdate() == expected.getdate());
  assert(got.gettime() == expected.gettime());
  assert(got.timetz() == expected.timetz());
  assert(got.tzinfo == expected.tzinfo);
  assert(got == expected);
}

proc test_aware_subtract() {
  // Ensure that utcoffset() is ignored when the operands have the
  // same tzinfo member.
  class OperandDependentOffset: TZInfo {
    override proc utcoffset(dt: datetime) {
      if dt.minute < 10 {
        // d0 and d1 equal after adjustment
        return new timedelta(minutes=dt.minute);
      } else {
        // d2 off in the weeds
        return new timedelta(minutes=59);
      }
    }
  }

  var base = new datetime(8, 9, 10, 11, 12, 13, 14, tzinfo=new shared OperandDependentOffset());
  var d0 = base.replace(minute=3, tzinfo=base.tzinfo);
  var d1 = base.replace(minute=9, tzinfo=base.tzinfo);
  var d2 = base.replace(minute=11, tzinfo=base.tzinfo);
  for x in (d0, d1, d2) {
    for y in (d0, d1, d2) {
      var got = x - y;
      var expected = new timedelta(minutes=x.minute - y.minute);
      assert(got == expected);
    }
  }
  // OTOH, if the tzinfo members are distinct, utcoffsets aren't
  // ignored.
  base = new datetime(8, 9, 10, 11, 12, 13, 14);
  d0 = base.replace(minute=3, tzinfo=new shared OperandDependentOffset());
  d1 = base.replace(minute=9, tzinfo=new shared OperandDependentOffset());
  d2 = base.replace(minute=11, tzinfo=new shared OperandDependentOffset());
  for x in (d0, d1, d2) {
    for y in (d0, d1, d2) {
      var got = x - y;
      var expected: timedelta;
      if (x == d0 || x == d1) && (y == d0 || y == d1) then
        expected = new timedelta(0);
      else if x == y && x == d2 then
        expected = new timedelta(0);
      else if x == d2 then
        expected = new timedelta(minutes=(11-59)-0);
      else {
        assert(y == d2);
        expected = new timedelta(minutes=0-(11-59));
      }
      assert(got == expected);
    }
  }
}

proc test_mixed_compare() {
  var t1 = new datetime(1, 2, 3, 4, 5, 6, 7);
  var t2 = new datetime(1, 2, 3, 4, 5, 6, 7);
  assert(t1 == t2);
  t2 = t2.replace(tzinfo=nil);
  assert(t1 == t2);
  t2 = t2.replace(tzinfo=new shared FixedOffset(0, ""));

  // In datetime w/ identical tzinfo objects, utcoffset is ignored.
  class Varies: TZInfo {
    var offset: timedelta;
    proc init() {
      offset = new timedelta(minutes=22);
    }
    override proc utcoffset(dt: datetime) {
      offset += new timedelta(minutes=1);
      return offset;
    }
  }

  var v = new shared Varies();
  t1 = t2.replace(tzinfo=v);
  t2 = t2.replace(tzinfo=v);
  assert(t1.utcoffset() == new timedelta(minutes=23));
  assert(t2.utcoffset() == new timedelta(minutes=24));
  assert(t1 == t2);

  // But if they're not identical, it isn't ignored.
  t2 = t2.replace(tzinfo=new shared Varies());
  assert(t1 < t2);  // t1's offset counter still going up
}

proc first_sunday_on_or_after(in dt) {
  var days_to_go = 6 - dt.weekday():int;
  if days_to_go != 0 then
    dt += new timedelta(days_to_go);
  return dt;
}

test_trivial();
test_even_more_compare();
test_zones();
test_combine();
test_extract();
test_tz_aware_arithmetic();
test_tzinfo_now();
test_tzinfo_fromtimestamp();
test_tzinfo_timetuple();
test_utctimetuple();
test_tzinfo_isoformat();
test_replace();
test_more_astimezone();
test_aware_subtract();
test_mixed_compare();
