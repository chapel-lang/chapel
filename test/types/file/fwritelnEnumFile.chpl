enum precipitation {mist, sprinkle, drizzle, rain, shower};
var todaysWeather: precipitation = precipitation.sprinkle;

var f: file = new file(filename = "_test_fwritelnEnumFile.txt", mode = FileAccessMode.write);

f.open();
f.writeln(todaysWeather);
