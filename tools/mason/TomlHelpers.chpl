

module TomlHelpers {
  import List.list;
  import MasonUtils.MasonError;
  import TOML.Toml;

  proc readStringsFromTable(toml: borrowed Toml, in fields: _tuple,
                            extraFieldsError=true,
                            missingFieldsError=true): fields.type throws {
    assert(toml.tomlType == "toml", "Expected a TOML table");
    proc _tuple.find(key: string) {
      for i in 0..<this.size do if this(i) == key then return i;
      return -1;
    }

    var res: fields.type;
    for k in toml.keys() {
      var idx = fields.find(k);
      if idx != -1 {
        if toml[k]!.tomlType != "string" then
          throw new MasonError("Expected '%s' to be a string".format(k));
        res[idx] = toml[k]!.s;
        fields[idx] = ""; // mark as found
      } else if extraFieldsError {
        throw new MasonError("Unexpected field '%s'".format(k));
      }
    }
    if missingFieldsError {
      for i in 0..<fields.size do if fields[i] != "" then
        throw new MasonError("Missing required field '%s'".format(fields(i)));
    }
    return res;
  }

  proc iterableToTomlArray(iterable, transform:?=none): [] shared Toml? {
    record myTransform {}
    proc myTransform.this(x) {
      const tempRes = if transform.type != nothing then transform(x) else x;
      if tempRes.type == shared Toml? then
        return tempRes;
      else
        return new shared Toml?(tempRes);
    }
    const myTransformInstance = new myTransform();
    const res = [i in iterable] myTransformInstance(i);
    return res;
  }

}
