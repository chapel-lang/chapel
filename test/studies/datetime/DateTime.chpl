module DateTime {

  private proc getTimeOfDay() {
    extern "struct timeval" record timeval {
      var tv_sec: int;
      var tv_usec: int;
    }

    extern "struct timezone" record timezone {
      var tz_minuteswest: int;
      var tz_dsttime: int;
    }

    extern proc gettimeofday(ref tv: timeval, tz): int;

    var tv: timeval;
    var tz: timezone;
    if gettimeofday(tv, nil) != 0 then
      halt("error in call to gettimeofday()");
    return (tv.tv_sec, tv.tv_usec);
  }

  // This is a pretty direct port of the Python date interface

  param MINYEAR = 1;
  param MAXYEAR = 9999;

  const DAYS_IN_MONTH = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
  const DAYS_BEFORE_MONTH = init_days_before_month();

  const unixEpoch = new datetime(1970, 1, 1);

  enum DayOfWeek {
    Monday =    0,
    Tuesday =   1,
    Wednesday = 2,
    Thursday =  3,
    Friday =    4,
    Saturday =  5,
    Sunday =    6
  }

  enum ISODayOfWeek {
    Monday =    1,
    Tuesday =   2,
    Wednesday = 3,
    Thursday =  4,
    Friday =    5,
    Saturday =  6,
    Sunday =    7
  }

  private proc init_days_before_month() {
    var DBM: [1..12] int;
    for i in 2..12 {
      DBM[i] = DBM[i-1] + DAYS_IN_MONTH[i-1];
    }
    return DBM;
  }

  proc isLeapYear(year: int) {
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
  }

  proc daysInYear(year: int) {
    return 365 + if isLeapYear(year) then 1 else 0;
  }

  proc daysBeforeYear(year: int) {
    const y = year - 1;
    return y*365 + y/4 - y/100 + y/400;
  }

  proc daysInMonth(year: int, month: int) {
    if month < 1 || month > 12 then
      halt("month must be between 1 and 12");
    if month == 2 && isLeapYear(year) then
      return 29;
    else
      return DAYS_IN_MONTH(month);
  }

  proc daysBeforeMonth(year: int, month: int) {
    if month < 1 || month > 12 then
      halt("month must be between 1 and 12");
    return DAYS_BEFORE_MONTH(month) + if (month > 2 && isLeapYear(year)) then 1 else 0;
  }

  proc ymdToOrd(year: int, month: int, day: int) {
    const dim = daysInMonth(year, month);
    if month < 1 || month > 12 then
      halt("month must be between 1 and 12");
    if day < 1 || day > dim then
      halt("day must be between 1 and ", dim);
    return daysBeforeYear(year) + daysBeforeMonth(year, month) + day;
  }

  const DI400Y = daysBeforeYear(401);
  const DI100Y = daysBeforeYear(101);
  const DI4Y   = daysBeforeYear(5);

  proc ordToYmd(in n: int) {
    n -= 1;
    const n400 = n / DI400Y;
    n = n % DI400Y;
    var year = n400*400 + 1;

    const n100 = n / DI100Y;
    n = n % DI100Y;

    const n4 = n / DI4Y;
    n = n % DI4Y;

    const n1 = n / 365;
    n = n % 365;

    year += n100*100 + n4*4 + n1;

    if n1 == 4 || n100 == 4 {
      assert(n == 0);
      return (year-1, 12, 31);
    }

    const leapyear = n1 == 3 && (n4 != 24 || n100 == 3);
    assert(leapyear == isLeapYear(year));

    var month = (n+50) >> 5;
    var preceding = daysBeforeMonth(year, month);
    if preceding > n {
      month -= 1;
      preceding -= daysInMonth(year, month);
    }
    n -= preceding;
    assert(0 <= n && n < daysInMonth(year, month));
    return (year, month, n+1);
  }

  record date {
    var chpl_year, chpl_month, chpl_day: int;

    /* Construct a new date object from a year, month, and day. All
       three arguments are required and must be in valid ranges.  The
       valid ranges are:

       1 <= year <= 9999
       1 <= month <= 12
       1 <= day <= the number of days in the given month and year
     */
    proc date(year, month, day) {
      if year < MINYEAR || year > MAXYEAR then
        halt("year is out of the valid range");
      if month < 1 || month > 12 then
        halt("month is out of the valid range");
      if day < 1 || day > daysInMonth(year, month) then
        halt("day is out of the valid range");

      this.chpl_year = year;
      this.chpl_month = month;
      this.chpl_day = day;
    }

    proc year {
      return chpl_year;
    }

    proc month {
      return chpl_month;
    }

    proc day {
      return chpl_day;
    }

    proc type today() {
      const timeSinceEpoch = getTimeOfDay();
      const td = new timedelta(seconds=timeSinceEpoch(1),
                               microseconds=timeSinceEpoch(2));

      return unixEpoch.getdate() + td;
    }

    proc type fromtimestamp(timestamp) {
      const sec = timestamp: int;
      const us = ((timestamp-sec) * 1000000 + 0.5):int;
      const td = new timedelta(seconds=sec, microseconds=us);
      return unixEpoch.getDate() + td;
    }

    proc type fromordinal(ord) {
      if ord < 1 || ord > date.max.toordinal() then
        halt("ordinal out of range");
      const (y,m,d) = ordToYmd(ord);
      return new date(y,m,d);
    }

    proc type min {
      return new date(MINYEAR, 1, 1);
    }

    proc type max {
      return new date(MAXYEAR, 12, 31);
    }

    proc type resolution {
      return new timedelta(days=1);
    }

    proc replace(year=0, month=0, day=0) {
      const newYear = if year > 0 then year else this.year;
      const newMonth = if month > 0 then month else this.month;
      const newDay = if day > 0 then day else this.day;
      return new date(newYear, newMonth, newDay);
    }

    proc timetuple() {
      halt("not implemented");
      // yday is the day number within the current year
      const yday = toordinal() - (new date(year, 1, 1)).toordinal() + 1;
      // in python this would be a time.struct_time, not a tuple
      return (year, month, day, 0, 0, 0, weekday(), yday, -1);
    }

    proc toordinal() {
      return ymdToOrd(year, month, day);
    }

    /* Return the day of the week as a DayOfWeek. Monday == 0, Sunday == 6 */
    proc weekday() {
      // January 1 0001 is a Monday
      return ((toordinal() + 6) % 7):DayOfWeek;
    }

    /* Return the day of the week as an ISODayOfWeek.  Monday == 1, Sunday == 7*/
    proc isoweekday() {
      return (weekday():int + 1): ISODayOfWeek;
    }

    proc isocalendar() {
      halt("not implemented");
    }

    /* Return the date as a string in ISO 8601 format: "YYYY-MM-DD" */
    proc isoformat() {
      var yearstr = year:string;
      var monthstr = month:string;
      var daystr = day:string;

      if year < 10 then
        yearstr = "000" + yearstr;
      else if year < 100 then
        yearstr = "00" + yearstr;
      else if year < 1000 then
        yearstr = "0" + yearstr;

      if month < 10 then
        monthstr = "0" + monthstr;
      if day < 10 then
        daystr = "0" + daystr;

      return yearstr + "-" + monthstr + "-" + daystr;
    }

    proc ctime() {
      halt("not implemented");
    }
  }

  proc +(d: date, t: timedelta): date {
    return date.fromordinal(d.toordinal() + t.days);
  }

  proc -(d: date, t: timedelta): date {
    return date.fromordinal(d.toordinal() - t.days);
  }

  proc -(d1: date, d2: date): timedelta {
    return new timedelta(days=d1.toordinal() - d2.toordinal);
  }

  proc <(d1: date, d2: date) {
    return d1.toordinal() < d2.toordinal();
  }

  proc <=(d1: date, d2: date) {
    return d1.toordinal() <= d2.toordinal();
  }

  proc >(d1: date, d2: date) {
    return d1.toordinal() > d2.toordinal();
  }

  proc >=(d1: date, d2: date) {
    return d1.toordinal() >= d2.toordinal();
  }

/* The DateTime module contains a record `timedelta` to represent an amount
   of time.

   Supported operations on `timedelta` objects are:
   Adding or subtracting two `timedelta`s
   Multiplying or dividing a `timedelta` by an integer
   Comparing two `timedelta`s using relational operators
   Casting a `timedelta` object to a `string`.
 */

  /* A record representing an amount of time.  A `timedelta` has fields
     representing days, seconds, and microseconds.  These fields are always
     kept within the following ranges:

     0 <= microseconds < 1000000
     0 <= seconds < 60*60*24
     -999999999 <= days <= 999999999

     It is an overflow error if `days` is outside the given range.
   */
  record timedelta {
    var chpl_days: int;
    var chpl_seconds: int;
    var chpl_microseconds: int;
    /* Construct a timedelta object.  All arguments are optional and
       default to 0. Since only days, seconds and microseconds are
       stored, the other arguments are converted to days, seconds
       and microseconds. */
    proc timedelta(weeks=0, days=0, hours=0, minutes=0,
                   seconds=0, milliseconds=0, microseconds=0) {
      param usps = 1000000,  // microseconds per second
            uspms = 1000,    // microseconds per millisecond
             spd = 24*60*60; // seconds per day

      //       us/sec   sec/min     min/hour
      //       |        |           |
      var us = usps * ((60 * (hours*60 + minutes)) + seconds) +
               uspms * milliseconds + microseconds;
      var sec = us / usps;
      us = us % usps;

      if us < 0 {
        sec -= 1;
        us = usps + us;
      }
      var day = weeks*7 + days + sec / spd;
      sec = sec % (spd);

      if sec < 0 {
        day -= 1;
        sec = spd + sec;
      }

      this.chpl_days = day;
      this.chpl_seconds = sec;
      this.chpl_microseconds = us;

      if this.days < -999999999 then
        halt("Overflow: days < -999999999");

      if this.days > 999999999 then
        halt("Overflow: days > 999999999");
    }

    /* Return the total number of seconds represented by this object */
    proc total_seconds(): real {
      return days*(24*60*60) + seconds + microseconds / 1000000.0;
    }

    proc days {
      return chpl_days;
    }

    proc seconds {
      return chpl_seconds;
    }

    proc microseconds {
      return chpl_microseconds;
    }

    /* Return the minimum representable `timedelta` object. */
    proc type min {
      return new timedelta(days=-999999999);
    }

    /* Return the maximum representable `timedelta` object. */
    proc type max {
      return new timedelta(days=999999999, hours=23, minutes=59, seconds=59, microseconds=999999);
    }

    /* Return the smallest positive value representable by a `timedelta`
       object.
     */
    proc type resolution {
      return new timedelta(microseconds=1);
    }
  }

  proc *(i: int, t: timedelta) {
    return new timedelta(days=i*t.days, seconds=i*t.seconds, microseconds=i*t.microseconds);
  }

  proc *(t: timedelta, i: int) {
    return new timedelta(days=i*t.days, seconds=i*t.seconds, microseconds=i*t.microseconds);
  }

  proc /(t: timedelta, i: int) {
    var day = t.days / i;
    var second = t.seconds + (t.days % i)*24*60*60;
    var microsecond = t.microseconds + (second % i)*1000000;
    var us_remainder = microsecond % i;
    second /= i;
    microsecond /= i;

    if us_remainder*2 >= i then
      microsecond += 1; // round up

    return new timedelta(days=day, seconds=second, microseconds=microsecond);
  }

  proc +(t: timedelta) {
    return t;
  }

  proc -(t: timedelta) {
    return new timedelta(days=-t.days, seconds=-t.seconds, microseconds=-t.microseconds);
  }

  proc +(lhs: timedelta, rhs: timedelta) {
    return new timedelta(days=lhs.days+rhs.days,
                         seconds=lhs.seconds+rhs.seconds,
                         microseconds=lhs.microseconds+rhs.microseconds);
  }

  proc -(lhs: timedelta, rhs: timedelta) {
    return new timedelta(days=lhs.days-rhs.days,
                         seconds=lhs.seconds-rhs.seconds,
                         microseconds=lhs.microseconds-rhs.microseconds);
  }

  proc >(lhs: timedelta, rhs: timedelta) {
    const ls = (lhs.days*(24*60*60) + lhs.seconds);
    const rs = (rhs.days*(24*60*60) + rhs.seconds);
    if ls > rs then return true;
    if rs > ls then return false;
    return lhs.microseconds > rhs.microseconds;
  }

  proc >=(lhs: timedelta, rhs: timedelta) {
    return lhs > rhs || lhs == rhs;
  }

  proc <(lhs: timedelta, rhs: timedelta) {
    const ls = (lhs.days*(24*60*60) + lhs.seconds);
    const rs = (rhs.days*(24*60*60) + rhs.seconds);
    if ls < rs then return true;
    if rs < ls then return false;
    return lhs.microseconds < rhs.microseconds;
  }

  proc <=(lhs: timedelta, rhs: timedelta) {
    return lhs < rhs || lhs == rhs;
  }

  /* Return the absolute value of `t`.  If `t` is negative, then returns `-t`,
     else returns `t`.
   */
  proc abs(t: timedelta) {
    if t.days < 0 then
      return -t;
    else
      return t;
  }

  proc _cast(type s, t: timedelta) where s == string {
    var str: string;
    if t.days != 0 {
      str = t.days:string + " day";
      if t.days != 1 && t.days != -1 then
        str += "s";
      str += ", ";
    }
    const seconds = t.seconds % 60;
    const minutes = (t.seconds / 60) % 60;
    const hours = t.seconds / (60*60);
    const microseconds = t.microseconds;

    str += hours:string + ":";
    if minutes < 10 then
      str += "0";
    str += minutes + ":";
    if seconds < 10 then
      str += "0";
    str += seconds;
    if microseconds != 0 {
      str += ".";
      const usLog10 = log10(microseconds):int;
      for i in 1..(5-usLog10) {
        str += "0";
      }

      str += microseconds: string;
    }
    return str;
  }


  class TZInfo {
    proc utcoffset():timedelta {
      halt("Abstract base method called");
      return new timedelta();
    }
    proc dst():timedelta {
      halt("Abstract base method called");
      return new timedelta();
    }
    proc tzname():string {
      halt("Abstract base method called");
      return "";
    }
  }
  record time {
    var chpl_hour, chpl_minute, chpl_second, chpl_microsecond: int;
    var chpl_tzinfo: TZInfo;

    proc time(hour=0, minute=0, second=0, microsecond=0, tzinfo:TZInfo=nil) {
      if hour < 0 || hour >= 24 then
        halt("hour out of range");
      if minute < 0 || minute >= 60 then
        halt("minute out of range");
      if second < 0 || second >= 60 then
        halt("second out of range");
      if microsecond < 0 || microsecond >= 1000000 then
        halt("microsecond out of range");
      this.chpl_hour = hour;
      this.chpl_minute = minute;
      this.chpl_second = second;
      this.chpl_microsecond = microsecond;
      this.chpl_tzinfo = tzinfo;
    }

    proc ~time() {
      // delete tzinfo if needed
    }

    proc hour {
      return chpl_hour;
    }

    proc minute {
      return chpl_minute;
    }

    proc second {
      return chpl_second;
    }

    proc microsecond {
      return chpl_microsecond;
    }

    proc tzinfo {
      return chpl_tzinfo;
    }

    proc type min {
      return new time();
    }

    proc type max {
      return new time(23, 59, 59, 999999);
    }

    proc type resolution {
      return new timedelta(microseconds=1);
    }

    proc replace(hour=-1, minute=-1, second=-1, microsecond=-1, tzinfo=nil) {
      const newhour = if hour != -1 then hour else this.hour;
      const newminute = if minute != -1 then minute else this.minute;
      const newsecond = if second != -1 then second else this.second;
      const newmicrosecond = if microsecond != -1 then microsecond else this.microsecond;

      // TODO: do the right thing with tzinfo
      return new time(newhour, newminute, newsecond, newmicrosecond);
    }

    proc isoformat() {
      halt("not implemented");
    }

    proc utcoffset() {
      if tzinfo == nil then
        return new timedelta();
      else
        return tzinfo.utcoffset();
    }

    proc dst() {
      if tzinfo == nil then
        return new timedelta();
      else
        return tzinfo.dst();
    }

    proc tzname() {
      if tzinfo == nil then
        return "";
      else
        return tzinfo.tzname();
    }
  }

  proc <(t1: time, t2: time) {
    if (t1.tzinfo == nil && t2.tzinfo == nil) || t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 +
                   t1.second + t1.microsecond/1000000.0;
      const sec2 = t2.hour*3600 + t2.minute*60 +
                   t2.second + t2.microsecond/1000000.0;
      return sec1 < sec2;
    } else {
      halt("timezone not implemented");
    }
    halt("unreachable");
    return false;
  }

  proc <=(t1: time, t2: time) {
    if (t1.tzinfo == nil && t2.tzinfo == nil) || t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 +
                   t1.second + t1.microsecond/1000000.0;
      const sec2 = t2.hour*3600 + t2.minute*60 +
                   t2.second + t2.microsecond/1000000.0;
      return sec1 <= sec2;
    } else {
      halt("timezone not implemented");
    }
    halt("unreachable");
    return false;
  }

  proc >(t1: time, t2: time) {
    if (t1.tzinfo == nil && t2.tzinfo == nil) || t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 +
                   t1.second + t1.microsecond/1000000.0;
      const sec2 = t2.hour*3600 + t2.minute*60 +
                   t2.second + t2.microsecond/1000000.0;
      return sec1 > sec2;
    } else {
      halt("timezone not implemented");
    }
    halt("unreachable");
    return false;
  }

  proc >=(t1: time, t2: time) {
    if (t1.tzinfo == nil && t2.tzinfo == nil) || t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 +
                   t1.second + t1.microsecond/1000000.0;
      const sec2 = t2.hour*3600 + t2.minute*60 +
                   t2.second + t2.microsecond/1000000.0;
      return sec1 >= sec2;
    } else {
      halt("timezone not implemented");
    }
    halt("unreachable");
    return false;
  }


  record datetime {
    var chpl_date: date;
    var chpl_time: time;

    proc datetime(year, month, day,
                  hour=0, minute=0, second=0, microsecond=0,
                  tzinfo:TZInfo=nil) {
      chpl_date = new date(year, month, day);
      chpl_time = new time(hour, minute, second, microsecond, tzinfo);
    }

    proc type today() {
      extern var timezone: c_long;

      const timeSinceEpoch = getTimeOfDay();
      const td = new timedelta(seconds=timeSinceEpoch(1) - timezone,
                               microseconds=timeSinceEpoch(2));

      return unixEpoch + td;
    }

    proc type now(tz: TZInfo = nil) {
      if tz == nil {
        extern var timezone: c_long;

        const timeSinceEpoch = getTimeOfDay();
        const td = new timedelta(seconds=timeSinceEpoch(1) - timezone,
                                 microseconds=timeSinceEpoch(2));

        return unixEpoch + td;
      } else {
        halt("not implemented");
        return unixEpoch;
      }
    }

    proc type utcnow() {
      const timeSinceEpoch = getTimeOfDay();
      const td = new timedelta(seconds=timeSinceEpoch(1),
                               microseconds=timeSinceEpoch(2));
      return unixEpoch + td;
    }

    proc type fromtimestamp(timestamp: real, tz: TZInfo = nil) {
      if tz == nil {
        const sec = timestamp: int;
        const us = ((timestamp-sec) * 1000000 + 0.5):int;
        const td = new timedelta(seconds=sec, microseconds=us);
        return unixEpoch + td;
      } else {
        halt("not implemented");
      }
    }

    proc type utcfromtimestamp(timestamp) {
      halt("not implemented");
    }

    proc type fromordinal(ordinal) {
      return datetime.combine(date.fromordinal(ordinal), new time());
    }

    proc type combine(d: date, t: time) {
      return new datetime(d.year, d.month, d.day,
                          t.hour, t.minute, t.second, t.microsecond, t.tzinfo);
    }

    proc type strptime(date_string, format) {
      halt("not implemented");
    }

    proc type min {
      return combine(date.min, time.min);
    }

    proc type max {
      return combine(date.max, time.max);
    }

    proc type resolution {
      return new timedelta(microseconds=1);
    }

    proc year {
      return chpl_date.year;
    }

    proc month {
      return chpl_date.month;
    }

    proc day {
      return chpl_date.day;
    }

    proc hour {
      return chpl_time.hour;
    }

    proc minute {
      return chpl_time.minute;
    }

    proc second {
      return chpl_time.second;
    }

    proc microsecond {
      return chpl_time.microsecond;
    }

    proc tzinfo {
      return chpl_time.tzinfo;
    }

    proc getdate() {
      return chpl_date;
    }

    proc gettime() {
      return new time(hour, minute, second, microsecond);
    }

    proc timetz() {
      return chpl_time;
    }

    proc replace(year=-1, month=-1, day=-1,
                 hour=-1, minute=-1, second=-1, microsecond=-1, tzinfo=nil) {
      return combine(new date(year, month, day),
                     new time(hour, minute, second, microsecond, tzinfo));
    }

    proc astimezone(tz: TZInfo) {
      halt("not implemented");
    }
    proc utcoffset() {
      halt("not implemented");
    }
    proc dst() {
      halt("not implemented");
    }
    proc tzname() {
      halt("not implemented");
    }
    proc timetuple() {
      halt("not implemented");
      // yday is the day number within the current year
      const yday = toordinal() - (new date(year, 1, 1)).toordinal() + 1;
      // in python this would be a time.struct_time, not a tuple.
      return (year, month, day, hour, minute, second, weekday(), yday, /*is_dst=*/-1);
    }
    proc utctimetuple() {
      halt("not implemented");
    }
    proc toordinal() {
      return getdate().toordinal();
    }
    proc weekday() {
      return getdate().weekday();
    }
    proc isoweekday() {
      return getdate().isoweekday();
    }
    proc isocalendar() {
      return getdate().isocalendar();
    }
    proc isoformat(sep="T") {
      halt("not implemented");
    }
  }

  proc +(td: timedelta, dt: datetime) {
    var newmicro = dt.microsecond + td.microseconds;
    var newsec = dt.second + td.seconds;
    var newmin = dt.minute;
    var newhour = dt.hour;

    // adjust to fit
    newsec += newmicro / 1000000;
    newmicro %= 1000000;

    newmin += newsec / 60;
    newsec %= 60;

    newhour += newmin / 60;
    newmin %= 60;

    var adddays = td.days + newhour / 24;
    newhour %= 24;

    return datetime.combine(date.fromordinal(dt.getdate().toordinal()+adddays),
                            new time(hour=newhour, minute=newmin,
                                     second=newsec, microsecond=newmicro,
                                     tzinfo=dt.tzinfo));

  }

  proc +(dt: datetime, td: timedelta) {
    return td + dt;
  }

  proc -(dt: datetime, td: timedelta) {
    var deltasec  = td.seconds % 60;
    var deltamin  = (td.seconds / 60) % 60;
    var deltahour = td.seconds / (60*60);

    var newmicro = dt.microsecond - td.microseconds;
    var newsec = dt.second - deltasec;
    var newmin = dt.minute - deltamin;
    var newhour = dt.hour - deltahour;

    var subDays = td.days;

    if newmicro < 0 {
      newsec -= 1;
      newmicro += 1000000;
    }
    if newsec < 0 {
      newmin -= 1;
      newsec += 60;
    }
    if newmin < 0 {
      newhour -= 1;
      newmin += 60;
    }
    if newhour < 0 {
      subDays += 1;
      newhour += 24;
    }
    return datetime.combine(date.fromordinal(dt.getdate().toordinal()-subDays),
                            new time(hour=newhour, minute=newmin,
                                     second=newsec, microsecond=newmicro,
                                     tzinfo=dt.tzinfo));
  }

  proc -(dt1: datetime, dt2: datetime) {
    if (dt1.tzinfo != nil && dt2.tzinfo == nil) ||
       (dt1.tzinfo == nil && dt2.tzinfo != nil) {
      halt("both datetimes must both be either naive or aware");
    }
    if dt1.tzinfo == nil || dt1.tzinfo == dt2.tzinfo {
      const newmicro = dt1.microsecond - dt2.microsecond,
            newsec = dt1.second - dt2.second,
            newmin = dt1.minute - dt2.minute,
            newhour = dt1.hour - dt2.hour,
            newday = dt1.toordinal() - dt2.toordinal();
      return new timedelta(days=newday, hours=newhour, minutes=newmin,
                           seconds=newsec, microseconds=newmicro);
    } else {
      halt("timezone aware subtraction not implemented");
    }
    halt("this should be unreachable");
    return new timedelta();
  }

  proc <(dt1: datetime, dt2: datetime) {
    if (dt1.tzinfo != nil && dt2.tzinfo == nil) ||
        (dt1.tzinfo == nil && dt2.tzinfo != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if dt1.tzinfo == nil || dt1.tzinfo == dt2.tzinfo {
      const date1 = dt1.getdate(),
            date2 = dt2.getdate();
      if date1 < date2 then return true;
      else if date2 < date1 then return false;
      else return dt1.gettime() < dt2.gettime();
    } else {
      halt("timezone aware comparison not implemented");
    }
    halt("this should be unreachable");
    return false;
  }

  proc <=(dt1: datetime, dt2: datetime) {
    if (dt1.tzinfo != nil && dt2.tzinfo == nil) ||
        (dt1.tzinfo == nil && dt2.tzinfo != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if dt1.tzinfo == nil || dt1.tzinfo == dt2.tzinfo {
      const date1 = dt1.getdate(),
            date2 = dt2.getdate();
      if date1 < date2 then return true;
      else if date2 < date1 then return false;
      else return dt1.gettime() <= dt2.gettime();
    } else {
      halt("timezone aware comparison not implemented");
    }
    halt("this should be unreachable");
    return false;

  }

  proc >(dt1: datetime, dt2: datetime) {
    if (dt1.tzinfo != nil && dt2.tzinfo == nil) ||
        (dt1.tzinfo == nil && dt2.tzinfo != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if dt1.tzinfo == nil || dt1.tzinfo == dt2.tzinfo {
      const date1 = dt1.getdate(),
            date2 = dt2.getdate();
      if date1 > date2 then return true;
      else if date2 > date1 then return false;
      else return dt1.gettime() > dt2.gettime();
    } else {
      halt("timezone aware comparison not implemented");
    }
    halt("this should be unreachable");
    return false;
  }

  proc >=(dt1: datetime, dt2: datetime) {
    if (dt1.tzinfo != nil && dt2.tzinfo == nil) ||
        (dt1.tzinfo == nil && dt2.tzinfo != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if dt1.tzinfo == nil || dt1.tzinfo == dt2.tzinfo {
      const date1 = dt1.getdate(),
            date2 = dt2.getdate();
      if date1 > date2 then return true;
      else if date2 > date1 then return false;
      else return dt1.gettime() >= dt2.gettime();
    } else {
      halt("timezone aware comparison not implemented");
    }
    halt("this should be unreachable");
    return false;
  }
}
