record R  { var x: int; }
record RR { var r: R;   } // record of record
class  CR { var r: R;   } // class of record

proc main() {
  var rr: RR;
  var cr = new CR();
  forall i in 1..1000 do
    atomic {
      rr.r.x += 1;
      cr.r.x += 2;
    }
  writeln("rr.r.x = ", rr.r.x);
  writeln("cr.r.y = ", cr.r.x);
}