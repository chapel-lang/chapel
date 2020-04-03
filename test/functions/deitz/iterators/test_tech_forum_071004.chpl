iter iter_string_chars(s: string) {
  var i = 0, limit = s.size;
  while i < limit {
    yield s[i];
    i += 1;
  }
}

for i in zip(1..3, iter_string_chars("ABC")) do
  writeln(i);

for i in iter_string_chars("ABC") do
  writeln(i);

class IC {
  var cursor: int;
  var value: string;
  var s: string;
  var i: int;
  var limit: int;
}

proc fun_string_chars(s: string) {
  var ic = new unmanaged IC(cursor=1, s=s);
  ic.next();
  return ic;
}

/*
proc IC.next() {
  select cursor {
    when 1 do goto L1;
    when 2 do goto L2;
  }
  label L1 i = 1;
  limit = length(s);
  while i <= limit {
    value = s[i];
    cursor = 2;
    return;
    label L2 i += 1;
  }
  cursor = 0;
}
*/
proc IC.next() {
  if cursor == 1 {
    i = 0;
    limit = s.size;
    if i < limit {
      value = s[i];
      cursor = 2;
      return;
    }
  } else if cursor == 2 {
    i += 1;
    if i < limit {
      value = s[i];
      cursor = 2;
      return;
    }
  }
  cursor = 0;
}

proc special_fun_string_chars(s: string) {
  var ic = new unmanaged IC(cursor=1, s=s);
  ic.i = 0;
  ic.limit = ic.s.size;
  ic.cursor = ic.i < ic.limit;
  if ic.cursor then
    ic.value = ic.s[ic.i];
  return ic;
}

proc IC.special_next() {
  i += 1;
  cursor = i < limit;
  if cursor then
    value = s[i];
}





var ic = fun_string_chars("ABC");

while ic.cursor {
  writeln(ic.value);
  ic.next();
}

delete ic;



ic = special_fun_string_chars("ABC");

while ic.cursor {
  writeln(ic.value);
  ic.special_next();
}

delete ic;
