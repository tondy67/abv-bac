package ;
/**
 * TT
 **/
import haxe.Int64;

using abv.sys.ST;

@:dce
class TT{

	public static inline var PSTART  = "start-";
	public static inline var PMIDDLE = "mid-";
	public static inline var PEND 	 = "end-";

 	public static inline function factorial(n:Int):Int64
	{ 
		var i = Int64.ofInt(n);
		if (Int64.compare(i, Int64.ofInt(1)) == 0)
			return Int64.ofInt(1);
		return Int64.mul(i, factorial(Int64.toInt(i) - 1)); 
	}// factorial() 

	public static inline function fillArray(len:Int, s="array")
	{
		var r = [PSTART + s];
		if (len < 0) len = 0;
		var m = (len/2).i();
		for (i in 1...m) r[i] = s + i;
		r[m] = PMIDDLE + s;
		for (i in (m+1)...len) r[i] = s + i;
		r[len] = PEND + s;
		return r;
	}// fillArray()

	public static inline function fillMap(len:Int,s="map")
	{
		var r = [PSTART + s => 0];
		if (len < 0) len = 0;
		var m = (len/2).i();
		for (i in 1...m) r.set(s + i, i);
		r.set(PMIDDLE + s, m);
		for (i in (m+1)...len) r.set(s + i, i);
		r.set(PEND + s, len);
		return r;
	}// fillMap()

	public static inline function fillString(len:Int,s="string")
	{
		var max = 268435456;
		var r = "";
		if (len < 0) len = 0;
		if (len > max) len = max;
		
		var i = 0;
		while (r.length < len) r += s + i++;
		
		return r;
	}// fillString()
	

}// TT

