/* This example demonstrates how to read in a CSV file
   from Kaggle.
   https://www.kaggle.com/allen-institute-for-ai/CORD-19-research-challenge?select=metadata.csv
   The included metadata-20lines.csv only has 20 lines of data including the
   header row that has the column names.
   Each line in the file is a row of data.  Columns are separated by commas.
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

config const fileName = "metadata-20lines.csv";
config const debug = true;

// Open up a file to work with.
var f = open(fileName, iomode.r);

// Now read the in the csv data. Way 1: formatted input into a list of maps
{
  var reader = f.reader();

  // read the first line to get the column names
  // assuming the following format, where num is not known ahead of time:
  //    col_name_1,col_name_2,...,col_name_num\n
  var num : int;
  var colNames : list(string);
  var colName : string;

  while (reader.readf("%S,", colName)) {
    writeln("colName = ", colName);
    colNames.append(colName);
  }
  writeln("colNames: ", colNames);

  reader.close();
}

