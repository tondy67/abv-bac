/**
 * 
 **/

#include "abv.h"
#include "abv-common.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
	if(init_sdl("test",SCREEN_WIDTH,SCREEN_HEIGHT)){
	}else{
		printf("no init sdl");
		return(1);
	};
	
	bool quit = false;
	int* e;
	SDL_Color tcolor={0,0,0xFF,0xFF };	
	SDL_Rect rect = {100,50,100,200};
	SDL_Color color = {0,255,0,255};
	SDL_Color borderColor = {0,0,255,0xDD};
	SDL_Rect tile = { 0, 24, 12, 12 };
	const char* txt = "Haxe is an open source toolkit based on a modern, high level, strictly typed programming language, a cross-compiler, a complete cross-platform standard library and ways to access each platform's native capabilities.";

	int r = 0;
	int i = 0;
	while(!quit){
		while((e = poll_event())[AE_EVENT] != 0){ 
			if(e[AE_MOUSE_DOWN] == AE_MOUSE_DOWN)printf("mouse down\n"); 
			else if(e[AE_MOUSE_UP] == AE_MOUSE_UP)printf("mouse up\n"); 
//			onMouseMove(e[AE_MOUSE_X],e[AE_MOUSE_X]);
			
			if(e[AE_KEY_DOWN] != 0) printf("key down\n"); 
			else if(e[AE_KEY_UP] != 0) printf("key up\n"); 
			if(e[AE_QUIT] == AE_QUIT) quit = true; 
		}

		clear_screen();
		rect.x++; color.b = i;
		render_quad(&rect, &color, 1, &borderColor);
		r = render_texture("../res/img/ts.png",50,30,&tile);
		r += render_texture("../res/img/0.120.png",0+(i*2),200);
		r += render_text("../res/fonts/regular.ttf",txt,100,200,tcolor,300);
		render_screen();

		i++;
		if(i > 255)i = 0;
		delay(30);
	};
	
	close_sdl();	
	return 0;
}
