class node {
  type t;
  var element : t;
  var next : node;
}

record foo {
  type t;
  var length : integer;
  var first : node;
  var last : node;

  function append(e : t) {
   var new : node = node(t);
    new.element = e;
    if length > 0 {
      last.next = new;
      last = new;
    } else {
      first = new;
      last = new;
    }
    length += 1;
    return this;
  }

  function prepend(e : t) {
    var new : node = node(t);
    new.element = e;
    if length > 0 {
      new.next = first;
      first = new;
    } else {
      first = new;
      last = new;
    }
    length += 1;
    return this;
  }
}

function write(f : foo) {
  write("(/");
  var tmp = f.first;
  while tmp != nil {
    write(tmp.element);
    tmp = tmp.next;
    if (tmp != nil) {
      write(", ");
    }
  }
  write("/)");
}

var f : foo of integer;

f.append(1);
f.append(2);
f.prepend(3);
f.prepend(4);

writeln(f);
