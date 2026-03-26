/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


module ChapelUnion {
  proc union.getActiveIndex() {
    return __primitive("get_union_id", this);
  }
  @chpldoc.nodoc
  proc _checkUnionAccess(union_: union, param idx: int) {
    import Reflection;
    import HaltWrappers.invalidUnionAccessHalt;
    const activeIdx = union_.getActiveIndex();
    if activeIdx != idx {
      if activeIdx < 0 then
        invalidUnionAccessHalt("illegal union access: no field is active");
      for param i in 0..<Reflection.getNumFields(union_.type) {
        if i != activeIdx then continue;
        param activeField =
          Reflection.getFieldName(union_.type, i);
        param attemptedField = Reflection.getFieldName(union_.type, idx);
        invalidUnionAccessHalt(
          "illegal union access: attempted to access field '" +
          attemptedField + "' but '" + activeField + "' is currently active");
      }
      // if we reach here and haven't halted, just halt
      invalidUnionAccessHalt("illegal union access");
    }
  }
  proc ref union.this(param idx: int) ref {
    import Reflection;
    if chpl_unionAccessChecking {
      _checkUnionAccess(this, idx);
    }
    return Reflection.getFieldRef(this, idx);
  }
  proc const union.this(param idx: int) const {
    import Reflection;
    if chpl_unionAccessChecking {
      _checkUnionAccess(this, idx);
    }
    return Reflection.getField(this, idx);
  }

  /*
    Get
  */
  proc type union.getFieldIndex(param name: string) param {
    import Reflection;
    param idx = Reflection.getFieldIndex(this, name);
    if idx < 0 || idx >= Reflection.getNumFields(this) then
      compilerError("invalid field name passed to getFieldIndex");
    return idx;
  }

  proc union.visit(funcs...) {
    import Reflection;
    if funcs.size != Reflection.getNumFields(this.type) then
      compilerError("wrong number of functions passed to visit");
    const idx = this.getActiveIndex();
    if chpl_unionAccessChecking {
      import HaltWrappers.invalidUnionAccessHalt;
      if idx < 0 then
        invalidUnionAccessHalt("illegal union access: no field is active");
      if idx >= funcs.size then // this should never happen!
        invalidUnionAccessHalt("union index out of bounds");
    }

    for param i in 0..<funcs.size do
      if i == idx then
        funcs[i](Reflection.getField(this, i));
  }
}
