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

  const DEBUG=false;
  // TODO: Add bool flags
  // TODO: Add positional arguments
  // TODO: Add pass-thru options following "-" or "--"
  // TODO: Add int opts
  
  // TODO: Implement Help message and formatting
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
  }

  // stores an argument definition
  class Action {
    // friendly name for this argument
    var _name:string;

    proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                ref rest:list(string)):int throws {
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

  // stores a subcommand name
  class SubCommand : Action {
    
    proc init(cmd:string) {
      super.init();
      this._name=cmd;
      this.complete();
      debugTrace("made new subcommand: " + this._name);
    }

    override proc _match(args:[?argsD]string, startPos:int, myArg:Argument,
                         ref rest:list(string))
                        throws {      
      var pos = startPos;
      var next = pos + 1;
      debugTrace("starting at pos: " + pos:string);
      while pos <= argsD.high 
      {      
        if args[pos] == this._name {
          myArg._values.append(args[pos]);
          //myArg._present=true;
          debugTrace("matched val: " + args[pos] + " at pos: " + pos:string);
          rest.extend(args[next..]);
          return next;
        }
        pos = next;
        next+=1;  
      }
      return pos;                  
    }

  }

  // stores the definition of an option
  class Option : Action {    
    // number of option flags that can indicate this argument
    var _numOpts:int;
    // value of option flag(s) that can indicate this argument
    var _opts:[0.._numOpts-1] string;
    // number of acceptable values to be present after argument is indicated
    var _numArgs:range;
    // whether or not the user is required to enter a value for this action
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
      this.complete();
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
    // TODO: Decouple the argument from the action
    // maybe pass a list to fill by reference and have the argparser populate
    // the argument instead?
    // also need a bool by ref to indicate presence of arg or not
    override proc _match(args:[?argsD]string, startPos:int, myArg:Argument, 
                         ref rest:list(string))
                        throws {
      var high = _numArgs.high;
      debugTrace("expecting between " + 
                 _numArgs.low:string + " and " + _numArgs.low:string);
      var matched = 0;
      var pos = startPos;
      var next = pos+1;
      debugTrace("starting at pos: " + pos:string);
      
      while matched < high && next <= argsD.high && !args[next].startsWith("-") 
      {
        pos=next;
        next+=1;
        matched+=1;
        //if high > myArg._values.size {
        myArg._values.append(args[pos]);
        //} else {
        //   throw new ArgumentError("\\".join(_opts) + " too many values");
        // }
        //myArg._present=true;
        debugTrace("matched val: " + args[pos] + " at pos: " + pos:string);     
      }
      // if matched < this._numArgs.low {
      //   throw new ArgumentError("\\".join(_opts) + " not enough values");
      // }
      return next;
    }

    override proc _validate(present:bool, valueCount:int):string {
        if !present && _required {
        return "Required value missing";
      } else if valueCount > _numArgs.high {
        return "Too many values";
      } else if valueCount < _numArgs.low && present {
        return "Not enough values";
      } else {
        return "";
      }
    }
 }

  record argumentParser {
    // store the arguments by their familiar names
    var _result: map(string, shared Argument);
    // store the actions by their familiar names
    var _actions: map(string, owned Action);
    // map an option string to its familiar name
    var _options: map(string, string);

    proc parseArgs(arguments:[?argsD] string) throws {
      compilerAssert(argsD.rank==1, "parseArgs requires 1D array");
      debugTrace("start parsing args");   
      var k = 0;
      // identify optionIndices where opts start
      var optionIndices : map(int, string);
      var argsList = new list(arguments);
      var rest = new list(string);
      var endPos = 0;

      for i in argsD {
        const arrElt = arguments[i];
        // look for = sign after opt, split into two elements
        if arrElt.startsWith("-") && arrElt.find("=") > 0 {
          var elems = new list(arrElt.split("=", 1));
          // replace this opt=val with opt val
          var idx = argsList.indexOf(arrElt);
          argsList.pop(idx);
          argsList.insert(idx, elems.toArray());
        }
      }
      
      for i in argsList.indices {
        const argElt = argsList[i];
        if _options.contains(argElt) {
          debugTrace("found option " + argElt);
          // create an entry for this index and the argument name
          optionIndices.add(i, _options.getValue(argElt));
          _result.getValue(_options.getValue(argElt))._present = true;          
        }
      }
      
      // get this as an array so we can sort it, because maps are order-less
      // TODO: Can we eliminate this extra logic by using an OrderedMap type?
      var arrayoptionIndices = optionIndices.toArray();
      sort(arrayoptionIndices);

      // check for undefined argument provided
      if arrayoptionIndices.size > 0 && arrayoptionIndices[0][0] != 0 {
          throw new ArgumentError("Found undefined values: " + argsList[0]);                                  
      }

      // try to match for each of the identified options
      for (idx, name) in arrayoptionIndices {
        // get a ref to the argument
        var arg = _result.getReference(name);
        debugTrace("got reference to argument " + name);
        // get the action to match
        const act = _actions.getBorrowed(name);
        // try to match values in argstring, get the last value position
        endPos = act._match(argsList.toArray(), idx, arg, rest);
        debugTrace("got end position " + endPos:string);
        k+=1;
        debugTrace("k val = " + k:string);
        debugTrace("arrayoptionIndices.size is " 
                 + arrayoptionIndices.size:string);
        debugTrace("argsList.size = " + argsList.size:string);
        debugTrace("argsD.high = " + argsD.high:string);
        // make sure we don't overrun the array,
        // then check that we don't have extra values
        if k < arrayoptionIndices.size {
          if endPos != arrayoptionIndices[k][0] {
            debugTrace("endpos != arrayoptionIndices[k][0] :"+endPos:string+" "
                     + arrayoptionIndices[k][0]:string);
            debugTrace("arrayoptionIndices " + arrayoptionIndices:string);
            throw new ArgumentError("\\".join(act._name) + " has extra values");
          }       
        } 
        // else if endPos < argsList.size && endPos + rest.size < argsList.size {
        //   throw new ArgumentError("\\".join(act._name) + " has extra values");
        // }
      }
      // make sure all options defined got values if needed
      _checkSatisfiedOptions();

      // assign and missing values their defaults, if supplied
      _assignDefaultsToMissingOpts();

      // check for when arguments passed but none defined
      if argsList.size > 0 && this._actions.size == 0 {
        throw new ArgumentError("unrecognized options/values encountered: " +
                                " ".join(argsList.these()));
      }

      // check for undefined argument provided
      if endPos < argsList.size && endPos + rest.size < argsList.size {
          throw new ArgumentError("Found undefined values: " + 
                                  " ".join(argsList.toArray()[endPos..]));
      }

      return rest;
    }



    proc _assignDefaultsToMissingOpts() {
      // set any default values as needed
      for name in this._actions.keys() {
        const act = this._actions.getBorrowed(name);
        const arg = this._result.getReference(name);
        if !arg._present && act._hasDefault() {
          arg._values.extend(act._getDefaultValue());
          arg._present = true;
        }
      }
    }

    proc _checkSatisfiedOptions() throws {
      // make sure we satisfied options that require a value
      for name in this._actions.keys() {
        const act = this._actions.getBorrowed(name);
        const arg = this._result.getReference(name);
        var rtnMsg = act._validate(arg._present, arg._values.size);
        if rtnMsg != "" {
          throw new ArgumentError(act._name + " " + rtnMsg);
        }        
      }
    }

    proc _addAction(in action : Action) throws { 

      // ensure option names are unique
      if _actions.contains(action._name) {
        throw new ArgumentError("Option name " + action._name + 
                                " is previously defined");
      }      
      
      //create, add, and return the shared argument
      var arg = new shared Argument();
      this._result.add(action._name, arg);
      // store the action
      debugTrace("added action: " + action._name);
      _actions.add(action._name, action);
      
      return arg;
    }

    proc addSubCommand(cmd:string) throws {       
      var act = new owned SubCommand(cmd);
      _options.add(cmd, cmd);
      return _addAction(act);
    }
    
    // define a new string option with fixed number of values expected
    proc addOption(name:string,
                   opts:[?optsD]string,
                   numArgs:int,
                   required=false,
                   defaultValue:?t=none) throws {
      return addOption(name=name,
                       opts=opts,
                       numArgs=numArgs..numArgs,
                       required=required,
                       defaultValue=defaultValue);
    }

    // define a new string option with a low bounded range of values expected
    proc addOption(name:string,
                   opts:[?optsD]string,
                   numArgs:range(boundedType=BoundedRangeType.boundedLow),
                   required=false,
                   defaultValue:?t=none) throws {                   
      return addOption(name=name,
                       opts=opts,
                       numArgs=numArgs.low..max(int),
                       required=required,
                       defaultValue=defaultValue);
    }

    // define a new string option with bounded range of values expected
    proc addOption(name:string,
                   opts:[?optsD]string,
                   numArgs:range,
                   required=false,
                   defaultValue:?t=none) throws {      
      for i in optsD {
        if !opts[i].startsWith("-") {
          throw new ArgumentError("Use '-' or '--' to indicate opt flags. " +
                                  "Positional arguments not yet supported");
        }

              
        // ensure we don't redefine an existing option flag
        if _options.contains(opts[i]) {
          throw new ArgumentError("Option flag " + opts[i] + " is previously " +
                                  "defined");
        }
      }
      
      var myDefault = new list(string);

      if isStringType(t) {
        myDefault.append(defaultValue);
      } else if t==list(string) {
        myDefault.extend(defaultValue);
      } else if !isNothingType(t) {
        throw new ArgumentError("Only string and list of strings are supported "
                                + "as default values at this time");
      }

      var action = new owned Option(name=name, 
                                    numOpts=opts.size,
                                    opts=opts,
                                    numArgs=numArgs,
                                    required=required,
                                    defaultValue=myDefault
                                    );
      // collect all the option strings
      for i in optsD do _options.add(opts[i], name);
      return _addAction(action);
    }
  }
  
  proc debugTrace(msg:string) {
    if DEBUG then try! {stderr.writeln(msg);}
  }
}
