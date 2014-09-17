// DO NOT EDIT - THIS FILE IS GENERATED AUTOMATICALLY.
// Ensure that an error is reported for a would-be var iterator variable.
var ABC: [1..3] int;
for aaaaa in ABC {
  begin {
    aaaaa = 5;
  }
  cobegin {
    aaaaa = 6;
    aaaaa = 7;
  }
  coforall iiiii in 1..3 {
    aaaaa = 8;
  }
}
coforall aaaaa in ABC {
  begin {
    aaaaa = 5;
  }
  cobegin {
    aaaaa = 6;
    aaaaa = 7;
  }
  coforall iiiii in 1..3 {
    aaaaa = 8;
  }
}
