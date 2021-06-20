#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H 1

#include <vector>
#include <initializer_list>

#ifdef DLK_LIST_TO_STR
#include <sstream>
#include <string>
#endif

namespace dst
{
	// A doubly linked list with a number of pop & push methods and an iterator.
	template <typename T>
	struct doubly_linked_list
	{
		// A doubly linked list node.
		struct node
		{
			node *next = nullptr; // The next node in the list.
			node *prev = nullptr; // The previous node in the list.
			T data;				  // The data held in the node.
		};

		node *head = nullptr;		// The front node in the list.
		node *tail = nullptr;		// The back node in the list.
		bool clear_on_free = false; // Whether to delete nodes when the list is freed.

		// Creates a new doubly linked list with default values.
		doubly_linked_list<T>()
		{
		}

		// Creates a new doubly linked list and populates it with the contents of the input vector.
		doubly_linked_list<T>(std::vector<T> init)
		{
			for (auto i : init)
				push_back(i);
		}

		// Creates a new doubly linked list from an initializer list.
		doubly_linked_list<T>(std::initializer_list<T> list)
		{
			for (auto i : list)
				push_back(i);
		}

		~doubly_linked_list<T>()
		{
			if (clear_on_free)
				clear();
		}

		// Frees all nodes in the list.
		void clear()
		{
			while (node *node = pop_back_node())
				if (node)
					delete node;
		}

		// Does a full check of the list size and returns it.
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

		// Returns the size of the list.
		size_t size()
		{
			return length;
		}

		// Pops the front node off, frees it, and returns the data.
		T pop_front()
		{
			auto node = pop_front_node();
			if (node)
			{
				T data = node->data;
				delete node;
				return data;
			}
			return NULL;
		}

		// Pops the back node off, frees it, and returns the data.
		T pop_back()
		{
			auto node = pop_back_node();
			if (node)
			{
				T data = node->data;
				delete node;
				return data;
			}
			return NULL;
		}

		// Pops the node off at the given index, frees it, and returns the data.
		T pop(size_t index)
		{
			auto node = pop_node(index);
			if (node)
			{
				T data = node->data;
				delete node;
				return data;
			}
			return NULL;
		}

		// Pops the front node off the list and returns it.
		node *pop_front_node()
		{
			if (!head && !tail)
				return nullptr;
			length--;
			if (head == tail)
			{
				node *popped = head;
				head = nullptr;
				tail = nullptr;
				return popped;
			}
			node *popped = head;
			popped->next->prev = nullptr;
			head = popped->next;
			popped->next = nullptr;
			return popped;
		}

		// Pops the back node off the list and returns it.
		node *pop_back_node()
		{
			if (!head && !tail)
				return nullptr;
			length--;
			if (head == tail)
			{
				node *popped = head;
				head = nullptr;
				tail = nullptr;
				return popped;
			}
			node *popped = tail;
			popped->prev->next = nullptr;
			tail = popped->prev;
			popped->prev = nullptr;
			return popped;
		}

		// Pops the node at the given index off the list and returns it.
		node *pop_node(const size_t index)
		{
			if (!head && !tail)
				return nullptr;
			node *popped = (*this)[index];
			if (!popped)
				return nullptr;
			if (head == tail)
			{
				head = nullptr;
				tail = nullptr;
				length--;
				return popped;
			}
			if (popped == head)
				return pop_front_node();
			if (popped == tail)
				return pop_back_node();
			if (popped->prev)
				popped->prev->next = popped->next;
			if (popped->next)
				popped->next->prev = popped->prev;
			popped->next = nullptr;
			popped->prev = nullptr;
			length--;
			return popped;
		}

		// Creates a new node with the given data and pushes it onto the back of the list.
		void push_back(T data)
		{
			push_back_node(new node{nullptr, nullptr, data});
		}

		// Creates a new node with the given data and pushes it onto the front of the list.
		void push_front(T data)
		{
			push_front_node(new node{nullptr, nullptr, data});
		}

		// Creates a new node with the given data and inserts it after the node at the given index.
		void insert_after(const size_t index, T data)
		{
			insert_after_node(index, new node{nullptr, nullptr, data});
		}

		// Creates a new node with the given data and inserts it before the node at the given index.
		void insert_before(const size_t index, T data)
		{
			insert_before_node(index, new node{nullptr, nullptr, data});
		}

		// Returns the data held in the front node.
		T front()
		{
			return head->data;
		}

		// Returns the data held in the back node.
		T back()
		{
			return tail->data;
		}

		// Pushes the given node onto the back of the list.
		void push_back_node(node *node)
		{
			if (!head && !tail)
				head = node;
			else if (head == tail)
			{
				head->next = node;
				node->prev = head;
			}
			else
			{
				node->prev = tail;
				tail->next = node;
			}
			tail = node;
			length++;
		}

		// Pushes the given node onto the front of the list.
		void push_front_node(node *node)
		{
			if (!head && !tail)
				tail = node;
			else if (head == tail)
			{
				tail->prev = node;
				node->next = tail;
			}
			else
			{
				node->next = head;
				head->prev = node;
			}
			head = node;
			length++;
		}

		// Inserts the given node after the node at the given index.
		void insert_after_node(const size_t index, node *insert)
		{
			node *target = (*this)[index];
			if (!target)
				return;
			if (target == tail)
				return push_back_node(insert);
			target->next->prev = insert;
			insert->next = target->next;
			target->next = insert;
			insert->prev = target;
			length++;
		}

		// Inserts the given node before the node at the given index.
		void insert_before_node(const size_t index, node *insert)
		{
			node *target = (*this)[index];
			if (!target)
				return;
			if (target == head)
				return push_front_node(insert);
			target->prev->next = insert;
			insert->prev = target->prev;
			insert->next = target;
			target->prev = insert;
			length++;
		}

#ifdef DLK_LIST_TO_STR
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

		// Returns the node at the given index in the list.
		node *operator[](const size_t index)
		{
			if (index >= length || index < 0)
				return nullptr;
			node *node = nullptr;
			if (index < length << 1)
			{
				node = head;
				for (int i = 0; i < index; i++)
					node = node->next;
			}
			else
			{
				node = tail;
				for (size_t i = length; i > 0; i--)
					node = node->prev;
			}
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
			iterator &operator--()
			{
				m_ptr = m_ptr->prev;
				return *this;
			}
			iterator operator--(int)
			{
				iterator tmp = *this;
				--(*this);
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

		// An iterator with the start of the list.
		iterator begin()
		{
			return iterator(head);
		}

		// An iterator with the end of the list (nullptr);
		iterator end()
		{
			return iterator(nullptr);
		}

	private:
		size_t length = 0;
	};

	template <typename T>
	using dlk_list = doubly_linked_list<T>;
}

#endif