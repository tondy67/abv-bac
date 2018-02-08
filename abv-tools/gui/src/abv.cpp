/**
 * 
 **/
#include "abv.h"
#include "abv-common.h"

//#undef main


class Texture
{
	public:
		Texture();

		~Texture();

		bool loadFromFile( const char* path );
		bool loadFromText(const char* font,const char* text, 
			SDL_Color color, int wrap);

		void free();

		int render( int x, int y, SDL_Rect* tile = NULL, float scale = 1.0 );

		int getWidth();
		int getHeight();

	private:
		SDL_Texture* texture;

		int width;
		int height;
};



std::map <const char*, Texture> Textures;
std::map <const char*, TTF_Font*> Fonts;

#ifdef ABV_AUDIO
std::map <const char*, Mix_Music*> Musics;
std::map <const char*, Mix_Chunk*> Sounds;
#endif


//
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

bool gFontError = false;
bool gImgError 	= false;
bool gSndError 	= false;
///
Texture::Texture()
{
	texture = NULL;
	width = 0;
	height = 0;
}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromFile( const char* path )
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* surface = IMG_Load( path );
	if( surface == NULL ){
		printf("%s\n",IMG_GetError());
	}else{
		SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB( surface->format, 0, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( gRenderer, surface );
		if( newTexture == NULL ){
			printf("%s\n",SDL_GetError());
		}else{
			width = surface->w;
			height = surface->h;
			texture = newTexture;
		}

		SDL_FreeSurface( surface );
	}

	return texture != NULL;
}// Texture::loadFromFile()

bool Texture::loadFromText(const char* font,const char* text,
	SDL_Color color, int wrap)
{
	free();

	TTF_Font* f = get_font(font,14);
	if(f == NULL) return false;
	
	SDL_Texture* newTexture = NULL;
	SDL_Surface* surface = NULL;
// FIXME: neko segfault here: TTF_RenderUTF8_Blended_*
	surface = TTF_RenderUTF8_Blended_Wrapped( f, text, color, wrap );

	if( surface == NULL ){ 
			printf("%s\n",SDL_GetError());
	}else{
		newTexture = SDL_CreateTextureFromSurface( gRenderer, surface );
		if( newTexture == NULL ){
			printf("%s\n",SDL_GetError());
		}else{
			width = surface->w;
			height = surface->h;
			SDL_FreeSurface( surface );
			
			texture = newTexture;
		}
	}
	return texture != NULL;
}// Texture::loadFromText()

void Texture::free()
{
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		width = 0;
		height = 0;
	}
}// Texture::free()

int Texture::render( int x, int y, SDL_Rect* tile, float scale )
{
	SDL_Rect dst = { x, y, width, height };

	if( tile != NULL )
	{
		dst.w = tile->w * scale;
		dst.h = tile->h * scale;
	}
//if(texture != NULL)printf("render: [%d,%d](%d:%d) %d,%d.%d,%d \n",width,height,x,y,tile->x,tile->y,tile->w,tile->h);
//else printf("null\n");
	return SDL_RenderCopy( gRenderer, texture, tile, &dst );
}// Texture::render()

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}
///
int render_quad( SDL_Rect* rect, SDL_Color* color, int border,
	SDL_Color* borderColor )
{ 
	int r = 0;	

	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
 
	SDL_Rect copyRect = { rect->x, rect->y, rect->w, rect->h};
	SDL_Rect outlineRect = { rect->x-border, rect->y-border, 
							rect->w+2*border, rect->h+2*border };
	if(color->a){
		SDL_SetRenderDrawColor( gRenderer, color->r, color->g, color->b, color->a );		
		SDL_RenderFillRect( gRenderer, &copyRect );
	}

	if(border){
		SDL_SetRenderDrawColor( gRenderer, borderColor->r, borderColor->g,
								borderColor->b, borderColor->a );		
		SDL_RenderDrawRect( gRenderer, &outlineRect );
	}
	return r;
}// render_quad()

int render_quad_c( int x, int y, int w, int h, int r, int g, int b, int a,
	int border, int br, int bg, int bb, int ba )
{ 
	SDL_Rect rect = {x,y,w,h};
	SDL_Color fillColor = {r,g,b,a};
	SDL_Color borderColor = {br,bg,bb,ba};
	return render_quad(&rect, &fillColor, border, &borderColor);
}// render_quad_c()

TTF_Font* get_font(const char* path, int size)
{
	TTF_Font* font = NULL;
	if(gFontError)return font;
	if( Fonts.find(path) == Fonts.end() ){
		font = TTF_OpenFont( path, size );
		if( font == NULL ){
			printf("%s:%s\n",SDL_GetError(),path);
			gFontError = true;
		}else{
			TTF_SetFontHinting(font, TTF_HINTING_NORMAL);
			Fonts[path] = font;
		}
	}else{
		font = Fonts[path];
	}
	
	return font;
}// get_font()

Texture* get_texture(const char* path)
{ 
	Texture* texture = NULL;
	if(gImgError)return texture;
	if( Textures.find(path) == Textures.end() ){
		texture = new Texture;
		if(texture->loadFromFile(path))Textures[path] = *texture;
		else gImgError = true;
	}else{
		texture = &Textures[path];
	}

	return texture;
}// get_texture()

Texture* get_text_texture(const char* font,const char* text, 
	SDL_Color color, int wrap)
{ 
	Texture* texture = NULL;
	const char* id = text;
	
	if( Textures.find(id) == Textures.end() ){
		texture = new Texture;
		if(texture->loadFromText(font,text,color,wrap))Textures[id] = *texture;
	}else{
		texture = &Textures[id];
	}
	
	return texture;
}// get_text_texture()

int render_texture(const char* path,int x, int y, SDL_Rect* tile, float scale)
{
	int r = 0;
	Texture* texture = get_texture(path);
	if(texture == NULL)r = 1;
	else texture->render(x,y,tile,scale);

	return r;
}// render_texture()

int render_texture_c(const char* path, int x, int y, int tx, int ty, int tw,
	int th, float scale)
{
	SDL_Rect* tile=NULL;

	if(tx || ty || tw || th){
		tile = new SDL_Rect;
		tile->x = tx;
		tile->y = ty;
		tile->w = tw;
		tile->h = th;
	} 

	return render_texture(path,x,y,tile,scale);
}// render_texture_c()

Mix_Music* get_music(const char* path)
{
	Mix_Music* music = NULL;
	if(gSndError) return music;
	if( Musics.find(path) == Musics.end() ){
		music = Mix_LoadMUS( path);
		if( music == NULL ){
			printf("Couldn't open: %s\n",path);
			gSndError = true;
		}else Musics[path] = music;
	}else{
		music = Musics[path];
	}
	
	return music;
}// get_music()

Mix_Chunk* get_sound(const char* path)
{
	Mix_Chunk* sound = NULL;
	if(gSndError) return sound;
	if( Sounds.find(path) == Sounds.end() ){
		sound = Mix_LoadWAV( path);
		if( sound == NULL ){
			printf("Couldn't open: %s\n",path);
			gSndError = true;
		}else Sounds[path] = sound;
	}else{
		sound = Sounds[path];
	}
	
	return sound;
}// get_sound()

int play_music(const char* path, int action)
{
	int r = 0;
	if(gSndError) return r;
#ifdef ABV_AUDIO
	if(action == M_SOUND){
		Mix_Chunk* sound = get_sound(path);	
		Mix_PlayChannel( -1, sound, 0 );
	}else{
		Mix_Music* music = get_music(path);
		
		if( Mix_PlayingMusic() == 0 ){
			Mix_PlayMusic( music, -1 );
		}else{
			if( Mix_PausedMusic() == 1 ) Mix_ResumeMusic();
		}
	}
#endif
	return r;
}// play_music()

void delay(Uint32 ms)
{
	SDL_Delay(ms);
}// delay()

void close_sdl()
{
	for(auto it = Textures.begin(); it != Textures.end(); it++){
		Textures[it->first].free();
	}
	Textures.clear();

	for(auto it = Fonts.begin(); it != Fonts.end(); it++){
		TTF_CloseFont( Fonts[it->first] );
	}
	Fonts.clear();
	
#ifdef ABV_AUDIO
	for(auto it = Musics.begin(); it != Musics.end(); it++){
		Mix_FreeMusic( Musics[it->first] );
	}
	Musics.clear();

	for(auto it = Sounds.begin(); it != Sounds.end(); it++){
		Mix_FreeChunk( Sounds[it->first] );
	}
	Sounds.clear();

	Mix_Quit();
#endif

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

}// close_sdl()

int* poll_event()
{
	SDL_Event e;
	//Handle events on queue
	static int r[MAX_EVENTS];
	int x, y;
	
	for(int i=0;i<MAX_EVENTS;i++)r[i] = 0;
	
	if(SDL_PollEvent( &e ) != 0 ){ // printf("%d\n",e.type);
		r[AE_EVENT] = 1;
		if( e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || 
			e.type == SDL_MOUSEBUTTONUP ){
			//Get mouse position
			SDL_GetMouseState( &x, &y );
			r[AE_MOUSE_X] = x; r[AE_MOUSE_Y] = y;
			if(e.type == SDL_MOUSEMOTION)r[AE_MOUSE_MOVE] = AE_MOUSE_MOVE;
			if(e.type == SDL_MOUSEBUTTONDOWN)r[AE_MOUSE_DOWN] = AE_MOUSE_DOWN;
			else if(e.type == SDL_MOUSEBUTTONUP)r[AE_MOUSE_UP] = AE_MOUSE_UP;
		}else if( e.type == SDL_KEYDOWN ){
			r[AE_KEY_DOWN] = abv_key(e.key.keysym.sym);
			r[AE_KEY_UP] = 0;
		}else if( e.type == SDL_KEYUP ){
			r[AE_KEY_UP] = abv_key(e.key.keysym.sym);
			r[AE_KEY_DOWN] = 0;
		}else if(e.type == SDL_QUIT)r[AE_QUIT] = AE_QUIT;
	}
	return r;
}// poll_event()

int render_text(const char* font,const char* text, int x, int y,
	SDL_Color color, int wrap)
{
	int r = 0;
	Texture* texture = get_text_texture(font,text,color,wrap);

	if(texture == NULL)r = 1;
	else texture->render(x,y);
	
	
	return r;
}// render_text()

int render_text_c(const char* font,const char* text, int x, int y,
	int r,int g,int b,int a, int wrap)
{
	SDL_Color color={r,g,b,a};

	return render_text(font,text,x,y,color,wrap);
}// render_text_c()




bool init_sdl(const char* name, int width, int height)
{
	bool ok = true;

	gScreenWidth = width;
	gScreenHeight = height;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO 
#ifdef ABV_AUDIO
		| SDL_INIT_AUDIO 
#endif
		) < 0 ){
			printf("%s\n",SDL_GetError());
		ok = false;
	}else{
//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf("Warning: Linear texture filtering not enabled!\n");
		}

		//Create window
		gWindow = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenWidth, gScreenHeight, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ){
			printf("%s\n",SDL_GetError());
			ok = false;
		}else{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC  );
			if( gRenderer == NULL )	{
				printf("%s\n",SDL_GetError());
				ok = false;
			}else {
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					printf("%s\n",SDL_GetError());
					ok = false;
				}
				if( TTF_Init() == -1 ){
					printf("%s\n",SDL_GetError());
					ok = false;
				}
#ifdef ABV_AUDIO
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf("%s\n",SDL_GetError());
					ok = false;
				}
#endif
			}
		}
	}

	return ok;
}// init_sdl()

void render_screen()
{
	SDL_RenderPresent( gRenderer );
}// render_screen()

int clear_screen()
{
	int r = 0;
	if(gRenderer != NULL){
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer ); 
		r = 1;
	}
	return r;
}// clear_screen()

int abv_key(int key)
{
	int r;
	switch (key){
	  case SDLK_a: r = KC_A; break;
	  case SDLK_b: r = KC_B; break;
	  case SDLK_c: r = KC_C; break;
	  case SDLK_d: r = KC_D; break;
	  case SDLK_e: r = KC_E; break;
	  case SDLK_f: r = KC_F; break;
	  case SDLK_g: r = KC_G; break;
	  case SDLK_h: r = KC_H; break;
	  case SDLK_i: r = KC_I; break;
	  case SDLK_j: r = KC_J; break;
	  case SDLK_k: r = KC_K; break;
	  case SDLK_l: r = KC_L; break;
	  case SDLK_m: r = KC_M; break;
	  case SDLK_n: r = KC_N; break;
	  case SDLK_o: r = KC_O; break;
	  case SDLK_p: r = KC_P; break;
	  case SDLK_q: r = KC_Q; break;
	  case SDLK_r: r = KC_R; break;
	  case SDLK_s: r = KC_S; break;
	  case SDLK_t: r = KC_T; break;
	  case SDLK_u: r = KC_U; break;
	  case SDLK_v: r = KC_V; break;
	  case SDLK_w: r = KC_W; break;
	  case SDLK_x: r = KC_X; break;
	  case SDLK_y: r = KC_Y; break;
	  case SDLK_z: r = KC_Z; break;
	  case SDLK_LALT: case SDLK_RALT: r = KC_ALT; break;
	  case SDLK_BACKQUOTE: r = KC_BACKQUOTE; break;
	  case SDLK_BACKSLASH: r = KC_BACKSLASH; break;
	  case SDLK_BACKSPACE: r = KC_BACKSPACE; break;
	  case SDLK_CAPSLOCK: r = KC_CAPSLOCK; break;
	  case SDLK_COMMA: r = KC_COMMA; break;
	  case SDLK_LGUI: case SDLK_RGUI: r = KC_COMMAND; break;
	  case SDLK_LCTRL: case SDLK_RCTRL: r = KC_CTRL; break;
	  case SDLK_DELETE: r = KC_DELETE; break;
	  case SDLK_DOWN: r = KC_DOWN; break;
	  case SDLK_END: r = KC_END; break;
	  case SDLK_RETURN: r = KC_RETURN; break;
	  case SDLK_EQUALS: r = KC_EQUAL; break;
	  case SDLK_ESCAPE: r = KC_ESC; break;
	  case SDLK_F1: r = KC_F1; break;
	  case SDLK_F2: r = KC_F2; break;
	  case SDLK_F3: r = KC_F3; break;
	  case SDLK_F4: r = KC_F4; break;
	  case SDLK_F5: r = KC_F5; break;
	  case SDLK_F6: r = KC_F6; break;
	  case SDLK_F7: r = KC_F7; break;
	  case SDLK_F8: r = KC_F8; break;
	  case SDLK_F9: r = KC_F9; break;
	  case SDLK_F10: r = KC_F10; break;
	  case SDLK_F11: r = KC_F11; break;
	  case SDLK_F12: r = KC_F12; break;
	  case SDLK_F13: r = KC_F13; break;
	  case SDLK_F14: r = KC_F14; break;
	  case SDLK_F15: r = KC_F15; break;
	  case SDLK_HOME: r = KC_HOME; break;
	  case SDLK_INSERT: r = KC_INSERT; break;
	  case SDLK_LEFT: r = KC_LEFT; break;
	  case SDLK_LEFTBRACKET: r = KC_LEFTBRACKET; break;
	  case SDLK_MINUS: r = KC_MINUS; break;
	  case SDLK_0: r = KC_N0; break;
	  case SDLK_1: r = KC_N1; break;
	  case SDLK_2: r = KC_N2; break;
	  case SDLK_3: r = KC_N3; break;
	  case SDLK_4: r = KC_N4; break;
	  case SDLK_5: r = KC_N5; break;
	  case SDLK_6: r = KC_N6; break;
	  case SDLK_7: r = KC_N7; break;
	  case SDLK_8: r = KC_N8; break;
	  case SDLK_9: r = KC_N9; break;
//	  case SDLK_a: r = KC_NUMPAD; break;
	  case SDLK_KP_0: r = KC_NUMPAD_0; break;
	  case SDLK_KP_1: r = KC_NUMPAD_1; break;
	  case SDLK_KP_2: r = KC_NUMPAD_2; break;
	  case SDLK_KP_3: r = KC_NUMPAD_3; break;
	  case SDLK_KP_4: r = KC_NUMPAD_4; break;
	  case SDLK_KP_5: r = KC_NUMPAD_5; break;
	  case SDLK_KP_6: r = KC_NUMPAD_6; break;
	  case SDLK_KP_7: r = KC_NUMPAD_7; break;
	  case SDLK_KP_8: r = KC_NUMPAD_8; break;
	  case SDLK_KP_9: r = KC_NUMPAD_9; break;
	  case SDLK_KP_PLUS: r = KC_NUMPAD_PLUS; break;
	  case SDLK_KP_DECIMAL: r = KC_NUMPAD_DECIMAL; break;
	  case SDLK_KP_DIVIDE: r = KC_NUMPAD_DIVIDE; break;
	  case SDLK_KP_ENTER: r = KC_NUMPAD_ENTER; break;
	  case SDLK_KP_MULTIPLY: r = KC_NUMPAD_MULTIPLY; break;
	  case SDLK_KP_MINUS: r = KC_NUMPAD_MINUS; break;
	  case SDLK_PAGEDOWN: r = KC_PAGEDOWN; break;
	  case SDLK_PAGEUP: r = KC_PAGEUP; break;
	  case SDLK_PERIOD: r = KC_PERIOD; break;
	  case SDLK_QUOTEDBL: r = KC_QUOTE; break;
	  case SDLK_RIGHT: r = KC_RIGHT; break;
	  case SDLK_RIGHTBRACKET: r = KC_RIGHTBRACKET; break;
	  case SDLK_SEMICOLON: r = KC_SEMICOLON; break;
	  case SDLK_LSHIFT: case SDLK_RSHIFT: r = KC_SHIFT; break;
	  case SDLK_SLASH: r = KC_SLASH; break;
	  case SDLK_SPACE: r = KC_SPACE; break;
	  case SDLK_TAB: r = KC_TAB; break;
	  case SDLK_UP: r = KC_UP; break;
	  default: r = KC_UNKNOWN;
	}
	return r;	
}// abv_key()


