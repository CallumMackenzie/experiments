#include <iostream>
#include "text_renderer.h"
#include "memory-aid.h"

COUNT_MEMORY

int main(int, char **)
{
    {
        text_renderer renderer(60, 30);
        if (!renderer.init("Text Renderer", "../../VeraMono.ttf"))
            return -1;
        char tmp = 39;
        for (size_t i = 0; i < 60; ++i)
            for (size_t j = 0; j < 30; ++j)
                renderer.set_char(i, j, ++tmp >= 127 ? (tmp = 39) : tmp);
        while (!renderer.window.should_close())
        {
            renderer.window.poll_events();
            renderer.window.clear();
            renderer.render_screen();
            renderer.window.swap_buffers();
        }
    }
    PRINT_MEMORY_SUMMARY
}
