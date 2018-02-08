/**
 * 
 **/
#ifndef STATIC_LINK
#	define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#	define NEKO_COMPATIBLE
#endif

#ifdef HX_WINDOWS
#  define EXPORTIT __declspec( dllexport )
#else
#  define EXPORTIT
#endif

#include <hx/CFFI.h>

// Haxe CFFI
extern "C"{
EXPORTIT value init_sdl_hx(value name, value width, value height);
EXPORTIT value close_sdl_hx();
EXPORTIT value clear_screen_hx();
EXPORTIT value render_screen_hx();
EXPORTIT value play_music_hx(value music, value action);
EXPORTIT value poll_event_hx();
EXPORTIT value render_text_hx(value* args, int nargs);
EXPORTIT value render_quad_hx(value* args, int nargs);
EXPORTIT value render_texture_hx(value* args, int nargs);
}
