use IO;

var file_name = "test_file.txt";
var writer = open( file_name, iomode.cw ).writer();

writer.writeln( "This is the first line" );
writer.writeln( "Isnt this easy?." );
writer.writeln( "Right! lets add our third and last line." );
writer.close();

var reader = open( file_name, iomode.r ).reader();

var line_style = new iostyle();
var line: string;
line_style.string_format = iostringformat.toend:uint(8);
line_style.string_end = 0x0a; // ascii newline.
while( reader.read( line, style=line_style ) ) do writeln( line );

