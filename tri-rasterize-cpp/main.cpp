
#define RENDER_TERM 1
#define RENDER_WIN 2
#define RENDER RENDER_WIN

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <time.h>

#include "memory-aid.h"
#include "rel-paths.h"
#include "galg.h"

#if RENDER == RENDER_WIN
#include "text_renderer.h"
#endif

COUNT_MEMORY

using namespace galg;

enum class winding
{
    clockwise,
    counter_clockwise
};

#define CULL_BACK 0
#define CULL_FRONT 1
#define CULL_NONE 2
#define CULLING CULL_BACK

struct camera_3d;
struct triangle_2d;
struct mesh_3d;

struct camera_3d
{
    fp_num fov = 80;
    fp_num near = 0.1;
    fp_num far = 100;
    fp_num aspect = 1;
    vec4fp rotation{0, 0, 0};
    vec4fp position{0, 0, 0};

    inline mat4fp view()
    {
        return mat4fp::look_at(position, mat4fp::rotation(rotation) * vec4fp{0, 0, 1} + position, vec4fp{0, 1, 0});
    }

    inline mat4fp perspective()
    {
        return mat4fp::perspective(fov, aspect, near, far);
    }

    vec4fp look_vector()
    {
        return mat4fp::rotation(rotation) * vec4fp{0, 0, 1};
    }
};

struct triangle_2d
{
    struct bounding_box
    {
        fp_num top, left, right, bottom = 0;
    };
    struct vert
    {
        vec2fp v;
        fp_num depth = 0;
        vec2fp t;
        char sym = ' ';
    };

    vert v[3];
    winding base_wind = winding::counter_clockwise;
    bool facing_view = false;

    triangle_2d() {}
    triangle_2d(vec2fp v1, vec2fp v2, vec2fp v3)
    {
        v[0].v = v1;
        v[1].v = v2;
        v[2].v = v3;
    }

    void set_sym(char sym)
    {
        for (size_t i = 0; i < 3; i++)
            v[i].sym = sym;
    }

    void get_bounding_box(bounding_box &box)
    {
        box.left = v[0].v[0];
        box.top = v[0].v[1];
        for (int i = 0; i < 3; i++)
        {
            if (v[i].v[1] < box.top)
                box.top = v[i].v[1];
            if (v[i].v[1] > box.bottom)
                box.bottom = v[i].v[1];
            if (v[i].v[0] < box.left)
                box.left = v[i].v[0];
            if (v[i].v[0] > box.right)
                box.right = v[i].v[0];
        }
    }

    winding get_wind()
    {
        if (facing_view)
            return base_wind;
        else if (base_wind == winding::counter_clockwise)
            return winding::clockwise;
        return winding::counter_clockwise;
    }
};

struct mesh_3d
{
    struct triangle
    {
        struct comp
        {
            vec4fp p;
            vec2fp t;
            vec4fp n;
        };

        comp v[3];
        char sym = '?';

        triangle() {}
        triangle(vec4fp p1, vec4fp p2, vec4fp p3)
        {
            v[0].p = p1;
            v[1].p = p2;
            v[2].p = p3;
        }
    };

    vec4fp position;
    vec4fp rotation;
    vec4fp scale{1, 1, 1};
    std::vector<triangle> tris;

    bool load_from_obj(const char *file_name, size_t char_min = 33, size_t char_max = 120)
    {
        using namespace std;
        ifstream f(file_name);
        if (!f.is_open())
            return false;
        vector<vec4fp> verts;
        vector<vec2fp> texs;
        vector<vec4fp> norms;
        bool has_tex = false;
        bool has_norm = false;
        size_t char_select = 0;
        size_t char_counter = 0;
        while (!f.eof())
        {
            char line[300];
            char junk;
            f.getline(line, 300);
            stringstream s;
            s << line;
            if (line[0] == 'v')
            {
                if (line[1] == 't')
                {
                    vec2fp v;
                    s >> junk >> junk >> v.x() >> v.y();
                    texs.push_back(v);
                }
                else if (line[1] == 'n')
                {
                    vec4fp v;
                    s >> junk >> junk >> v.x() >> v.y() >> v.z();
                    norms.push_back(v);
                    has_norm = true;
                }
                else
                {
                    vec4fp v;
                    s >> junk >> v.x() >> v.y() >> v.z();
                    verts.push_back(v);
                    has_tex = true;
                }
            }
            if (line[0] == 'f')
            {
                int face[3] = {0};
                int text[3] = {0};
                int norml[3] = {0};
                if (!has_tex && !has_norm)
                    s >> junk >> face[0] >> face[1] >> face[2];
                else if (!has_norm)
                    s >> junk >>
                        face[0] >> junk >> text[0] >>
                        face[1] >> junk >> text[1] >>
                        face[2] >> junk >> text[2];
                else if (has_norm && has_tex)
                    s >> junk >>
                        face[0] >> junk >> text[0] >> junk >> norml[0] >>
                        face[1] >> junk >> text[1] >> junk >> norml[1] >>
                        face[2] >> junk >> text[2] >> junk >> norml[2];
                triangle fTri;
                for (size_s j = 0; j < 3; ++j)
                    fTri.v[j].p = verts[face[j] - 1];
                if (has_tex)
                    for (size_s j = 0; j < 3; ++j)
                        fTri.v[j].t = texs[text[j] - 1];
                if (has_norm)
                    for (size_s j = 0; j < 3; ++j)
                        fTri.v[j].n = norms[norml[j] - 1];
                char symbol = (char_min + (char)++char_select) - (char_counter * (char_max - char_min));
                fTri.sym = symbol;
                if (symbol == char_max)
                    ++char_counter;
                tris.push_back(fTri);
            }
        }
        return tris.size() > 0;
    }
};

template <int WIDTH_, int HEIGHT_>
struct console_render_target
{
    union
    {
        char map[HEIGHT_][WIDTH_ + 1] = {0};
        char map_arr[HEIGHT_ * (WIDTH_ + 1)];
    };

    union
    {
        fp_num depth_buff[HEIGHT_][WIDTH_] = {0};
        fp_num depth_buff_arr[HEIGHT_ * WIDTH_];
    };

    fp_num depth_buff_clear = 0;
    char clear_char = ' ';

#if RENDER == RENDER_WIN
    text_renderer renderer = text_renderer(WIDTH_ + 2, HEIGHT_ + 2);
#endif

    console_render_target()
    {
        clear();
#if RENDER == RENDER_WIN
        if (!renderer.init("Console 3D - Callum Mackenzie", R_PATH("../../global/fonts/JetBrainsMono.ttf").c_str()))
            exit(-1);
#endif
    }

    void clear()
    {
        memset(map, clear_char, sizeof(map));
        for (size_t i = 0; i < WIDTH_ * HEIGHT_; ++i)
            depth_buff_arr[i] = depth_buff_clear;
        // memset(depth_buff, depth_buff_clear, sizeof(depth_buff));
        map_arr[(HEIGHT_ * (WIDTH_ + 1)) - 1] = '\0';
        for (size_t i = 0; i < HEIGHT_ * (WIDTH_ + 1); i += (WIDTH_ + 1))
            map_arr[i] = '\n';
    }

    void render_mesh_3d(mesh_3d &mesh, camera_3d &camera)
    {
        auto projection_m = camera.perspective();
        auto rotation_m = mat4fp::rotation(mesh.rotation);
        auto transform_m = mat4fp::scale(mesh.scale) *
                           rotation_m *
                           mat4fp::translation(mesh.position);
        auto clv = camera.look_vector();
        auto cam_view_m = camera.view().inverse();
        auto render_tri_3d = [&clv, &cam_view_m, &projection_m, &rotation_m, &transform_m, this](mesh_3d::triangle &tri)
        {
            triangle_2d rast_tri;
            for (size_s j = 0; j < 3; ++j)
            {
                fp_num normal_view_dot = (rotation_m * tri.v[j].n).dot(clv);
#if defined(CULLING) && defined(CULL_BACK) && defined(CULL_FRONT)
#if CULLING == CULL_BACK
                if (normal_view_dot > 0)
                    return;
#elif CULLING == CULL_FRONT
                if (normal_view_dot < 0)
                    return;
#endif
#endif
                rast_tri.facing_view = normal_view_dot < 0;
                vec4fp v_pos(projection_m * (cam_view_m * (transform_m * tri.v[j].p)));
                rast_tri.v[j].v.x() = v_pos.x() / v_pos.w() + 0.5;
                rast_tri.v[j].v.y() = v_pos.y() / v_pos.w() + 0.5;
                rast_tri.v[j].depth = v_pos.z() / v_pos.w();
                rast_tri.v[j].sym = tri.sym;
            }
            winding wind = rast_tri.get_wind();
            auto in_tri = [&rast_tri, &wind](vec2fp &p)
            {
                bool inside = true;
                if (wind == winding::clockwise)
                {
                    inside &= console_render_target::edge_fn(rast_tri.v[0].v, rast_tri.v[2].v, p);
                    inside &= console_render_target::edge_fn(rast_tri.v[1].v, rast_tri.v[0].v, p);
                    inside &= console_render_target::edge_fn(rast_tri.v[2].v, rast_tri.v[1].v, p);
                }
                else
                {
                    inside &= console_render_target::edge_fn(rast_tri.v[0].v, rast_tri.v[1].v, p);
                    inside &= console_render_target::edge_fn(rast_tri.v[1].v, rast_tri.v[2].v, p);
                    inside &= console_render_target::edge_fn(rast_tri.v[2].v, rast_tri.v[0].v, p);
                }
                return inside;
            };
            for (size_s i = 0; i < 3; ++i)
                rast_tri.v[i].v *= vec2fp(WIDTH_, HEIGHT_);
            triangle_2d::bounding_box box;
            rast_tri.get_bounding_box(box);
            if (box.top < 0)
                box.top = 0;
            if (box.left < 0)
                box.left = 0;
            if (box.right >= WIDTH_)
                box.right = WIDTH_ - 1;
            if (box.bottom >= HEIGHT_)
                box.bottom = HEIGHT_ - 1;
            for (size_t y = (size_t)box.top; y <= box.bottom; ++y)
                for (size_t x = (size_t)box.left; x <= box.right; ++x)
                {
                    vec2fp p((fp_num)x, (fp_num)y);
                    if (in_tri(p))
                    {
                        vec2fp fragCoord = p.normalized();
                        fp_num d0 = (fragCoord - rast_tri.v[0].v.normalized()).len();
                        fp_num d1 = (fragCoord - rast_tri.v[1].v.normalized()).len();
                        fp_num d2 = (fragCoord - rast_tri.v[2].v.normalized()).len();
                        fp_num frag_depth = (d0 * rast_tri.v[0].depth) +
                                            (d1 * rast_tri.v[1].depth) +
                                            (d2 * rast_tri.v[2].depth);
                        if (frag_depth >= depth_buff[y][x])
                        {
                            map[y][x] = rast_tri.v[0].sym;
                            depth_buff[y][x] = frag_depth;
                        }
                    }
                }
        };
        for (size_t i = 0; i < mesh.tris.size(); ++i)
            render_tri_3d(mesh.tris[i]);
    }

    void print()
    {
#if RENDER == RENDER_WIN
        renderer.print(map_arr);
#elif RENDER == RENDER_TERM
        printf("%s", map_arr);
#endif
    }

    void home_cursor()
    {
#if RENDER == RENDER_TERM
#ifdef _WIDTH_IN32
        std::system("cls");
#else
        printf("\x1b[HEIGHT_");
#endif
#elif RENDER == RENDER_WIN
        renderer.home_cursor();
#endif
    }

    void clear_screen()
    {
#if RENDER == RENDER_TERM
#ifdef _WIDTH_IN32
        std::system("cls");
#else
        printf("\x1b[2J");
#endif
#elif RENDER == RENDER_WIN
        renderer.clear();
#endif
    }

    static inline bool edge_fn(const vec2fp &a, const vec2fp &b, const vec2fp &c)
    {
        return ((c.x() - a.x()) * (b.y() - a.y()) - (c.y() - a.y()) * (b.x() - a.x()) >= 0);
    }
};

struct main_loop
{
    bool running = false;
    fp_num target_fps = 30;
    fp_num delta_time = 0;
    fp_num target_delta = 1.0 / target_fps;
    void start(int argc, char **argv)
    {
        running = on_start(argc, argv);
        if (!running)
            printf("Terminated\n");
        clock_t last_frame = clock();
        fp_num check_delta = 0;
        recheck_fps();
        while (running)
        {
            check_delta = ((fp_num)(clock() - last_frame) / (fp_num)CLOCKS_PER_SEC);
            if (check_delta >= target_delta)
            {
                last_frame = clock();
                delta_time = check_delta;
                on_update();
#if RENDER == RENDER_WIN
                render_target.renderer.window.clear();
                render_target.renderer.render_screen();
                render_target.renderer.window.swap_buffers();
                if (render_target.renderer.window.should_close())
                    running = false;
#endif
            }
#if RENDER == RENDER_WIN
            render_target.renderer.window.poll_events();
#endif
        }
    }
    void recheck_fps()
    {
        target_delta = 1.0 / target_fps;
    }

    console_render_target<200, 50> render_target;
    camera_3d camera{95, 0.1, 100, 0.5};
    mesh_3d cube;

    bool on_start(int argc, char **argv)
    {
#if RENDER == RENDER_WIN
        render_target.renderer.font_size = 0.2;
        render_target.renderer.margin = vec2fp(1, 1);
#endif
        if (argc <= 1)
        {
            if (!cube.load_from_obj(R_PATH("../../global/sphere.obj").c_str()))
                return false;
        }
        else if (!cube.load_from_obj(argv[1]))
            return false;
        render_target.clear_screen();
        return true;
    }
    void on_update()
    {
        render_target.home_cursor();
        cube.rotation += vec4fp(0.8, 1, 0.4) * delta_time;
        cube.position = vec4fp(0, 0, 2.5) + vec4fp(0, 0, sinf((float)clock() / (float)CLOCKS_PER_SEC));
        render_target.render_mesh_3d(cube, camera);
        render_target.print();
        render_target.clear();
        printf("\nFPS:%f", 1.0 / delta_time);
    }
};

int main(int argc, char **argv)
{
    {
        main_loop().start(argc, argv);
    }
    printf("\n");
    PRINT_MEMORY_SUMMARY;
}
