/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

// ChapelUtil.chpl
//
// Internal data structures module
//
module ChapelUtil {
  private use ChapelStandard, CTypes;

  //
  // safeAdd: If a and b are of type t, return true iff no
  //  overflow/underflow would occur for a + b
  //
  proc safeAdd(a: ?t, b: t) {
    if !isIntegralType(t) then
      compilerError("Values must be of integral type.");
    if a < 0 {
      if b >= 0 {
        return true;
      } else {
        if b < min(t) - a {
          return false;
        } else {
          return true;
        }
      }
    } else {
      if b <= 0 {
        return true;
      } else {
        if b > max(t) - a {
          return false;
        } else {
          return true;
        }
      }
    }
  }

  //
  // safeSub: If a and b are of type t, return true iff no
  //  underflow/overflow would occur for a - b
  //
  proc safeSub(a: ?t, b: t) {
    if !isIntegralType(t) then
      compilerError("Values must be of integral type.");
    if a < 0 {
      if b <= 0 {
        return true;
      } else {
        // This assumes min(t) = -max(t)+1 (as per the Chapel spec)
        // The reason you need to do this is b/c what you really want to say
        //  is b > abs(min(t)-a), but you can't be guaranteed tha abs()
        //  results is in range of t
        if b > max(t) + a {
          if safeAdd(max(t)+a, 1) {
            if b > max(t) + a + 1 {
              return false;
            } else {
              return true;
            }
          }
          return false;
        } else {
          return true;
        }
      }
    } else {
      if b >= 0 {
        if isUintType(t) then
          if b>a then return false;
        return true;
      } else {
        if b <= min(t) + a {
          return false;
        } else {
          return true;
        }
      }
    }
  }

  //
  // safeMul: If a and b are of type t, return true iff no
  //  overflow/underflow would occur for a * b
  //
  proc safeMul(a: ?t, b:t){
    if !isIntegralType(t) then
      compilerError("Values must be of integral type.");
    if a>0{
      if b>0 {
        if a > max(t)/b then return false;
      }
      else {
        if b < min(t)/a then return false;
      }
    }
    else {
      if b>0{
        if a < min(t)/b then return false;
      }
      else {
        if a!=0 && b < max(t)/a then return false;
      }
    }
    // //if all the case are false then multiplication is safe
    return true;

  }

  // param s is used for error reporting
  // TODO: we can't replace the param c_string here with a param ptr, so ideally
  //  we'd use a chapel string type here instead, but that will require some
  //  compiler changes to work.
  pragma "command line setting"
  proc _command_line_cast(param s: chpl_c_string, type t, x:c_ptrConst(c_char)) {
    if isSyncType(t) then
      compilerError("config variables of sync type are not supported");
    if isAtomicType(t) then
      compilerError("config variables of atomic type are not supported");

    try! {
      var str = string.createCopyingBuffer(x);
      if t == string {
        return str;
      } else {
        use Regex;
        if t==regex(string) || t==regex(bytes) then
          return new regex(str);
        else
          return str:t;
      }
    }
  }

  pragma "no default functions"
  extern record chpl_main_argument {
    var argc: int(64);
    // var argv: c_ptr(c_string);
    var return_value: int(32);
  }

  proc chpl_convert_args(arg: chpl_main_argument) {
    var local_arg = arg;
    pragma "fn synchronization free"
    extern proc chpl_get_argument_i(ref args:chpl_main_argument, i:int(32)):c_ptrConst(c_char);
    // This is odd.  Why are the strings inside the array getting destroyed?
    pragma "no auto destroy"
    var array: [0..#local_arg.argc] string;

    for i in 0..#arg.argc {
      // FIX ME: leak c_string
      try! {
        array[i] = string.createCopyingBuffer(chpl_get_argument_i(local_arg,
                                                                  i:int(32)));
      }
    }

    return array;
  }

  proc chpl_get_mli_connection(arg: chpl_main_argument) {
    var local_arg = arg;
    pragma "fn synchronization free"
    extern proc chpl_get_argument_i(ref args:chpl_main_argument, i:int(32)):c_ptrConst(c_char);
    var flag: c_ptrConst(c_char) = chpl_get_argument_i(local_arg,
                                             (local_arg.argc-2): int(32));
    if (flag != "--chpl-mli-socket-loc") {
      try! halt("chpl_get_mli_connection called with unexpected arguments, missing "
           + "'--chpl-mli-socket-loc <connection>', instead got " +
           string.createCopyingBuffer(flag));
    }
    var result: c_ptrConst(c_char) = chpl_get_argument_i(local_arg,
                                               (local_arg.argc-1): int(32));
    return result;
  }

  //
  // These two are called from the emitted chpl_gen_main(), and
  // defined in the runtime.
  //
  extern proc chpl_rt_preUserCodeHook();
  extern proc chpl_rt_postUserCodeHook();

  extern proc allocate_string_literals_buf(s: int): c_ptrConst(c_char);
  extern proc deallocate_string_literals_buf(): void;

  // Support for module deinit functions.
  config param printModuleDeinitOrder = false;

  proc chpl_addModule(moduleName: chpl_c_string, deinitFun: chpl_c_fn_ptr) {
    chpl_moduleDeinitFuns =
      new unmanaged chpl_ModuleDeinit(moduleName, deinitFun, chpl_moduleDeinitFuns);
  }

  export proc chpl_deinitModules() {
    extern proc printf(fmt:c_ptrConst(c_char));
    extern proc printf(fmt:c_ptrConst(c_char), arg:c_ptrConst(c_char));
    extern proc chpl_execute_module_deinit(deinitFun:chpl_c_fn_ptr);

    if printModuleDeinitOrder then
      printf("Deinitializing Modules:\n");
    var prev = chpl_moduleDeinitFuns;
    while prev {
      const curr = prev!;
      if printModuleDeinitOrder then
        printf("  %s\n", curr.moduleName);
      chpl_execute_module_deinit(curr.deinitFun);
      prev = curr.prevModule;
      delete curr;
    }

    chpl_moduleDeinitFuns = nil;

    // after all other modules are deinited, deallocate string literals
    deallocate_string_literals_buf();
  }
}
