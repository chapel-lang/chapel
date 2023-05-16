use IO, List;

config const programInput = "day24.test";
config const input = "";

param W=0, X=1, Y=2, Z=3;

var line: string;
var vals: [0..3] int;

var infile = open(programInput, ioMode.r).reader();
var code: list(string);
var numruns = 0;


while infile.readLine(line) {
  line = line.strip();
  code.pushBack(line);
}

if input=="" {
  run();
} else if input == "auto" {
  for i in 11111111111111..99999999999999 by -1 {
//  for i in 1..9 by -1 {
//  for i in 11111..99999 by -1 {
    var stri = i: string;
    if stri.find("0") == -1 {  // TODO: feature request for .contains()
      run(stri);
      if vals[Z] == 0 {
        writeln("Found answer: ", i);
        break;
      }
    }
  }
} else {
  run(input);
}

proc run(input: string = "") {
  numruns += 1;
//  if numruns % 1000000 == 0 then
    writeln("Running on input: ", input);
  var nextDigit = 0;
  vals = 0;
  for cmd in code {
//    writeln("Processing command: ", cmd);
    const op = cmd[0..2];
    ref lhs = vals[strToReg(cmd[4])];
    if op == "inp" {
      if input == "" {
        writeln("Waiting to read int: ");
        lhs = read(int);
      } else {
        lhs = input[nextDigit]:int;
        nextDigit += 1;
      }
    } else {
      const rhs = strToVal(cmd[6..]);
      select op {
        when "add" {
          lhs += rhs;
        }
        when "mul" {
          lhs *= rhs;
        }
        when "div" {
          lhs /= rhs;
        }
        when "mod" {
          lhs %= rhs;
        }
        when "eql" {
          lhs = lhs == rhs;
        }
        otherwise {
          halt("unexpected opcode: ", cmd[0..2]);
        }
      }
    }
  }
//  if numruns % 1000000 == 0 then
    writeln(vals);
}


proc strToReg(regname: string) {
//  writeln(regname);
  const regbyte = regname.toByte();
  if (regbyte >= "w".toByte() && regbyte <= "z".toByte()) {
    return regbyte - "w".toByte();
  } else {
    return -1;
  }
}

proc strToVal(regname: string) {
//  writeln(regname);
  const regbyte = regname[0].toByte();
  if (regbyte >= "w".toByte() && regbyte <= "z".toByte()) {
    return vals[strToReg(regname)];
  } else {
    return regname:int;
  }
}

proc readNextDigit() {
  return 9;
}
