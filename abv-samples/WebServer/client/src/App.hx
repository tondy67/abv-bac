package;

import abv.net.web.WClient;

using abv.sys.ST;

class App extends WClient{
	

	public function new()
	{ 
		super();

		var ts = haxe.Timer.stamp(); 
		var url = 'http://$host:$port/?ts=$ts';
		var arg = Sys.args()[0];
		if (arg != null) url = arg;
		ST.info("open",url,null);
		DBG.watch("start");
		var s = open(url);
		DBG.watch("open");
		if(s != "")ST.println(s);
		DBG.print();
	}

	public static function main()
	{
		var c = new App();
	}
}// App
