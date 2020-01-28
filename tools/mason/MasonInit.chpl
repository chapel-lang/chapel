/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

use Path; 
use MasonEnv;
use MasonBuild;
use MasonHelp;
use MasonUtils;
use MasonExample;
use FileSystem;

proc masonInit(args) throws {
	try! {
		var name = '';
		for arg in args[2..]{
			if arg == '' {
			name = arg;
			} else {
					name = arg;
			}
		}
		//checks if path is given as parameter or not
		if name == '' {
			//If Mason.toml is already present, we get an error
			if isFile("Mason.toml") {
				throw new owned MasonError("Mason.toml already exists.");
			} else {
				//If Mason.toml not present, proceed with init
				const cwd  = getEnv("PWD");
				const currDir = basename(cwd);
				var name = currDir;
				makeTomlFileFromName(name);            
			}
		} else {
			/*if a path is given as a parameter
				check if the path is already present 
				or if TOML file is already present 
				in the given path
			*/
			var path = name;
			if(isDir(path)) {
					throw new owned MasonError("Path already exists.");
			} else if (isFile(path + "/Mason.toml")) {
					throw new owned MasonError("Mason.toml file already exists in path");
			} else {
					//creates a directory and makes a TOML file.
					mkdir(path);
					makeTomlFileFromPath(path); 
			}
		}
	}
	catch e: MasonError {
		writeln(e.message());
		exit(1);
	}
}

proc makeTomlFileFromPath(path: string){
	var currDir = basename(path);
	const baseToml = '[brick]\n' +
										'name = "' + currDir + '"\n' +
										'version = "0.1.0"\n' +
										'chplVersion = "' + getChapelVersionStr() + '"\n' +
										'\n' +
										'[dependencies]' +
										'\n';
	var tomlFile = open(path+"/Mason.toml",iomode.cw);
	var tomlWriter = tomlFile.writer();
	tomlWriter.write(baseToml);
	tomlWriter.close();
}

proc makeTomlFileFromName(name: string){
	const baseToml = '[brick]\n' +
										'name = "' + name + '"\n' +
										'version = "0.1.0"\n' +
										'chplVersion = "' + getChapelVersionStr() + '"\n' +
										'\n' +
										'[dependencies]' +
										'\n';
	
	var tomlFile = open("Mason.toml",iomode.cw);
	var tomlWriter = tomlFile.writer();
	tomlWriter.write(baseToml);
	tomlWriter.close();
}
