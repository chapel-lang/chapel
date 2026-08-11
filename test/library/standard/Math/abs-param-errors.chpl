enum TestCases {
  Param64,
  Param32,
  Param16,
  Param8
}
config param testCase = TestCases.Param64;

select testCase {
  when TestCases.Param64 {
    param x: int(64) = min(int(64));
    param r = abs(x);
    writeln("abs(min(int(64))) = ", r);
  }
  when TestCases.Param32 {
    param x: int(32) = min(int(32));
    param r = abs(x);
    writeln("abs(min(int(32))) = ", r);
  }
  when TestCases.Param16 {
    param x: int(16) = min(int(16));
    param r = abs(x);
    writeln("abs(min(int(16))) = ", r);
  }
  when TestCases.Param8 {
    param x: int(8) = min(int(8));
    param r = abs(x);
    writeln("abs(min(int(8))) = ", r);
  }
}
