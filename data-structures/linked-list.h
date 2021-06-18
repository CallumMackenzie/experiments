#ifndef LINKED_LIST_H
#define LINKED_LIST_H 1

#include <vector>
#include <sstream>

template <typename T>
struct LinkedList
{
	struct Node
	{
		Node *next;
		T data;
	};

	Node *head = nullptr; // first node (start)
	Node *tail = nullptr; // last node (end)
	bool clearOnFree = false;
	size_t length = 0;

	LinkedList<T>()
	{
	}

	LinkedList<T>(std::vector<T> data)
	{
		for (auto item : data)
			push_end(item);
	}

	~LinkedList<T>()
	{
		if (clearOnFree)
			clear();
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

	void push_back(T data)
	{
		push_end(data);
	}

	void push_front(T data)
	{
		push_start(data);
	}

	void push_start(T data)
	{
		push_start_node(new Node{nullptr, data});
	}

	void push_end(T data)
	{
		push_end_node(new Node{nullptr, data});
	}

	void push_end_node(Node *node)
	{
		if (!tail && !head)
			tail = head = node;
		else
		{
			tail->next = node;
			tail = node;
		}
	}

	void push_start_node(Node *node)
	{
		if (!tail && !head)
			tail = head = node;
		else
		{
			node->next = head;
			head = node;
		}
	}

	T pop_start()
	{
		auto node = pop_start_node();
		if (node)
		{
			T data = node->data;
			delete node;
			return data;
		}
		return (T)NULL;
	}

	T pop_end()
	{
		auto node = pop_end_node();
		if (node)
		{
			T data = node->data;
			delete node;
			return data;
		}
		return (T)NULL;
	}

	Node *pop_start_node()
	{
		if (!head && !tail)
			return nullptr;
		if (head == tail)
		{
			Node *popped = head;
			head = nullptr;
			tail = nullptr;
			return popped;
		}
		Node *popped = head;
		head = head->next;
		return popped;
	}

	Node *pop_end_node()
	{
		if (!head && !tail)
			return nullptr;
		if (head == tail)
		{
			Node *popped = head;
			head = nullptr;
			tail = nullptr;
			return popped;
		}
		Node *beforeTail = head;
		do
			if (beforeTail->next == tail)
				break;
		while (beforeTail = beforeTail->next);
		Node *popped = tail;
		beforeTail->next = nullptr;
		tail = beforeTail;
		return popped;
	}

	std::string to_string()
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

	void clear()
	{
		while (Node *node = pop_start_node())
			if (node)
				delete node;
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

template <typename T>
using LkList = LinkedList<T>;

#endif