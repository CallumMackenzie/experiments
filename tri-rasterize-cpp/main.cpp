#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <time.h>

#include "memory-aid.h"
#include "math.h"

COUNT_MEMORY

using namespace cmm;

#define WINDING_CLOCKWISE 0
#define WINDING_COUNTER_CLOCKWISE 1
#define WINDING WINDING_COUNTER_CLOCKWISE

struct camera_3d;
struct triangle_2d;
struct mesh_3d;

struct camera_3d
{
    double fov = 80;
    double near = 0.1;
    double far = 100;
    double aspect = 1;
    lvec3<double> rotation{0, 0, 0};
    lvec3<double> position{0, 0, 0};

    mat<double, 4, 4> view()
    {
        lv4<double> up{0, 1, 0, 1};
        lv4<double> target{0, 0, 1, 1};
        lv4<double> cam_rot(MFNS<double, 4, 4>::rotation(rotation) * target);
        target = cam_rot + position.xyz_n(0);
        target.w() = 1;
        auto ret = MFNS<double, 4, 4>::look_at(position.xyz(), target.xyz(), up.xyz());
        return ret;
    }

    mat<double, 4, 4> perspective()
    {
        return MFNS<double, 4, 4>::projection(fov, aspect, near, far);
    }

    nvec<double, 4> look_vector()
    {
        return MFNS<double, 4, 4>::rotation(rotation) * nvec<double, 4>{0, 0, 1, 1};
    }
};

struct triangle_2d
{
    struct bounding_box
    {
        double top, left, right, bottom = 0;
    };
    struct vert
    {
        nvec<double, 2> v;
        double depth = 0;
        lv2<double> t;
        char sym = ' ';
    };

    vert v[3];

    triangle_2d() {}
    triangle_2d(nvec<double, 2> v1, nvec<double, 2> v2, nvec<double, 2> v3)
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
};

struct mesh_3d
{
    struct triangle
    {
        struct comp
        {
            lvec3<double> p;
            lvec2<double> t;
        };

        comp v[3];
        char sym = '?';

        triangle() {}
        triangle(lv3<double> p1, lv3<double> p2, lv3<double> p3)
        {
            v[0].p = p1;
            v[1].p = p2;
            v[2].p = p3;
        }
    };

    lv3<double> position;
    lv3<double> rotation;
    lv3<double> scale{1, 1, 1};
    std::vector<triangle> tris;

    bool load_from_obj(const char *file_name, size_t char_min = 33, size_t char_max = 120)
    {
        using namespace std;
        ifstream f(file_name);
        if (!f.is_open())
            return false;
        vector<lvec3<double>> verts;
        vector<lvec2<double>> texs;
        bool has_tex = false;
        bool has_norm = false;
        size_t char_select = 0;
        size_t char_counter = 0;
        while (!f.eof())
        {
            char line[255];
            char junk;
            f.getline(line, 255);
            stringstream s;
            s << line;
            if (line[0] == 'v')
            {
                if (line[1] == 't')
                {
                    lvec2<double> v;
                    s >> junk >> junk >> v[0] >> v[1];
                    texs.push_back(v);
                }
                else if (line[1] == 'n')
                    has_norm = true;
                else
                {
                    lvec3<double> v;
                    s >> junk >> v[0] >> v[1] >> v[2];
                    verts.push_back(v);
                    has_tex = true;
                }
            }
            if (line[0] == 'f')
            {
                int face[3] = {0};
                int text[3] = {0};
                int ijunk = 0;
                if (!has_tex && !has_norm)
                    s >> junk >> face[0] >> face[1] >> face[2];
                else if (!has_norm)
                    s >> junk >>
                        face[0] >> junk >> text[0] >>
                        face[1] >> junk >> text[1] >>
                        face[2] >> junk >> text[2];
                else if (has_norm && has_tex)
                    s >> junk >>
                        face[0] >> junk >> text[0] >> junk >> ijunk >>
                        face[1] >> junk >> text[1] >> junk >> ijunk >>
                        face[2] >> junk >> text[2] >> junk >> ijunk;
                triangle fTri;
                for (int j = 0; j < 3; j++)
                    fTri.v[j].p = verts[face[j] - 1];
                if (has_tex)
                    for (int j = 0; j < 3; j++)
                        fTri.v[j].t = texs[text[j] - 1];
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

template <int W, int H>
struct console_render_target
{
    union
    {
        char map[H][W + 1] = {0};
        char map_arr[H * (W + 1)];
    };

    union
    {
        double depth_buff[H][W] = {0};
        double depth_buff_arr[H * W];
    };

    console_render_target()
    {
        clear();
    }

    void clear()
    {
        memset(map, ' ', sizeof(map));
        memset(depth_buff, 0, sizeof(depth_buff));
        map_arr[(H * (W + 1)) - 1] = '\0';
        for (size_t i = 0; i < H * (W + 1); i += (W + 1))
            map_arr[i] = '\n';
    }

    void render_mesh_3d(mesh_3d &mesh, camera_3d &camera)
    {
        auto projection_m = camera.perspective();
        auto transform_m = MFNS<double, 4, 4>::scale(mesh.scale) *
                           MFNS<double, 4, 4>::rotation(mesh.rotation) *
                           MFNS<double, 4, 4>::translation(mesh.position);
        auto cam_view_m = camera.view().inverse();
        for (size_t i = 0; i < mesh.tris.size(); ++i)
        {
            triangle_2d rast_tri;
            for (size_t j = 0; j < 3; ++j)
            {
                lv4<double> v_pos(projection_m * (cam_view_m * (transform_m * mesh.tris[i].v[j].p.xyz1())));
                v_pos /= v_pos.w();
                rast_tri.v[j].v = v_pos.xy() + 0.5;
                rast_tri.v[j].depth = v_pos.z();
                rast_tri.v[j].sym = mesh.tris[i].sym;
            }
            rasterize(rast_tri);
        }
    }

    void print() const
    {
        printf("%s", map_arr);
    }

    void home_cursor() const
    {
#ifdef _WIN32
        std::system("cls");
#else
        printf("\x1b[H");
#endif
    }

    void clear_screen() const
    {

#ifdef _WIN32
        std::system("cls");
#else
        printf("\x1b[2J");
#endif
    }

    static inline bool edge_fn(const nvec<int, 2> &a, const nvec<int, 2> &b, const nvec<int, 2> &c)
    {
        return ((c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]) >= 0);
    }

    void rasterize(triangle_2d &tri_in)
    {
        triangle_2d tri_out;
        auto in_tri = [&tri_out](nvec<int, 2> p)
        {
            bool inside = true;
#if WINDING == WINDING_CLOCKWISE
            inside &= console_render_target::edge_fn(tri_out.v[0].v, tri_out.v[2].v, p);
            inside &= console_render_target::edge_fn(tri_out.v[1].v, tri_out.v[0].v, p);
            inside &= console_render_target::edge_fn(tri_out.v[2].v, tri_out.v[1].v, p);
#else
            inside &= console_render_target::edge_fn(tri_out.v[0].v, tri_out.v[1].v, p);
            inside &= console_render_target::edge_fn(tri_out.v[1].v, tri_out.v[2].v, p);
            inside &= console_render_target::edge_fn(tri_out.v[2].v, tri_out.v[0].v, p);
#endif
            return inside;
        };
        for (int i = 0; i < 3; ++i)
        {
            tri_out.v[i].v[0] = (int)(tri_in.v[i].v[0] * W);
            tri_out.v[i].v[1] = (int)(tri_in.v[i].v[1] * H);
        }
        triangle_2d::bounding_box box;
        tri_out.get_bounding_box(box);
        if (box.top < 0)
            box.top = 0;
        if (box.left < 0)
            box.left = 0;
        if (box.right >= W)
            box.right = W - 1;
        if (box.bottom >= H)
            box.bottom = H - 1;
        for (int y = (int)box.top; y <= box.bottom; ++y)
            for (int x = (int)box.left; x <= box.right; ++x)
            {
                nvec<int, 2> p{x, y};
                if (in_tri(p))
                {
                    nvec<double, 2> fragCoord{(double)p[0] / (double)W,
                                              (double)p[1] / (double)H};
                    double d0 = (fragCoord - tri_in.v[0].v).len();
                    double d1 = (fragCoord - tri_in.v[1].v).len();
                    double d2 = (fragCoord - tri_in.v[2].v).len();
                    double frag_depth = (d0 * tri_in.v[0].depth) +
                                        (d1 * tri_in.v[1].depth) +
                                        (d2 * tri_in.v[2].depth);
                    if (frag_depth >= depth_buff[y][x])
                    {
                        map[y][x] = tri_in.v[0].sym;
                        depth_buff[y][x] = frag_depth;
                    }
                }
            }
    }
};

struct main_loop
{
    bool running = false;
    double target_fps = 30;
    double delta_time = 0;
    void start(int argc, char **argv)
    {
        running = on_start(argc, argv);
        if (!running)
            printf("Terminated\n");
        clock_t last_frame = clock();
        double check_delta = 0;
        while (running)
        {
            check_delta = ((double)(clock() - last_frame) / (double)CLOCKS_PER_SEC);
            if (check_delta >= (1.0 / target_fps))
            {
                last_frame = clock();
                delta_time = check_delta;
                on_update();
            }
        }
    }

    console_render_target<200, 50> render_target;
    camera_3d camera{95, 0.1, 100, 0.5};
    mesh_3d cube;

    bool on_start(int argc, char **argv)
    {
        printf("argc: %d\n", argc);
        if (argc <= 1)
        {
            if (!cube.load_from_obj("../../resources/cube.obj"))
                return false;
        }
        else if (!cube.load_from_obj(argv[1]))
            return false;
        cube.position = {0, 0, 4};
        render_target.clear_screen();
        return true;
    }
    void on_update()
    {
        render_target.home_cursor();
        cube.rotation += lv3<double>{0.8, 1, 0.4} * delta_time;
        render_target.render_mesh_3d(cube, camera);
        render_target.print();
        render_target.clear();
    }
};

int main(int argc, char **argv)
{
    {
        main_loop().start(argc, argv);
    }
    printf("\n");
    PRINT_MEMORY_SUMMARY
}
