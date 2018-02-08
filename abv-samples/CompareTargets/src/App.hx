package;

import haxe.Json;

import abv.AM;
import abv.cpu.Timer;
import abv.DBG;

using abv.ds.TP;
using abv.ds.DT;
using abv.sys.ST;

typedef BT<K,V> = haxe.ds.BalancedTree<K,V>;
typedef As = {p1:String, p2:String,p3:String};

class App extends AM{

	public function new()
	{ 
		ST.VERBOSE 	= ST.DEBUG; 
		ST.SILENT 	= false;
		ST.COLORS 	= true;

		super("App");

	}// new()
    	
	override function create()
	{
		var t:Float;
		DBG.watch("start");
		var f = TT.factorial(45);
		DBG.watch("factorial",f);
		
		var max = 100000;
		var w = "blah";
		
		var a = TT.fillArray(max,w);
		var fa = DBG.watch("Fill Array",max);
		var m = TT.fillMap(max,w);
		var fm = DBG.watch("Fill Map", max);
		t = DBG.watch("a: "+TT.PSTART+w, a.indexOf(TT.PSTART+w));
		t = DBG.watch("m: "+TT.PSTART+w, m.get(TT.PSTART+w));

		t = DBG.watch("a: "+TT.PMIDDLE+w, a.indexOf(TT.PMIDDLE+w));
		t = DBG.watch("m: "+TT.PMIDDLE+w, m.get(TT.PMIDDLE+w));

		t = DBG.watch("a: "+TT.PEND+w, a.indexOf(TT.PEND+w));
		t = DBG.watch("m: "+TT.PEND+w, m.get(TT.PEND+w));

		var s = TT.fillString(10000, w);
		DBG.watch("Fill String", s.length);

		DBG.print();
/*
var ff = sys.io.File.read(file); 
var eof = true; 
while(eof){
	try{
		line = ff.readLine();
		t.push(line);
	}catch(d:Dynamic){ eof = false;}
}
ff.close(); 
*/
	}// create()
	
	override function update()
	{
	}// update()

	function help(opt="")
	{
		var r = "Usage: app [help]";

		if(opt=="help")r = "Help help help";

		return r;
	}// help()
 	
	
	public static function main() 
	{ 
		var s = new App();
		
	}

}// App

class Ttt{
	public var p1 = "";
	public var p2 = "";
	public var p3 = "";
	
	public function new(s1:String,s2:String,s3:String)
	{
		p1 = s1;
		p2 = s2;
		p3 = s3;
	}
}
