use Debugger;


proc main() {

  // DWARFDUMP: myBoundedRange
  // DWARFDUMP: range(int(64),both,one)
  // DWARFDUMP: myNegRange
  // DWARFDUMP: range(int(64),both,negOne)
  // DWARFDUMP: myStridedRange
  // DWARFDUMP: range(int(64),both,positive)
  // DWARFDUMP: myStridedNegativeRange
  // DWARFDUMP: range(int(64),both,negative)
  // DWARFDUMP: myAlignedRange
  // DWARFDUMP: myStridedAlignedRange
  var myBoundedRange = 1..10;
  var myNegRange = 10..#10 by -1;
  var myStridedRange = 1..10 by 2;
  var myStridedNegativeRange = 1..#10 by -2;
  var myAlignedRange = 1..10 align 4;
  var myStridedAlignedRange = 1..10 by 2 align 1;

  // DWARFDUMP: myUnboundedRange
  // DWARFDUMP: range(int(64),neither,one)
  // DWARFDUMP: myLowRange
  // DWARFDUMP: range(int(64),low,one)
  // DWARFDUMP: myHighRange
  // DWARFDUMP: range(int(64),high,one)
  // DWARFDUMP: myStridedUnboundedRange
  // DWARFDUMP: range(int(64),neither,positive)
  // DWARFDUMP: myStridedLowRange
  // DWARFDUMP: range(int(64),low,positive)
  // DWARFDUMP: myStridedHighRange
  // DWARFDUMP: range(int(64),high,positive)
  var myUnboundedRange = ..;
  var myLowRange = -11..;
  var myHighRange = ..11;
  var myStridedUnboundedRange = .. by 3;
  var myStridedLowRange = -11.. by 3;
  var myStridedHighRange = ..11 by 3;

  // DWARFDUMP: myInt32Range
  // DWARFDUMP: range(int(32),both,positive)
  // DWARFDUMP: myUint8Range
  // DWARFDUMP: range(uint(8),both,one)
  var myInt32Range = 1:int(32)..10:int(32) by 2;
  var myUint8Range = 1:uint(8)..10:uint(8);

  writeln(myBoundedRange, myNegRange, myStridedRange, myStridedNegativeRange, myAlignedRange, myStridedAlignedRange, sep =" | ");
  writeln(myUnboundedRange, myLowRange, myHighRange, myStridedUnboundedRange, myStridedLowRange, myStridedHighRange, sep =" | ");
  writeln(myInt32Range, myUint8Range, sep =" | ");
  breakpoint;

}
