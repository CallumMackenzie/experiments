#include <iostream>
#include "text_renderer.h"
#include "memory-aid.h"

COUNT_MEMORY

int main(int, char **)
{
    {
        text_renderer renderer;
        if (!renderer.init("Text Renderer", 100, 50, "../VeraMono.ttf"))
            return -2;
        renderer.window.set_clear_colour(0xFFFFFF);

        // shader_vf vfs;
        // vfs.init(TEST_VERT_SHADER, TEST_FRAG_SHADER);
        // uint vbo;
        // uint vao;
        // glGenBuffers(1, &vbo);
        // glGenVertexArrays(1, &vao);
        // glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // glBindVertexArray(vao);
        // float data[6] = {
        //     0.f, 0.f,
        //     1.f, 1.f,
        //     0.f, 1.f};
        // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, data, GL_STATIC_DRAW);
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        // vfs.use();

        while (!renderer.window.should_close())
        {
            renderer.window.poll_events();
            renderer.window.clear();
            renderer.render_text("Hello!\0", 25, 25, 1, vec3(0.5, 0.5, 1));
            // renderer.draw_char('A', 25, 25, 3, vec3(0.5, 0.5, 1));
            // vfs.use();
            // glBindVertexArray(vao);
            // glDrawArrays(GL_TRIANGLES, 0, 3);
            renderer.window.swap_buffers();
        }
    }
    PRINT_MEMORY_SUMMARY
}
