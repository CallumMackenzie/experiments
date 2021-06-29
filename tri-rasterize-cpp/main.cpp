#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "math.h"

using namespace cmm;

#define WINDING_CLOCKWISE 122
#define WINDING_COUNTER_CLOCKWISE 233
#define WINDING WINDING_COUNTER_CLOCKWISE

struct camera_3d
{
    double fov = 80;
    double near = 0.1;
    double far = 100;
    double aspect = 1;
    lvec3<double> rotation;
    lvec3<double> position;

    mat<double, 4, 4> view()
    {
        lv4<double> up{0, 1, 0};
        lv4<double> target{0, 0, 1, 1};
        lv4<double> cam_rot(MFNS<double, 4, 4>::rotation(rotation) * target);
        target = cam_rot + position.xyz1();
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
        for (int i = 0; i < 3; i++)
        {
            if (v[i].v[1] < box.top || i == 0)
                box.top = v[i].v[1];
            if (v[i].v[1] > box.bottom)
                box.bottom = v[i].v[1];
            if (v[i].v[0] < box.left || i == 0)
                box.left = v[i].v[0];
            if (v[i].v[0] > box.right)
                box.right = v[i].v[0];
        }
    }
};

template <int W, int H>
struct console_render_target
{
    union
    {
        char map[H][W];
        char map_arr[H * W];
    };

    double depth_buff[H][W] = {0};

    console_render_target()
    {
        clear();
    }

    inline char &point_at(double x, double y)
    {
        return map[(int)(y * (double)H)][(int)(x * (double)W)];
    }

    void clear()
    {
        memset(map, ' ', sizeof(map));
        memset(depth_buff, -0.001, sizeof(depth_buff));
    }

    void print() const
    {
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
                std::cout << map[i][j];
            std::cout << std::endl;
        }
    }

    void rasterize(triangle_2d &tri_in)
    {
        triangle_2d tri_out;
        auto edge_fn = [](const nvec<int, 2> &a, const nvec<int, 2> &b, const nvec<int, 2> &c)
        {
            int val = (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
            return (val >= 0);
        };
        auto in_tri = [&tri_out, &edge_fn](nvec<int, 2> p)
        {
            bool inside = true;
#if WINDING == WINDING_CLOCKWISE
            inside &= edge_fn(tri_out.v[0].v, tri_out.v[2].v, p);
            inside &= edge_fn(tri_out.v[1].v, tri_out.v[0].v, p);
            inside &= edge_fn(tri_out.v[2].v, tri_out.v[1].v, p);
#else
            inside &= edge_fn(tri_out.v[0].v, tri_out.v[1].v, p);
            inside &= edge_fn(tri_out.v[1].v, tri_out.v[2].v, p);
            inside &= edge_fn(tri_out.v[2].v, tri_out.v[0].v, p);
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
        for (int y = box.top; y <= box.bottom; ++y)
            for (int x = box.left; x <= box.right; ++x)
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
    };

    std::vector<triangle> tris;
    lvec3<double> scale{1, 1, 1};
    lvec3<double> position;
    lvec3<double> rotation;

    template <size_t W, size_t H>
    void render(console_render_target<W, H> &target, camera_3d &camera)
    {
        size_t n_tris = tris.size();
        auto projection_m = camera.perspective();
        auto translation_m = MFNS<double, 4, 4>::translation(position);
        auto rotation_m = MFNS<double, 4, 4>::rotation(rotation);
        auto scale_m = MFNS<double, 4, 4>::scale(scale);
        auto transform = scale_m * rotation_m * translation_m;
        auto cam_view = camera.view().inverse();
        for (size_t i = 0; i < n_tris; ++i)
        {
            triangle_2d rast_tri;
            mesh_3d::triangle &in_tri = tris[i];
            bool draw = true;
            for (size_t j = 0; j < 3; ++j)
            {
                lv4<double> v_pos(projection_m * (cam_view * (transform * in_tri.v[j].p.xyz1())));
                if (v_pos.w() == 0)
                {
                    draw = false;
                    continue;
                }
                v_pos /= v_pos.w();
                rast_tri.v[j].v = v_pos.xy().to_nvec();
                rast_tri.v[j].depth = v_pos.z();
                rast_tri.v[j].sym = in_tri.sym;
            }
            if (!draw)
                continue;
            target.rasterize(rast_tri);
        }
    }

    bool load_from_obj(std::string file_name)
    {
        using namespace std;
        ifstream f(file_name);
        if (!f.is_open())
            return false;

        vector<lvec3<double>> verts;
        vector<lvec2<double>> texs;
        bool has_tex = false;
        bool has_norm = false;

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
                {
                    has_norm = true;
                }
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
                tris.push_back(fTri);
            }
        }
        return true;
    }
};

#define MAP_SZ <140, 70>

int main(int, char **)
{
    std::cout << "Program begin." << std::endl;
    console_render_target MAP_SZ map;
    camera_3d camera;
    camera.position = {-2, -1.5, -5};

    mesh_3d mesh;
    mesh.position = {0, 0, 0};
    // mesh.scale = {1.2, 1.2, 1.2};
    if (!mesh.load_from_obj("../cube.obj"))
    {
        std::cerr << "Could not load OBJ." << std::endl;
        return 1;
    }
    int ctr = 0;
    const int max_char = 122;
    const int min_char = 33;
    for (int i = 0; i < mesh.tris.size(); i++)
    {
        char symbol = (min_char + i) - (ctr * (max_char - min_char));
        mesh.tris[i].sym = symbol;
        if (symbol == max_char)
            ++ctr;
    }

    printf("\x1b[2J");
    for (;;)
    {
        printf("\x1b[H");
        mesh.rotation += {0.007, 0.02, 0.01};
        // lvec3<double> forward;
        // lvec3<double> up{0, 1, 0};
        // lvec3<double> rotate;
        // nvec<double, 4> clv_nv = camera.look_vector();
        // lvec3<double> clv{clv_nv[0], clv_nv[1], clv_nv[2]};
        // switch (to_move)
        // {
        // case 'w':
        //     forward += clv;
        //     break;
        // case 's':
        //     forward -= clv;
        //     break;
        // case 'a':
        //     forward += clv.cross(up);
        //     break;
        // case 'd':
        //     forward -= clv.cross(up);
        //     break;
        // case 'q':
        //     forward += {0, 1, 0};
        //     break;
        // case 'e':
        //     forward -= {0, 1, 0};
        //     break;
        // case 'l':
        //     rotate += {0, 1, 0};
        //     break;
        // case 'j':
        //     rotate -= {0, 1, 0};
        //     break;
        // case 'i':
        //     rotate += {1, 0, 0};
        //     break;
        // case 'k':
        //     rotate -= {1, 0, 0};
        //     break;
        // default:
        //     break;
        // }
        // camera.position += forward.normalize() * amount;
        // camera.rotation += rotate.normalize() * amount;

        mesh.render MAP_SZ(map, camera);
        map.print();
        map.clear();
    }
}
