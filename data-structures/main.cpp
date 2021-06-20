#include <iostream>
#include <vector>

#include "memory-aid.h"
#include "linked-list.h"
#include "doubly-linked-list.h"
#include "array.h"
#include "binary-tree.h"
#include "tree.h"

COUNT_MEMORY

int main(int argc, char **argv)
{

    auto header = [](std::string txt)
    {
        std::cout << std::endl
                  << txt << std::endl;
    };

    {
        header("DST::LK_LIST");
        auto lkList = dst::lk_list<char>(std::vector<char>{'a', 'b', 'c', 'd'});
        for (auto i : lkList)
            std::cout << i->data << std::endl;
        std::cout << "2: " << lkList[2]->data << std::endl;
        lkList.clear();

        header("DST::DLK_LIST");
        auto dlkList = dst::dlk_list<char>(std::vector<char>{'w', 'x', 'y', 'z'});
        for (auto i : dlkList)
            std::cout << i->data << std::endl;
        std::cout << "2: " << dlkList[2]->data << std::endl;
        dlkList.clear();

        header("DST::ARRAY");
        auto arr = dst::array<int>(3, std::vector<int>{1, 2, 3, 4});
        for (auto i : arr)
            std::cout << i << std::endl;
        std::cout << "2: " << arr[2] << std::endl;

        header("DST::BINARY_TREE");
        auto btree = dst::binary_tree<int>();
        btree.set_root(1);
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
        std::cout << "length: " << btree.size() << std::endl;
        btree.clear();

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
    }

    PRINT_MEMORY_SUMMARY
    return 0;
}