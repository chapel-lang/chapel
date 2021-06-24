use Regex;
use IO;

writeln("Words words words");
var f = openmem();
{
  var w = f.writer();
  w.write("Words words words");
  w.close();
}

writeln("+match: One word no captures");
{
  var r = f.reader();
  var re = compile("\\w+");
  var m = r.match(re);
  writeln("match ", m);
  assert(m.matched && m.offset == 0);
  writeln("offset ", r.offset());
  assert(r.offset() == 0);
  r.close();
}
writeln("+match: nothing no captures");
{
  var r = f.reader();
  var re = compile("\\d+");
  var m = r.match(re);
  writeln("match ", m);
  assert(!m.matched);
  writeln("offset ", r.offset());
  assert(r.offset() == 0);
  r.close();
}
writeln("+match: one word with captures");
{
  var r = f.reader();
  var s:string;
  var re = compile("(\\w+)");
  var m = r.match(re, s);
  writeln("match ", m);
  assert(m.matched && m.offset == 0);
  writeln("cap ", s);
  writeln("offset ", r.offset());
  assert(r.offset() == 0);
  r.close();
}
writeln("+match: nothing with captures");
{
  var r = f.reader();
  var s:string;
  var re = compile("(\\d+)");
  var m = r.match(re, s);
  writeln("match ", m);
  assert(!m.matched);
  writeln("cap ", s);
  writeln("offset ", r.offset());
  assert(r.offset() == 0);
  r.close();
}
writeln("+match: nothing");
{
  var r = f.reader();
  var s:string;
  var re = compile("words");
  var m = r.match(re, s);
  writeln("match ", m);
  assert(!m.matched);
  writeln("cap ", s);
  writeln("offset ", r.offset());
  assert(r.offset() == 0);
  r.close();
}
writeln("+match: nothing with captures");
{
  var r = f.reader();
  var s:string;
  var re = compile("(words)");
  var m = r.match(re, s);
  writeln("match ", m);
  assert(!m.matched);
  writeln("cap ", s);
  writeln("offset ", r.offset());
  assert(r.offset() == 0);
  r.close();
}



writeln("+search: One word no captures");
{
  var r = f.reader();
  var re = compile("\\w+");
  var m = r.search(re);
  writeln("match ", m);
  assert(m.matched && m.offset == 0);
  writeln("offset ", r.offset());
  assert(r.offset() == 0);
  r.close();
}
writeln("+search: nothing no captures");
{
  var r = f.reader();
  var re = compile("\\d+");
  var m = r.search(re);
  writeln("match ", m);
  assert(!m.matched);
  writeln("offset ", r.offset());
  assert(r.offset() == 17);
  r.close();
}
writeln("+search: one word with captures");
{
  var r = f.reader();
  var s:string;
  var re = compile("(\\w+)");
  var m = r.search(re, s);
  writeln("match ", m);
  assert(m.matched && m.offset == 0);
  writeln("cap ", s);
  writeln("offset ", r.offset());
  assert(r.offset() == 0);
  r.close();
}
writeln("+search: nothing with captures");
{
  var r = f.reader();
  var s:string;
  var re = compile("(\\d+)");
  var m = r.search(re, s);
  writeln("match ", m);
  assert(!m.matched);
  writeln("cap ", s);
  writeln("offset ", r.offset());
  assert(r.offset() == 17);
  r.close();
}
writeln("+search: later word");
{
  var r = f.reader();
  var re = compile("words");
  var m = r.search(re);
  writeln("match ", m);
  assert(m.matched && m.offset == 6);
  writeln("offset ", r.offset());
  assert(r.offset() == 6);
  r.close();
}
writeln("+search: later word captured");
{
  var r = f.reader();
  var s:string;
  var re = compile("(words)");
  var m = r.search(re, s);
  writeln("match ", m);
  assert(m.matched && m.offset == 6);
  writeln("cap ", s);
  writeln("offset ", r.offset());
  assert(r.offset() == 6);
  r.close();
}




writeln("+matches: One word");
{
  var r = f.reader();
  var re = compile("\\w+");
  for (m,) in r.matches(re, 0, 1) {
    writeln("offset ", r.offset());
    writeln("match ", m);
    var s:string;
    r.extractMatch(m, s);
    writeln("string ", s);
  }
  writeln("offset ", r.offset());
  r.close();
}
writeln("+matches: All words");
{
  var r = f.reader();
  var re = compile("\\w+");
  for (m,) in r.matches(re, 0) {
    writeln("offset ", r.offset());
    writeln("match ", m);
    var s:string;
    r.extractMatch(m, s);
    writeln("string ", s);
  }
  writeln("offset ", r.offset());
  r.close();
}
writeln("+matches: All words capturing first letter");
{
  var r = f.reader();
  var re = compile("(\\w)\\w*");
  for (m,aFor) in r.matches(re, 1) {
    var a = aFor; // so we can pass 'a' to extractMatch()
    writeln("offset ", r.offset());
    writeln("match ", m);
    var s:string;
    var aStr:string;
    r.extractMatch(m, s);
    writeln("string ", s);
    writeln("cap match", a);
    r.extractMatch(m, a);
    writeln("cap string ", aStr);

  }
  writeln("offset ", r.offset());
  r.close();
}
