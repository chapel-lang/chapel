module DateTime {

  param MINYEAR = 1;
  param MAXYEAR = 9999;

  const DAYS_IN_MONTH = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
  const DAYS_BEFORE_MONTH = init_days_before_month();

  const unixEpoch = new datetime(1970, 1, 1);

  const DI400Y = daysBeforeYear(401);
  const DI100Y = daysBeforeYear(101);
  const DI4Y   = daysBeforeYear(5);

  // This is here to work around issue #5267
  const chpl_today = datetime.today();

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


  private proc getTimeOfDay() {
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

  extern "struct tm" record tm {
    var tm_sec:   c_int; //   seconds [0,61]
    var tm_min:   c_int; //   minutes [0,59]
    var tm_hour:  c_int; //  hour [0,23]
    var tm_mday:  c_int; //  day of month [1,31]
    var tm_mon:   c_int; //   month of year [0,11]
    var tm_year:  c_int; //  years since 1900
    var tm_wday:  c_int; //  day of week [0,6] (Sunday = 0)
    var tm_yday:  c_int; //  day of year [0,365]
    var tm_isdst: c_int; // daylight savings flag
  }

  private proc getLocalTime(t: 2*int) {
    extern type time_t;

    extern proc localtime_r(const ref t: time_t, ref resultp: tm): void;

    const t = getTimeOfDay();
    const t1:time_t = __primitive("cast", time_t, t(1));
    var breakDownTime: tm;

    localtime_r(t1, breakDownTime);
    return breakDownTime;
  }

  private proc init_days_before_month() {
    var DBM: [1..12] int;
    for i in 2..12 {
      DBM[i] = DBM[i-1] + DAYS_IN_MONTH[i-1];
    }
    return DBM;
  }

  private proc daysBeforeYear(year: int) {
    const y = year - 1;
    return y*365 + y/4 - y/100 + y/400;
  }

  private proc daysBeforeMonth(year: int, month: int) {
    if month < 1 || month > 12 then
      halt("month must be between 1 and 12");
    return DAYS_BEFORE_MONTH(month) + if (month > 2 && isLeapYear(year)) then 1 else 0;
  }

  private proc ymdToOrd(year: int, month: int, day: int) {
    const dim = daysInMonth(year, month);
    if month < 1 || month > 12 then
      halt("month must be between 1 and 12");
    if day < 1 || day > dim then
      halt("day must be between 1 and ", dim);
    return daysBeforeYear(year) + daysBeforeMonth(year, month) + day;
  }

  private proc ordToYmd(in n: int) {
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

  /* Return true if `year` is a leap year */
  proc isLeapYear(year: int) {
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
  }

  /* Return the number of days in month `month` during the year `year`.
     The number for a month can change from year to year due to leap years. */
  proc daysInMonth(year: int, month: int) {
    if month < 1 || month > 12 then
      halt("month must be between 1 and 12");
    if month == 2 && isLeapYear(year) then
      return 29;
    else
      return DAYS_IN_MONTH(month);
  }


  record date {
    var chpl_year, chpl_month, chpl_day: int;

    proc year {
      return chpl_year;
    }

    proc month {
      return chpl_month;
    }

    proc day {
      return chpl_day;
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
  }


  /* constructors/factories for date values */

  /* Construct a new date object from a year, month, and day. All
     three arguments are required and must be in valid ranges.  The
     valid ranges are:

     1 <= year <= 9999
     1 <= month <= 12
     1 <= day <= the number of days in the given month and year
  */
  proc date.date(year, month, day) {
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

  proc type date.today() {
    const timeSinceEpoch = getTimeOfDay();
    const td = new timedelta(seconds=timeSinceEpoch(1),
                             microseconds=timeSinceEpoch(2));

    return unixEpoch.getdate() + td;
  }

  proc type date.fromtimestamp(timestamp) {
    const sec = timestamp: int;
    const us = ((timestamp-sec) * 1000000 + 0.5):int;
    const td = new timedelta(seconds=sec, microseconds=us);
    return unixEpoch.getdate() + td;
  }

  proc type date.fromordinal(ord) {
    if ord < 1 || ord > date.max.toordinal() then
      halt("ordinal out of range");
    const (y,m,d) = ordToYmd(ord);
    return new date(y,m,d);
  }


  /* Methods on date values */

  proc date.replace(year=0, month=0, day=0) {
    const newYear = if year > 0 then year else this.year;
    const newMonth = if month > 0 then month else this.month;
    const newDay = if day > 0 then day else this.day;
    return new date(newYear, newMonth, newDay);
  }

  proc date.timetuple() {
    var timeStruct: tm;

    // no 0 init for extern records?
    timeStruct.tm_hour = 0;
    timeStruct.tm_min = 0;
    timeStruct.tm_sec = 0;

    timeStruct.tm_mday = day:int(32);
    timeStruct.tm_mon = month:int(32);
    timeStruct.tm_year = year:int(32);
    timeStruct.tm_wday = weekday():int(32);
    timeStruct.tm_yday = (toordinal() - (new date(year, 1, 1)).toordinal() + 1):int(32);
    timeStruct.tm_isdst = -1:int(32);
    return timeStruct; 
  }

  proc date.toordinal() {
    return ymdToOrd(year, month, day);
  }

  /* Return the day of the week as a DayOfWeek. Monday == 0, Sunday == 6 */
  proc date.weekday() {
    // January 1 0001 is a Monday
    return ((toordinal() + 6) % 7):DayOfWeek;
  }

  /* Return the day of the week as an ISODayOfWeek. Monday==1, Sunday==7 */
  proc date.isoweekday() {
    return (weekday():int + 1): ISODayOfWeek;
  }

  proc date.isocalendar() {
    proc findThursday(d: date) {
      var wd = d.weekday();
      return d + new timedelta(days = (DayOfWeek.Thursday - wd): int);
    }

    proc findyear(d: date) {
      return findThursday(d).year;
    }

    proc findFirstDayOfYear(year) {
      var thu = findThursday((new date(year, 1, 1)));
      if thu.year < year {
        return thu + new timedelta(days=4);
      } else { // thu.year == year
        return thu + new timedelta(days=-3);
      }
    }

    const y = findyear(this);
    const firstDay = findFirstDayOfYear(y);
    const delta = this - firstDay;

    return (y, 1+delta.days/7, isoweekday():int);
  }

  /* Return the date as a string in ISO 8601 format: "YYYY-MM-DD" */
  proc date.isoformat() {
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

  proc date.ctime() {
    const month = strftime("%b");
    const wday = strftime("%a");

    const extraSpace = if day < 10 then " " else "";
    return wday + " " + month +
           " " + extraSpace + day + " 00:00:00 " + year;
  }

  proc date.strftime(fmt: string) {
    extern proc strftime(s: c_void_ptr, size: size_t, format: c_string, ref timeStruct: tm);
    const bufLen: size_t = 100;
    var buf: [1..bufLen] c_char;
    var timeStruct: tm;

    timeStruct.tm_year = (year-1900): int(32); // 1900 based
    timeStruct.tm_mon = (month-1): int(32);    // 0 based
    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_wday = (weekday():int(32) + 1) % 7; // shift Sunday to 0
    timeStruct.tm_yday = (this - new date(year, 1, 1)).days: int(32);

    strftime(c_ptrTo(buf), bufLen, fmt.c_str(), timeStruct);
    var str = __primitive("cast", c_string, c_ptrTo(buf)): string;

    return str;
  }


  /* Operators on date values */

  proc +(d: date, t: timedelta): date {
    return date.fromordinal(d.toordinal() + t.days);
  }

  proc +(t: timedelta, d: date): date {
    return d + t;
  }

  proc -(d: date, t: timedelta): date {
    return date.fromordinal(d.toordinal() - t.days);
  }

  proc -(d1: date, d2: date): timedelta {
    return new timedelta(days=d1.toordinal() - d2.toordinal());
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


  record time {
    var chpl_hour, chpl_minute, chpl_second, chpl_microsecond: int;
    var chpl_tzinfo: TZInfo;

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
  }

  /* constructors/factories for time values */

  proc time.time(hour=0, minute=0, second=0, microsecond=0,
                 tzinfo:TZInfo=nil) {
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

  proc time.~time() {
    // delete tzinfo if needed
  }

  /* Methods on time values */

  proc time.replace(hour=-1, minute=-1, second=-1, microsecond=-1,
                    tzinfo:TZInfo) {
    const newhour = if hour != -1 then hour else this.hour;
    const newminute = if minute != -1 then minute else this.minute;
    const newsecond = if second != -1 then second else this.second;
    const newmicrosecond = if microsecond != -1 then microsecond else this.microsecond;

    return new time(newhour, newminute, newsecond, newmicrosecond, tzinfo);
  }

  proc time.isoformat() {
    proc makeNDigits(n, d) {
      var ret = d:string;
      while ret.length < n {
        ret = "0" + ret;
      }
      return ret;
    }

    var ret = makeNDigits(2, hour) + ":" +
              makeNDigits(2, minute) + ":" +
              makeNDigits(2, second);

    if microsecond != 0 {
      ret = ret + "." + makeNDigits(6, microsecond);
    }
    const offset = utcoffset();
    if offset.seconds != 0 {
      ret = ret + " " + makeNDigits(2, offset.seconds/(60*60)) + ":" +
                        makeNDigits(2, offset.seconds % (60*60) / 60);
    }
    return ret;
  }

  proc time.utcoffset() {
    if tzinfo == nil {
      return new timedelta();
    } else {
      // TODO: What should this do? python's time.utcoffset() seems to
      // return None even when tzinfo is set to e.g. central or pacific
      // >>> central = pytz.timezone("US/Central")
      // >>> time(12, 3, 4, 5, tzinfo=central).utcoffset()

      //return tzinfo.utcoffset();
      return new timedelta();
    }
  }

  proc time.dst() {
    if tzinfo == nil {
      return new timedelta();
    } else {
      // TODO: What should this do? python's time.utcoffset() seems to
      // return None even when tzinfo is set to e.g. central or pacific
      // >>> central = pytz.timezone("US/Central")
      // >>> time(12, 3, 4, 5, tzinfo=central).dst()

      //return tzinfo.dst();
      return new timedelta();
    }
  }

  proc time.tzname() {
    if tzinfo == nil then
      return "";
    else
      return tzinfo.tzname();
  }

  proc time.strftime(fmt: string) {
    extern proc strftime(s: c_void_ptr, size: size_t, format: c_string, ref timeStruct: tm);
    const bufLen: size_t = 100;
    var buf: [1..bufLen] c_char;
    var timeStruct: tm;

    timeStruct.tm_sec = second: int(32);
    timeStruct.tm_min = minute: int(32);
    timeStruct.tm_hour = hour: int(32);

    strftime(c_ptrTo(buf), bufLen, fmt.c_str(), timeStruct);
    var str = __primitive("cast", c_string, c_ptrTo(buf)): string;

    return str;
  }

  /* Operators on time values */

  proc <(t1: time, t2: time): bool {
    if (t1.tzinfo != nil && t2.tzinfo == nil) ||
        (t1.tzinfo == nil && t2.tzinfo != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 +
                   t1.second + t1.microsecond/1000000.0;
      const sec2 = t2.hour*3600 + t2.minute*60 +
                   t2.second + t2.microsecond/1000000.0;
      return sec1 < sec2;
    } else {
      // As far as I can tell, python's datetime.time() comparisons don't
      // pay attention to the timezones.
      // >>> central = pytz.timezone("US/Central")
      // >>> pacific = pytz.timezone("US/Pacific")
      // >>> datetime.time(12,3,4,5,tzinfo=central) >
      //     datetime.time(12,3,4,5,tzinfo=pacific)
      // False
      // >>> datetime.time(12,3,4,6,tzinfo=central) >
      //     datetime.time(12,3,4,5,tzinfo=pacific)
      // True
      //
      // This compares the time on a specific date, and factors in the
      // time zones.
      const dt1 = datetime.combine(new date(1900, 1, 1), t1);
      const dt2 = datetime.combine(new date(1900, 1, 1), t2);
      return dt1 < dt2;
      //return (t1.replace(tzinfo=nil) - t1.utcoffset()) <
      //       (t2.replace(tzinfo=nil) - t2.utcoffset());
    }
    halt("unreachable");
    return false;
  }

  proc <=(t1: time, t2: time): bool {
    if (t1.tzinfo != nil && t2.tzinfo == nil) ||
        (t1.tzinfo == nil && t2.tzinfo != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 +
                   t1.second + t1.microsecond/1000000.0;
      const sec2 = t2.hour*3600 + t2.minute*60 +
                   t2.second + t2.microsecond/1000000.0;
      return sec1 <= sec2;
    } else {
      const dt1 = datetime.combine(new date(1900, 1, 1), t1);
      const dt2 = datetime.combine(new date(1900, 1, 1), t2);
      return dt1 <= dt2;
      //return (t1.replace(tzinfo=nil) - t1.utcoffset()) <=
      //       (t2.replace(tzinfo=nil) - t2.utcoffset());
    }
    halt("unreachable");
    return false;
  }

  proc >(t1: time, t2: time): bool {
    if (t1.tzinfo != nil && t2.tzinfo == nil) ||
        (t1.tzinfo == nil && t2.tzinfo != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 +
                   t1.second + t1.microsecond/1000000.0;
      const sec2 = t2.hour*3600 + t2.minute*60 +
                   t2.second + t2.microsecond/1000000.0;
      return sec1 > sec2;
    } else {
      const dt1 = datetime.combine(new date(1900, 1, 1), t1);
      const dt2 = datetime.combine(new date(1900, 1, 1), t2);
      return dt1 > dt2;

      //return (t1.replace(tzinfo=nil) - t1.utcoffset()) >
      //       (t2.replace(tzinfo=nil) - t2.utcoffset());
    }
    halt("unreachable");
    return false;
  }

  proc >=(t1: time, t2: time): bool {
    if (t1.tzinfo != nil && t2.tzinfo == nil) ||
        (t1.tzinfo == nil && t2.tzinfo != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 +
                   t1.second + t1.microsecond/1000000.0;
      const sec2 = t2.hour*3600 + t2.minute*60 +
                   t2.second + t2.microsecond/1000000.0;
      return sec1 >= sec2;
    } else {
      const dt1 = datetime.combine(new date(1900, 1, 1), t1);
      const dt2 = datetime.combine(new date(1900, 1, 1), t2);
      return dt1 >= dt2;
      // can't subtract time - timedelta
      //return (t1.replace(tzinfo=nil) - t1.utcoffset()) >=
      //       (t2.replace(tzinfo=nil) - t2.utcoffset());
    }
    halt("unreachable");
    return false;
  }


  record datetime {
    var chpl_date: date;
    var chpl_time: time;

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
  }

  /* Constructors/factories for datetime values */

  proc datetime.datetime(year, month, day,
                hour=0, minute=0, second=0, microsecond=0,
                tzinfo:TZInfo=nil) {
    chpl_date = new date(year, month, day);
    chpl_time = new time(hour, minute, second, microsecond, tzinfo);
  }

  proc type datetime.today() {
    return this.now();
  }

  proc type datetime.now(tz: TZInfo = nil) {
    if tz == nil {
      const timeSinceEpoch = getTimeOfDay();
      const lt = getLocalTime(timeSinceEpoch);
      return new datetime(year=lt.tm_year+1900, month=lt.tm_mon+1,
                          day=lt.tm_mday,       hour=lt.tm_hour,
                          minute=lt.tm_min,     second=lt.tm_sec,
                          microsecond=timeSinceEpoch(2));
    } else {
      const timeSinceEpoch = getTimeOfDay();
      const td = new timedelta(seconds=timeSinceEpoch(1),
                               microseconds=timeSinceEpoch(2));
      const utcNow = unixEpoch + td;

      return (utcNow + tz.utcoffset(utcNow)).replace(tzinfo=tz);
    }
  }

  proc type datetime.utcnow() {
    const timeSinceEpoch = getTimeOfDay();
    const td = new timedelta(seconds=timeSinceEpoch(1),
                             microseconds=timeSinceEpoch(2));
    return unixEpoch + td;
  }

  proc type datetime.fromtimestamp(timestamp: real, tz: TZInfo = nil) {
    if tz == nil {
      var t = (timestamp:int, ((timestamp-timestamp:int)*1000000):int);
      const lt = getLocalTime(t);
      return new datetime(year=lt.tm_year+1900, month=lt.tm_mon+1,
                          day=lt.tm_mday,       hour=lt.tm_hour,
                          minute=lt.tm_min,     second=lt.tm_sec,
                          microsecond=t(2));
    } else {
      return (utcfromtimestamp(timestamp) + tz.utcoffset()).replace(tzinfo=tz);
    }
  }

  proc type datetime.utcfromtimestamp(timestamp) {
    return unixEpoch + new timedelta(seconds=timestamp);
  }

  proc type datetime.fromordinal(ordinal) {
    return datetime.combine(date.fromordinal(ordinal), new time());
  }

  proc type datetime.combine(d: date, t: time) {
    return new datetime(d.year, d.month, d.day,
                        t.hour, t.minute, t.second, t.microsecond, t.tzinfo);
  }

  /* Methods on datetime values */

  proc datetime.getdate() {
    return chpl_date;
  }

  proc datetime.gettime() {
    if chpl_time.tzinfo == nil then
      return chpl_time;
    else
      return new time(hour=hour, minute=minute,
                      second=second, microsecond=microsecond);
  }

  proc datetime.timetz() {
    return chpl_time;
  }

  proc datetime.replace(year=-1, month=-1, day=-1,
                        hour=-1, minute=-1, second=-1, microsecond=-1,
                        tzinfo: TZInfo) {
    return datetime.combine(
      new date(if year == -1 then this.year else year,
               if month == -1 then this.month else month,
               if day == -1 then this.day else day),
      new time(if hour == -1 then this.hour else hour,
               if minute == -1 then this.minute else minute,
               if second == -1 then this.second else second,
               if microsecond == -1 then this.microsecond else microsecond,
               tzinfo));
  }

  proc datetime.astimezone(tz: TZInfo) {
    if tzinfo == tz {
      return this;
    }
    const utc = (this - this.utcoffset()).replace(tzinfo=tz);
    return tz.fromutc(utc);
  }

  proc datetime.utcoffset() {
    if tzinfo == nil {
      halt("utcoffset called on naive datetime");
    } else {
      return tzinfo.utcoffset(this);
    }
  }

  proc datetime.dst() {
    if tzinfo == nil then
      halt("dst() called with nil tzinfo");
    return tzinfo.dst(this);
  }

  proc datetime.tzname() {
    if tzinfo == nil then
      halt("tzname() called with nil tzinfo");
    return tzinfo.tzname(this);
  }

  proc datetime.timetuple() {
    var timeStruct: tm;
    timeStruct.tm_sec = second: int(32);
    timeStruct.tm_min = minute: int(32);
    timeStruct.tm_hour = hour: int(32);
    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_mon = month: int(32);
    timeStruct.tm_year = year: int(32);
    timeStruct.tm_wday = weekday(): int(32);
    timeStruct.tm_yday = (toordinal() - (new date(year, 1, 1)).toordinal() + 1): int(32);

    if tzinfo == nil {
      timeStruct.tm_isdst = -1;
    } else if dst() == new timedelta(0) {
      timeStruct.tm_isdst = 0;
    } else {
      timeStruct.tm_isdst = 1;
    }

    return timeStruct;
  }

  proc datetime.utctimetuple() {
    if tzinfo == nil {
      var ret = timetuple();
      ret.tm_isdst = 0;
      return ret;
    } else {
      const utc = this.replace(tzinfo=nil) - utcoffset();
      var ret = utc.timetuple();
      ret.tm_isdst = 0;
      return ret;
    }
  }

  proc datetime.toordinal() {
    return getdate().toordinal();
  }

  proc datetime.weekday() {
    return getdate().weekday();
  }

  proc datetime.isoweekday() {
    return getdate().isoweekday();
  }

  proc datetime.isocalendar() {
    return getdate().isocalendar();
  }

  proc datetime.isoformat(sep="T") {
    proc zeroPad(nDigits: int, i: int) {
      var numStr = i:string;
      for i in 1..nDigits-numStr.length {
        numStr = "0" + numStr;
      }
      return numStr;
    }
    var micro = if microsecond > 0 then "." + zeroPad(6, microsecond) else "";
    var offset: string;
    if tzinfo != nil {
      offset = strftime("%z");
      offset = offset(1..3) + ":" + offset(4..5);
    }

    return strftime("%Y-%m-%d" + sep + "%H:%M:%S" + micro + offset);
  }

  proc type datetime.strptime(date_string: string, format: string) {
    extern proc strptime(buf: c_string, format: c_string, ref ts: tm);
    var timeStruct: tm;
    strptime(date_string.c_str(), format.c_str(), timeStruct);
    return new datetime(timeStruct.tm_year + 1900,
                        timeStruct.tm_mon + 1,
                        timeStruct.tm_mday,
                        timeStruct.tm_hour,
                        timeStruct.tm_min,
                        timeStruct.tm_sec);
  }


  proc datetime.strftime(fmt: string) {
    extern proc strftime(s: c_void_ptr, size: size_t, format: c_string, ref timeStruct: tm);
    const bufLen: size_t = 100;
    var buf: [1..bufLen] c_char;
    var timeStruct: tm;

    timeStruct.tm_hour = hour: int(32);
    timeStruct.tm_min = minute: int(32);
    timeStruct.tm_sec = second: int(32);

    if tzinfo != nil then
      timeStruct.tm_isdst = tzinfo.dst(this).seconds:int(32);
    else
      timeStruct.tm_isdst = -1: int(32);

    timeStruct.tm_year = (year-1900): int(32); // 1900 based
    timeStruct.tm_mon = (month-1): int(32);    // 0 based
    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_wday = (weekday():int(32) + 1) % 7; // shift Sunday to 0
    timeStruct.tm_yday = (this - new datetime(year, 1, 1)).days: int(32);

    strftime(c_ptrTo(buf), bufLen, fmt.c_str(), timeStruct);
    var str = __primitive("cast", c_string, c_ptrTo(buf)): string;

    return str;
  }


  /* Operators on datetime values */

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

  proc -(dt1: datetime, dt2: datetime): timedelta {
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
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) -
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
    halt("this should be unreachable");
    return new timedelta();
  }

  proc <(dt1: datetime, dt2: datetime): bool {
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
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) <
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
    halt("this should be unreachable");
    return false;
  }

  proc <=(dt1: datetime, dt2: datetime): bool {
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
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) <=
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
    halt("this should be unreachable");
    return false;
  }

  proc >(dt1: datetime, dt2: datetime): bool {
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
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) >
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
    halt("this should be unreachable");
    return false;
  }

  proc >=(dt1: datetime, dt2: datetime): bool {
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
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) >=
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
    halt("this should be unreachable");
    return false;
  }

// TIMEDELTA



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
      return new timedelta(days=999999999, hours=23, minutes=59,
                           seconds=59, microseconds=999999);
    }

    /* Return the smallest positive value representable by a `timedelta`
       object.
     */
    proc type resolution {
      return new timedelta(microseconds=1);
    }
  }

  /* Constructors/factories for timedelta values */

  /* Construct a timedelta object.  All arguments are optional and
     default to 0. Since only days, seconds and microseconds are
     stored, the other arguments are converted to days, seconds
     and microseconds. */
  proc timedelta.timedelta(days=0, seconds=0, microseconds=0,
                           milliseconds=0, minutes=0, hours=0, weeks=0) {
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

  proc timedelta.timedelta(timestamp: real) {
    return new timedelta(seconds = timestamp:int, microseconds=((timestamp - timestamp:int)*1000000):int);
  }


  /* Methods on timedelta values */

  /* Return the total number of seconds represented by this object */
  proc timedelta.total_seconds(): real {
    return days*(24*60*60) + seconds + microseconds / 1000000.0;
  }


  /* Operators on timedelta values */

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

  /* Abstract base class for time zones */
  class TZInfo {
    proc utcoffset(dt: datetime):timedelta {
      halt("Abstract base method called");
      return new timedelta();
    }
    proc dst(dt: datetime):timedelta {
      halt("Abstract base method called");
      return new timedelta();
    }
    proc tzname(dt: datetime):string {
      halt("Abstract base method called");
      return "";
    }
    proc fromutc(in dt: datetime): datetime {
      halt("Abstract base method called");
      return new datetime(0,0,0);
    }
  }
}
