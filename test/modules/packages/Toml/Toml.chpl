/*
Chapel's Library for `Tom's Obvious, Minimal Language (TOML)
              <https://github.com/toml-lang/toml>`_.
This module provides support for parsing and writing toml files.
*/
module TOML {


use TomlParser;
use TomlReader;


/*
Receives a channel to a TOML file as a parameter and outputs an associative
array.
*/
 proc parseToml(input) : Toml {
   const source = new Source(input);
   const parser = new Parser(source);
   const TomlFile =  parser.parseLoop();
   delete parser;
   delete source;
   return TomlFile;
 }
 
/*
Receives a string of TOML format as a parameter and outputs an associative
array.
 proc parseToml(input: string) : Toml {
   const source = new Source(input);
   const parser = new Parser(source);
   const TomlFile =  parser.parseLoop();
   delete parser;
   delete source;
   return TomlFile;
 }
*/



/* 
Parser module with the Toml class for the Chapel TOML library.
*/
 module TomlParser {

   use Regexp;
   use DateTime;

   // Prints a line by line output of parsing process
   config const debugTomlParser: bool = false;


   class Parser {

     var source;
     var D: domain(string);
     var table: [D] Toml;
     var rootTable = new Toml(table);
     var curTable: string;

     const doubleQuotes = '".*?"',
       singleQuotes = "'.*?'",
       digit = "\\d+",
       keys = "^\\w+";
     const Str = compile(doubleQuotes + '|' + singleQuotes),
       kv = compile('|'.join(doubleQuotes, singleQuotes, digit, keys)),
       dt = compile('^\\d{4}-\\d{2}-\\d{2}[ T]\\d{2}:\\d{2}:\\d{2}$'),
       realNum = compile("\\+\\d*\\.\\d+|\\-\\d*\\.\\d+|\\d*\\.\\d+"),
       ints = compile("(\\d+|\\+\\d+|\\-\\d+)"),
       inBrackets = compile("(\\[.*?\\])"),
       brackets = compile('\\[|\\]'),
       whitespace = compile("\\s"),
       comment = compile("(\\#)"),
       comma = compile("(\\,)");



     proc parseLoop() : Toml {

       while(readLine(source)) {
         var token = top(source);

         if token == '#' {
           parseComment();
         }
         else if inBrackets.match(token) {
           parseTable();
         }
         else if brackets.match(token) {
           parseSubTbl();
         }
         else if kv.match(token) {
           parseAssign();
         }
         else {
           halt("Unexpected token ->", getToken(source));
         }
         if debugTomlParser {
           debugPrint();
         }
       }
       return rootTable;
     }

     proc parseTable() {
       var toke = getToken(source);
       var tablename = brackets.sub('', toke);
       var tblD: domain(string);
       var tbl: [tblD] Toml;
       if !rootTable.pathExists(tablename) {
         rootTable[tablename] = new Toml(tbl);
       }
       curTable = tablename;
     }

     proc parseSubTbl() {
       skipNext(source);
       var tblname = getToken(source);
       skipNext(source);
       var tblD: domain(string);
       var tbl: [tblD] Toml;
       var (tblPath, tblLeaf) = splitTblPath(tblname);
       if !rootTable.pathExists(tblPath) then makePath(tblPath);
       rootTable.getIdx(tblPath)[tblLeaf] = new Toml(tbl);
       curTable = tblname;
     }

     /* Creates a path to a sub-table when no parent has been initialized */
     proc makePath(tblPath: string) {
       var path = tblPath.split('.');
       var firstIn = path.domain.first;
       var first = true;
       var i: int = 0;
       for parent in path {
         if first { 
           var tblD: domain(string);
           var tbl: [tblD] Toml;
           rootTable[parent] = new Toml(tbl);
           first = false;
         }
         else {
           var tblD: domain(string);
           var tbl: [tblD] Toml;
           var grandParent = '.'.join(path[..firstIn+i]);
           rootTable.getIdx(grandParent)[parent] = new Toml(tbl);
           i+=1;
         }
       }
     }

     proc parseInlineTbl(key: string) {
       var tblname: string;
       var tblD: domain(string);
       var tbl: [tblD] Toml;
       if curTable.isEmptyString() {
         tblname = key;
         rootTable[key] = new Toml(tbl);
       }
       else {
         tblname = '.'.join(curTable, key);
         var (tblPath, tblLeaf) = splitTblPath(tblname);
         rootTable.getIdx(tblPath)[tblLeaf] = new Toml(tbl);
       }
       var temp = curTable;
       curTable = tblname;
       while top(source) != '}' {
         parseAssign();
         if top(source) == ',' {
           skipNext(source);
         }
       }
       skipNext(source);
       curTable = temp; // resets curTable after assignments to inline
     }

     proc parseAssign() {
       var key = getToken(source);
       var equals = getToken(source);
       if top(source) == '{' {
         skipNext(source);
         parseInlineTbl(key);
       }
       else {
         var value = parseValue();
         if curTable.isEmptyString() then rootTable[key] = value;
         else rootTable.getIdx(curTable)[key] = value;
       }
     }
     
     proc parseComment() {
       skipLine(source);
     }

     /* Returns leaf of embedded table */
     proc splitTblPath(s: string) {
       var A = s.split('.');
       var fIdx = A.domain.first;
       var leaf = A[A.domain.last];
       var path = '.'.join(A[..A.domain.last-1]);
       if A.size == 1 then path = A[fIdx];
       return (path, leaf);
     }


     /* Creates and returns a Toml parsed from tokens into respective type */
     proc parseValue(): Toml {
       var val = top(source);
       // Array
       if val == '['  {
         skipNext(source);
         var nodeDom: domain(1);
         var array: [nodeDom] Toml;
         while top(source) != ']' {
           if comma.match(top(source)) {
             skipNext(source);
           }
           else if comment.match(top(source)) {
             skipLine(source);
           }
           else {
             var toParse = parseValue();
             array.push_back(toParse);
           }
         }
         skipNext(source);
         var nodeArray = new Toml(array);
         return nodeArray;
       }
       // Strings (includes multi-line) 
       else if Str.match(val) {
         var toStr: string;
         if val.startsWith('"""') {
           toStr += getToken(source).strip('"""', true, false);
           while toStr.endsWith('"""') == false {
             toStr += " " + getToken(source);
           }
           var mlString = new Toml(toStr.strip('"""'));
           return mlString;
         }
         else if val.startsWith("'''") {
           toStr += getToken(source).strip("'''", true, false);
           while toStr.endsWith("'''") == false {
             toStr += " " + getToken(source);
           }
           var mlString = new Toml(toStr.strip("'''"));
           return mlString;
         }
         else {
           toStr = getToken(source).strip('"').strip("'");
           var tomlStr = new Toml(toStr);
           return tomlStr;
         }
       }
       // DateTime
       else if dt.match(val) {
         var date = datetime.strptime(getToken(source), "%Y-%m-%dT%H:%M:%SZ");
         var Datetime = new Toml(date);
         return Datetime;
       }
       // Real
       else if realNum.match(val) {
         var token = getToken(source);
         var toReal = token: real;
         var realToml = new Toml(toReal);
         return realToml;
       }
       // Int
       else if ints.match(val) {
         var token = getToken(source);
         var toInt = token: int;
         var intToml = new Toml(toInt);
         return intToml;
       } 
       // Boolean
       else if val == "true" || val ==  "false" {
         var token = getToken(source);
         var toBool = token: bool;
         var boolToml = new Toml(toBool);
         return boolToml;
       }
       // Comments within arrays
       else if val == '#' {
         skipLine(source);
         return parseValue();
       }
       // Error
       else {
         halt("Unexpected Token: ", "'", val, "'");
       }
     }

     proc debugPrint() {
       writeln();
       writeln("========================= Debug Info  ==========================");
       source.debug();
       writeln();
       writeln(rootTable);
       writeln();
       writeln("================================================================");
     }
   }




enum fieldtag {fieldBool,
               fieldInt,
               fieldArr,
               fieldToml,
               fieldReal,
               fieldString,
               fieldEmpty,
               fieldDate};
 use fieldtag;

 /*
 Class to hold various types parsed from input
 Used to recursivly hold tables and respective values
 */
   class Toml {
     var i: int;
     var boo: bool;
     var re: real;
     var s: string;
     var dt: datetime;
     var dom: domain(1);
     var arr: [dom] Toml;
     var D: domain(string);
     var A: [D] Toml;
     var tag: fieldtag; 

     // Empty
     proc init() {
       this.tag = fieldEmpty;
     }

     // String
     proc init(s:string) {
       this.s = s;
       this.tag = fieldString;
     }

     // Toml
     proc init(A: [?D] Toml) where isAssociativeDom(D) {
       this.D = D;
       this.A = A;
       this.tag = fieldToml;
     }

     // Datetime
     proc init(dt: datetime) {
       this.dt = dt;
       this.tag = fieldDate;
     }

     // Int
     proc init(i: int) {
       this.i = i;
       this.tag = fieldInt;
     }

     // Boolean
     proc init(boo: bool) {
       this.boo = boo;
       this.tag = fieldBool;
     }

     // Real
     proc init(re: real) {
       this.re = re;
       this.tag = fieldReal;
     }

     // Array
     proc init(arr: [?dom] Toml) where isAssociativeDom(dom) == false  {
       this.dom = dom;
       this.arr = arr;
       this.tag = fieldArr;
     }

     proc this(idx: string) ref {
       return A[idx];
     }

     /* Returns the index of the table path given as a parameter */
     proc getIdx(tbl: string) ref : Toml {
       var indx = tbl.split('.');
       var top = indx.domain.first;
       if indx.size < 2 {
         if this.A.domain.member(tbl) == false {
           halt("Error in getIdx '", tbl, "' does not exist");
         }
         else {
           return this.A[tbl];
         }
       } 
       else {
         var next = '.'.join(indx[top+1..]);
         if this.A.domain.member(indx[top]) {
           return this.A[indx[top]].getIdx(next);
         }
         else {
           halt("Error in getIdx2");
         }
       }
     }

     /* Returns true if table path exists in rootTable */
     proc pathExists(tblpath: string) : bool {
       var path = tblpath.split('.');
       var top = path.domain.first;
       if path.size < 2 {
         if this.A.domain.member(tblpath) == false {
           return false;
         }
         else {
           return true;
         }
       }
       else {
         var next = '.'.join(path[top+1..]);
         if this.A.domain.member(path[top]) {
           return this.A[path[top]].pathExists(next);
         }
         else {
           return false;
         }
       }
     }

     /* Write a Table */
     proc writeThis(f) {
       var flatDom: domain(string);
       var flat: [flatDom] Toml;
       this.flatten(flat);       // Flattens containing Toml
       printValues(f, this);     // Prints key values in containing Toml
       printHelp(flat, f);       // Prints tables in containg Toml
     }

     
     /* Flatten tables into flat associative array for writing */
     proc flatten(flat: [?d] Toml, rootKey = '') : flat.type { 
       for (k, v) in zip(this.D, this.A) {
         if v.tag == fieldToml {
           var fullKey = k;
           if rootKey != '' then fullKey = '.'.join(rootKey, k);
           flat[fullKey] = v;
           v.flatten(flat, fullKey);
         }
       }
       return flat;
     }


     proc printHelp(flat: [?d] Toml, f:channel) {
       for k in d.sorted() {
         f.writeln('[', k, ']');
         printValues(f, flat[k]);
       }
     }


     /* Send values from table to toString for writing  */
     proc printValues(f: channel, v: Toml) {
       for (key, value) in zip(v.D, v.A) {
         select value.tag {
           when fieldToml do continue; // Table
           when fieldBool {
             f.writeln(key, ' = ', toString(value));
           }
           when fieldInt {
             f.writeln(key, ' = ', toString(value));
           }
           when fieldArr {
             var final: string;
             f.write(key, ' = ');
             final += '[';
             for k in value.arr {
               if value.arr.domain.size == 1 || k == value.arr[value.arr.domain.last] {
                 final += toString(k);
               }
               else {
                 final += toString(k) + ', ';
               }
             }
             final += ']';
             f.writeln(final);
           }
           when fieldReal {
             f.writeln(key, ' = ', toString(value));
           }
           when fieldString {
             f.writeln(key, ' = ', toString(value));
           }
           when fieldEmpty {
             halt("Keys have to have a value");
           }
           when fieldDate {
             f.writeln(key, ' = ', toString(value));
           }
           otherwise { 
             f.write("not yet supported");
           }
           }
       }
       f.writeln();
     }


     /* Return String representation of a value in a node */
     proc toString(val: Toml) : string { 
       select val.tag {
         when fieldBool do return val.boo;
         when fieldInt do return val.i;
         when fieldArr {
           var final: string;
           final += '[';
           for k in val.arr {
             if val.arr.domain.size == 1 || k == val.arr[val.arr.domain.last] {
               final += toString(k);
             }
             else {
               final += toString(k) + ', ';
             }
           }
           final += ']';
           return final;
         }
         when fieldReal do return val.re;
         when fieldString do return ('"' + val.s + '"');
         when fieldEmpty do return ""; // empty
         when fieldDate do return val.dt.isoformat();
         otherwise {
           halt("Error in printing '", val, "'");
         }
         }
     }


     /* 
      For the user to write values of a node as follows:
      Toml[key].toString()
     */
      proc toString() : string { 
        return toString(this);
     }


     proc deinit() {
       for a in A {
         delete a;
       }
     }
   }
 }



 /*
 Reader module for use in the Parser Class.
 */
 module TomlReader {

   use Regexp;

   /* Returns the next token in the current line without removing it */
   proc top(source) {
     if source.currentLine.isEmpty() {
       source.newLine();
     }
     return source.currentLine[source.currentLine.D.first];
   }

   /* Returns a boolean or wether or not another line can be read
   /  Also updates the currentLine if empty */
   proc readLine(source) {
     return source.nextLine();
   }

   proc skipNext(source) {
     source.currentLine.skip();
   }

   proc skipLine(source) {
     source.skipROL();
   }

   /* retrives the next token in currentline */
   proc getToken(source) {
     return source.nextToke();
   }


   class Source {

     var tomlFile: string;
     var tokenD = {1..0},
       tokenlist: [tokenD] Tokens;
     var currentLine: Tokens;


     proc init(tomlFile: string) {
       this.tomlFile = tomlFile;
       var openFile = open(tomlFile, iomode.r);
       this.genTokenlist(openFile);
     }


     /*
     proc init(tomlStr: string) {
      this.tomlStr = tomlStr;
      this.genTokenlist(tomlStr);
     }
      */

     /* generates list of Token objects */
     proc genTokenlist(input) {
       for line in input.lines() {
         splitLine(line);
       }
       currentLine = tokenlist[tokenD.first];
     }

     /*
     proc genTokenlist(input: string) {
       for line in input.split('\n') {
         splitLine(line);
       }
      currentLine = tokenlist[tokenD.first];
     } 
     */

     proc splitLine(line) {
       var linetokens: [1..0] string;
       const doubleQuotes = '".*?\\[^\\]?"',
          singleQuotes = "'.*?\\[^\\]?'",
          bracketContents = "(\\[\\w+\\])",
          brackets = "(\\[)|(\\])",
          comments = "(\\#)",
          commas = "(\\,)",
          equals = "(\\=)",
          curly = "(\\{)|(\\})";

       const pattern = compile('|'.join(doubleQuotes,
                                     singleQuotes,
                                     bracketContents,
                                     brackets,
                                     comments,
                                     commas,
                                     curly,
                                     equals));

       for token in pattern.split(line) {
         var strippedToken = token.strip();
         if strippedToken.length != 0  {
           linetokens.push_back(strippedToken);
         }
       }
       if linetokens.size != 0 {
         var tokens = new Tokens(linetokens);
         tokenlist.push_back(tokens);
       }
     }


     proc newLine() {
       if nextLine() { 
         if currentLine.isEmpty() {
           tokenlist.remove(tokenD.first);
           currentLine = tokenlist[tokenD.first];
         }
       }
       else {
         halt("Error: end of file");
       }
     }

     /* Reads next line into currentline */
     proc nextLine() {
       if currentLine.isEmpty() {
         if tokenD.size == 1 {
           return false;
         }
         else {
           tokenlist.remove(tokenD.first);
           currentLine = tokenlist[tokenD.first];
           return true;
         }
       }
       return true;
     }


     /* retrives next token in currentLine */
     proc nextToke() {
       newLine();
       return currentLine.next();
     }

     proc skipROL() {
       for token in currentLine {
         currentLine.skip();
       }
     }


     proc debug() {
       var lineCounter: int = 1;
       for line in tokenlist {
         write("line: ",lineCounter);
         for token in line {
           write(" token: " + token);
         }
         lineCounter += 1;
         writeln();
       }
     }

     proc deinit() { 
       for token in tokenlist {
         delete token;
       }
     }
   }



   /* Array wrapper */
   class Tokens {
     var D: domain(1);
     var A: [D] string;

     proc init(A: [?D] string) {
       this.D = D;
       this.A = A;
     }

     proc skip() {
       var idx =  D.first;
       var toke = A(idx);
       A.remove(idx);
     }

     proc next() {
       var idx =  D.first;
       var toke = A(idx);
       A.remove(idx);
       return toke;
     }

     proc isEmpty(): bool {
       return this.A.isEmpty();
     }

     proc this(idx) ref {
       return this.A[idx];
     }

     iter these() ref {
       for a in this.A {
         yield a;
       }
     }

     /* This makes the writeln(tokens) == writeln(tokens.A) */
     proc readWriteThis(f) {
       f <~> this.A;
     }
   }
 }
}