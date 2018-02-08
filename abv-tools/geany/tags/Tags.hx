package;
/*
 * Haxe tags for Geany Autocompletion
 * grep -r "public"   /usr/share/haxe/std |grep -Ev '_std|__init__|new|@|toString' > haxestd.txt
 * grep function `grep -lr "extern class"   /usr/share/haxe/std`|grep -Ev '_std|__init__|new|@|toString'  >> haxestd.txt
 */
import haxe.Json;
import haxe.io.Path;
import sys.io.File;

using StringTools;

class Tags {

	var datafile = "haxestd.txt";
	var stddir	 = "/usr/share/haxe/std/";
	var tagfile:String;
	var ver:String;
	var count = 0;

	public function new()
	{ 
		ver = getVersion(); 
		tagfile = 'std.haxe-$ver.hx.tags';
		create();
	}// new()

	function create()
	{ 
		var f = "# format=pipe\n";
		f += "# Auto-generated for Haxe " + ver + "\n";
		var map = new Map<String,String>();
		var s = File.getContent(datafile);
		var rgx = ~/[ ][ ]+/g;
		s = rgx.replace(s," "); 
		var lines = s.split("\n");

		for (it in lines){
			it = it.replace(stddir,"");
			if (it.indexOf("var ") != -1){
				map.set(mkVar(it),"");
			}else if (it.indexOf("function ") != -1){
				map.set(mkFunc(it),"");
			}
			if ((count % 100) == 0) Sys.print(".");
			count++;
		}
		
		lines = [];
		for (k in map.keys()) lines.push(k);
		
		f += lines.join("");
		Sys.println("\nSave: " + tagfile);
		File.saveContent(tagfile,f);
	}// create()

	function mkVar(s:String)
	{ 
		var r = "";
		var find = "var ";
		var s1:Int, s2:Int, s3:Int;
		var cls = "", val = "", type = "";
		s = s.replace(";","");
		s1 = s.indexOf("(");
		s2 = s.lastIndexOf(")");
		if ((s1!=-1)&&(s2!=-1))s = s.substring(0,s1) + s.substring(s2+1); 
		s1 = s.indexOf(":");
		s2 = s.indexOf(find) + find.length;
		s3 = s.lastIndexOf(":"); 
		cls = s.substring(0,s1);
		if (s3 > s1){
			val = s.substring(s2,s3).trim();
			type = ":"+s.substr(s3+1).trim();
		}else{
			val = s.substr(s2).trim();
		}
		if (s.indexOf("static ") != -1){//trace(s,s1,s2,s3);
			r =  basename(cls) + "." + val + "|||\n";
		}
		cls = cls.replace(".hx","").replace("/",".");
		r += val + "|" + type  + "|[" + cls + "]|\n";
		return r;
	}
	
	function mkFunc(s:String)
	{ 
		var r = "";
		var find = "function ";
		var s1:Int, s2:Int, s3:Int;
		var cls = "", val = "", type = "";
		s = s.replace(";","");
		s1 = s.indexOf(":");
		s2 = s.indexOf(find) + find.length;
		s3 = s.lastIndexOf("("); 
		cls = s.substring(0,s1);
		if (s3 > s1){
			val = s.substring(s2,s3).trim();
			type = s.substr(s3).replace("{ }","").trim();
		}else{
			val = s.substr(s2).trim();
		}
		if (s.indexOf("static ") != -1){//trace(s,s1,s2,s3);
			r =  basename(cls) + "." + val + "||()|\n";
		}
		cls = cls.replace(".hx","").replace("/",".");
		r += val + "|" + cls + ".|"+ type  + "|\n";
		return r;
	}
	
	function basename(path:String)
	{ 
		return Path.withoutExtension(Path.withoutDirectory(path)); 
	}
	
	function getVersion()
	{ 
		var p = new sys.io.Process("haxelib",["version"]); 
		var r = p.stdout.readAll()  +  ""; 
		return r.trim();
	}
	
	public static function main() 
	{ 
		var n = new Tags();		
	}

}// Tags


