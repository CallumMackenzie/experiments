#pragma once

#include <vector>
#include <sstream>

template <typename T>
struct DLkList
{
	struct Node
	{
		Node *next = nullptr;
		Node *prev = nullptr;
		T data;
	};

	Node *head = nullptr; // first node
	Node *tail = nullptr; // last node
	bool cleanOnFree = false;
	size_t length = 0;

	DLkList<T>()
	{
	}

	DLkList<T>(std::vector<T> init)
	{
		for (int i = 0; i < init.size(); i++)
			push_end(init[i]);
	}

	~DLkList<T>()
	{
		if (cleanOnFree)
			clean();
	}

	void clean()
	{
		while (Node *node = pop_end_node())
			if (node)
				delete node;
	}

	size_t size()
	{
		if (!head && !tail)
			return 0;
		size_t elems = 0;
		Node *node = head;
		do
			elems++;
		while (node = node->next);
		length = elems;
		return elems;
	}

	T pop_start()
	{
		auto node = pop_start_node();
		T data = node->data;
		delete node;
		return data;
	}

	T pop_end()
	{
		auto node = pop_end_node();
		T data = node->data;
		delete node;
		return data;
	}

	T pop(size_t index)
	{
		auto node = pop_node(index);
		T data = node->data;
		delete node;
		return data;
	}

	Node *pop_start_node()
	{
		if (!head && !tail)
			return nullptr;
		length--;
		if (head == tail)
		{
			Node *popped = head;
			head = nullptr;
			tail = nullptr;
			return popped;
		}
		Node *popped = head;
		popped->next->prev = nullptr;
		head = popped->next;
		popped->next = nullptr;
		return popped;
	}

	Node *pop_end_node()
	{
		if (!head && !tail)
			return nullptr;
		length--;
		if (head == tail)
		{
			Node *popped = head;
			head = nullptr;
			tail = nullptr;
			return popped;
		}
		Node *popped = tail;
		popped->prev->next = nullptr;
		tail = popped->prev;
		popped->prev = nullptr;
		return popped;
	}

	Node *pop_node(const size_t index)
	{
		if (!head && !tail)
			return nullptr;
		Node *popped = (*this)[index];
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
			return pop_start_node();
		if (popped == tail)
			return pop_end_node();
		if (popped->prev)
			popped->prev->next = popped->next;
		if (popped->next)
			popped->next->prev = popped->prev;
		popped->next = nullptr;
		popped->prev = nullptr;
		length--;
		return popped;
	}

	void push_end(T data)
	{
		push_end_node(new Node{nullptr, nullptr, data});
	}

	void push_start(T data)
	{
		push_start_node(new Node{nullptr, nullptr, data});
	}

	void insert_after(const size_t index, T data)
	{
		insert_after_node(index, new Node{nullptr, nullptr, data});
	}

	void insert_before(const size_t index, T data)
	{
		insert_before_node(index, new Node{nullptr, nullptr, data});
	}

	T pop_back()
	{
		return pop_end();
	}

	T pop_front()
	{
		return pop_start();
	}

	T front()
	{
		return head->data;
	}

	T back()
	{
		return tail->data;
	}

	void push_end_node(Node *node)
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

	void push_start_node(Node *node)
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

	void insert_after_node(const size_t index, Node *node)
	{
		Node *target = (*this)[index];
		if (!target)
			return;
		if (target == tail)
			return push_end_node(node);
		target->next->prev = node;
		node->next = target->next;
		target->next = node;
		node->prev = target;
		length++;
	}

	void insert_before_node(const size_t index, Node *node)
	{
		Node *target = (*this)[index];
		if (!target)
			return;
		if (target == head)
			return push_start_node(node);
		target->prev->next = node;
		node->prev = target->prev;
		node->next = target;
		target->prev = node;
		length++;
	}

	void push_back(T push)
	{
		push_end(push);
	}

	void push_front(T push)
	{
		push_start(push);
	}

	std::string toString()
	{
		if (!head && !tail)
			return "";
		std::ostringstream oss;
		Node *node = head;
		do
			oss << node->data << (node->next ? "->" : "");
		while (node = node->next);
		return oss.str();
	}

	Node *operator[](const size_t index)
	{
		if (index >= length || index < 0)
			return nullptr;
		Node *node = nullptr;
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

	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = const Node *;
		using pointer = value_type;
		using reference = value_type;

		Iterator(pointer ptr) : m_ptr(ptr)
		{
		}

		reference operator*() const { return m_ptr; }
		pointer operator->() { return m_ptr; }
		Iterator &operator++()
		{
			m_ptr = m_ptr->next;
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}
		Iterator &operator--()
		{
			m_ptr = m_ptr->prev;
			return *this;
		}
		Iterator operator--(int)
		{
			Iterator tmp = *this;
			--(*this);
			return tmp;
		}

		friend bool operator==(const Iterator &a, const Iterator &b)
		{
			return a.m_ptr == b.m_ptr;
		}
		friend bool operator!=(const Iterator &a, const Iterator &b)
		{
			return a.m_ptr != b.m_ptr;
		}

	private:
		pointer m_ptr;
	};

	Iterator begin()
	{
		return Iterator(head);
	}

	Iterator end()
	{
		return Iterator(nullptr);
	}
};
