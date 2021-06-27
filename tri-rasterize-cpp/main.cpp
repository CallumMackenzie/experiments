#include <iostream>
#include "math.h"
#include <cstring>

using namespace cmm;

struct tri2d
{
    struct bounding_box
    {
        double top, left, right, bottom = 0;
    };

    nvec<double, 2> v[3];

    void get_bounding_box(bounding_box &box)
    {
        for (int i = 0; i < 3; i++)
        {
            if (v[i][1] < box.top || i == 0)
                box.top = v[i][1];
            if (v[i][1] > box.bottom)
                box.bottom = v[i][1];
            if (v[i][0] < box.left || i == 0)
                box.left = v[i][0];
            if (v[i][0] > box.right)
                box.right = v[i][0];
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

    map2d()
    {
        for (int i = 0; i < (H * W); i++)
            map_arr[i] = '.';
    }

    inline char &point_at(double x, double y)
    {
        return map[(int)(y * (double)H)][(int)(x * (double)W)];
    }

    void clear()
    {
        memset(map, '.', sizeof(map));
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

    void rasterize(tri2d &tri_in)
    {
        tri2d tri;
        auto edge_fn = [](const nvec<int, 2> &a, const nvec<int, 2> &b, const nvec<int, 2> &c)
        {
            int val = (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
            return (val >= 0);
        };
        auto in_tri = [&tri, &edge_fn](nvec<int, 2> p)
        {
            bool inside = true;
            inside &= edge_fn(tri.v[1], tri.v[0], p);
            inside &= edge_fn(tri.v[2], tri.v[1], p);
            inside &= edge_fn(tri.v[0], tri.v[2], p);
            return inside;
        };

        for (int i = 0; i < 3; ++i)
        {
            tri.v[i][0] = (int)(tri_in.v[i][0] * W);
            tri.v[i][1] = (int)(tri_in.v[i][1] * H);
        }
        tri2d::bounding_box box;
        tri.get_bounding_box(box);
        for (int y = box.top; y <= box.bottom; ++y)
            for (int x = box.left; x <= box.right; ++x)
            {
                nvec<int, 2> p{x, y};
                if (in_tri(p) && y >= 0 && x >= 0 && y < H && x < W)
                {
                    nvec<double, 2> fragCoord{(double)p[0] / (double)W,
                                              (double)p[1] / (double)H};
                    double dist1 = (fragCoord - tri_in.v[0]).len();
                    double dist2 = (fragCoord - tri_in.v[1]).len();
                    double dist3 = (fragCoord - tri_in.v[2]).len();
                    if (dist1 < dist2 && dist1 < dist3)
                        map[y][x] = '0';
                    else if (dist2 < dist1 && dist2 < dist3)
                        map[y][x] = '1';
                    else
                        map[y][x] = '2';
                }
            }
    }
};

int main(int, char **)
{
    std::cout << "Program begin." << std::endl;
    map2d<100, 35> map;
    tri2d tri = tri2d{
        nvec<double, 2>{0.0, 0.0},
        nvec<double, 2>{1.0, 1.0},
        nvec<double, 2>{0.0, 1.0}};
    tri2d tri2 = tri2d{
        nvec<double, 2>{0.0, 0.0},
        nvec<double, 2>{1.0, 0.0},
        nvec<double, 2>{1.0, 1.0}};
    auto rot = mat<double, 2, 2>::rotation_2d(0.005);
    auto rot_pt = nvec<double, 2>{0.5, 0.5};
    printf("\x1b[2J");
    for (;;)
    {
        printf("\x1b[H");
        for (int i = 0; i < 3; ++i)
            tri.v[i] = (rot * (tri.v[i] - rot_pt)) + rot_pt;
        for (int i = 0; i < 3; ++i)
            tri2.v[i] = (rot * (tri2.v[i] - rot_pt)) + rot_pt;
        map.rasterize(tri);
        map.rasterize(tri2);
        map.print();
        map.clear();
    }
}
