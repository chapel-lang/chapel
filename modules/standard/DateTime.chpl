/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Support for representing dates, times, combined dates and times and
   timedeltas.  This module is modeled heavily off of the python module
   'datetime'.

   For more detailed descriptions of this functionality, please see the
   python docs:

   https://docs.python.org/2.7/library/datetime.html

   Operators are supported for adding, subtracting, and comparing dates,
   times, datetimes and timedeltas.

   Operators are also supported for multiplying and dividing timedeltas.
 */

module DateTime {
  private use HaltWrappers only ;
  private use SysCTypes;

  /* The minimum year allowed in `date` objects */
  param MINYEAR = 1;
  /* The maximum year allowed in `date` objects */
  param MAXYEAR = 9999;

  private const DAYS_IN_MONTH = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
  private const DAYS_BEFORE_MONTH = init_days_before_month();

  /* The Unix Epoch date and time */
  const unixEpoch = new datetime(1970, 1, 1);

  private const DI400Y = daysBeforeYear(401);
  private const DI100Y = daysBeforeYear(101);
  private const DI4Y   = daysBeforeYear(5);

  /* Days in the week, starting with `Monday` = 0 */
  enum DayOfWeek {
    Monday =    0,
    Tuesday =   1,
    Wednesday = 2,
    Thursday =  3,
    Friday =    4,
    Saturday =  5,
    Sunday =    6
  }

  /* Days in the week, starting with `Monday` = 1 */
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
    var ret = gettimeofday(tv, c_nil);
    assert(ret == 0);
    return (tv.tv_sec, tv.tv_usec);
  }

  private proc tm_zoneType type {
    if CHPL_TARGET_PLATFORM == "darwin" then
      return c_ptr(c_char); // char *
    else
      return c_string; // const char *
  }

  pragma "no doc"
  extern "struct tm" record tm {
    var tm_sec:    c_int;         // seconds [0,61]
    var tm_min:    c_int;         // minutes [0,59]
    var tm_hour:   c_int;         // hour [0,23]
    var tm_mday:   c_int;         // day of month [1,31]
    var tm_mon:    c_int;         // month of year [0,11]
    var tm_year:   c_int;         // years since 1900
    var tm_wday:   c_int;         // day of week [0,6] (Sunday = 0)
    var tm_yday:   c_int;         // day of year [0,365]
    var tm_isdst:  c_int;         // daylight savings flag
    var tm_gmtoff: c_long;        // Seconds east of UTC
    var tm_zone:   tm_zoneType; // Timezone abbreviation
  }

  private proc assertInRange(val, low, high) {
    assert(val >= low && val <= high);
  }

  private proc getLocalTime(t: 2*int) {
    extern type time_t;

    extern proc localtime_r(const ref t: time_t, ref resultp: tm): void;

    const t1: time_t = __primitive("cast", time_t, t(1));
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

  // assumes callee has checked for valid month range
  private proc daysBeforeMonth(year: int, month: int) {
    assertInRange(month, 1, 12);
    return DAYS_BEFORE_MONTH(month) + if (month > 2 && isLeapYear(year)) then 1 else 0;
  }

  // assumes callee has checked for valid month/day ranges
  private proc ymdToOrd(year: int, month: int, day: int) {
    assertInRange(month, 1, 12);
    const dim = try! daysInMonth(year, month);
    assertInRange(day, 1, dim);
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
      if month == 0 {
        month = 12;
        year -= 1;
      }
      const dim = try! daysInMonth(year, month);
      preceding -= dim;
    }
    n -= preceding;
    const dim = try! daysInMonth(year, month);
    assertInRange(n+1, 1, dim);
    return (year, month, n+1);
  }

  /* Return true if `year` is a leap year */
  proc isLeapYear(year: int) {
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
  }

  /* Return the number of days in month `month` during the year `year`.
     The number for a month can change from year to year due to leap years.

     :throws IllegalArgumentError: Thrown if `month` is out of range.
*/
  proc daysInMonth(year: int, month: int) throws {
    if month < 1 || month > 12 then
      throw new owned IllegalArgumentError("month must be between 1 and 12");
    if month == 2 && isLeapYear(year) then
      return 29;
    else
      return DAYS_IN_MONTH(month);
  }

/* A record representing a date */
  record date {
    pragma "no doc"
    var chpl_year, chpl_month, chpl_day: int;

    /* The year represented by this `date` value */
    proc year {
      return chpl_year;
    }

    /* The month represented by this `date` value */
    proc month {
      return chpl_month;
    }

    /* The day represented by this `date` value */
    proc day {
      return chpl_day;
    }

    /* The minimum representable `date` */
    proc type min {
      return new date(MINYEAR, 1, 1);
    }

    /* The maximum representable `date` */
    proc type max {
      return new date(MAXYEAR, 12, 31);
    }

    /* The minimum non-zero difference between two dates */
    proc type resolution {
      return new timedelta(days=1);
    }
  }


  /* initializers/factories for date values */

  pragma "no doc"
  proc date.init() {
  }

  /* Initialize a new `date` value from a `year`, `month`, and `day`. All
     three arguments are required and must be in valid ranges.  The
     valid ranges are:

     1 <= `year` <= 9999

     1 <= `month` <= 12

     1 <= `day` <= the number of days in the given month and year
  */
  proc date.init(year, month, day) {
    if year < MINYEAR-1 || year > MAXYEAR+1 then
      HaltWrappers.initHalt("year is out of the valid range");
    if month < 1 || month > 12 then
      HaltWrappers.initHalt("month is out of the valid range");
    const dim = try! daysInMonth(year, month);
    if day < 1 || day > dim then
      HaltWrappers.initHalt("day is out of the valid range");

    this.chpl_year = year;
    this.chpl_month = month;
    this.chpl_day = day;
  }

  /* A `date` object representing the current day */
  proc type date.today() {
    const timeSinceEpoch = getTimeOfDay();
    const td = new timedelta(seconds=timeSinceEpoch(1),
                             microseconds=timeSinceEpoch(2));

    return unixEpoch.getdate() + td;
  }

  /* The date that is `timestamp` seconds from the epoch */
  proc type date.fromtimestamp(timestamp) {
    const sec = timestamp: int;
    const us = ((timestamp-sec) * 1000000 + 0.5): int;
    const td = new timedelta(seconds=sec, microseconds=us);
    return unixEpoch.getdate() + td;
  }

  /* The `date` that is `ord` days from 1-1-0001 */
  proc type date.fromordinal(ord) {
    if ord < 0 || ord > 1+date.max.toordinal() then
      halt("ordinal (", ord, ") out of range");
    const (y,m,d) = ordToYmd(ord);
    return new date(y,m,d);
  }


  /* Methods on date values */

  /* Replace the `year`, `month` and/or `day` in a date to create a
     new `date` */
  proc date.replace(year=0, month=0, day=0) {
    const newYear = if year > 0 then year else this.year;
    const newMonth = if month > 0 then month else this.month;
    const newDay = if day > 0 then day else this.day;
    return new date(newYear, newMonth, newDay);
  }

  /* Return a filled record matching the C `struct tm` type for the given date */
  proc date.timetuple() {
    var timeStruct: tm;

    timeStruct.tm_hour = 0;
    timeStruct.tm_min = 0;
    timeStruct.tm_sec = 0;

    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_mon = month: int(32);
    timeStruct.tm_year = year: int(32);
    timeStruct.tm_wday = weekday(): int(32);
    timeStruct.tm_yday = (toordinal() - (new date(year, 1, 1)).toordinal() + 1): int(32);
    timeStruct.tm_isdst = (-1): int(32);
    return timeStruct;
  }

  /* Return the number of days since 1-1-0001 this `date` represents */
  proc date.toordinal() {
    return ymdToOrd(year, month, day);
  }

  /* Return the day of the week as a `DayOfWeek`.
     `Monday` == 0, `Sunday` == 6
   */
  proc date.weekday() {
    // January 1 0001 is a Monday
    return ((toordinal() + 6) % 7): DayOfWeek;
  }

  /* Return the day of the week as an `ISODayOfWeek`.
     `Monday` == 1, `Sunday` == 7 */
  proc date.isoweekday() {
    return (weekday(): int + 1): ISODayOfWeek;
  }

  /* Return the ISO date as a tuple containing the ISO year, ISO week number,
     and ISO day of the week
   */
  proc date.isocalendar() {
    proc findThursday(d: date) {
      var wd = d.weekday();
      return d + new timedelta(days = (DayOfWeek.Thursday:int - wd:int));
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

    return (y, 1+delta.days/7, isoweekday(): int);
  }

  /* Return the date as a `string` in ISO 8601 format: "YYYY-MM-DD" */
  proc date.isoformat() {
    var yearstr = year: string;
    var monthstr = month: string;
    var daystr = day: string;

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

  /* Return a `string` representing the date */
  proc date.ctime() {
    const month = strftime("%b");
    const wday = strftime("%a");

    const extraSpace = if day < 10 then " " else "";
    return wday + " " + month +
           " " + extraSpace + day:string + " 00:00:00 " + year: string;
  }

  /* Return a formatted `string` matching the `format` argument and the date */
  proc date.strftime(fmt: string) {
    extern proc strftime(s: c_void_ptr, size: size_t, format: c_string, ref timeStruct: tm);
    const bufLen: size_t = 100;
    var buf: [1..bufLen] c_char;
    var timeStruct: tm;

    timeStruct.tm_sec = 0;
    timeStruct.tm_min = 0;
    timeStruct.tm_hour = 0;
    timeStruct.tm_isdst = 0;
    timeStruct.tm_gmtoff = 0;
    timeStruct.tm_zone = nil;

    timeStruct.tm_year = (year-1900): int(32); // 1900 based
    timeStruct.tm_mon = (month-1): int(32);    // 0 based
    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_wday = (weekday(): int(32) + 1) % 7; // shift Sunday to 0
    timeStruct.tm_yday = (this - new date(year, 1, 1)).days: int(32);

    strftime(c_ptrTo(buf), bufLen, fmt.c_str(), timeStruct);
    var str: string;
    try! {
      str = createStringWithNewBuffer(c_ptrTo(buf):c_string);
    }
    return str;
  }

  private use IO;

  /* Read or write a date value from channel `f` */
  proc date.readWriteThis(f) throws {
    const dash = new ioLiteral("-");

    if f.writing {
      try! {
        f.write(isoformat());
      }
    } else {
      const binary = f.binary(),
            arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY),
            isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;

      if isjson then
        f <~> new ioLiteral('"');

      f <~> chpl_year <~> dash <~> chpl_month <~> dash <~> chpl_day;

      if isjson then
        f <~> new ioLiteral('"');
    }
  }


  /* Operators on date values */
  pragma "no doc"
  proc +(d: date, t: timedelta): date {
    return date.fromordinal(d.toordinal() + t.days);
  }

  pragma "no doc"
  proc +(t: timedelta, d: date): date {
    return d + t;
  }

  pragma "no doc"
  proc -(d: date, t: timedelta): date {
    return date.fromordinal(d.toordinal() - t.days);
  }

  pragma "no doc"
  proc -(d1: date, d2: date): timedelta {
    return new timedelta(days=d1.toordinal() - d2.toordinal());
  }

  pragma "no doc"
  proc <(d1: date, d2: date) {
    return d1.toordinal() < d2.toordinal();
  }

  pragma "no doc"
  proc <=(d1: date, d2: date) {
    return d1.toordinal() <= d2.toordinal();
  }

  pragma "no doc"
  proc >(d1: date, d2: date) {
    return d1.toordinal() > d2.toordinal();
  }

  pragma "no doc"
  proc >=(d1: date, d2: date) {
    return d1.toordinal() >= d2.toordinal();
  }


  /* A record representing a time */
  record time {
    pragma "no doc"
    var chpl_hour, chpl_minute, chpl_second, chpl_microsecond: int;
    pragma "no doc"
    var chpl_tzinfo: shared TZInfo?;

    /* The hour represented by this `time` value */
    proc hour {
      return chpl_hour;
    }

    /* The minute represented by this `time` value */
    proc minute {
      return chpl_minute;
    }

    /* The second represented by this `time` value */
    proc second {
      return chpl_second;
    }

    /* The microsecond represented by this `time` value */
    proc microsecond {
      return chpl_microsecond;
    }

    /* The timezone represented by this `time` value */
    proc tzinfo {
      return chpl_tzinfo;
    }

    /* The minimum representable `time` */
    proc type min {
      return new time();
    }

    /* The maximum representable `time` */
    proc type max {
      return new time(23, 59, 59, 999999);
    }

    /* The minimum non-zero difference between two times */
    proc type resolution {
      return new timedelta(microseconds=1);
    }
  }

  /* initializers/factories for time values */

  /* Initialize a new `time` value from the given `hour`, `minute`, `second`,
     `microsecond`, and `timezone`.  All arguments are optional
   */
  proc time.init(hour=0, minute=0, second=0, microsecond=0,
                 in tzinfo: shared TZInfo? = nil) {
    if hour < 0 || hour >= 24 then
      HaltWrappers.initHalt("hour out of range");
    if minute < 0 || minute >= 60 then
      HaltWrappers.initHalt("minute out of range");
    if second < 0 || second >= 60 then
      HaltWrappers.initHalt("second out of range");
    if microsecond < 0 || microsecond >= 1000000 then
      HaltWrappers.initHalt("microsecond out of range");
    this.chpl_hour = hour;
    this.chpl_minute = minute;
    this.chpl_second = second;
    this.chpl_microsecond = microsecond;
    this.chpl_tzinfo = tzinfo;
  }

  pragma "no doc"
  proc time.deinit() {
  }

  /* Methods on time values */

  /* Replace the `hour`, `minute`, `second`, `microsecond` and `tzinfo` in a
     `time` to create a new `time`. All arguments are optional.
   */
  proc time.replace(hour=-1, minute=-1, second=-1, microsecond=-1,
                    in tzinfo=this.tzinfo) {
    const newhour = if hour != -1 then hour else this.hour;
    const newminute = if minute != -1 then minute else this.minute;
    const newsecond = if second != -1 then second else this.second;
    const newmicrosecond = if microsecond != -1 then microsecond else this.microsecond;

    return new time(newhour, newminute, newsecond, newmicrosecond, tzinfo);
  }

  /* Return a `string` representing the `time` in ISO format */
  proc time.isoformat() {
    proc makeNDigits(n, d) {
      var ret = d: string;
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
    var offset = utcoffset();
    if tzinfo.borrow() != nil {
      var sign: string;
      if offset.days < 0 {
        offset = -offset;
        sign = "-";
      } else {
        sign = "+";
      }
      ret = ret + sign + makeNDigits(2, offset.seconds/(60*60)) + ":" +
                         makeNDigits(2, offset.seconds % (60*60) / 60);
    }
    return ret;
  }

  /* Return the offset from UTC */
  proc time.utcoffset() {
    if tzinfo.borrow() == nil {
      return new timedelta();
    } else {
      return tzinfo!.utcoffset(datetime.today());
    }
  }

  /* Return the daylight saving time offset */
  proc time.dst() {
    if tzinfo.borrow() == nil {
      return new timedelta();
    } else {
      return tzinfo!.dst(datetime.today());
    }
  }

  /* Return the name of the timezone for this `time` value */
  proc time.tzname() {
    if tzinfo.borrow() == nil then
      return "";
    else
      return tzinfo!.tzname(new datetime(1,1,1));
  }

  /* Return a `string` matching the `format` argument for this `time` */
  proc time.strftime(fmt: string) {
    extern proc strftime(s: c_void_ptr, size: size_t, format: c_string, ref timeStruct: tm);
    const bufLen: size_t = 100;
    var buf: [1..bufLen] c_char;
    var timeStruct: tm;

    timeStruct.tm_sec = second: int(32);
    timeStruct.tm_min = minute: int(32);
    timeStruct.tm_hour = hour: int(32);
    timeStruct.tm_year = 0;
    timeStruct.tm_mday = 1;
    timeStruct.tm_mon = 1;

    timeStruct.tm_wday = ((new date(1900, 1, 1)).weekday():int(32) + 1) % 7;
    timeStruct.tm_yday = 0;

    if tzinfo.borrow() != nil {
      timeStruct.tm_gmtoff = abs(utcoffset()).seconds: c_long;
      timeStruct.tm_zone = __primitive("cast", tm_zoneType, tzname().c_str());
      timeStruct.tm_isdst = dst().seconds: int(32);
    } else {
      timeStruct.tm_gmtoff = 0;
      timeStruct.tm_zone = __primitive("cast", tm_zoneType, "".c_str());
      timeStruct.tm_isdst = -1;
    }

    strftime(c_ptrTo(buf), bufLen, fmt.c_str(), timeStruct);
    var str: string;
    try! {
      str = createStringWithNewBuffer(c_ptrTo(buf):c_string);
    }

    return str;
  }

  /* Read or write a time value from channel `f` */
  proc time.readWriteThis(f) throws {
    const colon = new ioLiteral(":");
    if f.writing {
      try! {
        f.write(isoformat());
      }
    } else {
      const binary = f.binary(),
            arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY),
            isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;

      if isjson then
        f <~> new ioLiteral('"');

      f <~> chpl_hour <~> colon <~> chpl_minute <~> colon <~> chpl_second
        <~> new ioLiteral(".") <~> chpl_microsecond;

      if isjson then
        f <~> new ioLiteral('"');
    }
  }


  /* Operators on time values */

  pragma "no doc"
  proc ==(t1: time, t2: time): bool {
    var dt1 = datetime.combine(d=new date(2000, 1, 1), t=t1);
    var dt2 = datetime.combine(d=new date(2000, 1, 1), t=t2);
    return dt1 == dt2;
  }

  pragma "no doc"
  proc !=(t1: time, t2: time) {
    return !(t1 == t2);
  }

  pragma "no doc"
  proc <(t1: time, t2: time): bool {
    if (t1.tzinfo.borrow() != nil && t2.tzinfo.borrow() == nil) ||
        (t1.tzinfo.borrow() == nil && t2.tzinfo.borrow() != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 + t1.second;
      const usec1 = t1.microsecond;
      const sec2 = t2.hour*3600 + t2.minute*60 + t2.second;
      const usec2 = t2.microsecond;
      if sec1 < sec2 then
        return true;
      else if sec1 == sec2 then
        return usec1 < usec2;
      else
        return false;
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
  }

  pragma "no doc"
  proc <=(t1: time, t2: time): bool {
    if (t1.tzinfo.borrow() != nil && t2.tzinfo.borrow() == nil) ||
        (t1.tzinfo.borrow() == nil && t2.tzinfo.borrow() != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 + t1.second;
      const usec1 = t1.microsecond;
      const sec2 = t2.hour*3600 + t2.minute*60 + t2.second;
      const usec2 = t2.microsecond;
      if sec1 < sec2 then
        return true;
      else if sec1 == sec2 then
        return usec1 <= usec2;
      else
        return false;
    } else {
      const dt1 = datetime.combine(new date(1900, 1, 1), t1);
      const dt2 = datetime.combine(new date(1900, 1, 1), t2);
      return dt1 <= dt2;
    }
  }

  pragma "no doc"
  proc >(t1: time, t2: time): bool {
    if (t1.tzinfo.borrow() != nil && t2.tzinfo.borrow() == nil) ||
        (t1.tzinfo.borrow() == nil && t2.tzinfo.borrow() != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 + t1.second;
      const usec1 = t1.microsecond;
      const sec2 = t2.hour*3600 + t2.minute*60 + t2.second;
      const usec2 = t2.microsecond;
      if sec1 > sec2 then
        return true;
      else if sec1 == sec2 then
        return usec1 > usec2;
      else
        return false;
    } else {
      const dt1 = datetime.combine(new date(1900, 1, 1), t1);
      const dt2 = datetime.combine(new date(1900, 1, 1), t2);
      return dt1 > dt2;
    }
  }

  pragma "no doc"
  proc >=(t1: time, t2: time): bool {
    if (t1.tzinfo.borrow() != nil && t2.tzinfo.borrow() == nil) ||
        (t1.tzinfo.borrow() == nil && t2.tzinfo.borrow() != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if t1.tzinfo == t2.tzinfo {
      const sec1 = t1.hour*3600 + t1.minute*60 + t1.second;
      const usec1 = t1.microsecond;
      const sec2 = t2.hour*3600 + t2.minute*60 + t2.second;
      const usec2 = t2.microsecond;
      if sec1 > sec2 then
        return true;
      else if sec1 == sec2 then
        return usec1 >= usec2;
      else
        return false;
    } else {
      const dt1 = datetime.combine(new date(1900, 1, 1), t1);
      const dt2 = datetime.combine(new date(1900, 1, 1), t2);
      return dt1 >= dt2;
    }
  }

  /* A record representing a combined `date` and `time` */
  record datetime {
    pragma "no doc"
    var chpl_date: date;
    pragma "no doc"
    var chpl_time: time;

    /* The minimum representable `date` and `time` */
    proc type min {
      return this.combine(date.min, time.min);
    }

    /* The maximum representable `date` and `time` */
    proc type max {
      return this.combine(date.max, time.max);
    }

    /* The minimum non-zero difference between two datetimes */
    proc type resolution {
      return new timedelta(microseconds=1);
    }

    /* The year represented by this `datetime` value */
    proc year {
      return chpl_date.year;
    }

    /* The month represented by this `datetime` value */
    proc month {
      return chpl_date.month;
    }

    /* The day represented by this `datetime` value */
    proc day {
      return chpl_date.day;
    }

    /* The hour represented by this `datetime` value */
    proc hour {
      return chpl_time.hour;
    }

    /* The minute represented by this `datetime` value */
    proc minute {
      return chpl_time.minute;
    }

    /* The second represented by this `datetime` value */
    proc second {
      return chpl_time.second;
    }

    /* The microsecond represented by this `datetime` value */
    proc microsecond {
      return chpl_time.microsecond;
    }

    /* The timezone represented by this `datetime` value */
    proc tzinfo {
      return chpl_time.tzinfo;
    }
  }

  /* initializers/factories for datetime values */

  pragma "no doc"
  proc datetime.init() {
  }

  /* Initialize a new `datetime` value from the given `year`, `month`, `day`,
     `hour`, `minute`, `second`, `microsecond` and timezone.  The `year`,
     `month`, and `day` arguments are required, the rest are optional.
   */
  proc datetime.init(year, month, day,
                     hour=0, minute=0, second=0, microsecond=0,
                     in tzinfo: shared TZInfo? = nil) {
    chpl_date = new date(year, month, day);
    chpl_time = new time(hour, minute, second, microsecond, tzinfo);
  }

  /* Return a `datetime` value representing the current time and date */
  proc type datetime.today() {
    return this.now();
  }

  /* Return a `datetime` value representing the current time and date */
  proc type datetime.now(in tz: shared TZInfo? = nil) {
    if tz.borrow() == nil {
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

      return (utcNow + tz!.utcoffset(utcNow)).replace(tzinfo=tz);
    }
  }

  /* Return a `datetime` value representing the current time and date in UTC */
  proc type datetime.utcnow() {
    const timeSinceEpoch = getTimeOfDay();
    const td = new timedelta(seconds=timeSinceEpoch(1),
                             microseconds=timeSinceEpoch(2));
    return unixEpoch + td;
  }

  /* The `datetime` that is `timestamp` seconds from the epoch */
  proc type datetime.fromtimestamp(timestamp: real,
                                   in tz: shared TZInfo? = nil) {
    if tz.borrow() == nil {
      var t = (timestamp: int, ((timestamp - timestamp: int)*1000000): int);
      const lt = getLocalTime(t);
      return new datetime(year=lt.tm_year+1900, month=lt.tm_mon+1,
                          day=lt.tm_mday,       hour=lt.tm_hour,
                          minute=lt.tm_min,     second=lt.tm_sec,
                          microsecond=t(2));
    } else {
      var dt = datetime.utcfromtimestamp(timestamp);
      return (dt + tz!.utcoffset(dt)).replace(tzinfo=tz);
    }
  }

  /* The `datetime` that is `timestamp` seconds from the epoch in UTC */
  proc type datetime.utcfromtimestamp(timestamp) {
    return unixEpoch + new timedelta(seconds=timestamp: int, microseconds=((timestamp-timestamp: int)*1000000): int);
  }

  /* The `datetime` that is `ordinal` days from 1-1-0001 */
  proc type datetime.fromordinal(ordinal) {
    return datetime.combine(date.fromordinal(ordinal), new time());
  }

  /* Form a `datetime` value from a given `date` and `time` */
  proc type datetime.combine(d: date, t: time) {
    return new datetime(d.year, d.month, d.day,
                        t.hour, t.minute, t.second, t.microsecond, t.tzinfo);
  }

  /* Methods on datetime values */

  /* Get the `date` portion of the `datetime` value */
  proc datetime.getdate() {
    return chpl_date;
  }

  /* Get the `time` portion of the `datetime` value, with `tzinfo` = nil */
  proc datetime.gettime() {
    if chpl_time.tzinfo.borrow() == nil then
      return chpl_time;
    else
      return new time(hour=hour, minute=minute,
                      second=second, microsecond=microsecond);
  }

  /* Get the `time` portion of the `datetime` value including the
     `tzinfo` field
   */
  proc datetime.timetz() {
    return chpl_time;
  }

  /* Replace the `year`, `month`, `day`, `hour`, `minute`, `second`,
     `microsecond`, or `tzinfo` to form a new `datetime` object. All
     arguments are optional.
   */
  proc datetime.replace(year=-1, month=-1, day=-1,
                        hour=-1, minute=-1, second=-1, microsecond=-1,
                        in tzinfo=this.tzinfo) {
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

  /* Return the date and time converted into the timezone in the argument */
  proc datetime.astimezone(in tz: shared TZInfo) {
    if tzinfo == tz {
      return this;
    }
    const utc = (this - this.utcoffset()).replace(tzinfo=tz);
    return tz.borrow().fromutc(utc);
  }

  /* Return the offset from UTC */
  proc datetime.utcoffset() {
    if tzinfo.borrow() == nil {
      halt("utcoffset called on naive datetime");
    } else {
      return tzinfo!.utcoffset(this);
    }
  }

  /* Return the daylight saving time offset */
  proc datetime.dst() {
    if tzinfo.borrow() == nil then
      halt("dst() called with nil tzinfo");
    return tzinfo!.dst(this);
  }

  /* Return the name of the timezone for this `datetime` value */
  proc datetime.tzname() {
    if tzinfo.borrow() == nil then
      return "";
    return tzinfo!.tzname(this);
  }

  /* Return a filled record matching the C `struct tm` type for the given
     `datetime` */
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

    if tzinfo.borrow() == nil {
      timeStruct.tm_isdst = -1;
    } else if dst() == new timedelta(0) {
      timeStruct.tm_isdst = 0;
    } else {
      timeStruct.tm_isdst = 1;
    }

    return timeStruct;
  }

  /* Return a filled record matching the C `struct tm` type for the given
     `datetime` in UTC
   */
  proc datetime.utctimetuple() {
    if tzinfo.borrow() == nil {
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

  /* Return the number of days since 1-1-0001 this `datetime` represents */
  proc datetime.toordinal() {
    return getdate().toordinal();
  }

  /* Return the day of the week as a `DayOfWeek`.
     `Monday` == 0, `Sunday` == 6
   */
  proc datetime.weekday() {
    return getdate().weekday();
  }

  /* Return the day of the week as an `ISODayOfWeek`.
     `Monday` == 1, `Sunday` == 7
   */
  proc datetime.isoweekday() {
    return getdate().isoweekday();
  }

  /* Return the ISO date as a tuple containing the ISO year, ISO week number,
     and ISO day of the week
   */
  proc datetime.isocalendar() {
    return getdate().isocalendar();
  }

  /* Return the `datetime` as a `string` in ISO format */
  proc datetime.isoformat(sep="T") {
    proc zeroPad(nDigits: int, i: int) {
      var numStr = i: string;
      for i in 1..nDigits-numStr.length {
        numStr = "0" + numStr;
      }
      return numStr;
    }
    var micro = if microsecond > 0 then "." + zeroPad(6, microsecond) else "";
    var offset: string;
    if tzinfo.borrow() != nil {
      var utcoff = utcoffset();
      var sign: string;
      if utcoff < new timedelta(0) {
        sign = '-';
        utcoff = abs(utcoff);
      } else {
        sign = '+';
      }
      var hours = utcoff.seconds / (60*60);
      var minutes = (utcoff.seconds % (60*60)) / 60;
      offset = sign +
               (if hours < 10 then "0" + hours: string else hours: string) +
               ":" +
               (if minutes < 10 then "0" + minutes: string else minutes: string);
    }

    // on our Linux64 systems, the "%Y" format doesn't zero-pad to 4
    // characters on its own, so do it manually.
    var year = zeroPad(4, try! strftime("%Y"):int);
    return strftime(year + "-%m-%d" + sep + "%H:%M:%S" + micro + offset);
  }

  /* Create a `datetime` as described by the `date_string` and `format`
     string */
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

  /* Create a `string` from a `datetime` matching the `format` string */
  proc datetime.strftime(fmt: string) {
    extern proc strftime(s: c_void_ptr, size: size_t, format: c_string, ref timeStruct: tm);
    const bufLen: size_t = 100;
    var buf: [1..bufLen] c_char;
    var timeStruct: tm;

    timeStruct.tm_hour = hour: int(32);
    timeStruct.tm_min = minute: int(32);
    timeStruct.tm_sec = second: int(32);

    if tzinfo.borrow() != nil {
      timeStruct.tm_isdst = tzinfo!.dst(this).seconds: int(32);
      timeStruct.tm_gmtoff = tzinfo!.utcoffset(this).seconds: c_long;
      timeStruct.tm_zone = nil;
    } else {
      timeStruct.tm_isdst = -1: int(32);
      timeStruct.tm_gmtoff = 0;
      timeStruct.tm_zone = nil;
    }

    timeStruct.tm_year = (year-1900): int(32); // 1900 based
    timeStruct.tm_mon = (month-1): int(32);    // 0 based
    timeStruct.tm_mday = day: int(32);
    timeStruct.tm_wday = (weekday(): int(32) + 1) % 7; // shift Sunday to 0
    timeStruct.tm_yday = (this.replace(tzinfo=nil) - new datetime(year, 1, 1)).days: int(32);

    strftime(c_ptrTo(buf), bufLen, fmt.c_str(), timeStruct);
    var str: string;
    try! {
      str = createStringWithNewBuffer(c_ptrTo(buf):c_string);
    }

    return str;
  }

  /* Return a `string` from a `datetime` in the form:
     Wed Dec  4 20:30:40 2002
  */
  proc datetime.ctime() {
    return this.strftime("%a %b %e %T %Y");
  }

  /* Read or write a datetime value from channel `f` */
  proc datetime.readWriteThis(f) throws {
    const dash  = new ioLiteral("-"),
          colon = new ioLiteral(":");

    if f.writing {
      try! {
        f.write(isoformat());
      }
    } else {
      const binary = f.binary(),
            arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY),
            isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;

      if isjson then
        f <~> new ioLiteral('"');

      f <~> chpl_date.chpl_year <~> dash <~> chpl_date.chpl_month <~> dash
        <~> chpl_date.chpl_day <~> new ioLiteral("T") <~> chpl_time.chpl_hour
        <~> colon <~> chpl_time.chpl_minute <~> colon <~> chpl_time.chpl_second
        <~> new ioLiteral(".") <~> chpl_time.chpl_microsecond;

      if isjson then
        f <~> new ioLiteral('"');
    }
  }


  // TODO: Add a datetime.timestamp() method

  /* Operators on datetime values */

  pragma "no doc"
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

  pragma "no doc"
  proc +(dt: datetime, td: timedelta) {
    return td + dt;
  }

  pragma "no doc"
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

  pragma "no doc"
  proc -(dt1: datetime, dt2: datetime): timedelta {
    if (dt1.tzinfo.borrow() != nil && dt2.tzinfo.borrow() == nil) ||
       (dt1.tzinfo.borrow() == nil && dt2.tzinfo.borrow() != nil) {
      halt("both datetimes must both be either naive or aware");
    }
    if dt1.tzinfo == dt2.tzinfo {
      const newmicro = dt1.microsecond - dt2.microsecond,
            newsec = dt1.second - dt2.second,
            newmin = dt1.minute - dt2.minute,
            newhour = dt1.hour - dt2.hour,
            newday = dt1.toordinal() - dt2.toordinal();
      return new timedelta(days=newday, hours=newhour, minutes=newmin,
                           seconds=newsec, microseconds=newmicro);
    } else {
      return dt1.replace(tzinfo=nil) -
                                dt2.replace(tzinfo=nil) +
                                dt2.utcoffset() - dt1.utcoffset();
    }
  }

  pragma "no doc"
  proc ==(dt1: datetime, dt2: datetime): bool {
    if dt1.tzinfo.borrow() == nil && dt2.tzinfo.borrow() != nil ||
       dt1.tzinfo.borrow() != nil && dt2.tzinfo.borrow() == nil {
      halt("Cannot compare naive datetime to aware datetime");
    } else if dt1.tzinfo == dt2.tzinfo {
      // just ignore tzinfo
      var d1: date = dt1.replace(tzinfo=nil).getdate(),
          d2: date = dt2.replace(tzinfo=nil).getdate();
      var t1: time = dt1.replace(tzinfo=nil).gettime(),
          t2: time = dt2.replace(tzinfo=nil).gettime();

      return d1.year == d2.year && d1.month == d2.month && d1.day == d2.day &&
                        t1.hour == t2.hour && t1.minute == t2.minute &&
                        t1.second == t2.second &&
                        t1.microsecond == t2.microsecond;
    } else {
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) ==
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
  }

  pragma "no doc"
  proc !=(dt1: datetime, dt2: datetime) {
    return !(dt1 == dt2);
  }

  pragma "no doc"
  proc <(dt1: datetime, dt2: datetime): bool {
    if (dt1.tzinfo.borrow() != nil && dt2.tzinfo.borrow() == nil) ||
        (dt1.tzinfo.borrow() == nil && dt2.tzinfo.borrow() != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if dt1.tzinfo == dt2.tzinfo {
      const date1 = dt1.getdate(),
            date2 = dt2.getdate();
      if date1 < date2 then return true;
      else if date2 < date1 then return false;
      else return dt1.gettime() < dt2.gettime();
    } else {
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) <
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
  }

  pragma "no doc"
  proc <=(dt1: datetime, dt2: datetime): bool {
    if (dt1.tzinfo.borrow() != nil && dt2.tzinfo.borrow() == nil) ||
        (dt1.tzinfo.borrow() == nil && dt2.tzinfo.borrow() != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if dt1.tzinfo == dt2.tzinfo {
      const date1 = dt1.getdate(),
            date2 = dt2.getdate();
      if date1 < date2 then return true;
      else if date2 < date1 then return false;
      else return dt1.gettime() <= dt2.gettime();
    } else {
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) <=
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
  }

  pragma "no doc"
  proc >(dt1: datetime, dt2: datetime): bool {
    if (dt1.tzinfo.borrow() != nil && dt2.tzinfo.borrow() == nil) ||
        (dt1.tzinfo.borrow() == nil && dt2.tzinfo.borrow() != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if dt1.tzinfo == dt2.tzinfo {
      const date1 = dt1.getdate(),
            date2 = dt2.getdate();
      if date1 > date2 then return true;
      else if date2 > date1 then return false;
      else return dt1.gettime() > dt2.gettime();
    } else {
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) >
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
  }

  pragma "no doc"
  proc >=(dt1: datetime, dt2: datetime): bool {
    if (dt1.tzinfo.borrow() != nil && dt2.tzinfo.borrow() == nil) ||
        (dt1.tzinfo.borrow() == nil && dt2.tzinfo.borrow() != nil) {
      halt("both datetimes must both be either naive or aware");
    } else if dt1.tzinfo == dt2.tzinfo {
      const date1 = dt1.getdate(),
            date2 = dt2.getdate();
      if date1 > date2 then return true;
      else if date2 > date1 then return false;
      else return dt1.gettime() >= dt2.gettime();
    } else {
      return (dt1.replace(tzinfo=nil) - dt1.utcoffset()) >=
             (dt2.replace(tzinfo=nil) - dt2.utcoffset());
    }
  }



  /* A record representing an amount of time.  A `timedelta` has fields
     representing days, seconds, and microseconds.  These fields are always
     kept within the following ranges:

     0 <= `microseconds` < 1000000

     0 <= `seconds` < 60*60*24

     -999999999 <= `days` <= 999999999

     It is an overflow error if `days` is outside the given range.
   */
  record timedelta {
    pragma "no doc"
    var chpl_days: int;

    pragma "no doc"
    var chpl_seconds: int;

    pragma "no doc"
    var chpl_microseconds: int;

    /* The number of days this `timedelta` represents */
    proc days {
      return chpl_days;
    }

    /* The number of seconds this `timedelta` represents */
    proc seconds {
      return chpl_seconds;
    }

    /* The number of microseconds this `timedelta` represents */
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

  /* initializers/factories for timedelta values */

  /* Initialize a `timedelta` object.  All arguments are optional and
     default to 0. Since only `days`, `seconds` and `microseconds` are
     stored, the other arguments are converted to days, seconds
     and microseconds. */
  proc timedelta.init(days=0, seconds=0, microseconds=0,
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
      HaltWrappers.initHalt("Overflow: days < -999999999");

    if this.days > 999999999 then
      HaltWrappers.initHalt("Overflow: days > 999999999");
  }

  /* Create a `timedelta` from a given number of seconds */
  proc timedelta.init(timestamp: real) {
    this.init(seconds = timestamp: int, microseconds=((timestamp - timestamp: int)*1000000): int);
  }


  /* Methods on timedelta values */

  /* Return the total number of seconds represented by this object */
  proc timedelta.total_seconds(): real {
    return days*(24*60*60) + seconds + microseconds / 1000000.0;
  }


  /* Operators on timedelta values */

  pragma "no doc"
  proc *(i: int, t: timedelta) {
    return new timedelta(days=i*t.days, seconds=i*t.seconds, microseconds=i*t.microseconds);
  }

  pragma "no doc"
  proc *(t: timedelta, i: int) {
    return new timedelta(days=i*t.days, seconds=i*t.seconds, microseconds=i*t.microseconds);
  }

  pragma "no doc"
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

  pragma "no doc"
  proc +(t: timedelta) {
    return t;
  }

  pragma "no doc"
  proc -(t: timedelta) {
    return new timedelta(days=-t.days, seconds=-t.seconds, microseconds=-t.microseconds);
  }

  pragma "no doc"
  proc +(lhs: timedelta, rhs: timedelta) {
    return new timedelta(days=lhs.days+rhs.days,
                         seconds=lhs.seconds+rhs.seconds,
                         microseconds=lhs.microseconds+rhs.microseconds);
  }

  pragma "no doc"
  proc -(lhs: timedelta, rhs: timedelta) {
    return new timedelta(days=lhs.days-rhs.days,
                         seconds=lhs.seconds-rhs.seconds,
                         microseconds=lhs.microseconds-rhs.microseconds);
  }

  pragma "no doc"
  proc >(lhs: timedelta, rhs: timedelta) {
    const ls = (lhs.days*(24*60*60) + lhs.seconds);
    const rs = (rhs.days*(24*60*60) + rhs.seconds);
    if ls > rs then return true;
    if rs > ls then return false;
    return lhs.microseconds > rhs.microseconds;
  }

  pragma "no doc"
  proc >=(lhs: timedelta, rhs: timedelta) {
    return lhs > rhs || lhs == rhs;
  }

  pragma "no doc"
  proc <(lhs: timedelta, rhs: timedelta) {
    const ls = (lhs.days*(24*60*60) + lhs.seconds);
    const rs = (rhs.days*(24*60*60) + rhs.seconds);
    if ls < rs then return true;
    if rs < ls then return false;
    return lhs.microseconds < rhs.microseconds;
  }

  pragma "no doc"
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

  pragma "no doc"
  proc _cast(type s, t: timedelta) where s == string {
    var str: string;
    if t.days != 0 {
      str = t.days: string + " day";
      if t.days != 1 && t.days != -1 then
        str += "s";
      str += ", ";
    }
    const seconds = t.seconds % 60;
    const minutes = (t.seconds / 60) % 60;
    const hours = t.seconds / (60*60);
    const microseconds = t.microseconds;

    str += hours: string + ":";
    if minutes < 10 then
      str += "0";
    str += minutes + ":";
    if seconds < 10 then
      str += "0";
    str += seconds;
    if microseconds != 0 {
      str += ".";
      const usLog10 = log10(microseconds): int;
      for i in 1..(5-usLog10) {
        str += "0";
      }

      str += microseconds: string;
    }
    return str;
  }

  /* Abstract base class for time zones. This class should not be used
     directly, but concrete implementations of time zones should be
     derived from it. */
  class TZInfo {
    /* The offset from UTC this class represents */
    proc utcoffset(dt: datetime): timedelta {
      HaltWrappers.pureVirtualMethodHalt();
      return new timedelta();
    }

    /* The `timedelta` for daylight saving time */
    proc dst(dt: datetime): timedelta {
      HaltWrappers.pureVirtualMethodHalt();
      return new timedelta();
    }

    /* The name of this time zone */
    proc tzname(dt: datetime): string {
      HaltWrappers.pureVirtualMethodHalt();
      return "";
    }

    /* Convert a `time` in UTC to this time zone */
    proc fromutc(dt: datetime): datetime {
      HaltWrappers.pureVirtualMethodHalt();
      return new datetime(0,0,0);
    }
  }

  // TODO: Add a timezone class implementation
}
