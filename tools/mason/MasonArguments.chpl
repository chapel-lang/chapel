use List;
use Map;

/*

this is a start at making something with an API like Python argparse

class ArgumentError : Error {
  var msg:string;
  proc init(msg:string) {
    this.msg = msg;
  }
  override proc message() {
    return msg;
  }
}

// indicates a result of argument parsing
class Argument {
  var present: bool;
  var values: list(string);

  proc boolValue() throws {
    if values.size == 0 {
      return false;
    } else if values.size == 1 {
      if values.first == 'true' then
        return true;
      else if values.first == 'false' || values.first == '' then
        return false;
      else
        throw new ArgumentError("boolValue called on non-bool Argument");
    } else { 
      throw new ArgumentError("boolValue called on non-bool Argument");
    }
    return false;
  }
  proc listValue() {
    return values;
  }
  proc stringValue() throws {
    if values.size != 1 {
      throw new ArgumentError("stringValue called on non-string Argument");
    }
    return values.first;
  }
}

class ArgumentHandler {
  proc noteMatch(startIndex: int) {
    present = true;
    this.startIndex = startIndex;
  }

  // consumes matching arguments starting at args[i]
  // possibly by appending some to otherArgs
  // returns the number of arguments consumed
  proc consume(const args: list(string),
               i: int,
               ref result:map(string, Argument),
               out nconsumed:int) throws {
    return 0;
  }
}

record argumentParser {
  var handlers: list(ArgumentHandler);
  var result:map(string, Argument);

  // store_true
  proc addFlag(names:string...,
               value:string='true',
               dest:string="");

  proc addValue
  // this is what argparse would do
  proc addArgument(names:string...,
                   // store store_true store_false
                   // append
                   // help
                   // store_const append_const
                   // count version extend
                   action:string = '',
                   nargs:int = -1, // 0 if arg starts with - and 1 otherwise
                   storeValue:string = '', // argparse calls it const
                   default:string = '',
                   // argparse has type
                   // argparse has choices
                   required:bool = false,
                   help:string = "",
                   metavar:string = "",
                   dest:string = "") {
  }


                 flags: borrowed Flag ...) throws {
  present
    value
}
*/

/*

  This is a probably simplistic bunch of classes and a function
  to help with argument parsing. It would be better if adding the
  arguments to the processing was the same step as creating them.

  I think we could make something inspired by Python's argparse where
  the parse method returns a map of string -> Argument objects.
  Then, you'd call addArgument to the parser
  passing in something that indicates how to produce the Argument
  (but isn't actually the Argument).

  future work --

  multi-value flags
  flags with optional values
  build the help output from the argument descriptions
*/


class ArgumentError : Error {
  var msg:string;
  proc init(msg:string) {
    this.msg = msg;
  }
  override proc message() {
    return msg;
  }
}

class Flag {
  // was the flag present?
  var present: bool;
  // start index where the flag was present
  var startIndex: int;

  proc init() {
    this.complete();
    this.present = false;
  }

  proc noteMatch(startIndex: int) {
    present = true;
    this.startIndex = startIndex;
  }

  // consumes matching arguments starting at args[i]
  // possibly by appending some to otherArgs
  // returns the number of arguments consumed
  proc consume(i: int, const args: list(string),
               ref otherArgs: list(string)):int throws {
    return 0;
  }
}

class HelpFlag : Flag {
  var names: list(string);

  proc init() {
    super.init();
    this.complete();
    this.names.append('-h');
    this.names.append('--help');
  }

  proc init(names...) {
    super.init();
    this.complete();
    for name in names {
      this.names.append(name);
    }
  }

  override proc consume(i: int, const args: list(string),
                        ref otherArgs: list(string)):int throws {
    if this.names.contains(args[i]) {
      this.noteMatch(i);
      // consume all remaining args.
      return args.size - i;
    }
    return 0;
  }
}

class BooleanFlag : Flag {
  var yesNames: list(string);
  var noNames: list(string);
  var value: bool;

  proc init(name: string) {
    super.init();
    this.complete();
    this.yesNames.append(name);
    this.value = false;
  }
  proc init(yesName: string, noName:string, default:bool) {
    super.init();
    this.complete();
    this.yesNames.append(yesName);
    this.noNames.append(noName);
    this.value = default;
  }
  proc init(yesNames, noNames, default: bool) {
    super.init();
    this.complete();
    if !isNothingType(yesNames.type) {
      for name in yesNames {
        this.yesNames.append(name);
      }
    }
    if !isNothingType(noNames.type) {
      for name in noNames {
        this.noNames.append(name);
      }
    }
    this.value = default;
  }

  override proc consume(i: int, const args: list(string),
                        ref otherArgs: list(string)):int throws {
    if this.yesNames.contains(args[i]) {
      this.noteMatch(i);
      this.value = true;
      return 1;
    }
    if this.noNames.contains(args[i]) {
      this.noteMatch(i);
      this.value = false;
      return 1;
    }
    return 0;
  }
}

class ValueFlag : Flag {
  var names: list(string);
  var valueSet: bool;
  var value: string;
  proc init(names...) {
    this.complete();
    for name in names {
      this.names.append(name);
    }
  }
  override proc consume(i: int, const args: list(string),
                        ref otherArgs: list(string)):int throws {
    for name in this.names {
      const ref arg = args[i];
      if arg == name {
        if !args.indices.contains(i+1) {
          throw new ArgumentError("missing additional argument");
        }
        if this.valueSet {
          throw new ArgumentError("cannot provide " + name + " more than once");
        }
        this.noteMatch(i);
        this.value = args[i+1];
        return 2;
      } else if arg.startsWith(name + "=") {
        if this.valueSet {
          throw new ArgumentError("cannot provide " + name + " more than once");
        }
        this.noteMatch(i);
        var res = arg.split("=", maxsplit=2);
        this.value = res[1];
        return 2;
      }
    }
    return 0;
  }
}

class OtherArgsFlag : Flag {
  var names: list(string);

  proc init() {
    super.init();
    this.complete();
    names.append('-');
    names.append('--');
  }

  proc init(names...) {
    super.init();
    this.complete();
    for name in names {
      this.names.append(name);
    }
  }

  override proc consume(i: int, const args: list(string),
                        ref otherArgs: list(string)):int throws {
    if this.names.contains(args[i]) {
      this.noteMatch(i);
      // consume all remaining args and put them all in otherArgs
      for j in (i+1)..<args.size {
        otherArgs.append(args[j]);
      }
      return args.size - i;
    }
    return 0;
  }
}

class SubcommandFlag : Flag {
  var names: list(string);
  var args: list(string);

  proc init(names...) {
    super.init();
    this.complete();
    for name in names {
      this.names.append(name);
    }
  }

  override proc consume(i: int, const args: list(string),
                        ref otherArgs: list(string)):int throws {
    if this.names.contains(args[i]) {
      this.noteMatch(i);
      // consume all remaining args and put them all in this.args
      for j in (i+1)..<args.size {
        this.args.append(args[j]);
      }
      return args.size - i;
    }
    return 0;
  }
}


// Returns true if the arguments were well formed.
// In that event, the 'present' and 'values' fields of the flags
// will be set and any other args will be stored in otherArgs.
proc processArgs(args: list(string),
                 ref otherArgs: list(string),
                 flags: borrowed Flag ...) throws {

  otherArgs.clear();
  try {
    var i = 0;
    while i < args.size {
      // Give each flag a chance to consume some args
      var consumed = false;
      for f in flags {
        var amt = f.consume(i, args, otherArgs);
        if amt > 0 {
          i += amt;
          consumed = true;
          break;
        }
      }
      if !consumed {
        // Didn't find a flag so put the value into otherArgs.
        otherArgs.append(args[i]);
        i += 1;
      }
    }
    return true;
  } catch e: ArgumentError {
    return false;
  }
  return false;
}
