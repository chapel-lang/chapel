module MasonArgParse {
  private use List;
  private use Map;
  private use IO;
  private use Sort;

  const DEBUG=false;

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
      
      if !this.numArgs.hasHighBound() {
        high = 10000000000;
      } else {
        high = this.numArgs.high;
      }
      writeErr("expecting between " + numArgs.low:string + " and " + high:string);
      var matched = 0;
      var pos = startPos;
      writeErr("starting at pos: " + pos:string);
      while matched < high && pos+1 <= argsD.high && !args[pos+1].startsWith("-"){
        pos += 1;
        matched +=1;
        argument.values.append(args[pos]);
        writeErr("matched val: " + args[pos] + " at pos: " + pos:string);     
      }
      if matched < this.numArgs.low {
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
      writeErr("start parsing args");
      var pos = argsD.low;
      // TODO: Implement parsing
      // identify indices where opts start
      var indices : map(int, string);
      for i in argsD {
        if options.contains(arguments[i]) {
          writeErr("found option " + arguments[i]);
          // create an entry for this index and the argument name
          indices.add(i, options.getValue(arguments[i]));
          writeErr("added option " + arguments[i]);
        } 
      }

      // try to match for each of the identified options
      var knownIndex = indices.keysToArray();
      sort(knownIndex);
      var k = 0;
      var arrayIndices = indices.toArray();
      sort(arrayIndices);
      for (idx, name) in arrayIndices {
        // get a ref to the argument
        var arg = result.getReference(name);
        writeErr("got reference to argument " + name);
        // get the action to match
        var act = actions.getBorrowed(name);

        var endPos = act.match(arguments, idx, arg);
        writeErr("got end position " + endPos:string);
        k+=1;
        writeErr("k val = " + k:string);
        writeErr("knownIndex.size is " + knownIndex.size:string);
        if k < knownIndex.size {
          if endPos + 1 != knownIndex[k] {
            writeErr("endpos != knownIndex[k] :" + endPos:string + " " + knownIndex[k]:string);
            writeErr("knownIndex " + knownIndex:string);
            throw new ArgumentError("\\".join(act.opts) + " has extra values");
          }
        }else if endPos < argsD.high {
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
      // add all the option strings
      for opt in opts do options.add(opt, name);
      // store the action
      actions.add(name, action);
      //create, add, and return the shared argument
      var arg = new shared Argument();
      this.result.add(name, arg);
      return arg;
      }
  }
  
  proc writeErr(msg:string) {
    if DEBUG then try! {stderr.writeln(msg);}
  }

}