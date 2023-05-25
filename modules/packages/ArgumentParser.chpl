/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

    You must declare your main method to take an array of string arguments in
    order to get values from the command line.
    See the :ref:`quickstart example<argumentParser-quickstart>` below or the
    technote about :ref:`getting arguments from main()<technote-mainWithArgs>`.


  .. note::

    Chapel programs can make use of :ref:`configuration constants and variables <ug-configs>`.
    The ArgumentParser module can be used in place of, or in addition to,
    configuration variables and constants. In particular, it is common for Chapel
    programs to rely solely upon configuration variables and for such programs
    the ArgumentParser is not needed.

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

  .. _argumentParser-quickstart:

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
        parser.parseArgs(args);

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

  If a user asks for help with either ``-h`` or ``--help``, the following output
  would be automatically generated and displayed:

  .. code-block:: shell

    $ quickStart -h
    USAGE: quickStart <POSITIONAL> [-h, --help] [--debug] [--optional <OPTIONAL>]

    ARGUMENTS:
            POSITIONAL

    OPTIONS:
            -h, --help      Display this message and exit
            --debug
            --optional <OPTIONAL>


  .. _argumentParser-customizingHelp:

  Customizing Help Output
  ------------------------

  There are various levels of support for help handling that you can request
  when initializing an ``argumentParser``.

  By default, an ``argumentParser`` handles help requests in the form of ``-h``
  and ``--help``, builds a help message, builds a usage message, prints the
  combined help and usage message when either invalid input or help request
  is detected, and then exits.

  These actions are mostly customizable through arguments set when
  initializing an ``argumentParser`` or individual arguments, options, and flags.

  The text that describes each argument can be defined by setting a value
  for the ``help`` argument in the various `add` methods on the ``argumentParser``.

  Example usage:

  .. code-block:: chapel

    // to set the help text on an argument, flag, etc.
    var myArg = parser.addArgument(name="myArg",
                                   help="This text describes myArg for the user");


  To take full control over what happens when a user requests help, create a new class
  that inherits the ``HelpHandler`` and override the ``printHelp()`` method. Then,
  create a new instance of your custom class and set it as the ``helpHandler``
  argument when initializing the ``argumentParser``. This is designed to ease
  adoption when a program has existing functions to print its help messages.

  Example usage:

  .. code-block:: chapel

    class MyHelpHandler : HelpHandler {
      override proc printHelp() {
        // call some custom functions like ...
        // printMyCustomHelp();
        // maybe other things?
      }
    }
    var parser = new argumentParser(helpHandler=new MyHelpHandler());

  All help requests will now execute whatever is defined in ``printHelp()``
  and then exit, as the parser's ``exitOnError`` is `true` by default.

  For existing programs where the help text is defined in a string, the
  ``argumentParser`` can accept a value in the ``helpMessage`` argument during
  initialization.

  Example usage:

  .. code-block:: chapel

    const myHelpMessage = "A customized message that should be displayed\n" +
                          "instead of the usual help  and usage message.\n"
    var parser = new argumentParser(helpMessage=myHelpMessage);


  To turn off help handling completely, set the ``addHelp`` argument to `false`
  when initializing the ``argumentParser``.


 */

@unstable("ArgumentParser is unstable.")
module ArgumentParser {
  use List;
  use Map;
  use IO;
  use Path;
  use Sort;

  private config var DEBUG=false;

  // TODO: Add int opts
  // TODO: Add program metadata when setting up parser
  // TODO: Implement Help message and formatting
  // TODO: Move logic splitting '=' into '_match'
  // TODO: Add public github issue when available

  @chpldoc.nodoc
  enum argKind { positional, subcommand, option, flag, passthrough };

  // stores an argument definition
  @chpldoc.nodoc
  class ArgumentHandler {
    // friendly name for this argument
    var _name:string;
    // enum indicating the kind of argument represented (positional, option, etc)
    var _kind:argKind;
    // record that holds help information about the argument
    var _help:argumentHelp;
    // number of acceptable values to be present for this argument
    var _numArgs=1..1;

    proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                endPos:int) : int throws {
      return 0;
    }

    proc _hasDefault() : bool{
      return false;
    }

    proc _getDefaultValue() {
      return new list(string);
    }

    proc _isRequired() {
      return false;
    }

    proc _validate(present:bool, valueCount:int) : string {
        return "";
    }
    // get a string representing how this argument should be displayed in help
    proc _getHelpCommand() : string {
      return _help.valueName;
    }
    // get a string containing help information for this argument
    proc _getHelpMessage() : string {
      return _help.help;
    }
    // get a usage string for this argument
    proc _getUsageCommand() : string {
      var usage =  this._getHelpCommand();
      if this._numArgs.high > this._numArgs.low then
        usage += " ...";
      if this._isRequired() then
        usage = "<"+ usage + ">";
      else
        usage = "[" + usage + "]";
      return usage;
    }
  }

  /*
   argumentHelp record stores the information related only to the help
   message printout for the argument
  */
  @chpldoc.nodoc
  record argumentHelp {
    // the message to display for the argument
    var help="";
    // controls visibility in the printed help output
    var visible=true;
    // offers alternative display name to use for the value of flags, options,
    // and arguments
    var valueName="";
  }

  // stores a passthrough delimiter definition
  @chpldoc.nodoc
  class PassThrough : SubCommand {

    proc init(delimiter:string, help:argumentHelp) {
      super.init(cmd=delimiter, help=help);
      this._kind=argKind.passthrough;
    }
    // for passthrough, _match attempts to identify values at the index of the
    // delimiter at position startPos, then consumes the rest of
    // the arguments through endPos (startPos..endPos] collected to pass
    override proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                         endPos:int) throws {
      var pos = startPos;
      var next = pos + 1;

      if args[pos] == this._name {
        myArg._values.pushBack(args[pos+1..endPos]);
        return endPos + 1;
      } else {
        // failed to match the delimiter at this index, which shouldn't happen
        return pos;
      }
    }

  }

  // stores a subcommand definition
  @chpldoc.nodoc
  class SubCommand : ArgumentHandler {

    proc init(cmd:string, help:argumentHelp) {
      super.init(_name=cmd, _help=help, _kind=argKind.subcommand);
      // this._name=cmd;
      // this._help=help;
      // this._kind=argKind.subcommand;
    }

    // for subcommands, _match attempts to identify values at the index of the
    // subcommand at position startPos (inclusive) and through the
    // endPos (inclusive) parameter [startPos, endPos]
    override proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                         endPos:int) throws {
      var pos = startPos;
      var next = pos + 1;
      if args[pos] == this._name {
        myArg._values.pushBack(args[pos..endPos]);
        return endPos + 1;
      } else {
        // failed to match the cmd, this should not happen
        return pos;
      }
    }

  }

  @chpldoc.nodoc
  class Positional : ArgumentHandler {
    // indicates if this argument is required to be entered by the user
    var _required:bool;
    // default value to use when argument is not present
    var _defaultValue:list(string);


    proc init(name:string, defaultValue:?t=none, numArgs=1..1, help:argumentHelp) {
      super.init(_name=name, _help=help, _kind=argKind.positional, _numArgs=numArgs);
      // this._name=name;
      this._required = numArgs.low > 0;
      this._defaultValue = new list(string);
      // add default value(s) if supplied
      if isStringType(t) {
        this._defaultValue.pushBack(defaultValue);
      } else if t == list(string) || isArray(t) {
        this._defaultValue.pushBack(defaultValue);
      }
      // this._numArgs = numArgs;
      // this._help=help;
      // this._kind=argKind.positional;
    }

    override proc _hasDefault() : bool{
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
      // make sure we didn't already satisfy this positional
      if myArg._present && myArg._values.size == high then return startPos;
      do {
        myArg._values.pushBack(args[pos]);
        myArg._present = true;
        pos=next;
        next+=1;
        matched+=1;
      }
      while matched < high && pos < endPos;

      return pos;
    }

    override proc _validate(present:bool, valueCount:int) : string {
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
  @chpldoc.nodoc
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

    proc init(name:string, defaultValue:?t=none, required:bool=false,
              yesFlags:[]string, noFlags:[]string, numArgs=0..0, help:argumentHelp) {
      super.init(_name=name, _help=help, _kind=argKind.flag, _numArgs=numArgs);
      // this._name=name;
      this._required = required;
      this._defaultValue = new list(string);
      if isBoolType(t) then this._defaultValue.pushBack(defaultValue:string);
      this._yesFlags = new list(yesFlags);
      this._noFlags = new list(noFlags);
      // this._numArgs = numArgs;
      // this._help=help;
      // this._kind=argKind.flag;
    }

    override proc _hasDefault() : bool{
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
      var matched = 0;
      var pos = startPos;
      var next = pos+1;
      if _yesFlags.contains(args[pos]) && _numArgs.low == 0 {
        myArg._values.clear();
        myArg._values.pushBack("true");
      } else if _noFlags.contains(args[pos]) && _numArgs.low == 0 {
        myArg._values.clear();
        myArg._values.pushBack("false");
      }
      if high > 0 && next <= endPos {
        var flagVal:bool;
        if _convertStringToBool(args[next], flagVal) {
          myArg._values.clear();
          myArg._values.pushBack(flagVal:string);
          pos = next;
          next += 1;
        } else if _numArgs.low > 0 {
          throw new ArgumentError("Unrecognized value " + args[next]);
        }
      }

      return next;
    }

    override proc _validate(present:bool, valueCount:int) : string {
      if !present && _required {
        return "Required value missing";
      } else if valueCount < _numArgs.low && present {
        return "Not enough values: expected " + _numArgs:string +
               " got " + valueCount:string;
      } else {
        return "";
      }
    }

    override proc _getHelpCommand() : string {
      var message:string;

      if _numArgs.high > 0 && _numArgs.low > 0 {
        message += ", ".join(_yesFlags.toArray()) + " <" + _help.valueName + ">";
      } else if _numArgs.high > 0 {
        message += ", ".join(_yesFlags.toArray()) + " [" + _help.valueName + "]";
      } else {
        if _noFlags.isEmpty() {
          message += ", ".join(_yesFlags.toArray());
        } else {
          var flagsUsage:list(string);
          for flag in _yesFlags.these() {
            var flagStr = flag.strip('-', trailing=false);
            var noStr = "-[no]-"+flagStr;
            if flagStr.size > 1 {
              noStr = "-" + noStr;
            }
            flagsUsage.pushBack(noStr);
          }
          message += ", ".join(flagsUsage.toArray());
        }
      }
      return message;
    }
  }

  // stores an option definition
  @chpldoc.nodoc
  class Option : ArgumentHandler {
    // number of option flags that can indicate this argument
    var _numOpts:int;
    // value of option flag(s) that can indicate this argument
    var _opts:[0.._numOpts-1] string;
    // whether or not the user is required to enter a value for this argument
    var _required:bool=false;
    // one or more default values to assign if opt is not entered by user
    var _defaultValue:list(string);

    proc init(name:string, numOpts:int, opts:[?argsD] string, numArgs:range,
              required=false, defaultValue=new list(string), help:argumentHelp) {
      super.init(_name=name, _help=help, _kind=argKind.option, _numArgs=numArgs);
      // _name=name;
      _numOpts=numOpts;
      _opts=opts;
      // _numArgs=numArgs;
      _required=required;
      _defaultValue=defaultValue;
      // _help=help;
      // _kind=argKind.option;
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

    override proc _hasDefault() : bool {
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
      var matched = 0;
      var pos = startPos;
      var next = pos+1;
      while matched < high && next <= endPos
      {
        pos=next;
        next+=1;
        matched+=1;
        myArg._values.pushBack(args[pos]);
      }
      return next;
    }

    override proc _validate(present:bool, valueCount:int) : string {
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

    override proc _getHelpCommand() : string {
      var message:string;

      if _numArgs.high > 0 && _numArgs.low > 0 {
        message += ", ".join(_opts) + " <" + _help.valueName + ">";
      } else if _numArgs.high > 0 {
        message += ", ".join(_opts) + " [" + _help.valueName + "]";
      } else {
        message += ", ".join(_opts);
      }
      return message;
    }
  }

  // the helpWrapper record coordinates the help text generation and is
  // the entry point for the argumentParser to handle help message requests
  @chpldoc.nodoc
  record helpWrapper {
    // a help message handler to control what happens when help is called
    var _helpHandler: shared HelpHandler;
    // the name of the binary for the USAGE message
    var _binaryName: string;
    // use the generator to build help and usage messages
    var _helpGenerator: HelpGenerator;

    proc init(helpHandler=new shared HelpHandler(), binaryName="") {
      _helpHandler=helpHandler;
      _binaryName=binaryName;
      _helpGenerator=new HelpGenerator();
    }

    // print the help message unless the user
    // has overridden the HelpHandler.printHelp method to do other things
    proc printHelp() {
      _helpHandler.printHelp();
    }

    // sets the helpHandler's empty help message to generated message from
    // the helpGenerator, or overwrites the help message if `message` is a string.
    proc setHelp(message:?t=none) {
      if isNothingType(t) && _helpHandler._helpMessage.isEmpty() {
        var msg = _helpGenerator.generateHelp(binaryName=_binaryName);
        _helpHandler._helpMessage = msg;
      } else if isString(t) {
        _helpHandler._helpMessage = message;
      } else if !isNothingType(t) {
          compilerError("Error setting help message: Expected string, received " + t:string);
      }
    }

    // Notify the generator about the arguments defined in this parser
    proc setArguments(argStack: map(string, ArgumentHandler)) {
      _helpGenerator._setArguments(argStack);
    }
  }


  @chpldoc.nodoc
  class HelpGenerator {

    // create some sections to store the help text per section
    var argsSection:section;
    var optsSection:section;
    var subSection:section;
    // we need to know what argument handlers are defined in the parser
    var _argStack: map(string, borrowed ArgumentHandler);

    proc init() {
      argsSection = new section();
      optsSection = new section();
      subSection = new section();
    }

    // represents two columns for argument, the name/flag and value, plus the
    // help text
    record element {
      var command:string;
      var help:string;

      proc render(separator="\t") : string {
        var message = command;
        if !help.isEmpty() then
          message += separator + help;
        return message;
      }
    }

    // represents a help message section, such as flags, subcommands, arguments
    record section {
      // store the rows that make up this section
      var rows:list(element);

      proc size {
        return rows.size;
      }

      proc append(item) {
        rows.pushBack(item);
      }

      proc render(inset=1, separator="\t") : string {
        var elemList:list(string);
        for elt in rows.these() {
          elemList.pushBack(separator * inset + elt.render(separator));
        }
        return "\n".join(elemList.toArray());
      }
    }

    // represent a USAGE message as a list of string values
    record usageMessage {
      var components:list(string);

      proc render(separator=" ") : string {
        return separator.join(components.toArray());
      }
    }

    // needs to be called before generating usage or help
    proc _setArguments(argStack: map(string, ArgumentHandler)) {
      _argStack = new map(string, borrowed ArgumentHandler);
      for k in argStack.keys() {
        _argStack.addOrSet(k,try! argStack[k] );
      }
      generateSections();
    }

    // generate a usage message. This needs to be done just before displaying
    // the help because we might not know the binary name until we see
    // the arguments passed to main().
    proc generateUsage(binaryName:string) : usageMessage {
      // define an order for the usage message parts
      var usageOrder = [argKind.positional, argKind.flag,
                        argKind.option, argKind.subcommand];

      var usage = new usageMessage();
      usage.components.pushBack(binaryName);
      var hasSubcommands = false;
      for kindOfArg in usageOrder {
        for name in sorted(_argStack.keys()) {
          const handler = try! _argStack[name];
          if !handler._help.visible then continue;
          var elem = handler._getUsageCommand();
          if handler._kind == kindOfArg {
            select handler._kind {
              when argKind.positional {
                usage.components.pushBack(elem);
              }
              when argKind.option {
                usage.components.pushBack(elem);
              }
              when argKind.subcommand {
                hasSubcommands = true;
              }
              when argKind.flag {
                usage.components.pushBack(elem);
              }
            }
          }
        }
      }
      if hasSubcommands then
        usage.components.pushBack("[SUBCOMMAND]");
      return usage;
    }

    // generate all the data for each section. Needs to be done AFTER all
    // the arguments have been added to the argumentParser.
    proc generateSections() {
      for name in sorted(_argStack.keys()) {
        const handler = try! _argStack[name];
        if !handler._help.visible then continue;
        var elem = new element(handler._getHelpCommand(),
                               handler._getHelpMessage());
        select handler._kind {
          when argKind.positional {
            argsSection.append(elem);
          }
          when argKind.option {
            optsSection.append(elem);
          }
          when argKind.subcommand {
            subSection.append(elem);
          }
          when argKind.flag {
            optsSection.append(elem);
          }
        }
      }
    }

    // build the help message from the components of the usage string and the
    // elements contained in each section
    proc generateHelp(separator="\t", binaryName:string) : string {
      var helpMessage:string;

      helpMessage += "USAGE: "
                         + generateUsage(binaryName).render()
                         + "\n\n";

      if argsSection.size > 0 {
        helpMessage += "ARGUMENTS:\n"
                        + argsSection.render(separator=separator)
                        + "\n\n";
      }
      if optsSection.size > 0 {
        helpMessage += "OPTIONS:\n"
                        + optsSection.render(separator=separator)
                        + "\n\n";
      }
      if subSection.size > 0 {
        helpMessage += "SUBCOMMANDS:\n"
                        + subSection.render(separator=separator)
                        + "\n\n";
      }

      return helpMessage;
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

   * Counting the number of times a flag is found on the command line is not
     supported.

   * The argument parser doesn't support checking for mutually exclusive
     arguments or arguments that are conditionally required. Instead, you can
     check after parsing the arguments that the provided arguments are acceptable.

  */
  record argumentParser {
    // store the arguments by their familiar names
    @chpldoc.nodoc
    var _result: map(string, shared Argument);
    // store the argument handlers by their familiar names
    @chpldoc.nodoc
    var _handlers: map(string, owned ArgumentHandler);
    // map an option string to its familiar name
    @chpldoc.nodoc
    var _options: map(string, string);
    // store positional definitions
    @chpldoc.nodoc
    var _positionals: list(borrowed Positional);
    // store subcommand names
    @chpldoc.nodoc
    var _subcommands: list(string);
    // recognize help flags
    @chpldoc.nodoc
    var _helpFlags: list(string);
    // exit if error in arguments
    @chpldoc.nodoc
    var _exitOnError: bool;
    // automatically add and handle help flag (-h/--help)
    @chpldoc.nodoc
    var _addHelp: bool;
    // keep track of which help flag was used so a dev can check it later
    @chpldoc.nodoc
    var _helpUsed=(false,"");
    // should the parser exit after printing the help message
    @chpldoc.nodoc
    var _exitAfterHelp: bool;
    // store a help wrapper
    @chpldoc.nodoc
    var _help : helpWrapper;


    /*
      Initialize an instance of argumentParser.

      :arg addHelp: Determines if the ArgumentParser adds help flags and handles
                their presence in the command line arguments. Flag values are
                set to ``-h`` and ``--help``. Defaults to `true`.

      :arg exitOnError: Determines if ArgumentParser exits with error info when
                    an error occurs. Defaults to `true`.

      :arg exitAfterHelp: Determines if ArgumentParser exits after it finds a help
                      flag and prints the help message. Defaults to `true`.

      :arg helpHandler: Allows a user to define a custom HelpHandler to perform
                    printing of the help message and additional operations.
                    Defaults to a new instance of HelpHandler.
                    Cannot be set in conjunction with `helpMessage`.

      :arg helpMessage: Allows a user to set a customized message to display as
                        help output. Defaults to a help message generated by
                        the argumentParser.
                        Cannot be used in conjunction with `helpHandler`.
    */
    proc init(addHelp=true, exitOnError=true, exitAfterHelp=true,
              in helpHandler:?h=none, helpMessage:?t=none) {

      if (!isNothingType(h) && !isNothingType(t)) then
        compilerError("Cannot set help message and help handler, choose one.");
      _result = new map(string, shared Argument);
      _handlers = new map(string, owned ArgumentHandler);
      _options = new map(string, string);
      _positionals = new list(borrowed Positional);
      _subcommands = new list(string);
      _exitOnError = exitOnError;
      _addHelp = addHelp;
      _exitAfterHelp = exitAfterHelp;

      var _helpHandler = new shared HelpHandler();
      if !isNothingType(h) then
        _helpHandler = helpHandler: shared h.chpl_t;

      _help = new helpWrapper(_helpHandler);
      if isStringType(t) then
        _help.setHelp(helpMessage);


      this.complete();

      try! {
        // configure to allow consuming of -- if passed from runtime
        // storing into variable to avoid memory leak due to compiler bug #18391
        var tmp = addOption(name="dummyDashHandler", opts=["--"],
                            numArgs=0,
                            visible=false,
                            help="indicates all following arguments are not to be parsed");

        // handle help flag automatically
        // storing into variable to avoid memory leak due to compiler bug #18391
        if _addHelp then var _tmp = addHelpFlag();
      }
    }

    // setup automatic help handling on -h or --help
    @chpldoc.nodoc
    proc addHelpFlag(name="ArgumentParserAddedHelp",
                     opts:[?optsD]=["-h","--help"]) throws {
      _helpFlags.pushBack(opts);
      return addFlag(name, opts, defaultValue=false, help="Display this message and exit");
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

    :arg help: a message to display for this argument when help is requested

    :arg visible: determines if this argument should be displayed in the help
                  and usage messages. Defaults to `true`.

    :arg valueName: an alternative display name for the value in the help and
                    usage messages for this argument. Defaults to the
                    uppercase representation of the `name` field.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: `ArgumentError` in any of the following conditions:

             * `name` is already defined for this parser
             * `defaultValue` is something other than a string, array or list
               of strings.

    */
    proc addArgument(name:string,
                     numArgs=1,
                     defaultValue:?t=none,
                     help="",
                     visible=true,
                     valueName:?v=none) : shared Argument throws {
      return addArgument(name, numArgs..numArgs, defaultValue, help, visible, valueName);
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

    :arg help: a message to display for this argument when help is requested

    :arg visible: determines if this argument should be displayed in the help
                  and usage messages. Defaults to `true`.

    :arg valueName: an alternative display name for the value in the help and
                    usage messages for this argument. Defaults to the
                    uppercase representation of the `name` field.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: `ArgumentError` in any of the following conditions:

             * `name` is already defined for this parser
             * `defaultValue` is something other than a string, array or list
               of strings.
             * `numArgs` is neither low-bound nor fully-bound

    */
    proc addArgument(name:string,
                     numArgs:range(?),
                     defaultValue:?t=none,
                     help="",
                     visible=true,
                     valueName:?v=none) : shared Argument throws {
      var argName = name;
      var nArgs = _prepareRange(numArgs);
      if name.startsWith("-") then argName = name.strip("-", trailing=false);
      var helpName = argName;
      if isNothingType(v) then
        helpName = argName.toUpper();
      var argHelp = new argumentHelp(help=help,visible=visible,valueName=helpName);
      var handler = new Positional(argName, defaultValue, nArgs, argHelp);

      for arg in _positionals {
        if arg._numArgs.high >= 1 && arg._numArgs.low != arg._numArgs.high {
          throw new ArgumentError("Positional arguments that allow for range " +
                                  "of values must be last relative to other " +
                                  "positional arguments");
        }
      }

      _positionals.pushBack(handler.borrow());
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

    :arg help: a message to display for this option when help is requested

    :arg visible: determines if this option should be displayed in the help
                  and usage messages. Defaults to `true`.

    :arg valueName: an alternative display name for the value in the help and
                    usage messages when an option accepts values. Defaults to the
                    uppercase representation of the `name` field.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: `ArgumentError` in any of the following conditions:

             * `name` or `opts` are already defined for this parser
             * values in `opts` do not begin with a dash ``-``
             * `defaultValue` is something other than a string, array or list
               of strings.

    */
    proc addOption(name:string,
                   opts:[]string=_processNameToOpts(name),
                   numArgs=1,
                   required=false,
                   defaultValue:?t=none,
                   help="",
                   visible=true,
                   valueName:?v=none) : shared Argument throws {
      return addOption(name=name,
                       opts=opts,
                       numArgs=numArgs..numArgs,
                       required=required,
                       defaultValue=defaultValue,
                       help=help,
                       visible=visible,
                       valueName=valueName);
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

    :arg help: a message to display for this option when help is requested

    :arg visible: determines if this option should be displayed in the help
                  and usage messages. Defaults to `true`.

    :arg valueName: an alternative display name for the value in the help and
                    usage messages when an option accepts values. Defaults to the
                    uppercase representation of the `name` field.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: `ArgumentError` in any of the following conditions:

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
                   defaultValue:?t=none,
                   help="",
                   visible=true,
                   valueName:?v=none) : shared Argument throws {
      var nArgs = _prepareRange(numArgs);
      var argName = name;
      if name.startsWith("-") then argName = name.strip("-", trailing=false);
      _checkAndSaveOpts(opts, argName);
      var myDefault = new list(string);

      if isStringType(t) {
        myDefault.pushBack(defaultValue);
      } else if t==list(string) ||
                (isArray(t) && isString(defaultValue[0])) {
        myDefault.pushBack(defaultValue);
      } else if !isNothingType(t) {
        throw new ArgumentError("Only string and list of strings are supported "
                                + "as default values at this time");
      }
      var valName=argName.toUpper();
      if !isNothingType(v) then valName=valueName;
      var argHelp = new argumentHelp(help=help,visible=visible,valueName=valName);
      var handler = new owned Option(name=argName,
                                    numOpts=opts.size,
                                    opts=opts,
                                    numArgs=nArgs,
                                    required=required,
                                    defaultValue=myDefault,
                                    help=argHelp);
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

    :arg help: a message to display for this flag when help is requested

    :arg visible: determines if this flag should be displayed in the help
                  and usage messages. Defaults to `true`.

    :arg valueName: an alternative display name for the value in the help and
                    usage messages when a flag accepts a value. Defaults to the
                    uppercase representation of the `name` field.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: `ArgumentError` in any of the following conditions:

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
                 numArgs=0,
                 help="",
                 visible=true,
                 valueName:?v=none) : shared Argument throws {
      return addFlag(name=name,
                     opts=opts,
                     required=required,
                     defaultValue=defaultValue,
                     flagInversion=flagInversion,
                     numArgs=numArgs..numArgs,
                     help=help,
                     visible=visible,
                     valueName=valueName);
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

    :arg help: a message to display for this flag when help is requested

    :arg visible: determines if this flag should be displayed in the help
                  and usage messages. Defaults to `true`.

    :arg valueName: an alternative display name for the value in the help and
                    usage messages when a flag accepts a value. Defaults to the
                    uppercase representation of the `name` field.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: `ArgumentError` in any of the following conditions:

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
                 numArgs:range,
                 help="",
                 visible=true,
                 valueName:?v=none) : shared Argument throws {

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

      var noFlagOpts=new list(string);

      // if user chooses to automatically create 'no' version of flag
      if flagInversion {
        for i in optsD {
          var flagStr = opts[i].strip('-', trailing=false);
          var noStr = "-no-"+flagStr;
          if flagStr.size > 1 {
            noStr = "-" + noStr;
          }
          noFlagOpts.pushBack(noStr);
          _options.add(noStr, argName);
        }
      }

      var valName = argName.toUpper();
      if !isNothingType(v) then valName = valueName;
      var helpArg = new argumentHelp(help=help,
                                     visible=visible,
                                     valueName=valName);
      var handler = new owned Flag(name=argName,
                               required=required,
                               defaultValue=defaultValue,
                               yesFlags=opts,
                               noFlags=noFlagOpts.toArray(),
                               numArgs=numArgs,
                               help=helpArg);

      return _addHandler(handler);
    }

    /*
    Add a subcommand to the parser. A subcommand is typically indicated with an
    action word, like `commit` in `git commit`. The subcommand name and all values
    after the subcommand are collected in the returned shared `Argument` object.

    For example, a defining a subcommand like this:

    .. code-block:: chapel

      var subCmdRun = parser.addSubCommand("run");

    we would expect command line arguments like this:

    .. code-block:: shell

      $ programName <any args for programName> run <any args for run>

    :arg cmd: a keyword that is recognized as indicating the subcommand from the
              command line. For example, `commit` in `git commit`.

    :arg help: a message to display for this subcommand when help is requested

    :arg visible: determines if this subcommand should be displayed in the help
                  and usage messages. Defaults to `true`.

    :returns: a shared `Argument` where parsed values will be placed

    :throws: `ArgumentError` if `cmd` is already defined for this parser

    */
    proc addSubCommand(cmd:string, help="",
                       visible=true) : shared Argument throws {
      var argHelp = new argumentHelp(help, visible, cmd);
      var handler = new owned SubCommand(cmd, argHelp);
      _subcommands.pushBack(cmd);
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

    :arg delimiter: the pattern to use as the passthrough indicator

    :returns: a shared `Argument` where collected values will be placed for use
              by the developer

    :throws: `ArgumentError` if `delimiter` is already defined for this parser

    */
    proc addPassThrough(delimiter="--") : shared Argument throws {
      // remove the dummyHandler first
      if delimiter == "--" then _removeHandler("dummyDashHandler", ["--"]);
      var argHelp = new argumentHelp(visible=false,
                                     help="pass all following arguments without parsing",
                                     valueName=delimiter);
      var handler = new owned PassThrough(delimiter, argHelp);
      _subcommands.pushBack(delimiter);
      _options.add(delimiter, delimiter);
      return _addHandler(handler);
    }

    @chpldoc.nodoc
    proc _parsePositionals(arguments:[?argsD] string, endIdx:int) throws {
      var endPos = argsD.low;
      var idx = argsD.low;
      for handler in _positionals {
        var arg = _result[handler._name];
        endPos = handler._match(arguments, idx, arg, endIdx);
        idx = endPos;
        if idx == endIdx then break;
      }

      // check that we consumed everything we expected to
      if endPos < endIdx then {
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
        // after parser and arguments defined
        parser.parseArgs(args);
      }

    :arg arguments: The array of values passed from the command line to `main(args:[]string)`

    :throws: `ArgumentError` if argumentParser is initialized with
             `exitOnError=false`, and invalid or undefined command line
             arguments are found in `arguments`.
    */
    proc parseArgs(arguments:[?argsD] string) throws {
      // normal operation is to catch parsing error, write help message,
      // and exit. User may choose to handle errors themselves though.
      if _addHelp {
        if _help._binaryName.isEmpty() then
          _help._binaryName = basename(arguments[argsD.low]);
        _help.setArguments(this._handlers);
        _help.setHelp();
      }

      if _exitOnError {
        try {
          _tryParseArgs(arguments);
        } catch ex: Error {
          stderr.writeln(ex.message());
          if _addHelp {
            _help.printHelp();
            exit(1);
          }
        }
      } else {
        _tryParseArgs(arguments);
      }
    }

    @chpldoc.nodoc
    proc _tryParseArgs(arguments:[?argsD] string) throws {
      // TODO: Find out why the in intent is breaking here
      compilerAssert(argsD.rank==1, "parseArgs requires 1D array");
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
          var idx = argsList.find(arrElt);
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
          var optName = _options[argElt];
          var argRslt = _result[optName];
          // create an entry for this index and the argument name
          optionIndices.add(i, optName);
          argRslt._present = true;
          // if subcommand found, stop processing more args, save for subcmd
          if _subcommands.contains(argElt) then break;
          // check for the presence of a help flag and handle it or quit parsing
          if _helpFlags.contains(argElt) {
            _helpUsed = (true, argElt);
            if _addHelp {
              _help.printHelp();
              if _exitAfterHelp then exit(0);
            }
            return 0;
          }
        }
      }

      // check for when arguments passed but none defined
      if firstFlagIdx > 0 && _handlers.size == 0 {
        throw new ArgumentError("unrecognized options/values encountered: " +
                                " ".join(argsList.these()));
      }

      // get this as an array so we can sort it, because maps are orderless
      // TODO: Can we eliminate this extra logic by using an SortedMap type?
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
        var arg = _result[name];
        // get the argument handler to match
        const handler = _handlers[name].borrow();
        // try to match values in argstring, get the last value position
        var stopPos = argsList.size - 1;
        if arrayOptionIndices.size > i + 1 {
          stopPos = arrayOptionIndices[i+1][0] - 1;
        }
        // try to consume the values in the handler
        endPos = handler._match(argsList.toArray(), idx, arg, stopPos);
        k+=1;
        //check if we consumed the rest of the arguments
        if endPos == argsList.size {
          // stop processing more arguments, let subcommand eat the rest
          // needed when a subcommand defines same flag as parent command
          // or else the parent command will try to match on the subcommand arg
          break;
        }

        // make sure we don't overrun the array,
        // then check that we don't have extra values
        if k < arrayOptionIndices.size {
          if endPos != arrayOptionIndices[k][0] {
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

    /*
    Check if the parser identified a help flag from the command line

    :returns: a tuple with the first component being a boolean indicating if a
              help flag was found and the second element being the flag found.
              Second element will be the empty string if no help flag was found.
    */
    proc helpFlagPresent() : (bool, string) {
      return _helpUsed;
    }

    @chpldoc.nodoc
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

    @chpldoc.nodoc
    proc _assignDefaultsToMissingOpts() {
      // set any default values as needed
      for name in this._handlers.keys() {
        const handler = try! this._handlers[name].borrow();
        const arg = try! this._result[name];
        if !arg._present && handler._hasDefault() {
          arg._values.pushBack(handler._getDefaultValue());
          arg._present = true;
        }
      }
    }

    @chpldoc.nodoc
    proc _checkSatisfiedOptions() throws {
      // make sure we satisfied options that require a value
      for name in this._handlers.keys() {
        const handler = this._handlers[name].borrow();
        const arg = this._result[name];
        var rtnMsg = handler._validate(arg._present, arg._values.size);
        if rtnMsg != "" {
          throw new ArgumentError(handler._name + " " + rtnMsg);
        }
      }
    }

    @chpldoc.nodoc
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

    @chpldoc.nodoc
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
      _handlers.add(handler._name, handler);

      return arg;
    }

    // remove a handler for an option or flag
    @chpldoc.nodoc
    proc _removeHandler(name:string, opts:[?optsD]string) {
      if _result.remove(name) {
        _handlers.remove(name);
        for opt in opts do _options.remove(opt);
      }
    }
  }

  // A generic argument parser error
  @chpldoc.nodoc
  class ArgumentError : Error {
    proc init(msg:string) {
      super.init(msg);
    }
  }


  /*
  Stores the result of argument parsing.
  */
  class Argument {

    //indicates if an argument was entered on the command line
    @chpldoc.nodoc
    var _present: bool=false;
    // hold the values of the argument from the command line
    @chpldoc.nodoc
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


  /*
  The HelpHandler class is meant to be inheritable so a user can implement their
  own behavior when the help flag is recognized by the parser. Typically, there
  is no need to make a custom version of HelpHandler, as a default is supplied.

  */
  class HelpHandler {
    // the help message to display on ./progName --help
    @chpldoc.nodoc
    var _helpMessage:string;

    /*
    Prints the help message by default.
    Inherit this class and override this method to customize the operations.
    */
    proc printHelp() {
      writeln(_helpMessage);
    }
  }


  // helper to prepare numArgs ranges for use
  @chpldoc.nodoc
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
  @chpldoc.nodoc
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
  @chpldoc.nodoc
  proc _processNameToOpts(name:string) : []string {
    var opts:list(string);
    if name.startsWith("-") {
      opts.pushBack(name);
    } else {
      opts.pushBack("--" + name);
    }
    return opts.toArray();
  }

  @chpldoc.nodoc
  proc debugTrace(msg:string) {
    if DEBUG then try! {stderr.writeln(msg);}
  }
}
