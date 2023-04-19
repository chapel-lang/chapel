use List, Map;

class YamlDocument {
  var root: owned YamlValue;

}

class YamlValue { }

class YamlSequence: YamlValue {
  var sequence: list(owned YamlValue);

  proc this(idx: int): owned YamlValue throws {
    if idx < 0 || idx >= sequence.size then
      throw new YamlKeyError("index out of bounds");
    return sequence[idx];
  }

  proc asList(type t): list(t) throws {
    unimplemented();
    return new list(t);
  }

  proc size: int {
    return sequence.size;
  }

  proc init() {
    this.sequence = new list(owned YamlValue);
  }

  proc append(value: owned YamlValue) {
    this.sequence.append(value);
  }
}

class YamlMapping: YamlValue {
  var mapping: map(owned YamlValue, owned YamlValue);

  proc this(key: borrowed YamlValue): owned YamlValue throws {
    if !mapping.contains(key) then
      throw new YamlKeyError("key not found");
    return mapping[key];
  }

  proc this(key: string): owned YamlValue throws {
    const yKey = new owned YamlString(key);
    if !mapping.contains(yKey) then
      throw new YamlKeyError("key not found");
    return mapping[yKey];
  }

  proc asMap(type keyType, type valType): map(keyType, valType) throws {
    unimplemented();
    return new map(keyType, valType);
  }

  proc size: int {
    return mapping.size;
  }

  proc init() {
    this.mapping = new map(owned YamlValue, owned YamlValue);
  }

  proc add(key: owned YamlValue, value: owned YamlValue): bool {
    return this.mapping.add(key, value);
  }
}

class YamlString: YamlValue {
  var value: string;

  proc asString(): string {
    return value;
  }

  proc type from(s: string): owned YamlString {
    return new owned YamlString(s);
  }
}

class YamlBool: YamlValue {
  var value: bool;

  proc asBool(): bool {
    return value;
  }

  proc init(value: bool) {
    this.value = value;
  }
}

class YamlNumber: YamlValue {
  // TODO: use a union here
  var intValue: int = -1;
  var realValue: real = -1.0;
  var isReal: bool = false;

  proc asInt(): int throws {
    if isReal then
      throw new YamlTypeError("cannot retrieve integer value from real");
    return intValue;
  }

  proc asReal(): real throws {
    if !isReal then
      throw new YamlTypeError("cannot retrieve real value from integer");
    return realValue;
  }
}

class YamlNull: YamlValue { }

class YamlTagged: YamlValue {
  var tag: string;
  var value: owned YamlValue;
}

class YamlKeyError: Error {
  proc init(msg: string) {
    super.init(msg);
  }

  override proc message(): string {
    return "YAML Key Error: " + super.message();
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

inline proc unimplemented() {
  import Reflection.getRoutineName;
  writeln("'", getRoutineName(), "' unimplemented!");
}
