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

/*
  A module to be used for defining and parsing command line arguments.

  .. note::

    This module is in the initial stages of development and should be expected
    to change in future releases.


  Terminology
  -----------

  Descriptions of each argument type are given below.

  **Arguments**

    These are also known as positional values and do not have any identifiers
    aside from the relative order and count of values entered.

    Example:

    .. code-block:: shell

      $ myExecutable filename1 filename2 filename3


  **Options**

    These are values that follow some indicator, typically ``--option`` is
    used as a long option and ``-o`` might be the short indicator. Typically,
    0 or more arguments may follow.

    Examples:

    .. code-block:: shell

      $ myExecutable --foo value1
      $ myExecutable -f value1

  **Flags**

    These are boolean indicators whose presence alone can be used to
    derive a true/false value.

    Example:

    .. code-block:: shell

      $ myExecutable --debug
      $ myExecutable -d

  **Subcommands**

    These are values that can be used to trigger execution of other methods
    which take their own arguments. A subcommand typically consumes the remaining
    values from the command line.

    Example:

    .. code-block:: shell

      $ myExecutable build --force otherProgram

  **PassThrough**

    This is a delimiter value which designates that all the values which follow
    should be collected for later use by the developer. Typically this value is
    a double dash ``--``.

    Example:

    .. code-block:: shell

      $ myExecutable build --force otherProgram -- --flags --for --compiling otherProgram

  Quickstart Example
  -------------------

  .. code-block:: chapel

    module M {
      use ArgumentParser;
      proc main(args:[]string) throws {
        var parser = new argumentParser();
        var simpleArg = parser.addArgument(name="positional");
        var optionArg = parser.addOption(name="optional");
        var flagArg = parser.addFlag(name="debug", defaultValue=false);
        try!{
          parser.parseArgs(args);
        } catch ex : ArgumentError {
          writeln(ex.message());
          // print a usage message
          exit(1);
        }
        var debug = flagArg.valueAsBool();
        // we are assured a value here or else the parser would have thrown an error
        var foo = simpleArg.value();
        var bar:string;
        if optionArg.hasValue() then bar = optionArg.value();
      }
    }

  This program would accept any of the following command line arguments (and other combinations):

  .. code-block:: shell

    $ quickStart myFile
    $ quickStart myFile --optional valueA
    $ quickStart myFile --debug
    $ quickStart myFile --optional valueA --debug
    $ quickStart --optional val1 --debug myFile

  And will not accept any of the following (reason given below):

  .. code-block:: shell

    $ quickStart --debug
    positional argument is missing

    $ quickStart myFile --debug=true
    debug flag does not accept values

    $ quickStart myFile --debug --optional
    optional expects 1 value

 */

module ArgumentParser {
  use List;
  use Map;
  use IO;
  use Sort;

  private config var DEBUG=false;

  // TODO: Add int opts
  // TODO: Add automatic -h, --help flag generation
  // TODO: Add program metadata when setting up parser
  // TODO: Implement Help message and formatting
  // TODO: Move logic splitting '=' into '_match'
  // TODO: Add public github issue when available

  if chpl_warnUnstable then
    compilerWarning("ArgumentParser is unstable.");

  // stores an argument definition
  pragma "no doc"
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

  // stores a passthrough delimiter definition
  pragma "no doc"
  class PassThrough : SubCommand {

    proc init(delimiter:string) {
      super.init(delimiter);
    }
    // for passthrough, _match attempts to identify values at the index of the
    // delimiter at position startPos, then consumes the rest of
    // the arguments through endPos (startPos..endPos] collected to pass
    override proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                         endPos:int) throws {
      var pos = startPos;
      var next = pos + 1;
      debugTrace("starting at pos: " + pos:string);
      debugTrace("Searching positions from: " + pos:string + " to "
                 + endPos:string);

      if args[pos] == this._name {
        myArg._values.extend(args[pos+1..endPos]);
        debugTrace("matched delim.: " + args[pos] + " at pos: " + pos:string);
        return endPos + 1;
      } else {
        debugTrace("Tried to match delimiter " + _name + " at position "
                    + pos:string + " and failed...this shouldn't happen");
        return pos;
      }
    }

  }

  // stores a subcommand definition
  pragma "no doc"
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
        myArg._values.extend(args[pos..endPos]);
        debugTrace("matched cmd: " + args[pos] + " at pos: " + pos:string);
        return endPos + 1;
      } else {
        debugTrace("Tried to match cmd " + _name + " at position "
                    + pos:string + " and failed...this shouldn't happen");
        return pos;
      }
    }

  }

  pragma "no doc"
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
  pragma "no doc"
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
  pragma "no doc"
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

  /*
  A parser that performs the following functions:

    1. Stores definitions of valid arguments/flags/options
    2. Parses an array of arguments as passed from the command line
       (where args[0] is typically the name of the executable)
    3. Throws an ArgumentError if invalid or unrecognized arguments are encountered

  The parser can process command line arguments that include or omit an equals
  sign ``=``, with short ``-`` or long ``--`` indicators.

  We can declare a new argumentParser like so:

  .. code-block:: chapel

    var parser = new argumentParser();

  **Limitations**

   * Cannot place short flags together. That is, if a parser has defined short
     flags ``-c``, ``-a`` and ``-t``, we cannot handle ``-cat``.

   * Short options must be separated from their values with an equals sign or
     spaces. That is, ``-cValue`` is not supported and should be entered as either
     ``-c Value`` or ``-c=Value``.

   * Numeric types are not supported and will be passed as strings.

   * Chapel programs currently do config variable processing on the arguments.
     If you have arguments that conflict with config variables, you can place
     them after ``--`` on the command line in order to skip the config variable
     processing. If a passthrough is also specified as ``--``, then these
     arguments will also be captured by the passthrough. If that is not desired,
     you can create a different passthrough delimiter, e.g. ``++``.

   * Help and usage are not automatically generated or handled internally. This
     is planned for future development.

   * Counting the number of times a flag is found on the command line is not
     supported.

   * The argument parser doesn't support checking for mutually exclusive
     arguments or arguments that are conditionally required. Instead, you can
     check after parsing the arguments that the provided arguments are acceptable.

  */
  record argumentParser {
    // store the arguments by their familiar names
    pragma "no doc"
    var _result: map(string, shared Argument);
    // store the argument handlers by their familiar names
    pragma "no doc"
    var _handlers: map(string, owned ArgumentHandler);
    // map an option string to its familiar name
    pragma "no doc"
    var _options: map(string, string);
    // store positional definitions
    pragma "no doc"
    var _positionals: list(borrowed Positional);
    // store subcommand names
    pragma "no doc"
    var _subcommands: list(string);

    pragma "no doc"
    proc init() {
      _result = new map(string, shared Argument);
      _handlers = new map(string, owned ArgumentHandler);
      _options = new map(string, string);
      _positionals = new list(borrowed Positional);
      _subcommands = new list(string);
      this.complete();
      try! {
        // configure to allow consuming of -- if passed from runtime

        // storing into variable to avoid memory leak due to compiler bug #18391
        var tmp = addOption(name="dummyDashHandler", opts=["--"], numArgs=0);
      }
    }

    /*
    Add an argument to the parser. Arguments are also commonly known as
    "positional arguments" and are not indicated by anything other than a value.

    For example, defining an argument like this:

    .. code-block:: chapel

        var fileArg = parser.addArgument(name="filename");

    we would expect command line arguments like this:

    .. code-block:: shell

        $ programName myFile.ext

    :arg name: a friendly name to give this argument. This would typically be
               displayed in the `help` or `usage` message.

    :arg defaultValue: a value to assign the argument when it is not provided
                       on the command line. If `numArgs` is greater than 1,
                       an array or list may be passed. Defaults to `none`.

    :arg numArgs: an exact number of values expected from the command line.
                  An ArgumentError will be thrown if more or fewer values are entered.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: ArgumentError in any of the following conditions:

             * `name` is already defined for this parser
             * `defaultValue` is something other than a string, array or list
               of strings.

    */
    proc addArgument(name:string,
                     numArgs=1,
                     defaultValue:?t=none) : shared Argument throws {
      return addArgument(name, numArgs..numArgs, defaultValue);
    }

    /*
    An overload of ``addArgument``, described above, that accepts a range for
    `numArgs`.

    For example, defining an argument like this:

    .. code-block:: chapel

        var filesArg = parser.addArgument(name="filenames", numArgs=1..);

    we would expect command line arguments like either of these:

    .. code-block:: shell

        $ programName myFile1.ext myFile2.ext myFile3.ext ...
        $ programName myFile1.ext

    :arg name: a friendly name to give this argument. This would typically be
               displayed in the `help` or `usage` message.

    :arg defaultValue: a value to assign the argument when it is not provided
                       on the command line. If `numArgs` is greater than 1,
                       an array or list may be passed. Defaults to `none`.

    :arg numArgs: a range of values expected from the command line. If using a
                  range, the argument must be the only positional that accepts a range,
                  and must be specified last relative to other positional arguments
                  to avoid ambiguity.
                  An ArgumentError will be thrown if more or fewer values are entered.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: ArgumentError in any of the following conditions:

             * `name` is already defined for this parser
             * `defaultValue` is something other than a string, array or list
               of strings.
             * `numArgs` is neither low-bound nor fully-bound

    */
    proc addArgument(name:string,
                     numArgs:range(?),
                     defaultValue:?t=none) : shared Argument throws {
      var argName = name;
      var nArgs = _prepareRange(numArgs);
      if name.startsWith("-") then argName = name.strip("-", trailing=false);
      var handler = new owned Positional(argName, defaultValue, nArgs);

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

    /*
    Add an option to the parser. Options are typically indicated by one or two
    leading dashes ``-`` and may be followed by 0 or more values.

    For example, defining an option like this:

    .. code-block:: chapel

      var fileArg = parser.addOption(name="filename");

    we would expect command line arguments like either of these:

    .. code-block:: shell

      $ programName --filename=myFile1.ext
      $ programName --filename myFile1.ext



    :arg name: a friendly name to give this option, or a pattern to use to indicate
                this option from the command line. If providing a pattern, use a
                leading single dash ``-`` or double dash ``--``.

    :arg opts: an array of string values that can be used to indicate this option.
                Defaults to a long version of the `name` field with leading double
                dashes ``--``.

    :arg required: a bool to set this option as a required command line argument.
                    If set to `true`, an ArgumentError will be thrown if the user
                    fails to enter the option on the command line. Defaults to `false`.

    :arg defaultValue: a value to assign the argument when an option is not required,
                        and a is not entered on the command line. If `numArgs` is
                        greater than 1, an array or list may be passed. Defaults to `none`.

    :arg numArgs: the exact number of values expected to follow the option on the
                  command line. An ArgumentError will be thrown if more or fewer
                  values are entered.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: ArgumentError in any of the following conditions:

             * `name` or `opts` are already defined for this parser
             * values in `opts` do not begin with a dash ``-``
             * `defaultValue` is something other than a string, array or list
               of strings.

    */
    proc addOption(name:string,
                   opts:[]string=_processNameToOpts(name),
                   numArgs=1,
                   required=false,
                   defaultValue:?t=none) : shared Argument throws {
      return addOption(name=name,
                       opts=opts,
                       numArgs=numArgs..numArgs,
                       required=required,
                       defaultValue=defaultValue);
    }

    /*
    An overload of ``addOption`` described above that accepts a range of expected
    values for `numArgs`.

    For example, defining an option like this:

    .. code-block:: chapel

      var filesArg = parser.addOption(name="filenames", numArgs=1..);

    we would expect command line arguments like any of these:

    .. code-block:: shell

      $ programName --filenames myFile1.ext myFile2.ext myFile3.ext ...
      $ programName --filenames=myFile1.ext
      $ programName --filenames myFile1.ext --filenames myFile2.ext ...

    :arg name: a friendly name to give this option, or a pattern to use to indicate
                this option from the command line. If providing a pattern, use a
                leading single dash ``-`` or double dash ``--``.

    :arg opts: an array of string values that can be used to indicate this option.
                Defaults to a long version of the `name` field with leading double
                dashes ``--``.

    :arg required: a bool to set this option as a required command line argument.
                    If set to `true`, an ArgumentError will be thrown if the user
                    fails to enter the option on the command line. Defaults to `false`.

    :arg defaultValue: a value to assign the argument when an option is not required,
                        and a is not entered on the command line. If `numArgs` is
                        greater than 1, an array or list may be passed. Defaults to `none`.

    :arg numArgs: a range of values expected to follow the option on the
                  command line. This may be a fully-bound range like ``1..10``
                  or a lower-bound range like ``1..``. An ArgumentError will be
                  thrown if more or fewer values are entered.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: ArgumentError in any of the following conditions:

             * `name` or `opts` are already defined for this parser
             * values in `opts` do not begin with a dash ``-``
             * `defaultValue` is something other than a string, array or list
               of strings.
             * `numArgs` does not have a low-bound

    */
    proc addOption(name:string,
                   opts:[]string=_processNameToOpts(name),
                   numArgs:range(?),
                   required=false,
                   defaultValue:?t=none) : shared Argument throws {
      var nArgs = _prepareRange(numArgs);
      var argName = name;
      if name.startsWith("-") then argName = name.strip("-", trailing=false);
      _checkAndSaveOpts(opts, argName);
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

      var handler = new owned Option(name=argName,
                                    numOpts=opts.size,
                                    opts=opts,
                                    numArgs=nArgs,
                                    required=required,
                                    defaultValue=myDefault
                                    );
      return _addHandler(handler);
    }

    /*
    Add a flag to the parser. Flags may be setup in several ways depending on
    the user's needs.

    For example, defining a flag like this:

    .. code-block:: chapel

      var debugArg = parser.addFlag(name="debug");

    we would expect command line arguments like this:

    .. code-block:: shell

      $ programName --debug

    :arg name: a friendly name to give this flag, or a pattern to use to indicate
                this flag from the command line. If providing a pattern, use a
                leading single dash ``-`` or double dash ``--``.

    :arg opts: an array of string values that can be used to indicate this flag.
                Defaults to a long version of the `name` field with leading double
                dashes ``--``.

    :arg required: a bool to set this flag as a required command line argument.
                    If set to `true`, an ArgumentError will be thrown if the user
                    fails to enter the flag on the command line. Defaults to `false`.

    :arg defaultValue: a value to assign the argument when a flag is not required,
                        and a is not entered on the command line. Defaults to `none`.

    :arg flagInversion: a bool to indicate if the parser should automatically
                          create a ``--no-`` version of this flag to mean `false`
                          when entered from the command line. i.e. ``--[no-]build``

    :arg numArgs: the number of values expected to follow the flag on the command
                    line. Defaults to 0 and has a max value of 1.
                    when 0, then ``--flag`` is accepted while ``--flag=true`` is rejected
                    when 1, then ``--flag`` is rejected while ``--flag=true`` is accepted

    :returns: a shared `Argument` where parsed values will be placed

    :throws: ArgumentError in any of the following conditions:

              * `name` or `opts` are already defined for this parser
              * `numArgs` > 1
              * `flagInversion` is true and `numArgs` > 0
              * `flagInversion` is false and `defaultValue` is true and flag
                is required and `numArgs` < 1 (in this case the flag is meaningless
                because it will always be true)
              * values in `opts` do not begin with a dash ``-``

    */
    proc addFlag(name:string,
                 opts:[?optsD]=_processNameToOpts(name),
                 required=false, defaultValue:?t=none, flagInversion=false,
                 numArgs=0) : shared Argument throws {
      return addFlag(name=name,
                     opts=opts,
                     required=required,
                     defaultValue=defaultValue,
                     flagInversion=flagInversion,
                     numArgs=numArgs..numArgs);
    }

    /*
    An overload of ``addFlag`` described above, which accepts a range for `numArgs`.
    If a range of ``0..1`` is used, then either ``--flag true`` or ``--flag`` is
    acceptable from the command line.

    For example, defining a flag like this:

    .. code-block:: chapel

      var debugArg = parser.addFlag(name="debug",
                                    numArgs=0..1,
                                    defaultValue=false);

    we would expect command line arguments like either of these:

    .. code-block:: shell

      $ programName --debug
      $ programName --debug=true

    :arg name: a friendly name to give this flag, or a pattern to use to indicate
                this flag from the command line. If providing a pattern, use a
                leading single dash ``-`` or double dash ``--``.

    :arg opts: an array of string values that can be used to indicate this flag.
                Defaults to a long version of the `name` field with leading double
                dashes ``--``.

    :arg required: a bool to set this flag as a required command line argument.
                    If set to `true`, an ArgumentError will be thrown if the user
                    fails to enter the flag on the command line. Defaults to `false`.

    :arg defaultValue: a value to assign the argument when a flag is not required,
                        and a is not entered on the command line. Defaults to `none`.

    :arg flagInversion: a bool to indicate if the parser should automatically
                         create a `--no-` version of this flag to mean `false`
                         when entered from the command line. i.e. (--[no-]build)

    :arg numArgs: the range of values expected to follow the flag on the command
                   line.
                   when 0..1, then ``--flag`` or ``--flag=true`` are accepted

    :returns: a shared `Argument` where parsed values will be placed

    :throws: ArgumentError in any of the following conditions:

             * `name` or `opts` are already defined for this parser
             * `numArgs` high-bound > 1
             * `flagInversion` is true and `numArgs` > 0
             * `flagInversion` is false and `defaultValue` is true and flag
               is required and `numArgs` < 1 (in this case the flag is meaningless
               because it will always be true)
             * `numArgs` does not have a lower-bound or lower-bound < 0
             * values in `opts` do not begin with a dash ``-``

    */
    proc addFlag(name:string,
                 opts:[?optsD]=_processNameToOpts(name),
                 required=false, defaultValue:?t=none, flagInversion=false,
                 numArgs:range) : shared Argument throws {

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
      var argName = name;
      if name.startsWith("-") then argName = name.strip("-", trailing=false);
      _checkAndSaveOpts(opts, argName);

      var noFlagOpts:[optsD]string;

      // if user chooses to automatically create 'no' version of flag
      if flagInversion {
        for i in optsD {
          var flagStr = opts[i].strip('-', trailing=false);
          if flagStr.size == 1 {
            noFlagOpts[i] = "-no-"+flagStr;
          } else {
            noFlagOpts[i] = "--no-"+flagStr;
          }
          _options.add(noFlagOpts[i], argName);
        }
      }

      var handler = new owned Flag(name=argName,
                               required=required,
                               defaultValue=defaultValue,
                               yesFlags=opts,
                               noFlags=noFlagOpts,
                               numArgs=numArgs);

      return _addHandler(handler);
    }

    /*
    Add a subcommand to the parser. A subcommand is typically indicated with an
    action word, like `commit` in `git commit`. The subcommand name and all values
    after the subcommand are collected in the returned shared `Argument` object.

    For example, a defining a subcommand like this:

    .. code-block:: chapel

      var subCmdRun = parser.addSubcommand("run");

    we would expect command line arguments like this:

    .. code-block:: shell

      $ programName <any args for programName> run <any args for run>

    :arg cmd: a keyword that is recognized as indicating the subcommand from the
              command line. For example, `commit` in `git commit`.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: ArgumentError if `cmd` is already defined for this parser

    */
    proc addSubCommand(cmd:string) : shared Argument throws {
      var handler = new owned SubCommand(cmd);
      _subcommands.append(cmd);
      _options.add(cmd, cmd);
      return _addHandler(handler);
    }

    /*
    Add a delimiter to indicate all following values should not be parsed, but
    collected for later use by the developer. Convention dictates this is a
    double-dash ``--``, but the developer may choose to use something else.

    For example, defining a passthrough delimiter like this:

    .. code-block:: chapel

      var passedArgs = parser.addPassThrough();

    we would expect command line arguments like this:

    .. code-block:: shell

      $ programName <any args for programName> -- --preserved-flags --options etc

    :args delimiter: the pattern to use as the passthrough indicator

    :returns: a shared `Argument` where collected values will be placed for use
              by the developer

    :throws: ArgumentError if `delimiter` is already defined for this parser

    */
    proc addPassThrough(delimiter="--") : shared Argument throws {
      // remove the dummyHandler first
      if delimiter == "--" then _removeHandler("dummyDashHandler", ["--"]);
      var handler = new owned PassThrough(delimiter);
      _subcommands.append(delimiter);
      _options.add(delimiter, delimiter);
      return _addHandler(handler);
    }

    pragma "no doc"
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

    /*
    Parses an array of arguments as collected from the command line

    Example usage:

    .. code-block:: chapel

      proc main(args:[]string) throws {
        // parser and arguments defined
        try!{
          parser.parseArgs(args);
        } catch ex : ArgumentError {
          // display usage message
          writeln(ex.message());
        }
      }

    :arg arguments: The array of values passed from the command line to `main(args:[]string)`

    :throws: ArgumentError if invalid or undefined command line arguments found in `arguments`
    */
    proc parseArgs(arguments:[?argsD] string) throws {
      // TODO: Find out why the in intent is breaking here
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
      for i in arrayOptionIndices.domain {
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

    pragma "no doc"
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

    pragma "no doc"
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

    pragma "no doc"
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

    pragma "no doc"
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

    pragma "no doc"
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

    // remove a handler for an option or flag
    pragma "no doc"
    proc _removeHandler(name:string, opts:[?optsD]string) {
      if _result.remove(name) {
        _handlers.remove(name);
        for opt in opts do _options.remove(opt);
      }
    }
  }

  // A generic argument parser error
  pragma "no doc"
  class ArgumentError : Error {
    var _msg:string;

    proc init(msg:string) {
      this._msg = msg;
    }

    override proc message() {
      return _msg;
    }
  }

  /*
  Stores the result of argument parsing.
  */
  class Argument {

    //indicates if an argument was entered on the command line
    pragma "no doc"
    var _present: bool=false;
    // hold the values of the argument from the command line
    pragma "no doc"
    var _values: list(string);

    /*
    Return the single value collected from the command line, if any.
    If no value was collected, the program will halt as result of
    calling ``list.first()`` on an empty list.

    .. warning::
      This can only be called safely if you are sure a value was collected,
      either because the argument was required or there was a default value set.
      Calling `value()` when no argument was collected will cause the program
      to halt.
      To be safe, check the return of `hasValue()` before calling `value()`.

    Example usage:

    .. code-block:: chapel

      var argVal:string;
      if myArg.hasValue() then argVal = myArg.value();

    :returns: The string value for this argument as parsed from the command line

    */
    proc value() : string {
      return this._values.first();
    }

    /*
    Return an iterator over the values collected from the command line, if any.
    Typically collect these into a new list or var them into an array.

    Example usage:

    .. code-block:: chapel

      // to get an array of strings
      var argValues = myArg.values();
      // create a list of strings
      var argList = new list(myArg.values());

    :returns: An iterator over the parsed values

    */
    iter values() : string {
      for val in _values {
        yield val;
      }
    }

    /*
    Check to see if a value was collected from the command line

    :returns:
              * `true` -- when at least one value was parsed
              * `false` -- when the argument wasn't provided and no
                default was defined
    */
    proc hasValue() : bool {
      return !this._values.isEmpty() && this._present;
    }

    /*
    Interpret the value collected from the command line as a bool

    :returns:
              * `true` -- for true, 1, yes, y
              * `false` -- for false, 0, no, n

    :throws: `ArgumentError` if value cannot be interpreted as a bool
    */
    proc valueAsBool() : bool throws {
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

  // helper to prepare numArgs ranges for use
  pragma "no doc"
  proc _prepareRange(rIn: range(?)) : range throws {
    var nArgs:range;
    if rIn.hasLowBound() && !rIn.hasHighBound() {
      nArgs = rIn.low..max(int);
    } else if !rIn.hasLowBound() {
      throw new ArgumentError("numArgs must have low bound");
    } else {
      nArgs = rIn;
    }
    return nArgs;
  }

  // helper to convert string values to booleans
  pragma "no doc"
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

  // helper to provide a default value for opts, based on the name
  pragma "no doc"
  proc _processNameToOpts(name:string) : []string {
    var opts:list(string);
    if name.startsWith("-") {
      opts.append(name);
    } else {
      opts.append("--" + name);
    }
    return opts.toArray();
  }

  pragma "no doc"
  proc debugTrace(msg:string) {
    if DEBUG then try! {stderr.writeln(msg);}
  }
}
