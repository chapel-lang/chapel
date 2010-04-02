record R  { var x: int; }
record RR { var r: R;   } // record of record
class  CR { var r: R;   } // class of record

def main() {
  var rr: RR;
  var cr = new CR();
  atomic {
    rr.r.x = 1;
    cr.r.x = 2;
  }
}