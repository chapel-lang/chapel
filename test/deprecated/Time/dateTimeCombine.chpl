use Time;

var d = new date(1977, 12, 16);
var t = new time(hour=5);

var dt = dateTime.combine(d, t);
// compare behavior to alternate approach
assert(dt == new dateTime(d, t));

// same as above but with current dateTime
var nowDt = dateTime.now();
var nowDtReassembled = dateTime.combine(nowDt.getDate(), nowDt.getTime());
assert(nowDtReassembled == new dateTime(nowDt.getDate(), nowDt.getTime()));
