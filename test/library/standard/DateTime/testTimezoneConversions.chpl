use DateTime;

const ZERO = new timedelta(0);
const HOUR = new timedelta(hours=1);
const DAY = new timedelta(days=1);
// In the US, DST starts at 2am (standard time) on the first Sunday in April.
var DSTSTART = new datetime(1, 4, 1, 2);
// and ends at 2am (DST time; 1am standard time) on the last Sunday of Oct,
// which is the first Sunday on or after Oct 25.  Because we view 1:MM as
// being standard time on that day, there is no spelling in local time of
// the last hour of DST (that's 1:MM DST, but 1:MM is taken as standard time).
var DSTEND = new datetime(1, 10, 25, 1);

class USTimeZone: TZInfo {
  var stdoffset: timedelta;
  var reprname: string;
  var stdname: string;
  var dstname: string;
  proc init(hours, reprname, stdname, dstname) {
    this.stdoffset = new timedelta(hours=hours);
    this.reprname = reprname;
    this.stdname = stdname;
    this.dstname = dstname;
  }

  proc __repr__() {
    return reprname;
  }

  override proc tzname(dt: datetime) {
    if dst(dt) != new timedelta(0) then
      return dstname;
    else
      return stdname;
  }

  override proc utcoffset(dt: datetime) {
    return stdoffset + dst(dt);
  }

  override proc dst(dt: datetime) {
    if dt.tzinfo.borrow() == nil {
      // An exception instead may be sensible here, in one or more of
      // the cases.
      return ZERO;
    }
    assert(dt.tzinfo.borrow() == this);

    // Find first Sunday in April.
    var start = first_sunday_on_or_after(DSTSTART.replace(year=dt.year, tzinfo=DSTSTART.tzinfo));
    assert(start.weekday():int == 6 && start.month == 4 && start.day <= 7);

    // Find last Sunday in October.
    var end = first_sunday_on_or_after(DSTEND.replace(year=dt.year, tzinfo=DSTEND.tzinfo));
    assert(end.weekday():int == 6 && end.month == 10 && end.day >= 25);

    // Can't compare naive to aware objects, so strip the timezone from
    // dt first.
    if start <= dt.replace(tzinfo=nil) && dt.replace(tzinfo=nil) < end {
      return HOUR;
    } else {
      return ZERO;
    }
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

class FixedOffset: TZInfo {
  var offset: timedelta;
  var name: string;
  var dstoffset: timedelta;
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

proc first_sunday_on_or_after(in dt) {
  var days_to_go = 6 - dt.weekday():int;
  if days_to_go > 0 {
    dt += new timedelta(days_to_go);
  }
  return dt;
}

var Eastern  = new shared USTimeZone(-5, "Eastern",  "EST", "EDT");
var Central  = new shared USTimeZone(-6, "Central",  "CST", "CDT");
var Mountain = new shared USTimeZone(-7, "Mountain", "MST", "MDT");
var Pacific  = new shared USTimeZone(-8, "Pacific",  "PST", "PDT");
var utc_real = new shared FixedOffset(0, "UTC", 0);
var utc_fake = new shared FixedOffset(-12*60, "UTCfake", 0);

// DST switch times for 2002
var dston = new datetime(2002, 4, 7, 2);
var dstoff = new datetime(2002, 10, 27, 1);

proc checkinside(dt, tz:shared, utc:shared, dston, dstoff) {
  assert(dt.dst() == HOUR);

  // Conversion to our own timezone is always an identity.
  assert(dt.astimezone(tz) == dt);

  var asutc = dt.astimezone(utc);
  var there_and_back = asutc.astimezone(tz);

  // Conversion to UTC and back isn't always an identity here,
  // because there are redundant spellings (in local time) of
  // UTC time when DST begins:  the clock jumps from 1:59:59
  // to 3:00:00, and a local time of 2:MM:SS doesn't really
  // make sense then.  The classes above treat 2:MM:SS as
  // daylight time then (it's "after 2am"), really an alias
  // for 1:MM:SS standard time.  The latter form is what
  // conversion back from UTC produces.
  if dt.getdate() == dston.getdate() && dt.hour == 2 {
    // We're in the redundant hour, and coming back from
    // UTC gives the 1:MM:SS standard-time spelling.
    assert(there_and_back + HOUR == dt);
    // Although during was considered to be in daylight
    // time, there_and_back is not.
    assert(there_and_back.dst() == ZERO);
    // They're the same times in UTC.
    assert(there_and_back.astimezone(utc) == dt.astimezone(utc));
  } else {
    // We're not in the redundant hour.
    assert(dt == there_and_back);
  }

  // Because we have a redundant spelling when DST begins, there is
  // (unforunately) an hour when DST ends that can't be spelled at all in
  // local time.  When DST ends, the clock jumps from 1:59 back to 1:00
  // again.  The hour 1:MM DST has no spelling then:  1:MM is taken to be
  // standard time.  1:MM DST == 0:MM EST, but 0:MM is taken to be
  // daylight time.  The hour 1:MM daylight == 0:MM standard can't be
  // expressed in local time.  Nevertheless, we want conversion back
  // from UTC to mimic the local clock's "repeat an hour" behavior.
  var nexthour_utc = asutc + HOUR;
  var nexthour_tz = nexthour_utc.astimezone(tz);
  if dt.getdate() == dstoff.getdate() && dt.hour == 0 {
    // We're in the hour before the last DST hour.  The last DST hour
    // is ineffable.  We want the conversion back to repeat 1:MM.
    assert(nexthour_tz == dt.replace(hour=1, tzinfo=dt.tzinfo));
    nexthour_utc += HOUR;
    nexthour_tz = nexthour_utc.astimezone(tz);
    assert(nexthour_tz == dt.replace(hour=1, tzinfo=dt.tzinfo));
  } else {
    assert(nexthour_tz - dt == HOUR);
  }
}

proc checkoutside(dt, tz:shared, utc:shared) {
  assert(dt.dst() == ZERO);

  // Conversion to our own timezone is always an identity.
  assert(dt.astimezone(tz) == dt);

  // Converting to UTC and back is an identity too.
  var asutc = dt.astimezone(utc);
  var there_and_back = asutc.astimezone(tz);
  assert(dt == there_and_back);
}

proc convert_between_tz_and_utc(tz:shared, utc:shared) {
  var mydston = dston.replace(tzinfo=tz);
  // Because 1:MM on the day DST ends is taken as being standard time,
  // there is no spelling in tz for the last hour of daylight time.
  // For purposes of the test, the last hour of DST is 0:MM, which is
  // taken as being daylight time (and 1:MM is taken as being standard
  // time).
  var mydstoff = dstoff.replace(tzinfo=tz);
  for delta in (new timedelta(weeks=13),
                DAY,
                HOUR,
                new timedelta(minutes=1),
                new timedelta(microseconds=1)) {

    checkinside(mydston, tz, utc, mydston, mydstoff);
    for during in (mydston + delta, mydstoff - delta) {
      checkinside(during, tz, utc, mydston, mydstoff);
    }

    checkoutside(mydstoff, tz, utc);
    for outside in (mydston - delta, mydstoff + delta) {
      checkoutside(outside, tz, utc);
    }
  }
}


proc test_easy() {
  // Despite the name of this test, the endcases are excruciating.
  convert_between_tz_and_utc(Eastern, utc_real);
  convert_between_tz_and_utc(Pacific, utc_real);
  convert_between_tz_and_utc(Eastern, utc_fake);
  convert_between_tz_and_utc(Pacific, utc_fake);

  // The next is really dancing near the edge.  It works because
  // Pacific and Eastern are far enough apart that their "problem
  // hours" don't overlap.
  convert_between_tz_and_utc(Eastern, Pacific);
  convert_between_tz_and_utc(Pacific, Eastern);

  // OTOH, these fail!  Don't enable them.  The difficulty is that
  // the edge case tests assume that every hour is representable in
  // the "utc" class.  This is always true for a fixed-offset tzinfo
  // class (lke utc_real and utc_fake), but not for Eastern or Central.
  // For these adjacent DST-aware time zones, the range of time offsets
  // tested ends up creating hours in the one that aren't representable
  // in the other.  For the same reason, we would see failures in the
  // Eastern vs Pacific tests too if we added 3*HOUR to the list of
  // offset deltas in convert_between_tz_and_utc().
  //
  // self.convert_between_tz_and_utc(Eastern, Central)  # can't work
  // self.convert_between_tz_and_utc(Central, Eastern)  # can't work
}

proc test_tricky() {
  // 22:00 on day before daylight starts.
  var fourback = dston - new timedelta(hours=4);
  var ninewest = new shared FixedOffset(-9*60, "-0900", 0);
  fourback = fourback.replace(tzinfo=ninewest);
  // 22:00-0900 is 7:00 UTC == 2:00 EST == 3:00 DST.  Since it's "after
  // 2", we should get the 3 spelling.
  // If we plug 22:00 the day before into Eastern, it "looks like std
  // time", so its offset is returned as -5, and -5 - -9 = 4.  Adding 4
  // to 22:00 lands on 2:00, which makes no sense in local time (the
  // local clock jumps from 1 to 3).  The point here is to make sure we
  // get the 3 spelling.
  var expected = dston.replace(hour=3, tzinfo=dston.tzinfo);
  var got = fourback.astimezone(Eastern).replace(tzinfo=nil);
  assert(expected == got);

  // Similar, but map to 6:00 UTC == 1:00 EST == 2:00 DST.  In that
  // case we want the 1:00 spelling.
  var sixutc = dston.replace(hour=6, tzinfo=utc_real);
  // Now 6:00 "looks like daylight", so the offset wrt Eastern is -4,
  // and adding -4-0 == -4 gives the 2:00 spelling.  We want the 1:00 EST
  // spelling.
  expected = dston.replace(hour=1, tzinfo=dston.tzinfo);
  got = sixutc.astimezone(Eastern).replace(tzinfo=nil);
  assert(expected == got);

  // Now on the day DST ends, we want "repeat an hour" behavior.
  //  UTC  4:MM  5:MM  6:MM  7:MM  checking these
  //  EST 23:MM  0:MM  1:MM  2:MM
  //  EDT  0:MM  1:MM  2:MM  3:MM
  // wall  0:MM  1:MM  1:MM  2:MM  against these
  for utc in (utc_real, utc_fake) {
    for tz in (Eastern, Pacific) {
      var first_std_hour = dstoff - new timedelta(hours=2); // 23:MM
      // Convert that to UTC.
      first_std_hour -= tz.borrow().utcoffset(new datetime(1,1,1));
      // Adjust for possibly fake UTC.
      var asutc = first_std_hour + utc.borrow().utcoffset(new datetime(1,1,1));
      // First UTC hour to convert; this is 4:00 when utc=utc_real &
      // tz=Eastern.
      var asutcbase = asutc.replace(tzinfo=utc);
      for tzhour in (0, 1, 1, 2) {
        var expectedbase = dstoff.replace(hour=tzhour, tzinfo=dstoff.tzinfo);
        for minute in (0, 30, 59) {
          expected = expectedbase.replace(minute=minute, tzinfo=expectedbase.tzinfo);
          asutc = asutcbase.replace(minute=minute, tzinfo=asutcbase.tzinfo);
          var astz = asutc.astimezone(tz);
          assert(astz.replace(tzinfo=nil) == expected);
        }
        asutcbase += HOUR;
      }
    }
  }
}

proc test_fromutc() {
  var now = datetime.utcnow().replace(tzinfo=utc_real);
  now = now.replace(tzinfo=Eastern);   // insert correct tzinfo
  var enow = Eastern.fromutc(now);         // doesn't blow up
  assert(enow.tzinfo == Eastern); // has right tzinfo member

  // Always converts UTC to standard time.
  class FauxUSTimeZone: USTimeZone {
    override proc fromutc(dt: datetime) {
      return dt + stdoffset;
    }

    proc init(hours, reprname, stdname, dstname) {
      super.init(hours, reprname, stdname, dstname);
    }
  }

  var FEastern  = new shared FauxUSTimeZone(-5, "FEastern",  "FEST", "FEDT");
  var FEasternRef = FEastern.borrow(): borrowed USTimeZone;

  //  UTC  4:MM  5:MM  6:MM  7:MM  8:MM  9:MM
  //  EST 23:MM  0:MM  1:MM  2:MM  3:MM  4:MM
  //  EDT  0:MM  1:MM  2:MM  3:MM  4:MM  5:MM

  // Check around DST start.
  var start = dston.replace(hour=4, tzinfo=Eastern);
  var fstart = start.replace(tzinfo=FEastern);
  for wall in (23, 0, 1, 3, 4, 5) {
    var expected = start.replace(hour=wall, tzinfo=start.tzinfo);
    if wall == 23 {
      expected -= new timedelta(days=1);
    }
    var got = Eastern.fromutc(start);
    assert(expected == got);

    expected = fstart + FEasternRef.stdoffset;
    got = FEastern.fromutc(fstart);
    assert(expected == got);

    // Ensure astimezone() calls fromutc() too.
    got = fstart.replace(tzinfo=utc_real).astimezone(FEastern);
    assert(expected == got);

    start += HOUR;
    fstart += HOUR;
  }

  // Check around DST end.
  start = dstoff.replace(hour=4, tzinfo=Eastern);
  fstart = start.replace(tzinfo=FEastern);
  for wall in (0, 1, 1, 2, 3, 4) {
    var expected = start.replace(hour=wall, tzinfo=start.tzinfo);
    var got = Eastern.fromutc(start);
    assert(expected == got);

    expected = fstart + FEasternRef.stdoffset;
    got = FEastern.fromutc(fstart);
    assert(expected == got);

    // Ensure astimezone() calls fromutc() too.
    got = fstart.replace(tzinfo=utc_real).astimezone(FEastern);
    assert(expected == got);

    start += HOUR;
    fstart += HOUR;
  }
}

test_easy();
test_tricky();
test_fromutc();
