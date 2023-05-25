use IO;

config const debug = false;

// TODO: In general, it'd be nice to make this neater w.r.t. non-nilable,
// but I found it challenging

class Node {
  var val: int;
  var left, right: owned Node?;

  proc init(val: int) {
    this.val = val;
  }

  proc init(in l: owned Node?, in r: owned Node?) {
    this.val = -1;
    this.left = l;
    this.right = r;
  }

  proc print() {
    if val != -1 {
      write(val);
    } else {
      write("[");
      left!.print();
      write(",");
      right!.print();
      write("]");
    }
  }

  proc process() {
    do {
      var changed = lookForExplodes();
      if !changed then
        changed=lookForSplits();
    } while changed;
    print();
    writeln();
  }

  proc indentedWrite(depth: int, str: string) {
    if debug {
      for i in 1..depth do
        write("  ");
      print();
      writeln(str);
    }
  }

  proc lookForExplodes(depth = 0, ref lr = (-1, -1)): bool {
    ref (l,r) = lr;
    indentedWrite(depth, "...looking");
    if val == -1 {
      if depth < 4 {
        var exploded = left!.lookForExplodes(depth+1, lr);
	indentedWrite(depth, " looking left gave: " + lr:string);
        if r != -1 {
           r = right!.lookForLeftmostUpdate(r);
           return true;
        }
        if depth == 0 && debug then writeln("exploded = ", exploded);
        if !exploded {
          exploded = right!.lookForExplodes(depth+1, lr);
   	  indentedWrite(depth, " looking right gave: " + lr:string);
          if l != -1 {
            l = left!.lookForRightmostUpdate(l);
          }
        }
        return exploded;
      } else {
        if debug then writeln("We need to explode due to ", (left!.val, right!.val));
        lr = (left!.val, right!.val);
        this.val = 0;
        left = nil;
        right = nil;
        return true;
      }
    }
    return false;
  }

  proc lookForRightmostUpdate(delta: int): int {
    if val != -1 {
      if debug then writeln("Updating ", val);
      val += delta;
      return -1;
    } else {
      return right!.lookForRightmostUpdate(delta);
    }
  }

  proc lookForLeftmostUpdate(delta: int): int {
    if val != -1 {
      if debug then writeln("Updating ", val);
      val += delta;
      return -1;
    } else {
      return left!.lookForLeftmostUpdate(delta);
    }
  }

  proc lookForSplits(): bool {
    if val == -1 {
      var split = left!.lookForSplits();
      if !split then
        split = right!.lookForSplits();
      return split;
    } else if val < 10 {
      return false;
    } else {
      const l = val/2;
      this.left = new Node(val=l);
      this.right = new Node(val=val-l);
      this.val = -1;
      return true;
    }
  }

  proc magnitude(): int {
    if val == -1 {
      return 3*left!.magnitude() + 2*right!.magnitude();
    } else {
      return val;
    }
  }
}


operator +(ref lhs: owned Node?, ref rhs: owned Node?) {
  return new Node(lhs, rhs);
}

proc readNormalizedSnailfish(): Node? throws {
  var line: string;
  if readLine(line) {
    var pos = 0;
    var tree = lineToTree(line, pos); // TODO: See following TODO
    tree!.process();
    tree!.print(); writeln();
    return tree;
  } else {
    // TODO: Wanted to return nil and just making this return nilable,
    // but led to internal errors
    // return nil;
    //
    // return nil:Node?;
    //
    // var myNil: Node?;
    // return myNil;
    throw new Error("EOF");
  }
  halt("argh");
}

var sum: Node? = readNormalizedSnailfish();
try {
  while true {
    var newFish = readNormalizedSnailfish();
    sum = new Node(sum, newFish);
    if debug { sum!.print(); writeln(" (before)"); }
    sum!.process();
    if debug { sum!.print(); writeln(" (after)"); }
  }
} catch {
}
sum!.print(); writeln();
writeln("magnitude is: ", sum!.magnitude());

// TODO: Should be able to use this default to get rid of 'pos' above 
//proc lineToTree(line: string, ref pos = 0): owned Node? {

// TODO: Also, tried to use more non-nilables here, but failed b/c
// initializer wants to take ownership of classes, so needs a non-nilable
// passed in, but also needs it to be by 'ref' since it'll be changing it
proc lineToTree(line: string, ref pos): owned Node {
  var ch = line[pos];
  if line[pos] == "[" {
    pos += 1;  // skip past "["
    var left = lineToTree(line, pos);
    pos += 1;  // skip past ","
    var right = lineToTree(line, pos);
    pos += 1;  // skip past "]"
    return new Node(left, right);
  } else if ch >= "0" && ch <= "9" {
    pos += 1;
    return new Node(ch:int);
  } else {
    halt("Unhandled case: ", ch);
  }
}
