#pragma once

#include <deque>

template <typename T>
class mydeque {
	int offset;
public:
	std::deque<T> value;
	T& operator[](int x) {
		while (x - offset < 0)
		{
			if (value.size() == 0)
				offset = x;
			else
				offset--;
			value.push_front(T());
		}
		while (x - offset >= value.size())
		{
			if (value.size() == 0)
				offset = x;
			value.push_back(T());
		}
		return value[x - offset];
	}
	void pop_front() {
		offset++;
		value.pop_front();
	}
	void pop_back() { value.pop_back(); }
	void cutoff_front(int front) {
		while (front - offset < 0)
			pop_front();
	}
	void cutoff_back(int back) {
		while (back - offset >= value.size())
			pop_back();
	}
	mydeque(int offset = 0) :
		offset(offset) {}
	~mydeque() {}
};
