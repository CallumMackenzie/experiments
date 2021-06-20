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
			T data;							// The data the branch holds.
			branch *parent = nullptr;		// The parent branch.
			std::vector<branch *> children; // The children of this branch.

			// Creates a new branch with the given data.
			branch(T data)
			{
				this->data = data;
			}

			// Creates a branch with the given data and parent.
			branch(T data, branch *parent) : branch(data)
			{
				this->parent = parent;
			}

			// Adds a new child branch to this branch.
			void add(T data)
			{
				children.push_back(new branch(data, this));
			}

			// Frees all children.
			void clear()
			{
				for (size_t i = 0; i < children.size(); i++)
					if (children[i])
						delete children[i];
				children.clear();
				parent = nullptr;
			}

			// Frees all child branches and children thereof.
			void clear_recursive()
			{
				for (size_t i = 0; i < children.size(); i++)
					if (children[i])
					{
						children[i]->clear_recursive();
						delete children[i];
					}
			}

			// Frees all child branches and returns itself to be freed.
			branch *delete_recursive()
			{
				clear_recursive();
				return this;
			}

			// Returns a pointer to the child at the given index.
			branch *operator[](size_t index)
			{
				return children[index];
			}

			// Returns a pointer to the child at the given index.
			branch *get(size_t index)
			{
				return (*this)[index];
			}

			// Pops this branch off of the tree and returns it.
			branch *pop()
			{
				if (parent)
					for (size_t i = 0; i < parent->children.size(); i++)
						if (parent->children[i] == this)
						{
							size_t last_index = parent->children.size() - 1;
							parent->children[i] = parent->children[last_index];
							parent->children[last_index] = nullptr;
							parent->children.pop_back();
						}
				return this;
			}
		};

		branch *root = nullptr; // The first branch of the tree.

		// Sets the root to a new branch with the given data.
		void set_root(T data)
		{
			root = new branch(data);
		}

		// Frees all of the branches in the tree.
		void clear()
		{
			if (!root)
				return;
			root->clear_recursive();
			delete root;
			root = nullptr;
		}

		// Returns the child at the given index of the root branch.
		branch *operator[](size_t index)
		{
			if (!root)
				return nullptr;
			return (*root)[index];
		}

		// Adds a new child branch with the given data to the root.
		void add(T data)
		{
			root->add(data);
		}

		// Returns the child at the given index from the root.
		branch *get(size_t index)
		{
			return (*this)[index];
		}

		// Returns the number of branches in the tree.
		size_t size()
		{
			return branch_size(root);
		}

		// Returns the number of branches in the given branch.
		static size_t branch_size(branch *start)
		{
			if (!start)
				return 0;
			size_t size = 1;
			for (auto child : start->children)
				size += branch_size(child);
			return size;
		}
	};
}

#endif