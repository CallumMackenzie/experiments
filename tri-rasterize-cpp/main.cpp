#include <iostream>
#include "math.h"
#include <cstring>

using namespace cmm;

struct vert
{
    nvec<double, 2> v;
    double depth = 0; // Farthest
    char sym = ' ';
};

struct tri
{
    struct bounding_box
    {
        double top, left, right, bottom = 0;
    };

    vert v[3];

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
struct map2d
{
    union
    {
        char map[H][W];
        char map_arr[H * W];
    };

    double depth_buff[H][W] = {0};

    map2d()
    {
        for (int i = 0; i < (H * W); i++)
            map_arr[i] = ' ';
    }

    inline char &point_at(double x, double y)
    {
        return map[(int)(y * (double)H)][(int)(x * (double)W)];
    }

    void clear()
    {
        memset(map, ' ', sizeof(map));
        memset(depth_buff, 0.0, sizeof(depth_buff));
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

    void rasterize(tri &tri_in)
    {
        tri tri_out;
        auto edge_fn = [](const nvec<int, 2> &a, const nvec<int, 2> &b, const nvec<int, 2> &c)
        {
            int val = (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
            return (val >= 0);
        };
        auto in_tri = [&tri_out, &edge_fn](nvec<int, 2> p)
        {
            bool inside = true;
            inside &= edge_fn(tri_out.v[1].v, tri_out.v[0].v, p);
            inside &= edge_fn(tri_out.v[2].v, tri_out.v[1].v, p);
            inside &= edge_fn(tri_out.v[0].v, tri_out.v[2].v, p);
            return inside;
        };

        for (int i = 0; i < 3; ++i)
        {
            tri_out.v[i].v[0] = (int)(tri_in.v[i].v[0] * W);
            tri_out.v[i].v[1] = (int)(tri_in.v[i].v[1] * H);
        }

        tri::bounding_box box;
        tri_out.get_bounding_box(box);
        for (int y = box.top; y <= box.bottom; ++y)
            for (int x = box.left; x <= box.right; ++x)
            {
                nvec<int, 2> p{x, y};
                if (in_tri(p) && y >= 0 && x >= 0 && y < H && x < W)
                {
                    nvec<double, 2> fragCoord{(double)p[0] / (double)W,
                                              (double)p[1] / (double)H};
                    double d0 = (fragCoord - tri_in.v[0].v).len();
                    double d1 = (fragCoord - tri_in.v[1].v).len();
                    double d2 = (fragCoord - tri_in.v[2].v).len();

                    double coord_depth = (d0 * tri_in.v[0].depth) +
                                         (d1 * tri_in.v[1].depth) +
                                         (d2 * tri_in.v[2].depth);
                    if (coord_depth >= depth_buff[y][x])
                    {
                        map[y][x] = tri_in.v[0].sym;
                        depth_buff[y][x] = coord_depth;
                    }
                }
            }
    }
};

int main(int, char **)
{
    std::cout << "Program begin." << std::endl;
    map2d<100, 35> map;

    tri tri1;
    tri1.v[0].v = nvec<double, 2>{0.2, 0.2};
    tri1.v[1].v = nvec<double, 2>{0.8, 0.8};
    tri1.v[2].v = nvec<double, 2>{0.0, 0.8};
    tri1.set_sym('*');
    tri1.v[0].depth = 1;
    tri1.v[1].depth = 0;
    tri1.v[2].depth = 0.5;

    tri tri2;
    tri2.v[0].v = nvec<double, 2>{0.2, 0.2};
    tri2.v[1].v = nvec<double, 2>{0.8, 0.0};
    tri2.v[2].v = nvec<double, 2>{0.8, 0.8};
    tri2.set_sym('#');
    tri2.v[0].depth = 0.6;
    tri2.v[1].depth = 0.1;
    tri2.v[2].depth = 0.3;

    auto rot = mat<double, 2, 2>::rotation_2d(0.005);
    auto rot2 = mat<double, 2, 2>::rotation_2d(-0.004);
    auto rot_pt = nvec<double, 2>{0.5, 0.5};

    printf("\x1b[2J");
    for (;;)
    {
        printf("\x1b[H");
        for (int i = 0; i < 3; ++i)
            tri1.v[i].v = (rot * (tri1.v[i].v - rot_pt)) + rot_pt;
        for (int i = 0; i < 3; ++i)
            tri2.v[i].v = (rot2 * (tri2.v[i].v - rot_pt)) + rot_pt;
        map.rasterize(tri1);
        map.rasterize(tri2);
        map.print();
        map.clear();
    }
}
