/**
 * 
 **/
const int MAX_EVENTS = 10;


enum MusicAction
{
	M_SOUND 			= -1
};

// KeyCode = abv.io.KB.A...UNKNOWN
enum KeyCode
{
	KC_A = 65,
	KC_B = 66,
	KC_C = 67,
	KC_D = 68,
	KC_E = 69,
	KC_F = 70,
	KC_G = 71,
	KC_H = 72,
	KC_I = 73,
	KC_J = 74,
	KC_K = 75,
	KC_L = 76,
	KC_M = 77,
	KC_N = 78,
	KC_O = 79,
	KC_P = 80,
	KC_Q = 81,
	KC_R = 82,
	KC_S = 83,
	KC_T = 84,
	KC_U = 85,
	KC_V = 86,
	KC_W = 87,
	KC_X = 88,
	KC_Y = 89,
	KC_Z = 90,
	KC_ALT = 18,
	KC_BACKQUOTE = 192,
	KC_BACKSLASH = 220,
	KC_BACKSPACE = 8,
	KC_CAPSLOCK = 20,
	KC_COMMA = 188,
	KC_COMMAND = 15,
	KC_CTRL = 17,
	KC_DELETE = 46,
	KC_DOWN = 40,
	KC_END = 35,
	KC_RETURN = 13,
	KC_EQUAL = 187,
	KC_ESC = 27,
	KC_F1 = 112,
	KC_F2 = 113,
	KC_F3 = 114,
	KC_F4 = 115,
	KC_F5 = 116,
	KC_F6 = 117,
	KC_F7 = 118,
	KC_F8 = 119,
	KC_F9 = 120,
	KC_F10 = 121,
	KC_F11 = 122,
	KC_F12 = 123,
	KC_F13 = 124,
	KC_F14 = 125,
	KC_F15 = 126,
	KC_HOME = 36,
	KC_INSERT = 45,
	KC_LEFT = 37,
	KC_LEFTBRACKET = 219,
	KC_MINUS = 189,
	KC_N0 = 48,
	KC_N1 = 49,
	KC_N2 = 50,
	KC_N3 = 51,
	KC_N4 = 52,
	KC_N5 = 53,
	KC_N6 = 54,
	KC_N7 = 55,
	KC_N8 = 56,
	KC_N9 = 57,
	KC_NUMPAD = 21,
	KC_NUMPAD_0 = 96,
	KC_NUMPAD_1 = 97,
	KC_NUMPAD_2 = 98,
	KC_NUMPAD_3 = 99,
	KC_NUMPAD_4 = 100,
	KC_NUMPAD_5 = 101,
	KC_NUMPAD_6 = 102,
	KC_NUMPAD_7 = 103,
	KC_NUMPAD_8 = 104,
	KC_NUMPAD_9 = 105,
	KC_NUMPAD_PLUS = 107,
	KC_NUMPAD_DECIMAL = 110,
	KC_NUMPAD_DIVIDE = 111,
	KC_NUMPAD_ENTER = 108,
	KC_NUMPAD_MULTIPLY = 106,
	KC_NUMPAD_MINUS = 109,
	KC_PAGEDOWN = 34,
	KC_PAGEUP = 33,
	KC_PERIOD = 190,
	KC_QUOTE = 222,
	KC_RIGHT = 39,
	KC_RIGHTBRACKET = 221,
	KC_SEMICOLON = 186,
	KC_SHIFT = 16,
	KC_SLASH = 191,
	KC_SPACE = 32,
	KC_TAB = 9,
	KC_UP = 38,
	KC_UNKNOWN = 0
};



extern "C" bool init_sdl(const char* name, int width, int height);

extern "C" void close_sdl();

extern "C" int clear_screen();

extern "C" void render_screen();

extern "C" int render_texture_c(const char* path, int x, int y, int tx, int ty, int tw,
	int th, float scale);
	
extern "C" int render_text_c(const char* font,const char* text, int x, int y,
	int r,int g,int b,int a, int wrap);

extern "C" int* poll_event();

extern "C" int play_music(const char* path, int action);

extern "C" int render_quad_c( int x, int y, int w, int h, int r, int g, int b, int a,
	int border, int br, int bg, int bb, int ba );

