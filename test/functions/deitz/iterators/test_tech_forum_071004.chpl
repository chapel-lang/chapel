def iter_string_chars(s: string) {
  var i = 1, limit = length(s);
  while i <= limit {
    yield s.substring(i);
    i += 1;
  }
}

for i in (1..3, iter_string_chars("ABC")) do
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

def fun_string_chars(s: string) {
  var ic = new IC(cursor=1, s=s);
  ic.next();
  return ic;
}

def IC.next() {
  select cursor {
    when 1 do goto L1;
    when 2 do goto L2;
  }
  label L1 i = 1;
  limit = length(s);
  while i <= limit {
    value = s.substring(i);
    cursor = 2;
    return;
    label L2 i += 1;
  }
  cursor = 0;
}


def special_fun_string_chars(s: string) {
  var ic = new IC(cursor=1, s=s);
  ic.i = 1;
  ic.limit = length(ic.s);
  ic.cursor = ic.i <= ic.limit;
  if ic.cursor then
    ic.value = ic.s.substring(ic.i);
  return ic;
}

def IC.special_next() {
  i += 1;
  cursor = i <= limit;
  if cursor then
    value = s.substring(i);
}

var ic = fun_string_chars("ABC");
while ic.cursor {
  writeln(ic.value);
  ic.next();
}

ic = special_fun_string_chars("ABC");
while ic.cursor {
  writeln(ic.value);
  ic.special_next();
}
