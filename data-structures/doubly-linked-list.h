#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H 1

#include <vector>

#ifdef DLK_LIST_TO_STR
#include <sstream>
#include <string>
#endif

namespace dst
{
	template <typename T>
	struct doubly_linked_list
	{
		struct node
		{
			node *next = nullptr;
			node *prev = nullptr;
			T data;
		};

		node *head = nullptr;		// Start node (first)
		node *tail = nullptr;		// End node (last)
		bool clear_on_free = false; // Whether to delete nodes when the list is freed

		doubly_linked_list<T>()
		{
		}

		doubly_linked_list<T>(std::vector<T> init)
		{
			for (int i = 0; i < init.size(); i++)
				push_back(init[i]);
		}

		~doubly_linked_list<T>()
		{
			if (clear_on_free)
				clear();
		}

		void clear()
		{
			while (node *node = pop_back_node())
				if (node)
					delete node;
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

		void push_back(T data)
		{
			push_back_node(new node{nullptr, nullptr, data});
		}

		void push_front(T data)
		{
			push_front_node(new node{nullptr, nullptr, data});
		}

		void insert_after(const size_t index, T data)
		{
			insert_after_node(index, new node{nullptr, nullptr, data});
		}

		void insert_before(const size_t index, T data)
		{
			insert_before_node(index, new node{nullptr, nullptr, data});
		}

		T front()
		{
			return head->data;
		}

		T back()
		{
			return tail->data;
		}

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

		void insert_after_node(const size_t index, node *node)
		{
			node *target = (*this)[index];
			if (!target)
				return;
			if (target == tail)
				return push_back_node(node);
			target->next->prev = node;
			node->next = target->next;
			target->next = node;
			node->prev = target;
			length++;
		}

		void insert_before_node(const size_t index, node *node)
		{
			node *target = (*this)[index];
			if (!target)
				return;
			if (target == head)
				return push_front_node(node);
			target->prev->next = node;
			node->prev = target->prev;
			node->next = target;
			target->prev = node;
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
				for (int i = length; i > 0; i--)
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
	using dlk_list = doubly_linked_list<T>;
}

#endif