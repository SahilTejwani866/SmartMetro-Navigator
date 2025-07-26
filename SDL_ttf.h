#ifndef SDL_TTF_H_
#define SDL_TTF_H_

#include <SDL2/SDL.h>

// Version information
#define SDL_TTF_MAJOR_VERSION 2
#define SDL_TTF_MINOR_VERSION 20
#define SDL_TTF_PATCHLEVEL   2

// Font style definitions
#define TTF_STYLE_NORMAL        0x00
#define TTF_STYLE_BOLD          0x01
#define TTF_STYLE_ITALIC        0x02
#define TTF_STYLE_UNDERLINE     0x04
#define TTF_STYLE_STRIKETHROUGH 0x08

// Font structure
typedef struct _TTF_Font TTF_Font;

// Basic functions
extern DECLSPEC int SDLCALL TTF_Init(void);
extern DECLSPEC TTF_Font* SDLCALL TTF_OpenFont(const char *file, int ptsize);
extern DECLSPEC void SDLCALL TTF_CloseFont(TTF_Font *font);
extern DECLSPEC void SDLCALL TTF_Quit(void);

// Text rendering functions
extern DECLSPEC SDL_Surface* SDLCALL TTF_RenderText_Solid(TTF_Font *font,
                const char *text, SDL_Color fg);
extern DECLSPEC SDL_Surface* SDLCALL TTF_RenderText_Blended(TTF_Font *font,
                const char *text, SDL_Color fg);
extern DECLSPEC SDL_Surface* SDLCALL TTF_RenderText_Shaded(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg);

// Font style functions
extern DECLSPEC int SDLCALL TTF_GetFontStyle(const TTF_Font *font);
extern DECLSPEC void SDLCALL TTF_SetFontStyle(TTF_Font *font, int style);

// Error handling
extern DECLSPEC const char* SDLCALL TTF_GetError(void);

#endif /* SDL_TTF_H_ */

