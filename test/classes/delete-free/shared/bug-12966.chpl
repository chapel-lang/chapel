class C {
  var x: int;
  var next: shared C?;

  proc clone(): shared C {
    return new shared C(
      this.x,
      if next == nil then nil else this.next
    );
  }

/*  // This variant without ternary-if expression is okay.
  proc clone(): shared C {
    if next == nil then return new shared C(this.x, nil);
    return new shared C(this.x, this.next);
  }
*/
}

var x = new shared C(1);
var y = new shared C(2, x);

var y2 = y.clone();
var x2 = y.next!.clone();

writeln(x);
writeln(x2);
writeln(y);
writeln(y2);
