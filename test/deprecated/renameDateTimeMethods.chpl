use DateTime;

// deprecaed methods
var d = new date(2022, 6, 6);

writeln(d.isoweekday());
writeln(d.isocalendar());
writeln(d.isoformat());
writeln(d.toordinal());

var t = new time(12, 0,0);

writeln(t.utcoffset());

var dt = new datetime(2022, 6, 6, 12, 0, 0);
writeln(dt.toordinal());

// deprecated type methods
writeln(date.fromordinal(9999));
writeln(datetime.fromordinal(9999));

var uNow = datetime.utcNow();
writeln(date.fromtimestamp(9999.0));
writeln(datetime.utcfromtimestamp(9999.0));

var zone = new TZInfo();
writeln(zone.fromutc(dt)); // Halts
