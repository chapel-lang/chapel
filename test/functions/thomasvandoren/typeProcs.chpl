const MINYEAR = 1,
  MAXYEAR = 9999;

proc main() {
  var d1 = Date.fromIsoString("2014-09-04"),
    d2 = Date.fromIsoString("whatevs"),
    c = Calendar.fromDate(d1);
  writeln(d1);
  writeln(d2);
  writeln(c);
}

record Date {
  var year, month, day: int;
}

// Pseudo-static attributes using paren-less type proc.
proc type Date.max {
  return new Date(MAXYEAR, 12, 31);
}

proc type Date.min {
  return new Date(MINYEAR, 1, 1);
}

// Clearly this factory method would need to be improved upon...
proc type Date.fromIsoString(dateStr: string): Date {
  if dateStr == "2014-09-04" then
    return new Date(2014, 9, 4);
  else
    return Date.min;
}

class Calendar {
  var year, month: int;
}

proc type Calendar.fromDate(d: Date): Calendar {
  return new Calendar(d.year, d.month);
}
