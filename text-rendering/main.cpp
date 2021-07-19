#include <iostream>
#include "text_renderer.h"
#include "memory-aid.h"
#include <string>

COUNT_MEMORY

int main(int, char **)
{
    {
        text_renderer renderer(60, 30);
        if (!renderer.init("Text Renderer", "../../VeraMono.ttf"))
            return -1;
           
        size_t count = 0;
        while (!renderer.window.should_close())
        {
            renderer.window.poll_events();

            renderer.clear();
            renderer.print("Hello!\nHow r u? uWu\n");
            renderer.print(std::to_string(++count).c_str());

            renderer.window.clear();
            renderer.render_screen();
            renderer.window.swap_buffers();
        }
    }
    PRINT_MEMORY_SUMMARY
}
