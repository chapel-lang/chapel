use Time;

// test cast-from-string performance

config const timing = false;
config const numTrials = 1;

// Values below picked so that the sum is simple to verify:
//
// 12...8900 + 98...2100 == 11..11000
//                         + 11..1100
//                         + 55..5500
//                     ==   177..7600


var int64A = [ "12345678900", "  11111111100", "98765432100  ", "    55555555500     " ];
var int32A = [ "12345", "  11111", "98765  ", "      55555     " ];
var int16A = [ "123", "  111", "987  ", "     555    " ];
var int8A = [ "12", "  11", "98  ", "    55    " ];

var real64A = [ "1234567.89", "  1.11111111e6", "9876543.21  ", "   5555555.55  "];
var real32A = [ "12345.67", "  1.111111e4", "98765.43  ", "   55555.55   "];

var imag64A = [ "1234567.89i", "  1.11111111e6i", "9876543.21i  ", "   5555555.55i  "];
var imag32A = [ "12345.67i", "  1.111111e4i", "98765.43i  ", "   55555.55i   "];

var comp128A = [ "1234567.89 + 1234567.89i",
		"  1.11111111e6 +    2.22222222e6i",
		"9876543.21  + 9876543.21i  ",
		"   5555555.55   + 5555555.55i  "];

var comp64A  = [ "12345.67 + 12345.67i",
		"  1.111111e4 +    2.222222e4i",
		"98765.43  + 98765.43i   ",
		"   55555.55   + 55555.55i  "];

proc test_type(type t, input_arr, type acc_type) {
  var timer: Timer;
  var acc: acc_type;

  timer.start();
  for i in 1..numTrials {
    for j in input_arr {
      acc += j:t:acc_type;
    }
  }
  timer.stop();

  // Divide result/numTrials so that the .good file doesn't depend on
  // numTrials.  (modulo overflow / precision issues)
  if t == imag(32) || t == real(32) {
    // XXX imag(32)/int yields complex(128)...
    writeln(t:string, " ", acc/numTrials:real(32));
  } else {
    writeln(t:string, " ", acc/numTrials);
  }

  return timer.elapsed();
}

var int64_time = test_type(int(64), int64A, int);
var int32_time = test_type(int(32), int32A, int);
var int16_time = test_type(int(16), int16A, int);
var int8_time = test_type(int(8), int8A, int);
var real64_time = test_type(real(64), real64A, real);
var real32_time = test_type(real(32), real32A, real);
var imag64_time = test_type(imag(64), imag64A, imag);
var imag32_time = test_type(imag(32), imag32A, imag);
var comp128_time = test_type(complex(128), comp128A, complex);
var comp64_time = test_type(complex(64), comp64A, complex);

if (timing) {
  writeln("int64 ", int64_time);
  writeln("int32 ", int32_time);
  writeln("int16 ", int16_time);
  writeln("int8 ",  int8_time);
  writeln("real64 ", real64_time);
  writeln("real32 ", real32_time);
  writeln("imag64 ", imag64_time);
  writeln("imag32 ", imag32_time);
  writeln("complex128 ", comp128_time);
  writeln("complex64 ", comp64_time);
}
