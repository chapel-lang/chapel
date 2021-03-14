use ExampleErrors;

config const option = 2;

proc printErrors(errors: borrowed TaskErrors, depth:int)
{
  for e in errors { 
    var g = e:borrowed TaskErrors?;
    if g {
      writeln(" "*depth, "TaskErrors");
      printErrors(g!, depth + 1);
    } else
      writeln(" "*depth, "err:", e!.message());
  }
}


proc test() {
  try {
    writeln("before coforall block");
    coforall i in 1..2 {
      coforall j in 1..2 {
        coforall k in 1..2 {
          if k <= option then
            throw new owned StringError("test error");
        }
      }
    }
    writeln("after coforall block");
  } catch errors: TaskErrors {
    printErrors(errors,0);
  } catch e {
    writeln("Caught other error ", e.message());
  }
}

test();
