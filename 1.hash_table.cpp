/*
Реализуйте структуру данных типа “множество строк”
на основе динамической хеш-таблицы с открытой адресацией.
Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления
значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми.
Перехеширование выполняйте при добавлении
элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество,
удаления строки из множества и проверки принадлежности данной строки множеству.
1_1. Для разрешения коллизий используйте квадратичное пробирование.
i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

/*
Формат ввода
Каждая строка входных данных задает одну операцию над множеством.
Запись операции состоит из типа операции и следующей за ним через пробел строки,
над которой проводится операция.
Тип операции – один из трех символов:
+ означает добавление данной строки в множество;
- означает удаление строки из множества;
? означает проверку принадлежности данной строки множеству.
При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ,
что он отсутствует в этом множестве.
При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ,
что он присутствует в этом множестве.*/

/*
Формат вывода
Программа должна вывести для каждой операции одну из двух строк OK или FAIL,
в зависимости от того, встречается ли данное слово в нашем множестве.

*/
#include<iostream>
#include<string>
#include <vector>

template<class T>
class HashTable {
public:
	HashTable() = default;
	HashTable(const HashTable &) = delete;

	HashTable &operator=(const HashTable &) = delete;

	~HashTable();

	bool Add(const T &value);
	bool Remove(const T &value);
	bool Has(const T &value);
private:
	bool AddFlag;
	int temp_hash;
	void Rehash();
	int Hash(const std::string &value);
	int capacity = 8;
	int num_in_table = 0;
	float rehash_value = 3.0 / 4.0;
	int a = 15;

	struct Node {
		T Data;
		bool IsDeleted;
		explicit Node(const std::string& data) : Data(data), IsDeleted(false) {};
	};

	std::vector<Node *> table = std::vector<Node *>(capacity, nullptr);
};

template<class T>
HashTable<T>::~HashTable() {

	for (int i = 0; i < capacity; i++) {
		if (table[i] == nullptr) continue;
		delete table[i];
	}
	table.clear();
}

template<class T>
int HashTable<T>::Hash(const std::string &value) {
	const char *str = value.c_str();
	int hash = 0;
	for (; *str != 0; ++str) {
		hash = (hash * a + *str) % capacity;
	}
	return hash;
}
template<class T>
bool HashTable<T>::Add(const T &value) {
	
	AddFlag = true;
	//Quadrative probing c1,c2=(1/2)
	if (this->Has(value)) {
		return false;
	}
	//hash calculated by Has(value)
	int hash = temp_hash;
	if (hash < 0) { return false; }
	if (table[hash] == nullptr) {
		table[hash] = new Node(value);
	}
	else
		if (table[hash]->IsDeleted) {
			table[hash]->IsDeleted = false;
			table[hash]->Data = value;
		}
	
	num_in_table++;
	if (num_in_table >= rehash_value * capacity) {
		Rehash();
	}
	return true;
}

template<class T>
void HashTable<T>::Rehash() {
	capacity = capacity * 2;
	int old_capacity = capacity / 2;
	int elements = num_in_table;

	num_in_table = 0;
	std::vector<T> buffer(elements);

	for (int i = 0, j = 0; i < old_capacity; i++) {
		if (table[i] == nullptr) continue;
		if (!(table[i]->IsDeleted)) {
			buffer[j] = table[i]->Data;
			j++;
		}
		delete table[i];
	}
	table.clear();
	table.resize(capacity, nullptr);
	for (int i = 0; i < elements; i++) {
		this->Add(buffer[i]);
	}
}
template<class T>
bool HashTable<T>::Remove(const T &value) {
	if (this->Has(value)) {
		int hash = temp_hash;
		table[hash]->IsDeleted = true;
		num_in_table--;
		return true;
	}
	else {
		return false;
	}
}

template<class T>
bool HashTable<T>::Has(const T &value) {
	int hash = Hash(value);
	bool adding = AddFlag;
	AddFlag = false;
	//Quadr probing c1,c2=(1/2)
	int i = 0;
	for (int i = 0; i < capacity;i++) {
		if (table[hash] != nullptr) {
			if (table[hash]->IsDeleted) {
				if (adding)
				{	temp_hash = hash;
					return false;
				}
				hash = (hash + (i+1)) % capacity;
				continue;
			}
			if (table[hash]->Data == value) {
				temp_hash = hash;
				return true;
			}
		}
		else {
			temp_hash = hash;
			return false;
		}
		// Hash for next probing
		hash = (hash + (i+1)) % capacity;
	}
	temp_hash = -1;
	return false;
}
int main() {
	HashTable<std::string> hashtable;
	char command;
	std::string value;
	while (std::cin >> command >> value) {
		switch (command) {
		case '+':
			std::cout << (hashtable.Add(value) ? "OK" : "FAIL") << '\n';
			break;
		case '-':
			std::cout << (hashtable.Remove(value) ? "OK" : "FAIL") << '\n';
			break;
		case '?':
			std::cout << (hashtable.Has(value) ? "OK" : "FAIL") << '\n';
			break;
		default:
			return 0;
		}
	}
	return 0;
}
