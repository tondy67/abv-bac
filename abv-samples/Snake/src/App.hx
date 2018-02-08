package ;

/** 
  *  App
  **/
import abv.AM; 
import abv.bus.*;
import abv.io.*;
import abv.ui.*;
import abv.factory.IView;

using abv.sys.ST;

class App extends AM {

	var game:Game;
	var view:IView;
	var gui:Gui;
static var urlData = "";	
	public function new()
	{ 
		super("App");  
	}// new()
	
	override public function update()
	{   
		AD.apply(view);
	}// update()

	override function create() 
	{ 
		ST.COLORS = true;
		ST.SILENT = false;
//		ST.VERBOSE = ST.DEBUG;
		MS.setSlot(this , MD.KEY_DOWN); 
// customMessage register
		MS.cmCode("cmSound"); 
		
		view = new ViewGL("ViewGL");  
		if (!view.ready ){
			view.dispose();  
			view = new View("View");  
			if (!view.ready) ST.exit("no view"); 
			on2D(); 
		}
	
		setSize(); 
		game = new Game(AM.WIDTH , AM.HEIGHT); 
		AD.addRoot(game); 
	
		gui = new Gui(AM.WIDTH , AM.HEIGHT);    
		gui.context = 2; 
		gui.build("res/ui/default/gui.html");  
		AD.addRoot(gui);  
		onResize(); 		
///
//var n = new ttt.Node2();  trace(n);  n.getFamilyGR(); 
//var tr = new ttt.Tree();  trace(tr);  tr.getFamilyGR(); 
var s = abv.ds.FS.getText("res/Rect.svg"); 
//var s = abv.ds.FS.getText("res/haxe.svg"); 
if(ST.good(s)){ 
	var v = abv.parser.Svg.parse(s);   
	game.addChild(v);
}
///
//ST.zip("res","test.zip");
//ST.unzip("test.zip","ttt");
//DBG.bp("context",gui.context );
	}// create()

	override function dispatch(md:MD)
	{ 	//	trace(id + ": " + md); 
		switch(md.msg) {
			case MD.EXIT: ST.exit(); 
			case MD.KEY_DOWN: _onKeyDown_(md.f[0]); 
			case MD.MSG: 
				var cm = md.f[0];
				if (cm ==  MS.cmCode("cmSound") ){
					AM.sound = md.f[1] == 1 ? true : false; 
				}
		}
	}// dispatch()

	function _onKeyDown_(key:Float)
	{ 

		if (key == KB.SPACE ){ 
			game.dir.reset(); 
		}else if (key == KB.N1)trace(MS.info()); 
		else if (key == KB.N2)trace("n2");
	
		var a = [KB.UP , KB.RIGHT , KB.DOWN , KB.LEFT];
		for(i in 0...a.length ){
			if (key == a[i] ){
				switch(i ){
					case 0: game.setDir(0 ,  - 1);
					case 1: game.setDir(1 , 0);
					case 2: game.setDir(0 , 1);
					case 3: game.setDir( - 1 , 0);
				}
				break;
			}
		}
	}// _onKeyDown_()
}// com.tondy.snake.App
