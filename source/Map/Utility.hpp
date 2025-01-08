#pragma once
#include <memory>


struct Frame
{
	sf::IntRect textureRect;
	sf::Time dur;
	int localId;
};

template <typename T>
class DynArray
{
public:
	T &operator[](size_t index)
	{
		return m_arr[index];
	}
	const T &operator[](size_t index) const
	{
		return m_arr[index];
	}

	size_t size()
	{
		return m_size;
	}

	T *begin()
	{
		return m_arr.get();
	}
	T *end()
	{
		return m_arr.get() + m_size;
	}

	const T *begin() const
	{
		return m_arr.get();
	}
	const T *end() const
	{
		return m_arr.get() + m_size;
	}

	const T *cbegin() const
	{
		return m_arr.get();
	}
	const T *cend() const
	{
		return m_arr.get() + m_size;
	}

	void setSize(size_t size)
	{
		m_arr = std::make_unique<T[]>(size);
		m_size = size;
	}

private:
	std::unique_ptr<T[]> m_arr;
	size_t m_size;
};
