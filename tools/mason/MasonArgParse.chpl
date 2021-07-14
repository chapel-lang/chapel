module MasonArgParse {
    private use List;
    private use Map;
    private use IO;

  // TODO: Add public github issue when available
  if chpl_warnUnstable then
    compilerWarning("ArgumentParser is unstable.");

// A generic argument parser error
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
    var present: bool=false;
    var values: list(string);     
    
    proc getValue(){     
      return this.values.first();
    }
    iter getValues(){
      for val in values {
        yield val;
      }      
    }    
    proc hasValue(){
      return !this.values.isEmpty();
    }
  }

  // stores the definition of an option
  class Action {
   var name:string;
   var numOpts:int;
   var opts:[0..numOpts-1] string;
   var numArgs:range;

   proc match(args:[?argsD]string, startPos:int, ref argument:Argument) throws {
       var high = 0;
       
       if !this.numArgs.hasHighBound() then {
           high = 10000000000;
       } else {
           high = this.numArgs.high;
       }
       stderr.writeln("expecting between " + numArgs.low:string + " and " + high:string);
       var matched = 0;
       var pos = startPos;
       stderr.writeln("starting at pos: " + pos:string);
       while matched < high && pos <= argsD.high {
           pos += 1;
           if pos <= argsD.high && !args[pos].startsWith("-") then {
            matched +=1;
            argument.values.append(args[pos]);
           }    
       }
       if matched < this.numArgs.low then {
           throw new ArgumentError("\\".join(opts) + " not enough values");
       }
       return pos;
   }
 }

record argumentParser {
  var result: map(string, shared Argument);
  var actions: map(string, owned Action);
  var options: map(string, string);
  var unknownArgs: list(string);


  proc parseArgs(arguments:[?argsD]string) throws {
    compilerAssert(argsD.rank==1, "parseArgs requires 1D array");
    stderr.writeln("start parsing args");
    var pos = argsD.low;
    // TODO: Implement parsing
    // identify indices where opts start
    var indices : map(int, string);
    for i in argsD {
        if options.contains(arguments[i]) {
            stderr.writeln("found option " + arguments[i]);
            // create an entry for this index and the argument name
            indices.add(i, options.getValue(arguments[i]));
            //stderr.writeln("added option " + arguments[i]);
        } 
    }

    // try to match for each of the identified options
    var knownIndex = indices.keysToArray();
    var k = 0;
    for (idx, name) in indices.items() {
        // get a ref to the argument
        var arg = result.getReference(name);
        stderr.writeln("got reference to argument " + name);
        // get the action to match
        var act = actions.getBorrowed(name);

        var endPos = act.match(arguments, idx, arg);
        stderr.writeln("got end position " + endPos:string);
        k+=1;
        if k < knownIndex.size {
            if endPos != knownIndex[k] then {
                throw new ArgumentError("\\".join(act.opts) + " has extra values");
            }
        } else if endPos < argsD.high then {
            throw new ArgumentError("\\".join(act.opts) + " has extra values");
        }
    }

  }

  proc addOption(name:string,
                 opts:[]string,
                 numArgs:int) throws {
                  return addOption(name=name,
                                   opts=opts,
                                   numArgs=numArgs..numArgs);
                 }

  proc addOption(name:string,
                 opts:[]string,
                 numArgs:range) throws {
                   var action = new owned Action(name=name, 
                                          numOpts=opts.size,
                                          opts=opts,
                                          numArgs=numArgs);
                  for opt in opts do options.add(opt, name);
                  actions.add(name, action);
                  var arg = new shared Argument();
                  this.result.add(name, arg);
                  return arg;
                 }
  }


}