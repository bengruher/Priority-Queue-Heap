/**
  *@file PQHeap.h - max heap implementation of priority queue ADT with linear time build function
  *@author Ben Gruher
  *@see "Seattle University, CPSC 2430, Spring 2018"
*/

#pragma once
#include "adt/PriorityQueue.h"
#include <stdexcept>

template <typename T>
class PQHeap : public PriorityQueue<T> {
public:
	PQHeap();
	~PQHeap();
	PQHeap(const PQHeap& other);
	PQHeap& operator=(const PQHeap& other);
	PQHeap(PQHeap&& temp);
	PQHeap& operator=(PQHeap&& temp);
	const T& peek() const;
	void enqueue(const T& datum);
	void dequeue();
	bool empty() const;
	void clear();
	void build(T * a, int s);
	std::ostream& print(std::ostream& out) const;
private:
	T * heap;
	size_t size;
	size_t max;
	static size_t parent(size_t child);
	static size_t left(size_t p);
	static size_t right(size_t p);
	bool isLeaf(size_t p) const;
	bool valid(size_t i) const;
	void copy(T * a, size_t s);
	void percolate(size_t p);
	void bubble(size_t p);
	bool hasRight(size_t p);
	void resize();
	bool hasParent(size_t c);
};

template<typename T>
PQHeap<T>::PQHeap() : heap(nullptr), size(0), max(100) {
	heap = new T[max];
}

template<typename T>
PQHeap<T>::~PQHeap() {
	delete[] heap;
}

template<typename T>
PQHeap<T>::PQHeap(const PQHeap & other) : PQHeap<T>() {
	*this = other;
}

template<typename T>
PQHeap<T>& PQHeap<T>::operator=(const PQHeap & other) {
	if (this != other)
		copy(other, size);
	return *this;
}

template<typename T>
PQHeap<T>::PQHeap(PQHeap && temp) : PQHeap<T>() {
	*this = temp;
}

template<typename T>
PQHeap<T> & PQHeap<T>::operator=(PQHeap && temp) {
	copy(temp, size);
	return *this;
}

template<typename T>
const T & PQHeap<T>::peek() const {
	if (empty())
		throw std::invalid_argument("Empty heap");
	else
		return heap[0];
}

template<typename T>
void PQHeap<T>::enqueue(const T & datum) {
	if ((size + 1) > max)
		resize();
	size++;
	heap[size - 1] = datum;
	bubble(size - 1);
}

template<typename T>
void PQHeap<T>::dequeue() {
	size--;
	T temp = heap[size];
	heap[size] = heap[0];
	heap[0] = temp;
	percolate(0);
}

template<typename T>
bool PQHeap<T>::empty() const {
	return size == 0;
}

template<typename T>
void PQHeap<T>::clear() {
	size = 0;
}

template<typename T>
void PQHeap<T>::build(T * a, int s) {
	heap = new T[s];
	size = s;
	for (int i = 0; i < s; i++)
		heap[i] = a[i];
	for (int i = size / 2 - 1; i >= 0; i--)
		percolate(i);
}

template<typename T>
std::ostream & PQHeap<T>::print(std::ostream & out) const {
	for (int i = 0; i < size; i++)
		out << heap[i] << ", ";
	out << std::endl;
	return out;
}

template<typename T>
size_t PQHeap<T>::parent(size_t child) {
	return (child-1) / 2;
}

template<typename T>
size_t PQHeap<T>::left(size_t p) {
	return p * 2 + 1;
}

template<typename T>
size_t PQHeap<T>::right(size_t p) {
	return left(p) + 1;
}

template<typename T>
bool PQHeap<T>::isLeaf(size_t p) const {
	return !valid(left(p));
}

template<typename T>
bool PQHeap<T>::valid(size_t i) const {
  if (size == 0) {
    if (i ==0)
      return true;
    else
      return false;
  }
  else
	return i <= size-1 && i >= 0;
}

template<typename T>
void PQHeap<T>::copy(T* a, size_t s) {
	for (int i = 0; i < s; i++) {
		heap[i] = a[i];
	}
}

template<typename T>
void PQHeap<T>::percolate(size_t p) {
	while (!isLeaf(p)) {
		size_t child = left(p);
		if (hasRight(p) && heap[child] < heap[right(p)])
			child = right(p);
		if (heap[p] < heap[child]) {
			T temp = heap[p];
			heap[p] = heap[child];
			heap[child] = temp;
		}
        else {
          break;
        }   
		p = child;
	}
}

template<typename T>
void PQHeap<T>::bubble(size_t p) {
	while (hasParent(p) && heap[parent(p)] < heap[p]) {
		T temp = heap[p];
		heap[p] = heap[parent(p)];
		heap[parent(p)] = temp;
		p = parent(p);
	}
}

template<typename T>
bool PQHeap<T>::hasRight(size_t p) {
	return valid(right(p));
}

template<typename T>
void PQHeap<T>::resize() {
	T * temp = new T[max];
	for (int i = 0; i < max; i++)
		temp[i] = heap[i];
	max *= 2;
    delete[] heap;
	heap = new T[max];
	copy(temp, size);
	delete[] temp;
}

template<typename T>
bool PQHeap<T>::hasParent(size_t c) {
	return valid(parent(c));
}
