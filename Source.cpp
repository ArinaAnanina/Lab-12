#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct Date
{
	int Day, Month, Year;
};

struct Human
{
	string FIO;
	Date DateOfBirth;
	long long PassportNumber;

	void Print()
	{
		cout << endl;
		cout << "ФИО: " << FIO << endl;
		cout << "Дата рождения: " << DateOfBirth.Day << "." << DateOfBirth.Month << "." << DateOfBirth.Year << endl;
		cout << "Номер паспорта: " << PassportNumber << endl;
	}

	void GenerateData()
	{
		string names[] = { "Иван", "Юрий", "Алексей", "Артём", "Дмитрий" };
		string surnames[] = { "Иванов" , "Ананин", "Тарасов" };
		string patronymic[] = {"Андреевич", "Вадимович", "Михаилович", "Кирилович", "Ильич" };

		FIO = surnames[rand() % 3] + " " + names[rand() % 5] + " " + patronymic[rand() % 5];
		DateOfBirth.Day = 1 + rand() % 28;
		DateOfBirth.Month = 1 + rand() % 12;
		DateOfBirth.Year = 1900 + rand() % 200;
		PassportNumber = 5700000000 + (rand() % 100) * 1000000 + rand() % 1000000;
	}
};
vector<Human> listToDelete;

vector<Human> CreateArray(int size = 100)
{
	vector<Human> res;
	for (int i = 0; i < size; i++)
	{
		Human newEl;
		newEl.GenerateData();
		res.push_back(newEl);
	}
	return res;
}

void SaveToFile(vector<Human> humans)
{
	string path = "File.txt";
	ofstream out = ofstream();
	out.open(path);
	for (int i = 0; i < humans.size(); i++)
	{
		out << humans[i].FIO << endl;
		out << humans[i].DateOfBirth.Day << endl;
		out << humans[i].DateOfBirth.Month << endl;
		out << humans[i].DateOfBirth.Year << endl;
		out << humans[i].PassportNumber << endl;
	}
	out.close();
	cout << "Список записан в файл File.txt" << endl;
}

void LoadFromFile(vector<Human>& humans)
{
	string path = "File.txt";
	ifstream in = ifstream();
	in.open(path);
	if (in.is_open())
	{
		humans = vector<Human>();
		while (!in.eof())
		{
			Human newElement = Human();
			getline(in, newElement.FIO);
			in >> newElement.DateOfBirth.Day;
			in >> newElement.DateOfBirth.Month;
			in >> newElement.DateOfBirth.Year;
			in >> newElement.PassportNumber;
			in.ignore();
			humans.push_back(newElement);
		}
		humans.erase(humans.end() - 1);
		cout << "Загрузка из файла завершена" << endl;
	}
	else
	{
		cout << "Файл не открылся" << endl;
	}
	in.close();
}

void PrintArray(vector<Human> humans)
{
	if (humans.size() < 1)
	{
		cout << "Список пуст" << endl;
		return;
	}
	for (int i = 0; i < humans.size(); i++)
	{
		cout << "Элемент " << i + 1 << endl;
		humans[i].Print();
	}
}

struct SearchItem
{
	int Index;
	long long Data;
};

int FindInterPolationSearch(vector<Human> humans, Human info)
{
	long long key = info.PassportNumber;
	int size = humans.size();
	SearchItem* collection = new SearchItem[size];
	for (int i = 0; i < humans.size(); i++)
	{
		SearchItem item;
		item.Index = i;
		item.Data = humans[i].PassportNumber;
		collection[i] = item;
	}
  
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (collection[j].Data > collection[j + 1].Data)
			{
				SearchItem tmp = collection[j];
				collection[j] = collection[j + 1];
				collection[j + 1] = tmp;
			}
		}
	}
	int mid, left = 0, right = size - 1;

	while (collection[left].Data < key && collection[right].Data > key)
	{
		mid = left + (key - collection[left].Data) * (right - left) / (collection[right].Data - collection[left].Data);
		if (collection[mid].Data < key) {
			left = mid + 1;
		}
		else if (collection[mid].Data > key) {
			right = mid - 1;
		}
		else {
			return collection[mid].Index;
		}
	}

	if (collection[left].Data == key) {
		return collection[left].Index;
	}
	if (collection[right].Data == key) {
		return collection[right].Index;
	}
	return -1;
}

int FindSubstringInString(vector<Human> humans, Human key)
{
	string listOfNumbers;

	string substring = "";
	substring += to_string(key.PassportNumber);
	

	for (int i = 0; i < humans.size(); i++)
	{
		string newLine = "";
		newLine += to_string(humans[i].PassportNumber);
		listOfNumbers += newLine;
	}

	for (int i = 0; i <= listOfNumbers.size() - substring.size(); i++)
	{
		int j = 0;
		while (listOfNumbers[i + j] == substring[j] && j < substring.size())
			j++;
		if (j == substring.size())
			return i / 10;
	}

	return -1;
}

int LineSearch(vector<Human> humans, Human key)
{
	for (int i = 0; i < humans.size(); i++)
	{
		if (humans[i].PassportNumber == key.PassportNumber )
			return i;
	}
	return -1;
}

void Search(vector<Human> humans)
{
	if (humans.size() < 1)
	{
		cout << "" << endl;
		return;
	}

	Human key;
	cout << endl << "Введите номер паспорта, по которому необходимо искать элемент: ";
	cin >> key.PassportNumber;

	int ind = FindInterPolationSearch(humans, key);
	cout << endl << "Интерполяционный поиск" << endl;
	if (ind != -1)
		cout << "Человек с заданным номером паспорта есть: " << humans[ind].FIO << endl;
	else
		cout << "Человек с заданным номером паспорта отсутствует" << endl;

	ind = LineSearch(humans, key);
	cout << endl << "Линейный поиск" << endl;
	if (ind != -1)
		cout << "Человек с заданным номером паспорта есть: " << humans[ind].FIO << endl;
	else
		cout << "Человек с заданным номером паспорта отсутствует" << endl;

	ind = FindSubstringInString(humans, key);
	cout << endl << "Поиск подстроки в строке" << endl;
	if (ind != -1)
		cout << "Человек с заданным номером паспорта есть: " << humans[ind].FIO << endl;
	else
		cout << "Человек с заданным номером паспорта отсутствует" << endl;
}

void AddElement(vector<Human>& humans)
{
	char vvod;
	cout << "1 - добавить по номеру" << endl;
	cout << "2 - добавить по ключу" << endl;
	cin >> vvod;
	if (vvod == '1')
	{
		int ind;
		do
		{
			cout << "Введите номер элемента,на место которого необходимо вставить элемент: ";
			cin >> ind;
		} while (ind < 0 || ind > humans.size());
		Human t;
		t.GenerateData();
		humans.insert(humans.begin() + ind, t);
	}
	else if (vvod == '2')
	{
		Human key;
		cout << endl << "Введите номер паспорта элемента, на место которого надо вставить элемент: ";
		cin >> key.PassportNumber ;
		int ind = FindSubstringInString(humans, key);
		if (ind != -1)
		{
			Human t;
			t.GenerateData();
			humans.insert(humans.begin() + ind, t);
		}
		else
		{
			cout << "Элемент с заданным номером паспорта не найден" << endl;
		}
	}
	else
	{
		cout << "Введено некорректное значение." << endl;
	}
}

void DeleteElement(vector<Human>& humans)
{
	if (humans.size() == 0)
	{
		cout << "Список пуст" << endl;
		return;
	}
	char vvod;
	cout << "1 - удаление по номеру элемента" << endl;
	cout << "2 - удаление по ключу" << endl;
	cin >> vvod;
	if (vvod == '1')
	{
		int ind;
		do
		{
			cout << "Введите номер элемента, который необходимо удалить ";
			cin >> ind;
		} while (ind < 0 || ind >= humans.size());
		listToDelete.push_back(humans[ind]);
		humans.erase(humans.begin() + ind);
	}
	else if (vvod == '2')
	{
		Human key;
		cout << endl << "Введите номер паспорта, по которому надо удалить элемент: ";
		cin >> key.PassportNumber;
		int ind = FindSubstringInString(humans, key);
		if (ind != -1)
		{
			Human t;
			listToDelete.push_back(humans[ind]);
			humans.erase(humans.begin() + ind);
		}
		else
		{
			cout << "Такого элемента в списке нет." << endl;
		}
	}
	else
	{
		cout << "Введено некорректное значение." << endl;
	}
}


int main()
{
	system("color F0");
	setlocale(LC_ALL, "rus");
	vector<Human> humans;

	int vvod = -1;
	while (vvod != 7)
	{
		cout << "0 - поиск элемента" << endl;
		cout << "1 - создание списка" << endl;
		cout << "2 - считать из файла" << endl;
		cout << "3 - сохранить в файл" << endl;
		cout << "4 - распечатать список" << endl;
		cout << "5 - добавить элемент" << endl;
		cout << "6 - удалить элемент" << endl;
		cout << "7 - выход" << endl;
		cin >> vvod;

		switch (vvod)
		{
		case 0: Search(humans); break;
		case 1: humans = CreateArray(); break;
		case 2: LoadFromFile(humans); break;
		case 3: SaveToFile(humans); break;
		case 4: PrintArray(humans); break;
		case 5: AddElement(humans); break;
		case 6: DeleteElement(humans); break;
		default: if (vvod != 7) cout << "Введено некорректное значение, попробуйте снова:)" << endl;
		}
	}
	return 0;
}
