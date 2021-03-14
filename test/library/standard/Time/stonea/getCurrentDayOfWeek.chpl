use Time;

var day = getCurrentDayOfWeek():Day;

select day {
    when Day.sunday do writeln("Sunday");
    when Day.monday do writeln("Monday");
    when Day.tuesday do writeln("Tuesday");
    when Day.wednesday do writeln("Wednesday");
    when Day.thursday do writeln("Thursday");
    when Day.friday do writeln("Friday");
    when Day.saturday do writeln("Saturday");
}

