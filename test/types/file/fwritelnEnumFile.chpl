enum precipitation {mist, sprinkle, drizzle, rain, shower};
var todaysWeather: precipitation = sprinkle;

var f: file = file(filename = "_test_fwritelnEnumFile.txt", mode = "w");

f.open;
fwriteln(f, todaysWeather);
