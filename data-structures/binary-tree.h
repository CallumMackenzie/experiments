#ifndef BINARY_TREE_H
#define BINARY_TREE_H 1

namespace dst
{
	template <typename T>
	struct binary_tree
	{
		struct branch
		{
			branch *left = nullptr;
			branch *right = nullptr;
			branch *parent = nullptr;
			T data;

			branch(T data)
			{
				this->data = data;
			}

			branch(T data, branch *parent) : branch(data)
			{
				this->parent = parent;
			}

			void set_left(T data)
			{
				left = new branch(data, this);
			}

			void set_right(T data)
			{
				right = new branch(data, this);
			}

			void invert()
			{
				branch *tmp_left = left;
				branch *tmp_right = right;
				left = tmp_right;
				right = tmp_left;
			}
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
			}
		};

		branch *root = nullptr;

		binary_tree()
		{
		}

		void invert()
		{
			invert_branch_recursive(root);
		}

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

		void clear()
		{
			if (root)
			{
				root->clear_recursive();
				delete root;
				root = nullptr;
			}
		}

		void set_root(T data)
		{
			root = new branch(data);
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
			branch_size += size(start->left);
			branch_size += size(start->right);
			return branch_size;
		}
	};
};

#endif