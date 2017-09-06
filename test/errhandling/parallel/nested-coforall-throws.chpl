proc printErrors(errors: TaskErrors)
{
  for e in errors { 
    var g = e:TaskErrors;
    if g then
      printErrors(g);
    else
      writeln("err:", e.msg);
  }
}


proc test() {
  try {
    writeln("before coforall block");
    coforall i in 1..2 {
      coforall j in 1..2 {
        coforall k in 1..2 {
          throw new Error("test error");
        }
      }
    }
    writeln("after coforall block");
  } catch errors: TaskErrors {
    printErrors(errors);
  } catch e {
    writeln("Caught other error ", e.msg);
  }
}

test();
