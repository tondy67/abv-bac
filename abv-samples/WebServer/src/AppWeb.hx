package;
/**
 * AppWeb
 **/
import sys.io.File;
import haxe.io.Bytes;

import abv.net.web.WT;
import abv.net.web.WServer;
import abv.AM;

using abv.ds.TP;
using abv.ds.DT;
using abv.sys.ST;

class AppWeb{

	var urls = WServer.URLS;
	var host = WServer.HOST + ":" + WServer.PORT;
	var tmp = WServer.TMP; 
	var lhome = WT.LHOME + "<br>\n";

	public inline function new()
	{
	}// new()
	
	public function dispatch(ctx:WebContext,session:MapSS=null,form:MapSS=null)
	{ 
		ctx.mime = "";
		var path = ctx.path; //trace(ST.env());
		if((path == "/exit")||(path.starts("/pa/exit"))) mkExit(ctx);
		else if(path.starts("/upload"))mkUpload(ctx,session,form);
		else if(path.starts("/pa/cp/log"))mkLog(ctx);
		else ctx.body = mkPage("Home","",ctx.request).toBytes(); 
	}// dispatch()


	function mkUpload(ctx:WebContext,session:MapSS=null,form:MapSS=null)
	{
		var s = "",t = ""; 
		if(form != null){ 
			s += "<p>";
			for(k in form.keys()){ 
				if(form[k].starts("file:")){
					t = form[k].substr(5);
					var a = t.split(CC.SEP3);
					t = '$k: ${a[0]}'; 
					if(ST.exists(tmp)){ 
						try ST.save(tmp+"/"+a[0],a[2])
						catch(m:Dynamic){ST.error(m);}
					}
				}else{ 
					t = '<strong>$k:</strong> ${form[k]}'; 
				}
				s += '$t<br>'; 
			}
			s += "</p><p><strong>"+tmp+"</strong></p>";
			ctx.body = mkPage("upload",lhome+s).toBytes(); 
		}else ctx.body = mkPage("upload").toBytes(); 
	}// mkUpload()
	
	function mkExit(ctx:WebContext)
	{
		if(ctx.request == "/exit"){
			ctx.body = mkPage("Exit",'<center><h2>Stop WebServer?</h2>[ <a href="/pa/exit">Yes</a> ] ... [ <a href="/">No</a> ]<p>user: user<br>pass: pass</p></center>').toBytes(); 
		}else if(ctx.request == "/pa/exit"){  
			if(WT.referer(ctx,'http://$host/exit'))AM.delayExit = 1;
			else WT.redirect(ctx,'/exit'); 
		}
	}// mkExit()
	
	function mkPage(title="",body="",query="")
	{
		var q = query.good()?TP.urlDecode(query):"/";
		if(!title.good())title = q;
		var r = "?d="+TP.urlEncode(Date.now()+"");
		var p = '<a href="/path/$r">path</a>/<a href="/path/to/$r">to</a>/<a href="/path/to/app/$r">app</a>/<a href="/path/to/app/logic/$r">logic</a>';
		var tpl = '<p>${WT.LHOME}</p><strong>$q</strong><p>$p<br><a href="/pa/cp/log">Log</a><br><a href="/exit" >Exit</a></p>';
		var s = "<br />";
		var env = ST.env();
		for(k in env.keys()) s += '<strong>$k</strong>: ${env[k]}<br />';
		if(!body.good()) body = tpl + s; 
		return WT.mkPage(body,title);
	}// mkPage()
	
	function mkLog(ctx:WebContext)
	{
		var s = "";

		for(it in ST.getLog()){
			if(it.starts(WT.LERROR))it = it.replace(WT.LERROR,'<span style="color:red">') + "</span>";
			else if(it.starts(WT.LWARN))it = it.replace(WT.LWARN,'<span style="color:green">') + "</span>";
			else if(it.starts(WT.LINFO))it = it.replace(WT.LINFO,'<span style="color:gray">') + "</span>";
			else if(it.starts(WT.LDEBUG))it = it.replace(WT.LDEBUG,'<span style="color:blue">') + "</span>";
			else if(it.starts(WT.LLOG))it = it.replace(WT.LLOG,'<span style="color:yellow">') + "</span>";
			s += it + "<br>";
		}

		ctx.body = mkPage("WebServer Log",'$lhome<h2>Log</h2><p>$s</p>').toBytes(); 
		
	}// mkLog

	public function toString()
	{
		return "AppWeb()";
	}// toString()

}// AppWeb

