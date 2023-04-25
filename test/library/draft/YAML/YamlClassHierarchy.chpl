use List, Map;

enum ScalarType {
  YamlFloat,
  YamlInt,
  YamlStr,
  YamlBool,
  YamlBinary,
  Implicit,
  UserDefined,
}

class YamlValue {
  /* index into a YAML mapping by string */
  proc this(key: string): borrowed YamlValue throws {
    throw new YamlKeyError("indexing not supported");
  }

  /* index into a YAML mapping by YAML value */
  proc this(key: owned YamlValue): borrowed YamlValue throws {
    throw new YamlKeyError("indexing not supported");
  }

  /* index into a YAML sequence */
  proc this(idx: int): borrowed YamlValue throws {
    throw new YamlKeyError("indexing not supported");
  }

  /* get the size of a YAML sequence or mapping */
  proc size: int {
    throw new YamlKeyError("size not supported");
  }

  proc tag: string {
    return "";
  }

  proc asString(strict: bool = false): string throws {
    throw new YamlTypeError("cannot convert to string");
  }

  proc asBytes(strict: bool = false): bytes throws {
    throw new YamlTypeError("cannot convert to bytes");
  }

  proc asReal(strict: bool = false): real throws {
    throw new YamlTypeError("cannot convert to real");
  }

  proc asInt(strict: bool = false): int throws {
    throw new YamlTypeError("cannot convert to int");
  }

  proc asBool(strict: bool = false): bool throws {
    throw new YamlTypeError("cannot convert to bool");
  }

  proc asMapOf(type valType): map(string, valType) throws {
    throw new YamlTypeError("cannot convert to map");
  }

  proc asListOf(type t): list(t) throws {
    throw new YamlTypeError("cannot convert to list");
  }

  @chpldoc.nodoc
  proc writeThis(fw) throws {
    if tag != "" then
      fw.write(tag, " ");
  }

  @chpldoc.nodoc
  proc _asKey(): string {
    return this.tag;
  }
}

class YamlNull: YamlValue {
  @chpldoc.nodoc
  override proc _asKey(): string {
    return "null";
  }
}

class YamlScalar: YamlValue {
  var yamlType: ScalarType;
  var userType: string = "";
  var value: string;

  proc init(rawValue: string) {
    if rawValue.startsWith("!!") {
      const (typeTag, _, value) = rawValue.partition(" ");
      select typeTag {
        when "!!float" do this.yamlType = ScalarType.YamlFloat;
        when "!!int" do this.yamlType = ScalarType.YamlInt;
        when "!!str" do this.yamlType = ScalarType.YamlStr;
        when "!!bool" do this.yamlType = ScalarType.YamlBool;
        when "!!binary" do this.yamlType = ScalarType.YamlBinary;
        otherwise {
          writeln("Unknown Yaml-type tag: ", typeTag);
          halt(1);
        }
      }
      this.value = value;
    } else if rawValue.startsWith("!") {
      const (typeTag, _, value) = rawValue.partition(" ");
      this.yamlType = ScalarType.UserDefined;
      this.userType = typeTag[1..];
      this.value = value;
    } else {
      this.yamlType = ScalarType.Implicit;
      if rawValue.startsWith("|") then
        this.value = rawValue[1..].dedent().strip();
      else if rawValue.startsWith(">") then
        this.value = rawValue[1..].dedent().strip().replace("\n", " ");
      else
        this.value = rawValue;
    }
  }

  override proc tag: string {
    if this.yamlType == ScalarType.UserDefined then
      return "!" + this.userType;
    else
      return "";
  }

  override proc asString(strict: bool = false): string throws {
    if strict && (
        yamlType != ScalarType.YamlStr ||
        yamlType != ScalarType.Implicit
    ) then
        throw new YamlTypeError("cannot convert value to string");

    return value;
  }

  override proc asBytes(strict: bool = false): bytes throws {
    if strict && (
        yamlType != ScalarType.YamlBinary ||
        yamlType != ScalarType.YamlStr ||
        yamlType != ScalarType.Implicit
    ) then
        throw new YamlTypeError("cannot convert value to bytes");

    return value: bytes;
  }

  override proc asReal(strict: bool = false): real throws {
    if strict && (
        yamlType != ScalarType.YamlFloat ||
        yamlType != ScalarType.Implicit
    ) then
        throw new YamlTypeError("cannot convert value to real");

    try {
      return value : real;
    } catch {
      throw new YamlTypeError("cannot convert value to real");
    }
  }

  override proc asInt(strict: bool = false): int throws {
    if strict && (
        yamlType != ScalarType.YamlInt ||
        yamlType != ScalarType.Implicit
    ) then
        throw new YamlTypeError("cannot convert value to int");

    try {
      return value : int;
    } catch {
      throw new YamlTypeError("cannot convert value to int");
    }
  }

 override proc asBool(strict: bool = false): bool throws {
    if strict && (
        yamlType != ScalarType.YamlInt ||
        yamlType != ScalarType.Implicit
    ) then
        throw new YamlTypeError("cannot convert value to bool");

    if value == "Yes" then return true;
    if value == "No" then return false;
    throw new YamlTypeError("cannot convert to bool");
  }

  @chpldoc.nodoc
  override proc writeThis(fw) throws {
    super.writeThis(fw);
    select this.yamlType {
      when ScalarType.YamlFloat do fw.write("!!float ");
      when ScalarType.YamlInt do fw.write("!!int ");
      when ScalarType.YamlStr do fw.write("!!str ");
      when ScalarType.YamlBool do fw.write("!!bool ");
      when ScalarType.YamlBinary do fw.write("!!binary ");
      when ScalarType.UserDefined do fw.write("!", this.userType, " ");
      otherwise { }
    }
    fw.write(value);
  }

  @chpldoc.nodoc
  override proc _asKey(): string {
    return value;
  }
}

class YamlSequence: YamlValue {
  var sequence: list(owned YamlValue);

  override proc this(idx: int): borrowed YamlValue throws {
    if idx < 0 || idx >= sequence.size then
      throw new YamlKeyError("index out of bounds");
    return sequence[idx];
  }

  override proc size: int {
    return sequence.size;
  }

  override proc asListOf(type t): list(t) throws {
    var l = new list(t);
    for s in sequence do l.append(s: t);
    return l;
  }

  proc init() {
    this.sequence = new list(owned YamlValue);
  }

  proc _append(in value: owned YamlValue) {
    this.sequence.append(value);
  }

  @chpldoc.nodoc
  override proc writeThis(fw) throws {
    super.writeThis(fw);
    fw.write("[");
    for i in 0..sequence.size-1 do {
      if i > 0 then
        fw.write(", ");
      sequence[i].writeThis(fw);
    }
    fw.write("]");
  }

  @chpldoc.nodoc
  override proc _asKey(): string {
    var s = "[";
    for i in 0..sequence.size-1 do {
      if i > 0 then
        s = s + ", ";
      s = s + sequence[i]._asKey();
    }
    s = s + "]";
    return s;
  }
}

class YamlMapping: YamlValue {
  // var mapping: map(owned YamlValue, owned YamlValue);
  var mapping: map(string, owned YamlValue);

  override proc this(key: string): borrowed YamlValue throws {
    if !mapping.contains(key) then
      throw new YamlKeyError("key not found");
    return mapping[key];
  }

  override proc this(key: owned YamlValue): borrowed YamlValue throws {
    if !mapping.contains(key._asKey()) then
      throw new YamlKeyError("key not found");
    return mapping[key._asKey()];
  }

  override proc size: int {
    return mapping.size;
  }

  override proc asMapOf(type valType): map(string, valType) throws {
    var m = new map(string, valType);
    for k in mapping.keys() do m.add(k, mapping[k]: valType);
    return m;
  }

  proc init() {
    this.mapping = new map(string, owned YamlValue);
  }

  @chpldoc.nodoc
  proc _add(in key: owned YamlValue, in value: owned YamlValue): bool {
    return this.mapping.add(key._asKey(), value);
  }

  @chpldoc.nodoc
  override proc writeThis(fw) throws {
    super.writeThis(fw);
    this.mapping.writeThis(fw);
  }

  @chpldoc.nodoc
  override proc _asKey(): string {
    var s = "{";
    const keys = mapping.keys();
    for i in 0..keys.size-1 do {
      if i > 0 then
        s = s + ", ";
      s = s + keys[i] + ": " + mapping[keys[i]]._asKey();
    }
    s += "}";
    return s;
  }
}

class YamlAlias: YamlValue {
  var alias: string;

  proc init(alias: string) {
    this.alias = alias[1..];
  }

  @chpldoc.nodoc
  override proc writeThis(fw) throws {
    super.writeThis(fw);
    fw.write("*", alias);
  }

  @chpldoc.nodoc
  override proc _asKey(): string {
    return "*" + alias;
  }
}

class YamlTypeError: Error {
  proc init(msg: string) {
    super.init(msg);
  }

  override proc message(): string {
    return "YAML Type Error: " + super.message();
  }
}

class YamlKeyError: Error {
  proc init(msg: string) {
    super.init(msg);
  }

  override proc message(): string {
    return "YAML Key Error: " + super.message();
  }
}

inline operator :(x:YamlValue, type t:string) throws do
  return x.asString();

inline operator :(x:YamlValue, type t:bytes) throws do
  return x.asBytes();

inline operator :(x:YamlValue, type t:real) throws do
  return x.asReal();

inline operator :(x:YamlValue, type t:int) throws do
  return x.asInt();

inline operator :(x:YamlValue, type t:bool) throws do
  return x.asBool();
