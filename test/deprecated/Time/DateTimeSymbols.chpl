use Time;

var t = new time();
if t.tzinfo != nil then
  writeln("time Timezone!");

var dt = dateTime.now();
if dt.tzinfo != nil then
  writeln("dateTime Timezone!");

var wd = dt.isoweekday();
var cal = dt.isocalendar();

var tzi: owned TZInfo?;
