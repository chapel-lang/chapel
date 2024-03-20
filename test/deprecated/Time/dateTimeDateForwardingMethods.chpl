use Time;

var dt = dateTime.now();

var calendarTuple : (int, int, int) = dt.isoCalendar();
var ordinal : int = dt.toOrdinal();
var weekday : dayOfWeek = dt.weekday();
var isoWeekday : isoDayOfWeek = dt.isoWeekday();

// test manually forwarding gives same behavior
assert(calendarTuple == dt.getDate().isoWeekDate());
assert(ordinal == dt.getDate().toOrdinal());
assert(weekday == dt.getDate().weekday());
// date.isoWeekday() is also deprecated and tested elsewhere, so skip it
// assert(isoWeekday == dt.getDate().isoWeekday()); 
