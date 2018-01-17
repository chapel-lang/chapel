/*

use similar;
writeln(similar.str2num('2000'));

*/

module similar{
	proc num2str(num:int){
		var i:int;
		var rem:int;
		var len:int = 0;
		var n:int = num;
		var dummy:int = num;
		var str:string;
		var u:int;
		while(n != 0){
			len+=1;
			n=n/10;
		}

		for x in 1..len {
			rem = dummy%10;
			dummy = (dummy-rem)/10;
			u=u*10+rem;
		}
		for x in 1..len{
			str += u%10;
			u = u/10;
		}
		return str;
	}
	proc str2num(str:string){
		var len:int = str.length;
		var i:int;
		var num:int = 0;
		var dummy:string;
		for i in 1..len{
			dummy = str[i];
			select dummy{
				when '0'do num = num*10;
				when '1'do num = num*10 + 1;
				when '2'do num = num*10 + 2;
				when '3'do num = num*10 + 3;
				when '4'do num = num*10 + 4;
				when '5'do num = num*10 + 5;
				when '6'do num = num*10 + 6;
				when '7'do num = num*10 + 7;
				when '8'do num = num*10 + 8;
				when '9'do num = num*10 + 9;
				otherwise {
					writeln("Error: Not an Integer");
				}
			}
		}
		return num;
	}
}


