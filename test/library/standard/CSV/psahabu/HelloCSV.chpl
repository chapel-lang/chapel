use CSV;

type fremontTypes = (string, int, int);

var table = openCSV("fremont_bike_counts.csv");
writeln(table);

writeln();

var df = csvDataFrame("fremont_bike_counts.csv", fremontTypes);
writeln(df);

writeln();

var df_idx = df.set_index(string, "Date");
writeln(df_idx);
