/* This example demonstrates how to read in a CSV file.

   The example csv file is from Kaggle,
   https://www.kaggle.com/allen-institute-for-ai/CORD-19-research-challenge?select=metadata.csv
   The included metadata-20lines.csv only has 20 lines of data including the
   header row that has the column names.

   Each line in csv files is a row of data.  Columns are separated by commas.
   Some of the cells of data have quotes because the data string itself includes
   a comma or commas.

   There are many options in terms of data structures that such data could be
   read into.  Here are the two illustrated here.
       (1) pythonic -- list of Map instances where there is an entry in the Map
               per column (key=column name, value=column value for that row)
       (2) chapeltastic -- associative array with the key being the column name
               and the value being an array of strings
 */
use IO;
use List; // FIXME: put in a link to the list primer
use Map;

config const fileName = "metadata-20lines.csv";
config const debug = true;

// Open up a file to work with.
var f = open(fileName, iomode.r);

// Now read the in the csv data. Way 1: formatted input into a list of maps
{
  var reader = f.reader();

  // read the first line to get the column names
  // assuming the following format, where the number of columns (ncol) is not 
  // known ahead of time:
  //    col_name_1,col_name_2,...,col_name_ncol\n
  var line : string;

  if (!reader.readline(line)) then
    writeln("ERROR: ", fileName, " appears to be empty");

  var colNames : list(string);
  colNames = createListOfColNames(line);

  if debug {
    writeln("line = ", line);
    writeln("colNames: ", colNames);
  }

  // Create a list of maps with one map per row of data.
  var dataRows : list(map(string,string));

  while (reader.readline(line)) {
    var aRowMap = new map(string, string);
    var start = 0;
    var colIdx = 0;
    while (start<line.size && colIdx<colNames.size) {
      var (nextVal,nextCommaIdx) = nextField(line,start);
      aRowMap.add(colNames[colIdx],nextVal);
      start = nextCommaIdx + 1;
      colIdx = colIdx + 1;
    }
    dataRows.append(aRowMap);
    if debug then writeln("aRowMap = ", aRowMap);
  }

  if debug then writeln("dataRows = ", dataRows);

  reader.close();
}

// Returns the index of the next comma or the string length if no
// next comma is found.  Commas inside quoted strings will be ignored.
proc findNextCommaNotInQuotes(str : string, start : int) {
  var inQuotes = false;
  for i in start..(str.size-1) {
    if !inQuotes && str[i]==',' then return i;
    if str[i]=='"' then inQuotes = !inQuotes;
  }
  return str.size;
}

// Given a line from a csv file and a starting index
// returns a tuple of the string starting at the starting index and
// ending right before the next comma not in a quoted string, and
// the index of the next comma.
//      input: "ab,cd,"ef",f"   and start=3
//      output: "cd"
//      input: "ab,cd,"ef",f"   and start=6
//      output: ""ef""
// Assumes that the starting index given is not within a quoted string.
proc nextField(line:string, start:int) {
    var commaIdx = findNextCommaNotInQuotes(line,start);
    return (line[start..(commaIdx-1)],commaIdx);
}

// Given the string that is the first line of a csv file that has column names
// returns a list of column names in order.
proc createListOfColNames(line : string) {
  var colNames : list(string);
  var start = 0;
  while (start<line.size) {
    var (nextVal,commaIdx) = nextField(line,start);
    colNames.append(nextVal);
    start = commaIdx+1;
  }
  return colNames;
} 
