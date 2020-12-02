enum color {red, green, blue};
enum size {small=1, medium, large};
enum gender {decline, male=1, female};
enum mode {read=0b100, write=0b010, rw=0b110};

proc testEnum(type t: enum,
              param e: t,
              param expectStr: string,
              param expectVal: int) {
  param str = e: string;
  if (str != expectStr) then
    compilerWarning("enum->string cast was a surprise for " + expectStr);

  param e2 = str: t;
  if (e != e2) then
    compilerWarning("string->enum cast was a surprise for " + expectStr);

  if (expectVal >= 0) {
    param i = e: int;
    if (i != expectVal) then
      compilerWarning("enum->int cast was a surprise for " + expectStr);

    param e3 = i: t;
    if (e3 != e) then
      compilerWarning("int->enum cast was a surprise for " + expectStr);
  }
}

testEnum(color, color.red, "red", -1);
testEnum(color, color.green, "green", -1);
testEnum(color, color.blue, "blue", -1);
testEnum(size, size.small, "small", 1);
testEnum(size, size.medium, "medium", 2);
testEnum(size, size.large, "large", 3);
testEnum(gender, gender.female, "female", 2);
testEnum(gender, gender.male, "male", 1);
testEnum(gender, gender.decline, "decline", -1);
testEnum(mode, mode.rw, "rw", 6);
testEnum(mode, mode.read, "read", 4);
testEnum(mode, mode.write, "write", 2);
testEnum(gender, gender.decline, "decline", 0);
testEnum(color, color.red, "red", 1);
