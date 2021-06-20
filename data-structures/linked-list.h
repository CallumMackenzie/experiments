#ifndef LINKED_LIST_H
#define LINKED_LIST_H 1

#include <vector>

#ifdef LK_LIST_TO_STR
#include <sstream>
#include <string>
#endif

namespace dst
{
	// A linked list with push & pop methods and an iterator.
	template <typename T>
	struct linked_list
	{
		// A linked list node.
		struct node
		{
			node *next; // The next node in the list.
			T data;		// The data held in the node.
		};

		node *head = nullptr;		// The front node of the list.
		node *tail = nullptr;		// The back node of the list.
		bool clear_on_free = false; // Whether to delete nodes when the list is freed.

		// Creates a new linked list with default values.
		linked_list<T>()
		{
		}

		// Creates a new linked list and populates it with the value from the given vector.
		linked_list<T>(std::vector<T> data)
		{
			for (auto item : data)
				push_back(item);
		}

		~linked_list<T>()
		{
			if (clear_on_free)
				clear();
		}

		// Returns the number of nodes in the list after counting them.
		size_t size_check()
		{
			if (!head && !tail)
				return 0;
			size_t elems = 0;
			node *node = head;
			do
				elems++;
			while (node = node->next);
			return (length = elems);
		}

		// Returns the length of the list.
		size_t size()
		{
			return length;
		}

		// Creates a new node with the given data and pushes it onto the front of the list.
		void push_front(T data)
		{
			push_front_node(new node{nullptr, data});
		}

		// Creates a new node with the given data and pushes it onto the back of the list.
		void push_back(T data)
		{
			push_back_node(new node{nullptr, data});
		}

		// Pushes the given node onto the back of the list.
		void push_back_node(node *node)
		{
			if (!tail && !head)
				tail = head = node;
			else
			{
				tail->next = node;
				tail = node;
			}
			++length;
		}

		// Pushes the given node onto the front of the list.
		void push_front_node(node *node)
		{
			if (!tail && !head)
				tail = head = node;
			else
			{
				node->next = head;
				head = node;
			}
			++length;
		}

		// Frees the front node and returns its data.
		T pop_front()
		{
			auto node = pop_front_node();
			if (node)
			{
				T data = node->data;
				delete node;
				return data;
			}
			return (T)NULL;
		}

		// Frees the back node and returns its data.
		T pop_back()
		{
			auto node = pop_back_node();
			if (node)
			{
				T data = node->data;
				delete node;
				return data;
			}
			return (T)NULL;
		}

		// Pops the front node off the list and returns it.
		node *pop_front_node()
		{
			if (!head && !tail)
				return nullptr;
			--length;
			if (head == tail)
			{
				node *popped = head;
				head = nullptr;
				tail = nullptr;
				return popped;
			}
			node *popped = head;
			head = head->next;
			return popped;
		}

		// Pops the back node off the list and returns it.
		node *pop_back_node()
		{
			if (!head && !tail)
				return nullptr;
			--length;
			if (head == tail)
			{
				node *popped = head;
				head = nullptr;
				tail = nullptr;
				return popped;
			}
			node *beforeTail = head;
			do
				if (beforeTail->next == tail)
					break;
			while (beforeTail = beforeTail->next);
			node *popped = tail;
			beforeTail->next = nullptr;
			tail = beforeTail;
			return popped;
		}

#ifdef LK_LIST_TO_STR
		std::string to_string()
		{
			if (!head && !tail)
				return "";
			std::ostringstream oss;
			node *node = head;
			do
				oss << node->data << (node->next ? "->" : "");
			while (node = node->next);
			return oss.str();
		}
#endif

		// Frees all nodes in the list.
		void clear()
		{
			while (node *node = pop_front_node())
				if (node)
					delete node;
		}

		// Returns the node at the given index in the list.
		node *operator[](size_t index)
		{
			if (index >= length || index < 0)
				return nullptr;
			node *node = head;
			for (size_t i = 0; i < index; i++)
				node = node->next;
			return node;
		}

		struct iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = const node *;
			using pointer = value_type;
			using reference = value_type;

			iterator(pointer ptr) : m_ptr(ptr)
			{
			}

			reference operator*() const { return m_ptr; }
			pointer operator->() { return m_ptr; }
			iterator &operator++()
			{
				m_ptr = m_ptr->next;
				return *this;
			}
			iterator operator++(int)
			{
				iterator tmp = *this;
				++(*this);
				return tmp;
			}

			friend bool operator==(const iterator &a, const iterator &b)
			{
				return a.m_ptr == b.m_ptr;
			}
			friend bool operator!=(const iterator &a, const iterator &b)
			{
				return a.m_ptr != b.m_ptr;
			}

		private:
			pointer m_ptr;
		};

		// An iterator with the beginning of the list.
		iterator begin()
		{
			return iterator(head);
		}

		// An iterator with the end of the list (nullptr).
		iterator end()
		{
			return iterator(nullptr);
		}

	private:
		size_t length = 0;
	};

	template <typename T>
	using lk_list = linked_list<T>;
}

#endif