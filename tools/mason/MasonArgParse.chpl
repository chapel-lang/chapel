/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

module MasonArgParse {
  private use List;
  private use Map;
  private use IO;
  private use Sort;

  private config var DEBUG=false;

  // TODO: Add int opts
  // TODO: Add automatic -h, --help flag generation
  // TODO: Add program metadata when setting up parser
  // TODO: Recognize '--' as separator between flags/options and positionals
  // TODO: Implement Help message and formatting
  // TODO: Move logic splitting '=' into '_match'
  // TODO: Add public github issue when available

  if chpl_warnUnstable then
    compilerWarning("ArgumentParser is unstable.");

  // A generic argument parser error
  class ArgumentError : Error {
    var _msg:string;

    proc init(msg:string) {
      this._msg = msg;
    }

    override proc message() {
      return _msg;
    }
  }

  // indicates a result of argument parsing
  class Argument {

    //indicates if an argument was entered on the command line
    var _present: bool=false;
    // hold the values of the argument from the command line
    var _values: list(string);

    proc value(){
      return this._values.first();
    }

    iter values(){
      for val in _values {
        yield val;
      }
    }

    proc hasValue(){
      return !this._values.isEmpty() && this._present;
    }

    // get the value back as a boolean, if possible
    proc valueAsBool() throws {
      var rtn:bool;

      if !this.hasValue() {
        throw new ArgumentError("No value in this argument to convert");
      } else if _convertStringToBool(this._values.first(), rtn) {
        return rtn;
      }
      else {
        throw new ArgumentError("Boolean requested but could not convert " +
                                this._values.first():string + " to bool");
      }
    }
  }


  // stores an argument definition
  class ArgumentHandler {
    // friendly name for this argument
    var _name:string;

    proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                endPos:int):int throws {
      return 0;
    }

    proc _hasDefault():bool{
      return false;
    }

    proc _getDefaultValue() {
      return new list(string);
    }

    proc _isRequired() {
      return false;
    }

    proc _validate(present:bool, valueCount:int):string {
        return "";
    }
  }

  // stores a subcommand definition
  class SubCommand : ArgumentHandler {

    proc init(cmd:string) {
      super.init();
      this._name=cmd;
    }

    // for subcommands, _match attempts to identify values at the index of the
    // subcommand at position startPos (inclusive) and through the
    // endPos (inclusive) parameter [startPos, endPos]
    override proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                         endPos:int) throws {
      var pos = startPos;
      var next = pos + 1;
      debugTrace("starting at pos: " + pos:string);
      debugTrace("Searching positions from: " + pos:string + " to "
                 + endPos:string);

      if args[pos] == this._name {
        myArg._values.extend(args[pos..]);
        debugTrace("matched cmd: " + args[pos] + " at pos: " + pos:string);
        return argsD.high + 1;
      } else {
        debugTrace("Tried to match cmd " + _name + " at position "
                    + pos:string + " and failed...this shouldn't happen");
        return pos;
      }
    }

  }

  class Positional : ArgumentHandler {
    // indicates if this argument is required to be entered by the user
    var _required:bool;
    // default value to use when argument is not present
    var _defaultValue:list(string);
    // number of acceptable values to be present for this argument
    var _numArgs:range;

    proc init(name:string, defaultValue:?t=none, numArgs=1..1) {
      super.init();
      this._name=name;
      this._required = numArgs.low > 0;
      this._defaultValue = new list(string);
      // add default value(s) if supplied
      if isStringType(t) {
        this._defaultValue.append(defaultValue);
      } else if t == list(string) || isArray(t) {
        this._defaultValue.extend(defaultValue);
      }
      debugTrace("positional arg: " + name + " has default value of "
                 + _defaultValue:string);
      this._numArgs = numArgs;
    }

    override proc _hasDefault():bool{
      return !this._defaultValue.isEmpty();
    }

    override proc _getDefaultValue() {
      return this._defaultValue;
    }

    override proc _isRequired() {
      return this._required;
    }

    override proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                         endPos:int) throws {
      // make sure we didn't receive an empty array
      if argsD.high < 0 then return 0;
      var high = _numArgs.high;
      var matched = 0;
      var pos = startPos;
      var next = pos+1;
      debugTrace("expecting between " +
                 _numArgs.low:string + " and " + _numArgs.high:string);
      debugTrace("searching from: " + pos:string + " to " + endPos:string);
      debugTrace("currently have " + myArg._values.size:string + " values");
      debugTrace("current values are: " + myArg._values:string);
      // make sure we didn't already satisfy this positional
      if myArg._present && myArg._values.size == high then return startPos;
      do {
        myArg._values.append(args[pos]);
        myArg._present = true;
        debugTrace("matched val: " + args[pos] + " at pos: " + pos:string);
        pos=next;
        next+=1;
        matched+=1;
      }
      while matched < high && pos < endPos;

      return pos;
    }

    override proc _validate(present:bool, valueCount:int):string {
      debugTrace("expected " + _numArgs:string + " got " + valueCount:string);
      debugTrace("present="+present:string + " required="+_required:string);
      if !present && _required {
        return "Required value missing";
      } else if valueCount < _numArgs.low {
        return "Not enough values: expected " + _numArgs:string +
               " got " + valueCount:string;
      } else {
        return "";
      }
    }
  }

  // stores the definition of a Flag (bool) argument
  class Flag : ArgumentHandler {
    // indicates if this flag is required to be entered by the user
    var _required:bool;
    // default value to use when flag is not present
    var _defaultValue:list(string);
    // number of option flags that can indicate this argument
    var _numFlags:int;
    // value of flag(s) that can indicate true for this argument
    var _yesFlags:list(string);
    // value of flag(s) that can indicate false for this argument
    var _noFlags:list(string);
    // number of acceptable values to be present after argument is indicated
    var _numArgs:range;

    proc init(name:string, defaultValue:?t=none, required:bool=false,
              yesFlags:[]string, noFlags:[]string, numArgs=0..0) {
      super.init();
      this._name=name;
      this._required = required;
      this._defaultValue = new list(string);
      if isBoolType(t) then this._defaultValue.append(defaultValue:string);
      this._yesFlags = new list(yesFlags);
      this._noFlags = new list(noFlags);
      this._numArgs = numArgs;
    }

    override proc _hasDefault():bool{
      return !this._defaultValue.isEmpty();
    }

    override proc _getDefaultValue() {
      return this._defaultValue;
    }

    override proc _isRequired() {
      return this._required;
    }

    override proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                         endPos:int) throws {
      var high = _numArgs.high;
      debugTrace("expecting between " +
                 _numArgs.low:string + " and " + _numArgs.high:string);
      var matched = 0;
      var pos = startPos;
      var next = pos+1;
      debugTrace("starting at pos: " + pos:string);
      debugTrace("searching from: " + pos:string + " to " + endPos:string);
      if _yesFlags.contains(args[pos]) && _numArgs.low == 0 {
        myArg._values.clear();
        myArg._values.append("true");
        debugTrace("matched val: " + args[pos] + " at pos: " + pos:string);
      } else if _noFlags.contains(args[pos]) && _numArgs.low == 0 {
        myArg._values.clear();
        myArg._values.append("false");
        debugTrace("matched val: " + args[pos] + " at pos: " + pos:string);
      }
      if high > 0 && next <= endPos {
        var flagVal:bool;
        if _convertStringToBool(args[next], flagVal) {
          myArg._values.clear();
          myArg._values.append(flagVal:string);
          pos = next;
          next += 1;
        } else if _numArgs.low > 0 {
          throw new ArgumentError("Unrecognized value " + args[next]);
        }
      }

      return next;
    }

    override proc _validate(present:bool, valueCount:int):string {
      debugTrace("expected " + _numArgs:string + " got " + valueCount:string);
      debugTrace("present="+present:string + " required="+_required:string);
      if !present && _required {
        return "Required value missing";
      } else if valueCount < _numArgs.low && present {
        return "Not enough values: expected " + _numArgs:string +
               " got " + valueCount:string;
      } else {
        return "";
      }
    }
  }

  // stores an option definition
  class Option : ArgumentHandler {
    // number of option flags that can indicate this argument
    var _numOpts:int;
    // value of option flag(s) that can indicate this argument
    var _opts:[0.._numOpts-1] string;
    // number of acceptable values to be present after argument is indicated
    var _numArgs:range;
    // whether or not the user is required to enter a value for this argument
    var _required:bool=false;
    // one or more default values to assign if opt is not entered by user
    var _defaultValue:list(string);


    proc init(name:string, numOpts:int, opts:[?argsD] string, numArgs:range,
              required=false, defaultValue=new list(string)) {
      super.init();
      _name=name;
      _numOpts=numOpts;
      _opts=opts;
      _numArgs=numArgs;
      _required=required;
      _defaultValue=defaultValue;

      // make sure that if we make an argument required no default set
      assert(!(_required && _defaultValue.size > 0),
              "Required options do not support default values");
    }

    override proc _isRequired() {
      return _required;
    }

    override proc _getDefaultValue() {
      return _defaultValue;
    }

    override proc _hasDefault():bool {
      return !_defaultValue.isEmpty();
    }
    // TODO: Decouple the argument from the argument handler
    // maybe pass a list to fill by reference and have the handler populate
    // the argument values directly instead?
    // also need a bool by ref to indicate presence of arg or not

    // for option values, _match attempts to identify values after the option
    // at position startPos (exclusive) and through the endPos (inclusive)
    // parameter (startPos, endPos]
    override proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                         endPos:int) throws {
      var high = _numArgs.high;
      debugTrace("expecting between " +
                 _numArgs.low:string + " and " + _numArgs.high:string);
      var matched = 0;
      var pos = startPos;
      var next = pos+1;
      debugTrace("starting at pos: " + pos:string);
      debugTrace("searching from: " + pos:string + " to " + endPos:string);
      while matched < high && next <= endPos && !args[next].startsWith("-")
      {
        pos=next;
        next+=1;
        matched+=1;
        myArg._values.append(args[pos]);
        debugTrace("matched val: " + args[pos] + " at pos: " + pos:string);
      }
      return next;
    }

    override proc _validate(present:bool, valueCount:int):string {
        if !present && _required {
        return "Required value missing";
      } else if valueCount > _numArgs.high {
        return "Too many values: expected " + _numArgs:string +
               " got " + valueCount:string;
      } else if valueCount < _numArgs.low && present {
        return "Not enough values: expected " + _numArgs:string +
               " got " + valueCount:string;
      } else {
        return "";
      }
    }
 }

  record argumentParser {
    // store the arguments by their familiar names
    var _result: map(string, shared Argument);
    // store the argument handlers by their familiar names
    var _handlers: map(string, owned ArgumentHandler);
    // map an option string to its familiar name
    var _options: map(string, string);
    // store positional definitions
    var _positionals: list(borrowed Positional);
    // store subcommand names
    var _subcommands: list(string);

    proc _parsePositionals(arguments:[?argsD] string, endIdx:int) throws {
      var endPos = argsD.low;
      var idx = argsD.low;
      debugTrace("Parsing Positionals...");
      debugTrace(arguments:string + " " + endIdx:string);
      for handler in _positionals {
        var arg = _result.getReference(handler._name);
        debugTrace("begin matching " + handler._name);
        endPos = handler._match(arguments, idx, arg, endIdx);
        idx = endPos;
        debugTrace("got endPos " + endPos:string);
        if idx == endIdx then break;
      }

      // check that we consumed everything we expected to
      if endPos < endIdx then{
        throw new ArgumentError("Found some unrecognizable arguments: " +
                                " ".join(arguments[endPos..<endIdx]));
      }
      return endPos;
    }

    // TODO: Find out why the in intent is breaking here
    proc parseArgs(arguments:[?argsD] string) throws {
      compilerAssert(argsD.rank==1, "parseArgs requires 1D array");
      debugTrace("Begin parsing args...");
      var k = 0;
      // identify optionIndices where opts start
      var optionIndices : map(int, string);
      var argsList = new list(arguments[argsD.low+1..]);
      var endPos = 0;

      // as noted in the comments on PR#18141, breaking up the arguments
      // when they contain = disconnects the resulting array's indices from
      // the original.
      // skip the first value in the args b/c it is expected to be the
      // name of the program or subcommand
      for i in argsD.low + 1..argsD.high {
        const arrElt = arguments[i];
        if _subcommands.contains(arrElt) then break;
        // look for = sign after opt, split into two elements
        if arrElt.startsWith("-") && arrElt.find("=") > 0 {
          var elems = new list(arrElt.split("=", 1));
          // replace this opt=val with opt val
          var idx = argsList.indexOf(arrElt);
          argsList.pop(idx);
          argsList.insert(idx, elems.toArray());
        }
      }
      // assume we should read the whole list
      var firstFlagIdx = argsList.size;
      // identify the index values where known options/flags are located
      for i in argsList.indices {
        const argElt = argsList[i];
        if _options.contains(argElt) {
          var optName = _options.getValue(argElt);
          var argRslt = _result.getValue(optName);
          debugTrace("found option " + argElt);
          // create an entry for this index and the argument name
          optionIndices.add(i, optName);
          argRslt._present = true;
          // if subcommand found, stop processing more args, save for subcmd
          if _subcommands.contains(argElt) then break;
        }
      }

      // check for when arguments passed but none defined
      if firstFlagIdx > 0 && _handlers.size == 0 {
        throw new ArgumentError("unrecognized options/values encountered: " +
                                " ".join(argsList.these()));
      }

      // get this as an array so we can sort it, because maps are orderless
      // TODO: Can we eliminate this extra logic by using an OrderedMap type?
      var arrayOptionIndices = optionIndices.toArray();
      sort(arrayOptionIndices);

      // checking for positionals at beginning of input
      if arrayOptionIndices.size > 0 then
        firstFlagIdx = arrayOptionIndices[0][0];
      if firstFlagIdx > 0 then
        endPos = _parsePositionals(argsList.toArray(), firstFlagIdx);

      // try to match for each of the identified options
      for i in arrayOptionIndices.indices {
        var idx = arrayOptionIndices[i][0];
        var name = arrayOptionIndices[i][1];
        // get a ref to the argument
        var arg = _result.getReference(name);
        debugTrace("got reference to argument " + name);
        // get the argument handler to match
        const handler = _handlers.getBorrowed(name);
        // try to match values in argstring, get the last value position
        var stopPos = argsList.size - 1;
        if arrayOptionIndices.size > i + 1 {
          stopPos = arrayOptionIndices[i+1][0] - 1;
        }
        debugTrace("set stopPos = " + stopPos:string);
        endPos = handler._match(argsList.toArray(), idx, arg, stopPos);
        debugTrace("got end position " + endPos:string);
        k+=1;
        debugTrace("k val = " + k:string);
        debugTrace("arrayOptionIndices.size is "
                 + arrayOptionIndices.size:string);
        debugTrace("argsList.size = " + argsList.size:string);
        debugTrace("argsD.high = " + argsD.high:string);
        //check if we consumed the rest of the arguments
        if endPos == argsList.size {
          // stop processing more arguments, let subcommand eat the rest
          // needed when a subcommand defines same flag as parent command
          // or else the parent command will try to match on the subcommand arg
          debugTrace("Subcommand " + handler._name +
                     " consumes rest of arguments");
          break;
        }

        // make sure we don't overrun the array,
        // then check that we don't have extra values
        if k < arrayOptionIndices.size {
          if endPos != arrayOptionIndices[k][0] {
            debugTrace("endpos != arrayOptionIndices[k][0]:"+endPos:string+"!="
                     + arrayOptionIndices[k][0]:string);
            debugTrace("arrayOptionIndices " + arrayOptionIndices:string);
            // check for undefined argument provided or possible positional args
            // intermixed in command string
            _checkTrailingPositionals(argsList.toArray()[endPos..],
                                      arrayOptionIndices[k][0],
                                      argsList.size);
          }
        }
      }

      // check for undefined argument provided or possible positional args
      // placed at end of command string
      _checkTrailingPositionals(argsList.toArray()[endPos..], argsList.size,
                                argsList.size);

      // make sure all options defined got values if needed
      _checkSatisfiedOptions();

      // assign missing values their defaults, if supplied
      _assignDefaultsToMissingOpts();

      return 0;
    }

    proc _checkTrailingPositionals(remainingArgs:[?argsD]string, stopPos:int,
                                   argLen:int) throws {
      // check if unidentified value is a positional or an error
      var curPos = argsD.low;
      if curPos < argLen {
        var oldEnd=curPos;
        curPos = _parsePositionals(remainingArgs, stopPos);
        if oldEnd == curPos then
          throw new ArgumentError("Found undefined values: " +
                                  " ".join(remainingArgs[..<stopPos]));
      }
    }

    proc _assignDefaultsToMissingOpts() {
      // set any default values as needed
      for name in this._handlers.keys() {
        const handler = this._handlers.getBorrowed(name);
        const arg = this._result.getReference(name);
        if !arg._present && handler._hasDefault() {
          debugTrace("Assigning default value to " + name);
          arg._values.extend(handler._getDefaultValue());
          arg._present = true;
        }
      }
    }

    proc _checkSatisfiedOptions() throws {
      // make sure we satisfied options that require a value
      for name in this._handlers.keys() {
        const handler = this._handlers.getBorrowed(name);
        const arg = this._result.getReference(name);
        var rtnMsg = handler._validate(arg._present, arg._values.size);
        if rtnMsg != "" {
          throw new ArgumentError(handler._name + " " + rtnMsg);
        }
      }
    }

    proc _checkAndSaveOpts(opts:[?optsD], name:string) throws {
      // validate supplied opt vals have valid prefix and don't conflict
      // then collect them
      for i in optsD {
        if !opts[i].startsWith("-") {
          throw new ArgumentError("Use '-' or '--' to indicate option/flag, " +
                                  "or use positional arguments.");
        }
        // ensure we don't redefine an existing option
        if _options.contains(opts[i]) {
          throw new ArgumentError("Option/flag " + opts[i] + " is previously " +
                                  "defined");
        }
        // collect all the option strings
        _options.add(opts[i], name);
      }
    }


    proc _addHandler(in handler : ArgumentHandler) throws {

      // ensure option names are unique
      if _handlers.contains(handler._name) {
        throw new ArgumentError("Option name " + handler._name +
                                " is previously defined");
      }

      //create, add, and return the shared argument
      var arg = new shared Argument();
      this._result.add(handler._name, arg);
      // store the handler
      debugTrace("added handler: " + handler._name);
      _handlers.add(handler._name, handler);

      return arg;
    }

    // convention would dictate this default to `--`, however since the
    // chapel runtime currently recognizes and consumes the first `--`
    // in any command string, we are using `++` as a workaround.
    // this can be overridden by the developer if they prefer something else
    proc addPassThrough(identifier="++") throws {
      return addSubCommand(identifier);
    }

    proc addFlag(name:string, opts:[?optsD],
                 required=false, defaultValue:?t=none, flagInversion=true,
                 numArgs=0) throws {
      return addFlag(name=name,
                     opts=opts,
                     required=required,
                     defaultValue=defaultValue,
                     flagInversion=flagInversion,
                     numArgs=numArgs..numArgs);
    }

    proc addFlag(name:string, opts:[?optsD],
                 required=false, defaultValue:?t=none, flagInversion=true,
                 numArgs:range) throws {

      if (flagInversion && numArgs.high > 0) {
        throw new ArgumentError("Creating 'no' flag options prevents " +
                                "using value to set flag");
      }

      if isBoolType(t) {
        if !flagInversion && defaultValue && numArgs.high < 1 && required {
          throw new ArgumentError("Setting up a required flag that defaults " +
                                  "to true with no way for user to set false");
        }
      }

      if numArgs.high > 1 {
        throw new ArgumentError("Maximum number of values for a flag is 1");
      }

      _checkAndSaveOpts(opts, name);

      var noFlagOpts:[optsD]string;

      // if user chooses to automatically create 'no' version of flag
      if flagInversion {
        for i in optsD {
          var flagStr = opts[i].strip('-',leading=true, trailing=false);
          if flagStr.size == 1 {
            noFlagOpts[i] = "-no-"+flagStr;
          } else {
            noFlagOpts[i] = "--no-"+flagStr;
          }
          _options.add(noFlagOpts[i], name);
        }
      }

      var handler = new owned Flag(name=name,
                               required=required,
                               defaultValue=defaultValue,
                               yesFlags=opts,
                               noFlags=noFlagOpts,
                               numArgs=numArgs);

      return _addHandler(handler);
    }

    proc addSubCommand(cmd:string) throws {
      var handler = new owned SubCommand(cmd);
      _subcommands.append(cmd);
      _options.add(cmd, cmd);
      return _addHandler(handler);
    }

    proc addArgument(name:string, numArgs=1, defaultValue:?t=none) throws {
      return addArgument(name, numArgs..numArgs, defaultValue);
    }

    proc addArgument(name:string,
                     numArgs:range(?),
                     defaultValue:?t=none) throws {
      var nArgs = _prepareRange(numArgs);
      var handler = new owned Positional(name, defaultValue, nArgs);

      for arg in _positionals {
        if arg._numArgs.high >= 1 && arg._numArgs.low != arg._numArgs.high {
          throw new ArgumentError("Positional arguments that allow for range " +
                                  "of values must be last relative to other " +
                                  "positional arguments");
        }
      }

      _positionals.append(handler.borrow());
      //create, add, and return the shared argument
      return _addHandler(handler);
    }

    // define a new string option with fixed number of values expected
    proc addOption(name:string,
                   opts:[?optsD]string,
                   numArgs=1,
                   required=false,
                   defaultValue:?t=none) throws {
      return addOption(name=name,
                       opts=opts,
                       numArgs=numArgs..numArgs,
                       required=required,
                       defaultValue=defaultValue);
    }

    // define a new string option with bounded range of values expected
    proc addOption(name:string,
                   opts:[?optsD]string,
                   numArgs:range(?),
                   required=false,
                   defaultValue:?t=none) throws {
      var nArgs = _prepareRange(numArgs);
      _checkAndSaveOpts(opts, name);
      var myDefault = new list(string);

      if isStringType(t) {
        myDefault.append(defaultValue);
      } else if t==list(string) ||
                (isArray(t) && isString(defaultValue[0])) {
        myDefault.extend(defaultValue);
      } else if !isNothingType(t) {
        throw new ArgumentError("Only string and list of strings are supported "
                                + "as default values at this time");
      }

      var handler = new owned Option(name=name,
                                    numOpts=opts.size,
                                    opts=opts,
                                    numArgs=nArgs,
                                    required=required,
                                    defaultValue=myDefault
                                    );
      return _addHandler(handler);
    }
  }

  // helper to prepare numArgs ranges for use
  proc _prepareRange(rIn: range(?)) :range throws {
      var nArgs:range;
      if !rIn.hasHighBound() {
        nArgs = rIn.low..max(int);
      } else if !rIn.hasLowBound() {
        throw new ArgumentError("numArgs must have low bound");
      } else {
        nArgs = rIn;
      }
      return nArgs;
  }

  // helper to convert string values to booleans
  proc _convertStringToBool(strVal:string, inout boolVal:bool) : bool {
    var strippedVal = strVal.strip(" ").toLower();

    if strippedVal == "1"
       || strippedVal == "true"
       || strippedVal == "yes"
       || strippedVal == "y" {
      boolVal = true;
      return true;
    } else if strippedVal == "0"
              || strippedVal == "false"
              || strippedVal == "no"
              || strippedVal == "n" {
      boolVal = false;
      return true;
    }
    return false;
  }

  proc debugTrace(msg:string) {
    if DEBUG then try! {stderr.writeln(msg);}
  }
}
