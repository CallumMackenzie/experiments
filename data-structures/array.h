#ifndef ARRAY_CST_H
#define ARRAY_CST_H 1

#include <vector>
#include <initializer_list>
#include <string.h>

#ifdef DST_ARRAY_TO_STR
#include <sstream>
#include <string>
#endif

namespace dst
{
	// A simple array wrapper with a fixed size and iterator.
	template <typename T>
	struct array
	{
		// Creates a new array with the given length.
		array(const size_t size)
		{
			length = size;
			arr = new T[length];
		}

		// Creates a new array with the given vector.
		array(std::vector<T> init) : array(init.size())
		{
			for (size_t i = 0; i < length; i++)
				arr[i] = init[i];
		}

		// Creates a new array with the given size, populating it with the contents of the vector.
		array(size_t size, std::vector<T> init) : array(size)
		{
			for (size_t i = 0; i < length; i++)
				if (i < init.size())
					arr[i] = init[i];
				else
					break;
		}

		// Creates a new array from an initializer list.
		array(std::initializer_list<T> list) : array(list.size())
		{
			size_t i = 0;
			for (auto item : list)
			{
				arr[i] = item;
				++i;
			}
		}

		~array()
		{
			delete[] arr;
		}

		// Returns the length of the array.
		size_t size()
		{
			return length;
		}

		// Returns the data at the given index.
		T operator[](size_t index)
		{
			return arr[index];
		}

#ifdef DST_ARRAY_TO_STR
		std::string to_string()
		{
			std::ostringstream oss;
			for (size_t i = 0; i < length; i++)
				oss << (i == 0 ? "{" : "") << arr[i] << (i == (length - 1) ? "}" : ", ");
			return oss.str();
		}
#endif

		struct iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T *;
			using reference = T &;

			iterator(pointer ptr) : m_ptr(ptr)
			{
			}

			reference operator*() const { return *m_ptr; }
			pointer operator->() { return m_ptr; }
			iterator &operator++()
			{
				m_ptr++;
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

		// An iterator repersenting the start of the array.
		iterator begin()
		{
			return iterator(&arr[0]);
		}

		// An iterator repersenting the end of the array.
		iterator end()
		{
			return iterator(&arr[length]);
		}

	private:
		T *arr;
		size_t length = 0;
	};

}

#endif