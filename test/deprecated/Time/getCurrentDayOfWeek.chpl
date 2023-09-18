use Time;

var weekday = getCurrentDayOfWeek():day;

// ensure same behavior as replacement functionality, with adjustment for what
// day-of-week enum is used
writeln((((weekday:int + 6) % 7) + 1) : dayOfWeek == date.utcToday().weekday());

select weekday {
    when day.sunday do writeln("Sunday");
    when day.monday do writeln("Monday");
    when day.tuesday do writeln("Tuesday");
    when day.wednesday do writeln("Wednesday");
    when day.thursday do writeln("Thursday");
    when day.friday do writeln("Friday");
    when day.saturday do writeln("Saturday");
}

