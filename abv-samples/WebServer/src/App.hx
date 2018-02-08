package;

import abv.AM;
import abv.net.web.WServer;
import AppWeb;

using abv.sys.ST;

class App extends AM{

	var srv:WServer;
	var webapp:AppWeb;

	public function new()
	{ 
#if android
		super("App");
		ST.VERBOSE 	= ST.DEBUG; 
		ST.SILENT 	= true;
		ST.COLORS 	= true;
#else
		ST.VERBOSE 	= ST.DEBUG; 
		ST.SILENT 	= true;
		ST.COLORS 	= true;

		super("App");
#end 
	}// new()
	
	override function create()
	{
		super.create();
  		srv = new WServer();
		srv.log = log;

		webapp = new AppWeb();  
		srv.app = webapp.dispatch;
		
		srv.update = function()
			{ 
				//srv.info();
			};
		srv.start(); 

	}// create()
	
	function log(s:String)
	{
		if (!ST.SILENT) s = ST.msg(s,null,null);
		ST.println(s);
	}// log()
	
	override function update()
	{
		AM.delayExit *= 2; 
		if(AM.delayExit > 2){ 
			if(ST.SILENT)ST.printLog();
			srv.stop();
			ST.exit();
		}
	}
		
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

