#include <cassert>
#include <iostream>
#include <cstring>
//Задача 3		1
///////////////////////////////////////////////////////////////
// Queue.h
/*
Во всех задачах из следующего списка следует написать структуру данных,
обрабатывающую команды push* и pop*.
Формат входных данных.
В первой строке количество команд n. n ≤ 1000000.
Каждая команда задаётся как 2 целых числа: a b.
a = 1 - push front
a = 2 - pop front
a = 3 - push back
a = 4 - pop back
Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
Для очереди используются команды 2 и 3. Для дека используются все
четыре команды.
Если дана команда pop*, то число b - ожидаемое значение.
Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
Формат выходных данных.
Требуется напечатать YES - если все ожидаемые значения совпали.
Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

3_1. Реализовать очередь с динамическим зацикленным буфером.
*/

class Queue {
public:
	explicit Queue();

	Queue(const Queue& other) = delete;

	Queue(Queue&& other) { *this = std::move(other); }

	Queue& operator=(const Queue& other) = delete;

	Queue& operator=(Queue&& other);

	Queue MakeCopy() const;

	int PopFront();
	void PushBack(int arg);
	~Queue();
	bool IsEmpty();
	void Print();

private:
	int BufSize = 4;
	int *Buffer = NULL;
	int head = 0;
	int tail = 0;
	void Grow();
	void clean();
};

Queue Queue::MakeCopy() const
{
	Queue copy;
	copy.Buffer = new int[BufSize];
	memcpy(copy.Buffer, Buffer, BufSize);
	return copy;
}

Queue& Queue::operator=(Queue&& queue)
{
	std::swap(Buffer, queue.Buffer);
	return *this;
}

Queue::Queue() {
	Buffer = new int[BufSize]();
	head = tail = 0;
}

Queue::~Queue() {
	clean();
}
void Queue::clean() {
	if (Buffer != NULL) {
		delete[] Buffer;
	}
}
void Queue::Grow() {
	int* NewBuffer = new int[BufSize * 2]();
	int i = head;
	int j = 0;
	while (head != tail) {
		NewBuffer[j] = Buffer[head];
		j++;
		head = (head + 1) % BufSize;
	}
	head = 0;
	tail = BufSize - 1;
	BufSize = BufSize * 2;
	delete[] Buffer;
	Buffer = NewBuffer;

}


void Queue::Print() {
	int temphead = head;
	while ((tail != temphead)) {
		std::cout << Buffer[temphead] << ' ';
		temphead = (temphead + 1) % BufSize;
	}
	std::cout << '\n';
}
void Queue::PushBack(int elem) {
	if (head == (tail + 1) % BufSize) Grow();
	Buffer[tail] = elem;
	tail = (tail + 1) % BufSize;

}

int Queue::PopFront() {
	if (IsEmpty()) {
		return -1;
	}
	else {
		int temp = Buffer[head];
		head = (head + 1) % BufSize;
		return temp;
	}

}

bool Queue::IsEmpty() {
	if (head == tail)
		return 1;
	else
		return 0;
}

struct Operation {
	int Code = 0;
	int Value = 0;
};

bool do_operation(Queue& queue, Operation op)
{
	switch (op.Code) {

	case 3:
		queue.PushBack(op.Value);
		return true;
	case 2: {
		const int result = !queue.IsEmpty() ? queue.PopFront() : -1;
		return (result == op.Value);
	}
	}
	return false;
}


int main() {
	int n = 0;
	std::cin >> n;

	Queue queue;

	for (int i = 0; i < n; i++) {
		Operation op;
		std::cin >> op.Code >> op.Value;
		if (!do_operation(queue, op)) {
			std::cout << "NO";
			return 0;
		}
		
	}
	std::cout << "YES";
	//system("pause");
	return 0;
}
