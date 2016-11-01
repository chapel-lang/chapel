config const m = 9;
config const n = 7;
config const o = 5;

//
// This test exercises the three branches of the bulk transfer calls
// for DefaultRectangular arrays
//
enum testType { localGet, localPut, remoteGet };

config const ttype = testType.localGet;

config const initialize = true;
config const printOutput = false;
config const printTiming = false;

const shift = (123, 456, 789);

use Random, Time;

if numLocales < 3 then halt("numLocales must be >= 3");
var declLocale = Locales(numLocales-1);
var putLocale = Locales(0);
var remoteLocale = Locales(divfloorpos(numLocales, 2));
if printOutput {
  if ttype==testType.localGet then
    writeln("Exercising local get on ", declLocale);
  if ttype==testType.localPut then
    writeln("Exercising local put on ", putLocale);
  if ttype==testType.remoteGet then
    writeln("Exercising remote get on ", remoteLocale);
}

var Dom: domain(3) = {1..m, 1..n, 1..o};

var Ref: [Dom] real;
var B: [Dom] real;
if initialize {
  fillRandom(B, 31415);
  fillRandom(Ref, 31415);
}
on declLocale {
  var A: [Dom] real;

  A = -1;
  var dt = assignMe(A, B);
  for i in A.domain do
    if A[i] != Ref[i] then
      writeln("ERROR in whole array assignment: ", i);
  if printOutput then
    writeln("whole array assignment ", (m,n,o), ":\n", A);
  if printTiming then
    writeln("whole array assignment ", (m,n,o), ": ", dt);

}

var C: [Dom.translate(shift)] real;
if initialize {
  fillRandom(C, 92653);
  fillRandom(Ref, 92653);
}
on declLocale {
  var A: [Dom] real;

  A = -1;
  var dt = assignMe(A, C);
  for i in A.domain do
    if A[i] != Ref[i] then
      writeln("ERROR in whole array assignment (+ offset): ", i);
  if printOutput then
    writeln("whole array assignment (+ offset) ", (m,n,o), ":\n", A);
  if printTiming then
    writeln("whole array assignment (+ offset) ", (m,n,o), ": ", dt);
}

var D: [Dom.translate(-shift)] real;
if initialize {
  fillRandom(D, 58979);
  fillRandom(Ref, 58979);
}
on declLocale {
  var A: [Dom] real;
  A = -1;
  var dt = assignMe(A, D);
  for i in A.domain do
    if A[i] != Ref[i] then
      writeln("ERROR in whole array assignment (- offset): ", i);
  if printOutput then
    writeln("whole array assignment (- offset) ", (m,n,o), ":\n", A);
  if printTiming then
    writeln("whole array assignment (- offset) ", (m,n,o), ": ", dt);
}

// aliased arrays
var DomSlice = {1..m/2, 1..n, 1..o};
var Refa => Ref[DomSlice];
var Ba => B[DomSlice];
if initialize then {
  fillRandom(Ba, 31415);
  fillRandom(Refa, 31415);
}
on declLocale {
  var DomSlice = {1..m/2, 1..n, 1..o}; // this declaration is here due to a bug
  var A: [Dom] real;
  var Aa => A[DomSlice];
  A = 0;
  Aa = -1;
  var dt = assignMe(Aa, Ba);
  for i in Aa.domain do
    if Aa[i] != Refa[i] then
      writeln("ERROR in whole array assignment (alias): ", i);
  if printOutput then
    writeln("whole array assignment (alias) ", (m,n,o), ":\n", Aa);
  if printTiming then
    writeln("whole array assignment (alias) ", (m,n,o), ": ", dt);
}

var Ca => C[DomSlice.translate(shift)];
if initialize {
  fillRandom(Ca, 92653);
  fillRandom(Refa, 92653);
}
on declLocale {
  var DomSlice = {1..m/2, 1..n, 1..o}; // this declaration is here due to a bug
  var A: [Dom] real;
  var Aa => A[DomSlice];
  A = 0;
  Aa = -1;
  var dt = assignMe(Aa, Ca);
  for i in Aa.domain do
    if Aa[i] != Refa[i] then
      writeln("ERROR in whole array assignment (alias, + offset): ", i);
  if printOutput then
    writeln("whole array assignment (alias, + offset) ", (m,n,o), ":\n", Aa);
  if printTiming then
    writeln("whole array assignment (alias, + offset) ", (m,n,o), ": ", dt);
}

var Da => D[DomSlice.translate(-shift)];
if initialize {
  fillRandom(Da, 58979);
  fillRandom(Refa, 58979);
}
on declLocale {
  var DomSlice = {1..m/2, 1..n, 1..o}; // this declaration is here due to a bug
  var A: [Dom] real;
  var Aa => A[DomSlice];
  A = 0;
  Aa = -1;
  var dt = assignMe(Aa, Da);
  for i in Aa.domain do
    if Aa[i] != Refa[i] then
      writeln("ERROR in whole array assignment (alias, - offset): ", i);
  if printOutput then
    writeln("whole array assignment (alias, - offset) ", (m,n,o), ":\n", Aa);
  if printTiming then
    writeln("whole array assignment (alias, - offset) ", (m,n,o), ": ", dt);
}


proc assignMe(A, B) {
  var st, dt: real;
  select ttype {
    when testType.localGet {
      st = getCurrentTime();
      A = B;
      dt = getCurrentTime()-st;
    }
    when testType.localPut {
      on putLocale {
        st = getCurrentTime();
        A = B;
        dt = getCurrentTime()-st;
      }
    }
    when testType.remoteGet {
      on remoteLocale {
        st = getCurrentTime();
        A = B;
        dt = getCurrentTime()-st;
      }
    }
  }
  return dt;
}
