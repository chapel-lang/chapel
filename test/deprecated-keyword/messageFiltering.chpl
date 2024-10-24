// See: https://chapel-lang.org/docs/tools/chpldoc/chpldoc.html#inline-markup-2
// and: https://chapel-lang.org/docs/tools/chpldoc/chpldoc.html#paragraph-level-markup

// No filtering should be applied:
@deprecated(notes="--- No filtering should be applied: ---")
var x1000 = 1000;
@deprecated(notes="sample_message")
var x1001 = 1001;
@deprecated(notes="Lorem ipsum dolor sit amet")
var x1002 = 1002;
@deprecated(notes="Lorem_ipsum_dolor_sit_amet")
var x1003 = 1003;
@deprecated(notes="mod proc iter data const var param type class record")
var x1004 = 1004;
@deprecated(notes=":mod proc:`abc` :iter data:`def` :const var:`ghi`")
var x1005 = 1005;
@deprecated(notes=":param type:`jkl` :class record:`mno`")
var x1006 = 1006;

// Test filtering at start, middle, and end of msg
@deprecated(notes="--- Test filtering at start, middle, and end of msg (with and without content): ---")
var x2000 = 2000;
@deprecated(notes=":proc:`abc` Lorem ipsum dolor sit amet")
var x2001 = 2001;
@deprecated(notes="Lorem ipsum :proc:`abc` dolor sit amet")
var x2002 = 2002;
@deprecated(notes="Lorem ipsum dolor sit amet :proc:`abc`")
var x2003 = 2003;
@deprecated(notes=":proc:`abc` Lorem :proc:`def` ipsum dolor sit amet :proc:`ghi`")
var x2004 = 2004;

// Test different text in ::s (all should filter)
@deprecated(notes="--- Test different text in ::s (all should filter) ---")
var x3000 = 3000;
@deprecated(notes="Lorem ipsum :mod:`test` dolor sit amet")
var x3001 = 3001;
@deprecated(notes="Lorem ipsum :proc:`test` dolor sit amet")
var x3002 = 3002;
@deprecated(notes="Lorem ipsum :iter:`test` dolor sit amet")
var x3003 = 3003;
@deprecated(notes="Lorem ipsum :data:`test` dolor sit amet")
var x3004 = 3004;
@deprecated(notes="Lorem ipsum :const:`test` dolor sit amet")
var x3005 = 3005;
@deprecated(notes="Lorem ipsum :var:`test` dolor sit amet")
var x3006 = 3006;
@deprecated(notes="Lorem ipsum :param:`test` dolor sit amet")
var x3007 = 3007;
@deprecated(notes="Lorem ipsum :type:`test` dolor sit amet")
var x3008 = 3008;
@deprecated(notes="Lorem ipsum :class:`test` dolor sit amet")
var x3009 = 3009;
@deprecated(notes="Lorem ipsum :record:`test` dolor sit amet")
var x3010 = 3010;
@deprecated(notes="Lorem ipsum :attr:`test` dolor sit amet")
var x3011 = 3011;
@deprecated(notes="Lorem ipsum :enum:`test` dolor sit amet")
var x3012 = 3012;
@deprecated(notes="Lorem ipsum :enumconstant:`test.constant` dolor sit amet")
var x3013 = 3013;
@deprecated(notes="Lorem :mod:`abc` ipsum :proc:`def` dolor :iter:`ghi` sit :data:`jkl` amet")
var x3014 = 3014;
@deprecated(notes="Lorem :const:`abc` ipsum :var:`def` dolor :param:`ghi` sit :type:`jkl` amet")
var x3015 = 3015;
@deprecated(notes="Lorem :class:`abc` ipsum :record:`def` dolor :attr:`ghi` sit amet")
var x3016 = 3016;

// Test different text in ::s (all should not filter)
@deprecated(notes="--- Test different text in ::s (all should not filter) ---")
var x4000 = 4000;
@deprecated(notes="Lorem ipsum ::`abc` dolor sit amet")
var x4001 = 4001;
@deprecated(notes="Lorem ipsum :a:`abc` dolor sit amet")
var x4002 = 4002;
@deprecated(notes="Lorem ipsum :abc123def:`abc` dolor sit amet")
var x4003 = 4003;
@deprecated(notes="Lorem ipsum :123abc123def:`abc` dolor sit amet")
var x4004 = 4004;
@deprecated(notes="Lorem ipsum :abc123def123:`abc` dolor sit amet")
var x4005 = 4005;
@deprecated(notes="Lorem ipsum :123abc123def123:`abc` dolor sit amet")
var x4006 = 4006;
@deprecated(notes="Lorem ipsum :abc_123_def:`abc` dolor sit amet")
var x4007 = 4007;

// Test different text in ``s
@deprecated(notes="--- Test different text in ``s ---")
var x5000 = 5000;
@deprecated(notes="Lorem ipsum :proc:`` dolor sit amet (should not filter)")
var x5001 = 5001;
@deprecated(notes="Lorem ipsum :proc:`a` dolor sit amet")
var x5002 = 5002;
@deprecated(notes="Lorem ipsum :proc:`abc123def` dolor sit amet")
var x5003 = 5003;
@deprecated(notes="Lorem ipsum :proc:`abc_123_def` dolor sit amet")
var x5004 = 5004;
@deprecated(notes="Lorem ipsum :proc:`123abc_123_def` dolor sit amet")
var x5005 = 5005;
@deprecated(notes="Lorem ipsum :proc:`abc_123_def123` dolor sit amet")
var x5006 = 5006;
@deprecated(notes="Lorem ipsum :proc:`123abc_123_def123` dolor sit amet")
var x5007 = 5007;
@deprecated(notes="Lorem ipsum :proc:`_abc_123_def_` dolor sit amet")
var x5008 = 5008;
@deprecated(notes="Lorem ipsum :proc:`abc:def` dolor sit amet (should not filter)")
var x5009 = 5009;
@deprecated(notes="Lorem ipsum :proc:`proc` dolor sit amet")
var x5010 = 5010;
@deprecated(notes="Lorem ipsum :proc:`:proc:` dolor sit amet (should not filter)")
var x5011 = 5011;
@deprecated(notes="Lorem ipsum :proc:`:proc:abc` dolor sit amet (should not filter)")
var x5012 = 5012;
@deprecated(notes="Lorem ipsum :proc:`title target` dolor sit amet (should not filter)")
var x5013 = 5013;
@deprecated(notes="Lorem ipsum :proc:`abc.def` dolor sit amet")
var x5014 = 5014;

// Other :s in message:
@deprecated(notes="--- Other :s in message: ---")
var x6000 = 6000;
@deprecated(notes="Instead of using foo(a: int, b: string) use :proc:`bar`.")
var x6001 = 6001;
@deprecated(notes="Instead of using foo(a:int,b:string) use :proc:`bar`.")
var x6002 = 6002;
@deprecated(notes="Instead of using foo(a:int):int use :proc:`bar`.")
var x6003 = 6003;
@deprecated(notes="Instead of using foo(proc: int, proc: string) use :proc:`bar`.")
var x6004 = 6004;
@deprecated(notes="Instead of using foo(proc:int,proc:string) use :proc:`bar`.")
var x6005 = 6005;
@deprecated(notes="Instead of using foo(proc:int):proc use :proc:`bar`.")
var x6006 = 6006;
@deprecated(notes="proc foo(x: int) is deprecated, use proc foo(x: real) instead")
var x6007 = 6007;

// Word boundaries (should filter)
@deprecated(notes="--- Word boundaries (should filter) ---")
var x7000 = 7000;
@deprecated(notes="First (:proc:`middle`) end")
var x7001 = 7001;
@deprecated(notes="First, :proc:`middle`, end")
var x7002 = 7002;
@deprecated(notes="First,:proc:`middle`,end")
var x7003 = 7003;

// Word boundaries (should not filter)
@deprecated(notes="--- Word boundaries (should not filter) ---")
var x8000 = 8000;
@deprecated(notes="First:proc:`middle` end")
var x8001 = 8001;
@deprecated(notes="First :proc:`middle`end")
var x8002 = 8002;
@deprecated(notes="First:proc:`middle`end")
var x8003 = 8003;
@deprecated(notes="First:proc:`middle`:end")
var x8004 = 8004;

// Dollars in identifier (should filter)
@deprecated(notes="--- Dollars in identifier (should filter) ---")
var x9000 = 9000;
@deprecated(notes="Lorem ipsum :proc:`abc$` dolor sit amet")
var x9001 = 9001;
@deprecated(notes="Lorem ipsum :proc:`abc$def` dolor sit amet")
var x9002 = 9002;
@deprecated(notes="Lorem ipsum :proc:`abc$def$` dolor sit amet")
var x9003 = 9003;
@deprecated(notes="Lorem ipsum :proc:`abc$def$ghi` dolor sit amet")
var x9004 = 9004;

// Proc lead with ! (should filter)
@deprecated(notes="--- Identifier is led with ! (should filter if well formed) ---")
var x1101 = 1101;
@deprecated(notes="Lorem ipsum :proc:`!abc` dolor sit amet")
var x1102 = 1102;
@deprecated(notes="Lorem ipsum :proc:`a!bc` dolor sit amet")
var x1103 = 1103;

// Prefixed with ~, should filter to just last component
@deprecated(notes="--- Prefixed with ~, should filter to just last component if well formed ---")
var x1200 = 1200;
@deprecated(notes="Lorem ipsum :proc:`~abc.def` dolor sit amet")
var x1201 = 1201;
@deprecated(notes="Lorem ipsum :proc:`a~bc.def` dolor sit amet")
var x1202 = 1202;


// Using rst hyperlink syntax
@deprecated(notes="--- Text using the rst hyperlink syntax ---")
var x1300 = 1300;
@deprecated(notes="Lorem ipsum :proc:`abc<def.ghi>` dolor sit amet")
var x1301 = 1301;
@deprecated(notes="Lorem ipsum :proc:`abc with spaces<def.ghi>` dolor sit amet")
var x1302 = 1302;
@deprecated(notes="Lorem ipsum :proc:`a-b_c<def.ghi>` dolor sit amet")
var x1303 = 1303;
// this should not filter
@deprecated(notes="Lorem ipsum :proc:`abc <def.ghi>` dolor sit amet")
var x1304 = 1304;

// I purposefully access each variable on a separate line so the produced warning messages
// will also have unique lines:

writeln(x1000);
writeln(x1001);
writeln(x1002);
writeln(x1003);
writeln(x1004);
writeln(x1005);
writeln(x1006);

writeln(x2000);
writeln(x2001);
writeln(x2002);
writeln(x2003);
writeln(x2004);

writeln(x3000);
writeln(x3001);
writeln(x3002);
writeln(x3003);
writeln(x3004);
writeln(x3005);
writeln(x3006);
writeln(x3007);
writeln(x3008);
writeln(x3009);
writeln(x3010);
writeln(x3011);
writeln(x3012);
writeln(x3013);
writeln(x3014);
writeln(x3015);
writeln(x3016);

writeln(x4000);
writeln(x4001);
writeln(x4002);
writeln(x4003);
writeln(x4004);
writeln(x4005);
writeln(x4006);
writeln(x4007);

writeln(x5000);
writeln(x5001);
writeln(x5002);
writeln(x5003);
writeln(x5004);
writeln(x5005);
writeln(x5006);
writeln(x5007);
writeln(x5008);
writeln(x5009);
writeln(x5010);
writeln(x5011);
writeln(x5012);
writeln(x5013);
writeln(x5014);

writeln(x6000);
writeln(x6001);
writeln(x6002);
writeln(x6003);
writeln(x6004);
writeln(x6005);
writeln(x6006);
writeln(x6007);

writeln(x7000);
writeln(x7001);
writeln(x7002);
writeln(x7003);

writeln(x8000);
writeln(x8001);
writeln(x8002);
writeln(x8003);
writeln(x8004);

writeln(x9000);
writeln(x9001);
writeln(x9002);
writeln(x9003);
writeln(x9004);

writeln(x1101);
writeln(x1102);
writeln(x1103);

writeln(x1200);
writeln(x1201);
writeln(x1202);

writeln(x1300);
writeln(x1301);
writeln(x1302);
writeln(x1303);
writeln(x1304);
