#ifndef LINKED_LIST_H
#define LINKED_LIST_H 1

#include <vector>

#ifdef LK_LIST_TO_STR
#include <sstream>
#include <string>
#endif

namespace dst
{
	// A linked list class
	template <typename T>
	struct linked_list
	{
		struct node
		{
			node *next;
			T data;
		};

		node *head = nullptr;		// First node (start)
		node *tail = nullptr;		// Last node (end)
		bool clear_on_free = false; // whether to delete nodes when the list is freed

		linked_list<T>()
		{
		}

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

		size_t size_check()
		{
			if (!head && !tail)
				return 0;
			size_t elems = 0;
			node *node = head;
			do
				elems++;
			while (node = node->next);
			length = elems;
			return elems;
		}

		size_t size()
		{
			return length;
		}

		void push_front(T data)
		{
			push_front_node(new node{nullptr, data});
		}

		void push_back(T data)
		{
			push_back_node(new node{nullptr, data});
		}

		void push_back_node(node *node)
		{
			if (!tail && !head)
				tail = head = node;
			else
			{
				tail->next = node;
				tail = node;
			}
			length++;
		}

		void push_front_node(node *node)
		{
			if (!tail && !head)
				tail = head = node;
			else
			{
				node->next = head;
				head = node;
			}
			length++;
		}

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
			head = head->next;
			return popped;
		}

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

		void clear()
		{
			while (node *node = pop_front_node())
				if (node)
					delete node;
		}

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

		iterator begin()
		{
			return iterator(head);
		}

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