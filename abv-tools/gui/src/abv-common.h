/**
 * 
 **/
#ifdef ANDROID
#include <android/log.h>
#endif

#ifdef HX_LINUX
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
	#ifdef ABV_AUDIO
#include <SDL2/SDL_mixer.h>
	#endif
#else
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
	#ifdef ABV_AUDIO
#include <SDL_mixer.h>
	#endif
#endif

#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cmath>
// ./emcc tests/sdl2glshader.c -s USE_SDL=2 -s LEGACY_GL_EMULATION=1 -o sdl2.html
// emscripten

//Screen dimension 
extern int gScreenWidth;
extern int gScreenHeight;
extern SDL_Window* gWindow;

enum AbvEvent
{
	AE_EVENT 			= 0,
	AE_QUIT 			= 1,
	AE_MOUSE_MOVE 		= 2,
	AE_MOUSE_DOWN 		= 3,
	AE_MOUSE_UP 		= 4,
	AE_MOUSE_X 			= 5,
	AE_MOUSE_Y 			= 6,
	AE_KEY_DOWN 		= 7,
	AE_KEY_UP 			= 8
};



extern "C" int render_quad( SDL_Rect* rect, SDL_Color* color, int border,
	SDL_Color* borderColor );

extern "C" TTF_Font* get_font(const char* path, int size);

//Texture* get_texture(const char* path);

extern "C" int render_texture(const char* path,int x, int y, SDL_Rect* tile = NULL, float scale = 1);

extern "C" Mix_Music* get_music(const char* path);

extern "C" Mix_Chunk* get_sound(const char* path);

extern "C" void delay(Uint32 ms);

extern "C" int render_text(const char* font,const char* text, int x, int y,
	SDL_Color color, int wrap);

extern "C" int abv_key(int key);

