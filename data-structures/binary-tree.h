#ifndef BINARY_TREE_H
#define BINARY_TREE_H 1

namespace dst
{
	template <typename T>
	struct binary_tree
	{
		struct branch
		{
			branch *left = nullptr;	  // The left branch.
			branch *right = nullptr;  // The right branch.
			branch *parent = nullptr; // The parent branch.
			T data;					  // The data the branch holds.

			// Creates a new branch with the input data and parent.
			branch(T data)
			{
				this->data = data;
			}

			// Creates a new branch with the input data and parent.
			branch(T data, branch *parent) : branch(data)
			{
				this->parent = parent;
			}

			// Creates a new branch from the data input, assigning it to the left.
			void set_left(T data)
			{
				left = new branch(data, this);
			}

			// Creates a new branch from the data input, assigning it to the right.
			void set_right(T data)
			{
				right = new branch(data, this);
			}

			// Switches this branch's left and right nodes.
			void invert()
			{
				branch *tmp_left = left;
				branch *tmp_right = right;
				left = tmp_right;
				right = tmp_left;
			};

			// Frees the two child branches of this branch.
			void clear()
			{
				if (left)
					delete left;
				if (right)
					delete right;
				left = nullptr;
				right = nullptr;
				parent = nullptr;
			}

			// Frees all child branches.
			void clear_recursive()
			{
				if (left)
				{
					left->clear_recursive();
					delete left;
				}
				if (right)
				{
					right->clear_recursive();
					delete right;
				}
			};

			// Frees all child branches and returns itself to be freed.
			branch *delete_recursive()
			{
				clear_recursive();
				return this;
			}

			// Pops this branch off of the tree and returns it.
			branch *pop()
			{
				if (parent)
				{
					if (this == parent->left)
						parent->left = nullptr;
					else if (this == parent->right)
						parent->right = nullptr;
				}
				return this;
			}
		};

		branch *root = nullptr; // The first branch of the tree.

		// Creates a new binary tree.
		binary_tree()
		{
		}

		// Inverts the binary tree.
		void invert()
		{
			invert_branch_recursive(root);
		}

		// Inverts a branch and all of its children.
		void invert_branch_recursive(branch *br)
		{
			if (!br)
				return;
			br->invert();
			if (br->left)
				invert_branch_recursive(br->left);
			if (br->right)
				invert_branch_recursive(br->right);
		}

		// Frees all branches in the tree.
		void clear()
		{
			if (root)
			{
				root->clear_recursive();
				delete root;
				root = nullptr;
			}
		}

		// Sets the root to a branch holding the given data.
		void set_root(T data)
		{
			root = new branch(data);
		}

		// Returns the number of branches in the tree.
		size_t size()
		{
			return size(root);
		}

		// Returns the number of branches in the given branch.
		static size_t size(branch *start)
		{
			if (!start)
				return 0;
			size_t branch_size = 1;
			branch_size += size(start->left);
			branch_size += size(start->right);
			return branch_size;
		}
	};
};

#endif