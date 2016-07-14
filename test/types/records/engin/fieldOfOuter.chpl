class outerClass {
  var field = 5;
  var r: innerRecord;
  // var r = new innerRecord(); //a possible workaround

  record innerRecord{
    proc innerRecordProc(){
      // following line doesn't work with "var r: innerRecord" above
      writeln(outer.field); //outer or no outer doesn't make a difference
    }
  }
}

var c = new outerClass();
c.r.innerRecordProc();
