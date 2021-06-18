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
			T data;

			void invert()
			{
				branch *tmp_left = left;
				branch *tmp_right = right;
				left = tmp_right;
				right = tmp_left;
			}
		};

		branch *root;

		void invert()
		{
			invert_branch_recursive(root);
		}

		void invert_branch_recursive(branch *branch)
		{
			if (!branch)
				return;
			branch->invert();
			if (branch->left)
				invert_branch_recursive(branch->left);
			if (branch->right)
				invert_branch_recursive(branch->right);
		}
	};
};

#endif