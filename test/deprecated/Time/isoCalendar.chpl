use Time;

// January 1, 1977, which is ISO week date 1976-W53-6
var myDate = new date(1977, 1, 1);

var calendarTuple = myDate.isoCalendar();
writeln(calendarTuple);
writeln(calendarTuple == myDate.isoWeekDate());
