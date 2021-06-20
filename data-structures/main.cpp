#include <iostream>
#include <vector>

#include "memory-aid.h"
#include "linked-list.h"
#include "doubly-linked-list.h"
#include "array.h"
#include "binary-tree.h"
#include "tree.h"
#include <time.h>

// #define DST_LK_LIST
// #define DST_DLK_LIST
// #define DST_ARRAY
#define STD_VECTOR
// #define DST_BIN_TREE
// #define DST_TREE

COUNT_MEMORY

#define TEST       \
    start_clock(); \
    for (size_t i = 0; i < 10000; i++)

#define RES(str) \
    ;            \
    results(str)

#define LIST                                                                          \
    {                                                                                 \
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 \
    }

int main(int argc, char **argv)
{
    clock_t clk;

    auto header = [&clk](std::string txt)
    {
        std::cout << std::endl
                  << txt << std::endl;
        clk = clock();
    };

    auto start_clock = [&clk]()
    {
        clk = clock();
    };

    auto results = [&clk](std::string foot)
    {
        double diff = (((double)(clock() - clk)) / ((double)CLOCKS_PER_SEC)) * ((double)1000);
        std::cout << "   " << foot << " x10,000 done in " << diff << "ms" << std::endl;
    };

    {
#ifdef DST_LK_LIST
        {
            header("DST::LK_LIST");
            auto lkList = dst::lk_list<char> LIST;
            lkList.clear_on_free = true;
            TEST lkList[4] RES("operator[4]");
            TEST lkList.push_back('a') RES("push_back");
            TEST lkList.push_front((char)(i + 2)) RES("push_front");
            TEST lkList.pop_front() RES("pop_front");
        }
#endif

#ifdef DST_DLK_LIST
        {
            header("DST::DLK_LIST");
            auto dlkList = dst::dlk_list<int> LIST;
            dlkList.clear_on_free = true;
            TEST dlkList[4] RES("operator[4]");
            TEST dlkList.push_back('a') RES("push_back");
            TEST dlkList.push_front((char)(i + 2)) RES("push_front");
            TEST dlkList.pop_back() RES("pop_back");
            TEST dlkList.pop_front() RES("pop_front");
            TEST dlkList.insert_after(2, '2') RES("insert_after");
            TEST dlkList.insert_before(1, 'b') RES("insert_before");
            TEST dlkList.pop(9) RES("pop");
        }
#endif

#ifdef DST_ARRAY
        {
            header("DST::ARRAY");
            auto arr = dst::array<int> LIST;
            TEST arr[4] RES("operator[4]");
        }
#endif

#ifdef STD_VECTOR
        {
            header("STD::VECTOR");
            auto vec = std::vector<int> LIST;
            TEST vec[4] RES("operator[4]");
            TEST vec.push_back(10) RES("push_back");
            TEST vec.pop_back() RES("pop_back");
        }
#endif

#ifdef DST_BIN_TREE
        header("DST::BINARY_TREE");
        auto btree = dst::binary_tree<int>();
        btree.set_root(1);
        for (size_t i = 0; i < 100; i++)
        {
            btree.root->set_left(2);
            btree.root->left->set_right(3);
            btree.root->left->right->set_right(4);
            btree.root->set_right(5);
            btree.root->left->set_left(6);
            btree.root->right->set_right(7);
            btree.root->right->set_left(8);
            btree.root->right->left->set_left(9);
            btree.root->right->right->set_left(10);
            delete btree.root->right->pop()->delete_recursive();
            btree.invert();
        }
        std::cout << "length: " << btree.size() << std::endl;
        btree.clear();
        footer("1,200 operations done in ");
#endif

#ifdef DST_TREE
        header("DST::TREE");
        auto tree = dst::tree<int>();
        tree.set_root(1);
        tree.add(2);
        tree.add(3);
        tree.add(4);
        tree.get(0)->add(5);
        tree.get(0)->add(6);
        tree.get(1)->add(7);
        tree.get(2)->add(8);
        tree.get(1)->add(9);
        tree.get(1)->get(0)->add(10);
        tree.get(2)->get(0)->add(11);
        delete tree.get(2)->pop()->delete_recursive();
        std::cout << "length: " << tree.size() << std::endl;
        tree.clear();
        footer();
#endif
    }

    std::cout << std::endl;
    PRINT_MEMORY_SUMMARY
    return 0;
}