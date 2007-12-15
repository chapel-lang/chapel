config const sourceText = "<a><ii>end</ii><none /></a>";
const AllIndices: domain(1) = [1..length(sourceText)];
const AllPairs: domain(2) = [1..length(sourceText), 1..length(sourceText)];
var StartIndices: sparse subdomain(AllIndices);
var EndIndices: sparse subdomain(AllIndices);



class XmlElement {
  var length: int;
  def print { printHelp(""); }
  def printHelp(indent) { }
}
class XmlPCData : XmlElement {
  var data: string;
  def printHelp(indent) { writeln(indent, "PCData(", data, ")"); }
}
class XmlTag : XmlElement {
  var name: string;
  var attNames: domain(string);
  var attValues: [attNames] string;
  var numChildren: int;
  var childrenValueSpace: domain(1) = [1..2];
  var childrenValues: [childrenValueSpace] XmlElement;
  def printHelp(indent) {
    writeln(indent, "<", name, ">");
    for child in [1..numChildren] do
       childrenValues[child].printHelp(indent + "  ");
  }
}

var parsedElements: [AllPairs] single XmlElement;

def main {
  forall z in AllIndices do {
    if sourceText.substring[z] == '<' then {
      StartIndices += z;
      if z > 1 && sourceText.substring[z-1] != ">" then EndIndices += z-1;
    }
    else if sourceText.substring[z] == '>' then {
      EndIndices += z;
      if z < length(sourceText) && sourceText.substring[z+1] != "<" then StartIndices += z+1;
    }
  }
  coforall xy in [StartIndices, EndIndices] do {
    const (x,y) = xy;
    if (x <= y) {
      processTag(x,y);
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
}

def hasIndex(start, end, indices) {
  return || reduce ([i in indices] (i >= start && i <= end));
}

def hasSpace(str) {
  for i in [1..length(str)] do
     if str.substring(i) == " " return true;
  return false;
}

def processTag(i,j) {
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
    var elt = XmlPCData(j-i+1, sourceText.substring[i..j]);
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
  var end = max reduce ([x in StartIndices] if x < j then x);
  if (end == i && sourceText.substring[j-1] == "/" && sourceText.substring[i+1] != " ") {
    /* at a self-closing tag? */
    var name : string = "";
    for end in [i+2..j-2] do
      if sourceText.substring[end] == ' ' {
        name = sourceText.substring[i+1..end-1];
        break;
      }
    var elt = XmlTag(j-i+1, name);
    parsedElements(i,j) = elt;
    writeln("Self-closed : ", elt.name);
    return;
  }
  if (sourceText.substring[end+1] != "/") then {
    /* are we at an end tag? */
    parsedElements(i,j) = nil;
    writeln("Not ending with end tag");
    return;
  }
  if (j - end < 3) {
    /* is this an empty tag? */
    parsedElements(i,j) = nil;
    writeln("Empty tag");
    return;
  }
  var tagName = sourceText.substring[end+2..j-1];
  var tagLen = length(tagName);
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
  var elt = XmlTag(j-i+1, tagName);
  start = min reduce ([x in StartIndices] if x > start then x);
  while (start < end) {
    var item : XmlElement = nil;
    for e in EndIndices do
      if e > start && e < end &&
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
        elt.childrenValueSpace = [1..curSize*2];
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


