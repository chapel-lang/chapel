/* Convert a Roman number to an Arabic number using string->enum conversions */

enum numeral {I=1, V=5, X=10, L=50, C=100, D=500, M=1000};

var enumEq1000: numeral = numeral.M;
var enumEqFive = 5:numeral;

param enumParamEq1000 = numeral.M;
param enumParamEqFive = numeral.V;


int8or32orDefault(enumEq1000, enumEqFive);
int8or32orDefault(1000, 5);
writeln();

int8vs32(numeral.I);
int8vs32(numeral.M);
int8vs32(enumEq1000);
int8vs32(enumEqFive);
int8vs32(enumParamEq1000);
int8vs32(enumParamEqFive);
writeln();

int8paramVsNot(1);
int8paramVsNot(numeral.I);
int8paramVsNot(numeral.M);
int8paramVsNot(enumEq1000);
int8paramVsNot(enumEqFive);
int8paramVsNot(enumParamEq1000);
int8paramVsNot(enumParamEqFive);
writeln();

int16paramVsNot(1);
int16paramVsNot(numeral.I);
int16paramVsNot(numeral.M);
int16paramVsNot(enumEq1000);
int16paramVsNot(enumEqFive);
int16paramVsNot(enumParamEq1000);
int16paramVsNot(enumParamEqFive);
writeln();

int8vs16(1);
int8vs16(numeral.I);
int8vs16(numeral.M);
int8vs16(enumEq1000);
int8vs16(enumEqFive);
int8vs16(enumParamEq1000);
int8vs16(enumParamEqFive);
writeln();

int8vs16vsDefault(1);
int8vs16vsDefault(numeral.I);
int8vs16vsDefault(numeral.M);
int8vs16vsDefault(enumEq1000);
int8vs16vsDefault(enumEqFive);
int8vs16vsDefault(enumParamEq1000);
int8vs16vsDefault(enumParamEqFive);
writeln();


////

proc int8or32orDefault(x: int(8), y: int(8)) {
  writeln("In int(8) int8or32orDefault");
}

proc int8or32orDefault(x: int(32), y: int(32)) {
  writeln("In int(32) int8or32orDefault");
}

proc int8or32orDefault(x: int, y: int) {
  writeln("In int int8or32orDefault");
}

////

proc int8vs32(x: int(8)) {
  writeln("In int(8) int8vs32");
}

proc int8vs32(param x: int(8)) {
  writeln("In int(8) param int8vs32");
}

proc int8vs32(x: int(32)) {
  writeln("In int(32) int8vs32");
}

proc int8vs32(param x: int(32)) {
  writeln("In int(32) param int8vs32");
}


////

proc int8paramVsNot(x: int(8)) {
  writeln("In int(8) int8paramVsNot");
}

proc int8paramVsNot(param x: int(8)) {
  writeln("In param int(8) int8paramVsNot");
}


////

proc int16paramVsNot(x: int(16)) {
  writeln("In int(16) int16paramVsNot");
}

proc int16paramVsNot(param x: int(16)) {
  writeln("In param int(16) int16paramVsNot");
}


////

proc int8vs16(x: int(8)) {
  writeln("In int(8) int8vs16");
}

proc int8vs16(param x: int(8)) {
  writeln("In param int(8) int8vs16");
}

proc int8vs16(x: int(16)) {
  writeln("In int(16) int8vs16");
}

proc int8vs16(param x: int(16)) {
  writeln("In param int(16) int8vs16");
}



////

proc int8vs16vsDefault(x: int(8)) {
  writeln("In int(8) int8vs16vsDefault");
}

proc int8vs16vsDefault(param x: int(8)) {
  writeln("In param int(8) int8vs16vsDefault");
}

proc int8vs16vsDefault(x: int(16)) {
  writeln("In int(16) int8vs16vsDefault");
}

proc int8vs16vsDefault(param x: int(16)) {
  writeln("In param int(16) int8vs16vsDefault");
}

proc int8vs16vsDefault(x: int) {
  writeln("In int int8vs16vsDefault");
}

proc int8vs16vsDefault(param x: int) {
  writeln("In param int int8vs16vsDefault");
}

