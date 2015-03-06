config const sourceText = "<a><ii>end</ii><none /></a>";
const AllIndices: domain(1) = {1..(sourceText.length)};
const AllPairs: domain(2) = {1..(sourceText.length), 1..(sourceText.length)};
var StartIndices: sparse subdomain(AllIndices);
var EndIndices: sparse subdomain(AllIndices);
var lock: sync int = 0;


class XmlElement {
  var length: int;
  proc print { printHelp(""); }
  proc printHelp(indent) { }
}
class XmlPCData : XmlElement {
  var data: string;
  proc printHelp(indent) { writeln(indent, "PCData(", data, ")"); }
}
class XmlTag : XmlElement {
  var name: string;
  var attNames: domain(string);
  var attValues: [attNames] string;
  var numChildren: int;
  var childrenValueSpace: domain(1) = {1..2};
  var childrenValues: [childrenValueSpace] XmlElement;
  proc printHelp(indent) {
    writeln(indent, "<", name, ">");
    for child in 1..numChildren do
       childrenValues[child].printHelp(indent + "  ");
  }
}

var parsedElements: [AllPairs] single XmlElement;

proc main {
  forall z in AllIndices with (ref StartIndices, ref EndIndices) do {
    if sourceText.substring[z] == '<' then {
      lock;
      StartIndices += z;
      if z > 1 && sourceText.substring[z-1] != ">" then EndIndices += z-1;
      lock = 0;
    }
    else if sourceText.substring[z] == '>' then {
      lock;
      EndIndices += z;
      if z < (sourceText.length) && sourceText.substring[z+1] != "<" then StartIndices += z+1;
      lock = 0;
    }
  }
  coforall x in StartIndices do {
    coforall y in EndIndices do {
      if (x <= y) {
        processTag(x,y);
      }
    }
  }
  var minindex = min reduce ([i in StartIndices] i);
  var maxindex = max reduce ([j in EndIndices] j);
  writeln("[", minindex, ",", maxindex, "] = ");
  if (parsedElements(minindex,maxindex) == nil) then
    writeln("Parse failed");
  else {
    writeln("Parse succeeded!");
    parsedElements(minindex,maxindex).print;
  }

  for i in AllPairs do
    if parsedElements(i).readXX() {
      var pe = parsedElements(i);
      delete pe;
    }
}

proc hasIndex(start, stop, indices) {
  return || reduce ([i in indices] (i >= start && i <= stop));
}

proc hasSpace(str) {
  for i in 1..(str.length) do
     if str.substring(i) == " " then return true;
  return false;
}

proc processTag(i,j) {
  if (i > j) then {
    /* invalid range */
    parsedElements(i,j) = nil;
    writeln("\t[", i, "--", j, "] = Invalid range!");
    return;
  }
  writeln("\t[", i, "--", j, "] = ", sourceText.substring[i..j]);
  if (!(hasIndex(i+1, j, StartIndices) || hasIndex(i, j-1, EndIndices)) &&
      (sourceText.substring(i) != "<" && sourceText.substring(j) != ">")) then {
    /* all text? assumes all entities are escaped*/
    var elt = new XmlPCData(j-i+1, sourceText.substring[i..j]);
    parsedElements(i,j) = elt;
    writeln("PCData : ", elt.data);
    return;
  }
  if (sourceText.substring(i) != "<" || sourceText.substring(j) != ">") {
    /* can't be PCData, and isn't a tag */
    parsedElements(i,j) = nil;
    writeln("Not a legitimate tag or PCdata");
    return;
  }
  var stop = max reduce ([x in StartIndices] if x < j then x);
  if (stop == i && sourceText.substring[j-1] == "/" && sourceText.substring[i+1] != " ") {
    /* at a self-closing tag? */
    var name : string = "";
    for stop in i+2..j-2 do
      if sourceText.substring[stop] == ' ' {
        name = sourceText.substring[i+1..stop-1];
        break;
      }
    var elt = new XmlTag(j-i+1, name);
    parsedElements(i,j) = elt;
    writeln("Self-closed : ", elt.name);
    return;
  }
  if (sourceText.substring[stop+1] != "/") then {
    /* are we at an end tag? */
    parsedElements(i,j) = nil;
    writeln("Not ending with end tag");
    return;
  }
  if (j - stop < 3) {
    /* is this an empty tag? */
    parsedElements(i,j) = nil;
    writeln("Empty tag");
    return;
  }
  var tagName = sourceText.substring[stop+2..j-1];
  var tagLen = tagName.length;
  if (hasSpace(tagName)) {
    parsedElements(i,j) = nil;
    writeln("End tag has spaces in it");
    return;
  }
  if (!(sourceText.substring[i+1..(i+tagLen)] == tagName &&
        (sourceText.substring[i+1 + tagLen] == " " ||
         sourceText.substring[i+1 + tagLen] == ">"))) {
    writeln("Start and end tags disagree : '", sourceText.substring[i+1..(i+tagLen)], "' <=> '", tagName, "'");
    parsedElements(i,j) = nil;
    return;
  }
  var start = min reduce ([x in EndIndices] if x > i then x);
  var elt = new XmlTag(j-i+1, tagName);
  start = min reduce ([x in StartIndices] if x > start then x);
  while (start < stop) {
    var item : XmlElement = nil;
    for e in EndIndices do
      if e > start && e < stop &&
        item == nil && parsedElements(start, e) != nil {
        item = parsedElements(start,e);
        break;
      }
    if item == nil {
      writeln("Couldn't find a consistent parse at index ", start);
      parsedElements(i,j) = nil;
      return;
    } else {
      var curSize = elt.childrenValues.numElements;
      if curSize == elt.numChildren then
        elt.childrenValueSpace = {1..curSize*2};
      elt.numChildren += 1;
      elt.childrenValues(elt.numChildren) = item;
      start += item.length;
    }     
  }
  parsedElements(i,j) = elt;
  writeln("[", i, "-", j, "] = ELEMENT(", tagName, ")");
  /*
    writeln("(", i, ", ", j, ") => ", sourceText.substring[i..j]);
    }*/
  return;
}


