/**
 * Haxe CFFI
 **/
#include "abv.h"
#include "hx.h"

value clear_screen_hx()
{
	int r = clear_screen();
	return alloc_int(r);
}// clear_screen_hx()


value render_screen_hx()
{
	render_screen();
	return alloc_int(0);
}// render_screen_hx()

value init_sdl_hx(value name, value width, value height)
{
	bool result = false;
	if(!val_is_int(width) || !val_is_int(height)) return alloc_int(result);

	result = init_sdl(val_string(name), val_int(width),val_int(height));

	return alloc_int(result);
}// init_sdl_hx()

value close_sdl_hx()
{
	close_sdl();
	return alloc_int(0);
}// close_sdl_hx()

value render_text_hx(value* args, int nargs)
{ 
	const char* font = val_string(args[0]);
	const char* text = val_string(args[1]);

	int x = val_int(args[2]);
	int y = val_int(args[3]);
	int r = val_int(args[4]);
	int g = val_int(args[5]);
	int b = val_int(args[6]);
	int a = val_int(args[7]);
	int wrap = val_int(args[8]);

	int result = render_text_c(font,text, x, y,r,g,b,a, wrap);
	
	return alloc_int(result);
}// render_text_hx()

value poll_event_hx()
{ 
	int* r;
	r = poll_event();
		
	value result = alloc_array(MAX_EVENTS);
	for(int i=0;i<MAX_EVENTS;i++){
		val_array_set_i(result,i,alloc_int( r[i] ) );
	}
   
	return result;
}// poll_event_hx()

value get_window_size_hx()
{
	int w = 0, h=0;
//	SDL_GetWindowSize(gWindow, &w, &h);
	
	value result = alloc_array( 2 );
	val_array_set_i(result,0,alloc_int( w ) );
	val_array_set_i(result,1,alloc_int( h ) );
  
	return result;
}// get_window_size_hx()

value render_texture_hx(value* args, int nargs)
{ 
	const char* path = val_string(args[0]);
	int x = val_int(args[1]);
	int y = val_int(args[2]);

	int tx = val_int(args[3]);
	int ty = val_int(args[4]);
	int tw = val_int(args[5]);
	int th = val_int(args[6]);

	float scale = val_float(args[7]);

//		printf("id: %s, (%d:%d) %d,%d,%d,%d\n",path.c_str(),x,y,tile->x,tile->y,tile->w,tile->h);
	int r = render_texture_c(path,x,y,tx,ty,tw,th,scale);
///
	return alloc_int(r);
}// render_texture_hx()

value play_music_hx(value path, value action)
{
	int r = 0;
#ifdef ABV_AUDIO
	const char* id = val_string(path);
	int cmd = val_int(action);
	r = play_music(id,cmd);
#endif
	return alloc_int(r);
}// play_music_hx()

value render_quad_hx(value* args, int nargs)
{ 
	int x = val_int(args[0]);
	int y = val_int(args[1]);
	int w = val_int(args[2]);
	int h = val_int(args[3]);

	int r = val_int(args[4]);
	int g = val_int(args[5]);
	int b = val_int(args[6]);
	int a = val_int(args[7]);

	int border = val_int(args[8]);
	int br = val_int(args[9]);
	int bg = val_int(args[10]);
	int bb = val_int(args[1]);
	int ba = val_int(args[12]);

	int result = render_quad_c( x, y, w, h, r, g, b, a, border, br, bg, bb, ba );

	return alloc_int(result);
}// render_quad_hx()



DEFINE_PRIM(init_sdl_hx, 3);
DEFINE_PRIM(poll_event_hx, 0); 
DEFINE_PRIM(render_screen_hx, 0); 
DEFINE_PRIM(close_sdl_hx, 0); 
DEFINE_PRIM(clear_screen_hx, 0); 
DEFINE_PRIM(play_music_hx, 2); 
DEFINE_PRIM(get_window_size_hx, 0); 
// DEFINE_PRIM_MULT for args > 5  
DEFINE_PRIM_MULT(render_text_hx); 
DEFINE_PRIM_MULT(render_quad_hx);
DEFINE_PRIM_MULT(render_texture_hx);

///
