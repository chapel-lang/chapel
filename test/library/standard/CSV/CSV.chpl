/*
 * Copyright 2004-2018 Cray Inc.
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

module CSV {
  use IO;
  use DataFrames;

  // TODO: depends on #9790, #7134
  /*
  iter linesCSV(path: string) {
    var f = try! open(path, iomode.r);
    try! {
      for line in f.lines() {
        var tokens = line.split(",");
        for i in tokens.domain do
          tokens[i] = tokens[i].strip();
        yield tokens;
      }
    }
  }
   */

  proc openCSV(path: string) throws {
    var f = try open(path, iomode.r);
    var lines = try f.lines().these();

    var longestRow = 0;
    for line in lines {
      var ntokens = line.count(",") + 1;
      if ntokens > longestRow then
        longestRow = ntokens;
    }

    var csvTable: [1..lines.size, 1..longestRow] string;
    forall (row, line) in zip(1..lines.size, lines) {
      var tokens = line.split(",");
      for (col, token) in zip(1..tokens.size, tokens) do
        csvTable[row, col] = token.strip();
    }
    return csvTable;
  }

  // Imports the first row as the Series headers.
  proc csvDataFrame(path: string, type serTypes) throws {
    var csvTable = try openCSV(path);
    var csvRows = csvTable.shape(1);
    var serDom = {1..csvRows-1};

    var df = new DataFrame();
    for param serTypeIdx in 1..serTypes.size {
      type serType = serTypes(serTypeIdx);
      var serData: [serDom] serType;
      var serValid: [serDom] bool;
      for (serRow, csvRow) in zip(serDom, 2..csvRows) {
        try {
          serData[serRow] = csvTable[csvRow, serTypeIdx]: serType;
          serValid[serRow] = true;
        } catch {
          serValid[serRow] = false;
        }
      }

      var ser = new TypedSeries(serData, serValid);
      var serLab = csvTable[1, serTypeIdx];
      df.insert(serLab, ser);
      // TODO: have df.insert() take an owned Series
      delete ser;
    }
    return df;
  }
}
