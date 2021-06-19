#ifndef DST_TREE_H
#define DST_TREE_H 1

#include <vector>

namespace dst
{
    template <typename T>
    struct tree
    {
        struct branch
        {
            T data;
            branch *parent = nullptr;
            std::vector<branch *> children;

            branch(T data)
            {
                this->data = data;
            }

            branch(T data, branch *parent) : branch(data)
            {
                this->parent = parent;
            }

            void add(T data)
            {
                children.push_back(new branch(data, this));
            }

            void clear()
            {
                for (size_t i = 0; i < children.size(); i++)
                    if (children[i])
                        delete children[i];
                children.clear();
                parent = nullptr;
            }

            void clear_recursive()
            {
                for (size_t i = 0; i < children.size(); i++)
                    if (children[i])
                    {
                        children[i]->clear_recursive();
                        delete children[i];
                    }
            }

            branch *operator[](size_t index)
            {
                return children[index];
            }

            branch *get(size_t index)
            {
                return (*this)[index];
            }
        };

        branch *root = nullptr;

        void set_root(T data)
        {
            root = new branch(data);
        }

        void clear()
        {
            if (!root)
                return;
            root->clear_recursive();
            delete root;
            root = nullptr;
        }

        branch *operator[](size_t index)
        {
            return (*root)[index];
        }

        void add(T data)
        {
            root->add(data);
        }

        branch *get(size_t index)
        {
            return (*this)[index];
        }

        size_t size()
        {
            return size(root);
        }

        static size_t size(branch *start)
        {
            if (!start)
                return 0;
            size_t branch_size = 1;
            for (auto child : start->children)
                branch_size += size(child);
            return branch_size;
        }
    };
}

#endif