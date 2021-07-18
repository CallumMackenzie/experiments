#include <iostream>
#include "opengl.h"
#include "memory-aid.h"

COUNT_MEMORY

int main(int, char **)
{
    {
        gl_window window;
        window.init();
        window.set_clear_colour(0xfa01eb);
        while (!window.should_close())
        {
            window.poll_events();
            window.clear();
            window.swap_buffers();
        }
    }

    PRINT_MEMORY_SUMMARY
}
