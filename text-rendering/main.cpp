#include <iostream>
#include "text_renderer.h"
#include "memory-aid.h"

COUNT_MEMORY

int main(int, char **)
{
    {
        text_renderer renderer;
        if (!renderer.init("Text Renderer", 100, 50))
            return -2;
        while (!renderer.window.should_close())
        {
            renderer.window.poll_events();
            renderer.render_text();
        }
    }
    PRINT_MEMORY_SUMMARY
}
