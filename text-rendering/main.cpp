#include <iostream>
#include "text_renderer.h"
#include "memory-aid.h"

COUNT_MEMORY

int main(int, char **)
{
    {
        text_renderer renderer(60, 30);
        if (!renderer.init("Text Renderer", "../VeraMono.ttf"))
            return -1;

        while (!renderer.window.should_close())
        {
            renderer.window.poll_events();
            renderer.window.clear();
            // renderer.render_text("Hello!\0", 25, 25, 1, vec3(0.5, 0.5, 1));
            renderer.render_screen();
            renderer.window.swap_buffers();
        }
    }
    PRINT_MEMORY_SUMMARY
}
