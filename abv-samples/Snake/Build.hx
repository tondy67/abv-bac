package;
/**
 * Build 'abv' build script
 **/
class Build{

	static inline var NAME	  = "abv";
	static inline var DEPS 	  = "abv";
	static inline var GIT_URL = "https://github.com/tondy67/";
	static inline var LINE	  = "-------------------------------------------";

	macro public static function abv():haxe.macro.Expr
	{
		print(LINE);
		print("Build: 'abv library' default build script");
		print(LINE);
		for (it in DEPS.split(" ")) setupLib(it);
			
		var s = '-main abv.sys.ST -lib abv -D build -neko $NAME.n';
		print("Build",NAME);
		Sys.command("haxe",s.split(" "));
		
		print("Convert","to executable");
		s = 'boot $NAME.n';
		Sys.command("nekotools",s.split(" "));
		
		print("Remove",NAME+".n");
		rm("abv.n");

		print("Save","haxe.info");
		s = "Date: " + Date.now() + "\n";
		s += LINE + "\n";
		s += "Haxe: " + exec("haxelib",["version"]);
		s += "Neko: " + exec("neko",["-version"]) + "\n";
		s += "Haxelib:\n" + exec("haxelib",["list"]); 
		try sys.io.File.saveContent("haxe.info",s);
		
		s = Sys.getCwd() + NAME;
		print(LINE);
		print("Project info");
		Sys.command(s,["info"]);
		print();
		Sys.command(s);
	
		return null;
	}// abv()
	
	static inline function exec(cmd:String, args:Array<String>=null)
	{ 
		var r = "-1";
		if(cmd != ""){
			if(args == null) args = [];
			try{
				var p = new sys.io.Process(cmd,args); 
				r = p.stdout.readAll()  +  ""; 
			}catch(e:Dynamic){ trace(e);}
		}
		return r;
	}// exec()

	static inline function read()
	{
		return Sys.stdin().readLine();
	}// read()

	static inline function isLib(name:String)
	{
		var r = false;
		var s = exec("haxelib",["list",name]);		
		if (s.indexOf(name+":") != -1) r = true;
		return r;
	}// isLib()

	static inline function setupLib(name:String)
	{
		print("Check", name);
		if (isLib(name)) return;
		print('Do you want to install $name library? (y/n)');
		var s = read();	
		if (s == "y"){
			Sys.command("haxelib",["git",name,GIT_URL + name]);
			if (!isLib(name)) exit("Bye");
		}else{
			exit("Bye");
		}
	}// setupLib()

	static inline function exit(s="")
	{ 
		var code = 0;
		if(s != ""){
			print(s);
			code = 1;
		}
		Sys.exit(code);
	}// exit()

	static inline function print(s="",d="")
	{ 
		if (s == "") Sys.println("");
		else if (d != "") Sys.println(s + ": " + d);
		else Sys.println(s);
	}// print()

	static inline function rm(path:String)
	{ 
		try sys.FileSystem.deleteFile(path)
		catch(e:Dynamic){trace(e);}
	}// rm()
	
}// Build


