use Time;

var weekday = getCurrentDayOfWeek():day;

select weekday {
    when day.sunday do writeln("Sunday");
    when day.monday do writeln("Monday");
    when day.tuesday do writeln("Tuesday");
    when day.wednesday do writeln("Wednesday");
    when day.thursday do writeln("Thursday");
    when day.friday do writeln("Friday");
    when day.saturday do writeln("Saturday");
}

