enum color {red, green, blue};
enum size {small=1, medium, large};
enum gender {decline, male=1, female};
enum mode {read=0b100, write=0b010, rw=0b110};

proc testEnum(type t: enum,
              e: t,
              expectStr: string,
              expectVal: int,
              param tryInt: bool = true) {
  const str = e: string;
  if (str != expectStr) then
    writeln("enum->string cast was a surprise for " + expectStr);

  const e2 = str: t;
  if (e != e2) then
    writeln("string->enum cast was a surprise for " + expectStr);

  if (tryInt == true) {
    const i = e: int;
    if (i != expectVal) then
      writeln("enum->int cast was a surprise for " + expectStr);

    const e3 = i: t;
    if (e3 != e) then
      writeln("int->enum cast was a surprise for " + expectStr);

    writeln(t:string, " ", e, " ", str, " ", e2, " ", i, " ", e3);
  } else {
    writeln(t:string, " ", e, " ", str, " ", e2);
  }
}

testEnum(color, color.red, "red", -1, false);
testEnum(color, color.green, "green", -1, false);
testEnum(color, color.blue, "blue", -1, false);
testEnum(size, size.small, "small", 1);
testEnum(size, size.medium, "medium", 2);
testEnum(size, size.large, "large", 3);
testEnum(gender, gender.female, "female", 2);
testEnum(gender, gender.male, "male", 1);
testEnum(gender, gender.decline, "decline", -1, false);
testEnum(mode, mode.rw, "rw", 6);
testEnum(mode, mode.read, "read", 4);
testEnum(mode, mode.write, "write", 2);
