#include "game/system.h"

#include "SDL.h"

static int use_virtual_keyboard(void)
{
#if defined (__vita__) || defined(__SWITCH__)
    return 1;
#else
    return 0;
#endif
}

void system_keyboard_set_input_rect(int x, int y, int width, int height)
{
    SDL_Rect rect = {x, y, width, height};
    SDL_SetTextInputRect(&rect);
}

static int is_showing(void)
{
    return SDL_IsTextInputActive();
}

void system_keyboard_show(void)
{
    if (use_virtual_keyboard() && !is_showing()) {
        SDL_StartTextInput();
    }
}

void system_keyboard_hide(void)
{
    if (use_virtual_keyboard() && is_showing()) {
        SDL_StopTextInput();
    }
}
