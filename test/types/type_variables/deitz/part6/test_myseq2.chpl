class myseq_node {
  type elementType;
  var element : elementType;
  var next : unmanaged myseq_node(elementType)?;
}

class myseq {
  type elementType;

  var  length : int;
  var  first  : unmanaged myseq_node(elementType)?;
  var  last   : unmanaged myseq_node(elementType)?;

  proc deinit() {
    var ptr = first;

    while ptr != nil {
      var head = ptr;

      ptr = ptr!.next;

      delete head;
    }
  }

  proc append(e : elementType) : unmanaged myseq(elementType) {
    var anew : unmanaged myseq_node(elementType) = new unmanaged myseq_node(elementType);
    anew.element = e;
    if length > 0 {
      last!.next = anew;
      last = anew;
    } else {
      first = anew;
      last = anew;
    }
    length += 1;
    return _to_unmanaged(this);
  }

  proc print() {
    write("Sequence (length = ", length, ") (/");
    var tmp = first;
    while tmp != nil {
      write(tmp!.element, " ");
      tmp = tmp!.next;
    }
    writeln("/)");
  }
}

var s : unmanaged myseq(int) = new unmanaged myseq(int);

s.append(2);
s.append(3);

s.print();

delete s;
