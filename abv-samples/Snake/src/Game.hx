package ;

import abv.AM;
import abv.ds.FS;
import abv.css.*;
import abv.style.Style;
import abv.*;
import abv.anim.*;
import abv.factory.*;
import abv.math.*;
import abv.ui.*;
import abv.bus.*;
import abv.io.AD;

using abv.sys.ST;
using StringTools;
//
class Game extends Root{

	var snake:Array<Component> = [];
	var food:Array<Component> = [];
	var corners:Array<Point> = [];
	public var dir:Point;
	var old:Point;
	var np:Point;
	var sp:Point;
	var lastDir:Point;
	var world:Rect;
	var speed:Int;
	var sWidth:Int;
	var sLength:Int;
	var fLength:Int;
	var hitTween = false;
	
	public function new(w:Float , h:Float)
	{
		super("Game" , w , h); 
// register custom msg
		MS.cmCode("cmSpeed"); 
		MS.cmCode("cmFood"); 
		MS.cmCode("cmPlay"); 
		MS.cmCode("cmStyle"); 

		init(); 
	}// new()

	public function init() 
	{  
		delChildren();  	 
		dir = new Point(); 
		old = new Point(); 
		speed = 2;
		snake.clear(); 
		corners.clear(); 
		food.clear(); 
		sLength = 0;
		fLength = 0;
		state = MD.STOP;

		build("res/Snake.html");  

		for(it in getChildren() ){  
			if (it.name.startsWith("snake"))snake.push(it); 
			else if (it.name.startsWith("food"))food.push(it); 
		}
		sWidth = Style.stylesheet[".snake"].width.i(); 
		world = new Rect(0 , 0 , width - sWidth , height - sWidth);  
		
		corners[0] = new Point(snake[0].x,snake[0].y);
	}// init()

	function reset()
	{
		init(); 
		food[0].visible = true;
		var rp = randPoint();
		food[0].x = rp.x;
		food[0].y = rp.y;
		
		state = MD.PLAY; 

		addFood(); 
		Juggler.clear(); 
		var ts = TS.LINEAR;
		var path = Curve.circle(new Point(150 , 250) , 150 , ts); 
		var tween = new Tween(path , food[1].moveBy , 20 , ts , 2); 
		tween.onComplete = function( ){hitTween = false; }; 
		Juggler.add(tween);  
	}// reset()
	
	function addSnake()
	{
		var pt:Point;
		var len = snake.length;
		if (len == 0)return;
		else if (len == 1)pt = new Point(snake[0].x - sWidth , snake[0].y); 
		else{ 
			pt = new Point(snake[len - 1].x - lastDir.x,
				snake[len - 1].y - lastDir.y);  
		}

		snake[len] = new Component("snake_"  +  len);  
		Style.sync(snake[len] , snake[len - 1].style);  
		snake[len].x = pt.x;
		snake[len].y = pt.y;
		
		addChild(snake[len]);  
	}//
	
	function randPoint()
	{
		var x = sWidth  +  Math.random()  *  (width  -  2 * sWidth);  
		var y = sWidth  +  Math.random()  *  (height  -  2 * sWidth);  
		return new Point(x , y); 
	}// randPoint()
	
	function addFood()
	{
		var len = food.length;
		food[len] = new Component("food_" + len);  
		var rp = randPoint();
		food[len].x = rp.x;  
		food[len].y = rp.y;  
		Style.sync(food[len] , Style.stylesheet[".food"]);  
		addChild(food[len]);  
	}//
	
	function delFood(ix:Int)
	{
		var f = food[ix];
		if (f != null ){
			food.remove(f);  
			delChild(f); 
		}
	}//
	
	public override function update()
	{
		var ix =  - 1;
		var rect:Rect;
		var len = snake.length;
		if (state != MD.PLAY)return;
		
		if (dir.eq(old.opposite())){
			dir.copy(old); 
			return;
		}
		
		if (!old.eq(dir) ){ 
			sp = new Point(snake[0].x, snake[0].y);  
			corners.insert(1 , sp);  
//for(k in 0...corners.length)trace(k + ":" + corners[k]); 
			old.copy(dir); 
		}
		
		np = dir.clone().scale(speed); 	
		snake[0].x += np.x; 
		snake[0].y += np.y; 
var tp = new Point();	
		for(i in 1...len ){
			tp.set(snake[i].x,snake[i].y);
			for(j in 0...corners.length - 1 ){ 
					if (tp.between(corners[j] , corners[j + 1]) ){ 
						np.copy(corners[j]); 
						if ((i == (len - 1)) && (corners.length > 2) ){
							corners.pop(); 
						}
						break;					
					}
					np.copy(corners[j + 1]); 
			}
		
			np = np.sub(new Point(snake[i].x,snake[i].y));  
			if (np.length > 0 ){ 
				if (i == (len - 1) ){
					lastDir = np.clone(); 
					lastDir.scale(sWidth/lastDir.length); 
				}
				np.scale(speed/np.length);  
				snake[i].x += np.x; 
				snake[i].y += np.y; 
			}
		}
		
		for(j in 1...corners.length - 1 ){
			tp.set(snake[0].x,snake[0].y); 
			if (tp.between(corners[j] , corners[j + 1])) theEnd(); 
		}
		
		if (!world.containsPoint(new Point(snake[0].x,snake[0].y))) theEnd(); 
							
		for(i in 0...food.length ){
			rect = new Rect(food[i].x - sWidth , food[i].y - sWidth , 2 * sWidth , 2 * sWidth);  
			if (rect.containsPoint(new Point(snake[0].x,snake[0].y)) ){ 
				if (i != 1 ){
					tp = randPoint();
					food[i].x = tp.x;  
					food[i].y = tp.y;  
					addSnake();  
				}else if (!hitTween ){
					addSnake(); 
					hitTween = true; 
				}
//				if ((len%5) == 0)addFood(); 
				AD.play("res/audio/scratch.wav"); 
				break;
			} 
		}
		AD.render(this); 
	}// update()
	
	function theEnd()
	{
		dir.reset(); 
		msgBox(LG.get("GameOver"));  //trace("GameOver"); 
		AD.play("res/audio/beat.wav"); 
	}// theEnd()
	
	override function dispatch(md:MD)
	{ 	
		switch(md.msg) {
			case MD.NEW: reset(); setDir(1 , 0);   
			case MD.MSG: 
				var cm = md.f[0];
				var act = md.f[1];
				if (cm ==  MS.cmCode("cmSpeed") ){
					speed = act.i()  +  1;
				}else if (cm ==  MS.cmCode("cmPlay") ){
					if (act == 1 ){ 
						if (state == MD.STOP)reset(); 
						state = MD.PLAY; 
						dir.copy(old); 
					}else{
						state = MD.PAUSE; 
						dir.set(0 , 0); 
					}
				}else if (cm ==  MS.cmCode("cmStyle") ){
		//			trace("style: "  +  act); 
					for(it in Style.stylesheet)trace(it); 
					switch(act ){
						case 0: 
							snake[0].style.copy(Style.stylesheet[".orange"]);  
							for(i in 1...snake.length)snake[i].style.copy(Style.stylesheet[".orange1"]); 
							draw(); 
						case 1: cast(MS.getComm(0,"RECT"),Component).style.copy(Style.stylesheet[".blue"]); 
							snake[0].style.copy(Style.stylesheet[".blue"]); 
							for(i in 1...snake.length)snake[i].style.copy(Style.stylesheet[".blue1"]); 
							draw(); 
						case 2: Style.sync(cast(MS.getComm(0,"RECT"),Component),Style.stylesheet[".ttt"]); 
							snake[0].style.copy(Style.stylesheet[".green"]); 
							for(i in 1...snake.length)snake[i].style.copy(Style.stylesheet[".green1"]); 
							draw(); 
					}
				}
		}
	}// dispatch()

	public inline function setDir(x:Int , y:Int)
	{  
		if (state == MD.PLAY)dir.set(x , y);   
	}// setDir()

	function msgBox(s="")
	{
		MS.call(new MD(this , "mboxLabel" , MD.NEW , null , s));  
		MS.call(new MD(this , "mbox" , MD.OPEN));  
	}//
	
}// com.tondy.snake.Game

