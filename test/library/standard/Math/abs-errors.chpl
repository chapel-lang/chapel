enum TestCases {
  Min64,
  Min32,
  Min16,
  Min8
}
config const testCase = TestCases.Min64;


select testCase {
  when TestCases.Min64 {
    const m64: int(64) = min(int(64));
    const r = abs(m64);
    writeln("abs(min(int(64))) = ", r);
  }
  when TestCases.Min32 {
    const m32: int(32) = min(int(32));
    const r = abs(m32);
    writeln("abs(min(int(32))) = ", r);
  }
  when TestCases.Min16 {
    const m16: int(16) = min(int(16));
    const r = abs(m16);
    writeln("abs(min(int(16))) = ", r);
  }
  when TestCases.Min8 {
    const m8: int(8) = min(int(8));
    const r = abs(m8);
    writeln("abs(min(int(8))) = ", r);
  }
}
