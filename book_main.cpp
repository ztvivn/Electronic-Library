#include <iostream>
#include <fstream>
#include <string> 
#include <cstring>
#include <cctype>

struct Node//сам узел
{
	std::string UDK;//УДК
	std::string author;//автор
	std::string name;//название
	int year;//год издания
	int quantity;//количесво экземпляров
	Node *next;//указатель на следующий элемент

	Node(std::string  UDK, std::string author, std::string name, int year, int quantity = 1)
	{
		this->UDK = UDK;
		this->author = author;
		this->name = name;
		this->year = year;
		this->quantity = quantity;
		next = nullptr;
	}
};

struct List//реализация возможностей узла
{
	Node *first;//указатель на первый узел
	Node *last;//указатель на крайний узел
	int size = 0;

	List()
	{
		first = nullptr;
		last = nullptr;
	}

	bool is_empty()//функция, которая проверяет является ли первый узел пустым
	{
		return first == nullptr;
	}

	void push_back(std::string  UDK, std::string author, std::string name, int year, int quantity = 1)//функция добавления
	{
		size++;
 		Node *p = new Node(UDK, author, name, year, quantity);//создаем узел
 		if (is_empty())//если узлов нет
 		{
 			first = p;
 			last = p;
 			return;
 		}

 		last->next = p;//добавляем узел к крайнему узлу через указатель в узле, на который указывает указатель на крайний элемент
 		last = p;//присваиваем адрес нового узла указателю, который указывает на крайний элемент
	}

	void print()//функция вывода
	{
		int number = 1;
		if(is_empty()) 
		{
			std::cout << "Ваша библиотека пуста :(\n";
			return;
		}
		Node *p = first;
		while(p)
		{
			std::cout << number << ") " << "УДК: "<< p->UDK << ", " << "Автор: " << p->author << ", " << "Название: " << p->name << ", " << "Год: " << p->year << ", " << "Количество: " << p->quantity << std::endl;
			p = p->next;
			number++;
		}
	}

	Node *find(std::string  UDK, std::string author, std::string name, int year)//поиск нужного узла по данным полей
	{
		Node *p = first;//сделали копию указателья на первый элемент
		while(p && (p->UDK != UDK && p->author != author && p->name != name && p->year != year)) p = p->next;//ищем нужный элемент
		return(p && (p->UDK == UDK && p->author == author && p->name == name && p->year == year)) ? p : nullptr;//если нужный возвращаем указатель на этот узел, иначе 0

	}

	void remove_first()//функция удаления первого узла
	{
		if (is_empty()) return;
		size--;
		Node *p = first;
		first = p->next;
		delete p;
	}

	void remove_last()//функция удаления последнего узла
	{
		if(is_empty()) return;
		size--;
		if (first == last)//если всего в списке один элемент
		{
			remove_first();
			return;
		}

		Node *p = first;//если в списке больше одного элемента

		while(p->next != last) p = p->next;//ищем последний элемент
		p->next = nullptr;
		delete last;//удаляем его
		last = p;//присваиваем указателю на крайний элемент новое значение(предпоследний элемент)
	}

	void remove(std::string  UDK, std::string author, std::string name, int year)//удаление элемента по его полям
	{
		if(is_empty()) return;
		size--;
		if(first->UDK == UDK && first->author == author && first->name == name && first->year == year)//если элемент первый
		{
			remove_first();
			return;
		}
		else if(last->UDK == UDK && last->author == author && last->name == name && last->year == year)//если элемент крайний
		{
			remove_last();
			return;
		}

		Node *slow = first;//вариант, когда искомый элемент не первый и не крайний
		Node *fast = first->next;
		while(fast && (fast->UDK != UDK && fast->author != author && fast->name != name && fast->year != year))//ищем элемент
		{
			fast = fast->next;
			slow = slow->next;
		}
		if (!fast)//если указатель пустой
		{
			std::cout << "Этот элемент не существует :(" << std::endl;
		}

		slow->next = fast->next;//меняем указатели
		delete fast;//удаляем указатель
	}

	Node *operator[](const int index)//обращение к элементу по индексу
	{
		if(is_empty()) return nullptr;//проверка на пустоту списка
		Node *p = first;
		for (int i = 0; i < index; ++i)
		{
			p = p->next;
			if(!p) return nullptr;
		}
		return p;//возвращаем объект
	}

};

struct Interface//интерефейс
{
	List list;//объект, который хранит структуру
	std::string index;//нужный нам пункт в начале

	Interface()
	{
		std::cout << "\t\tЗдравствуйте, вы попали в электронную библиотеку\n\n";
		download_from_file();
	}

	~Interface()
	{
		std::ofstream file("Книги.txt");
		std::string copy;
		if(!file.is_open())
		{
			std::cout << "\nФайл с книгами не найден, обратитесь к администратору!\n";
		}


		for (int i = 0; i < list.size; ++i)
		{
			file << list[i]->UDK << ", " << list[i]->author << ", " << list[i]->name << ", " << list[i]->year << ", " << list[i]->quantity << std::endl;
		}

		file.close(); 
	}

	void download_from_file()//загрузка данных из файла
	{
		std::ifstream file;
		file.open("Книги.txt", std::ios::app);//открытие файла и запись в конец
		if(!file.is_open())
		{
			std::cout << "\nФайл с книгами не найден, обратитесь к администратору!\n";
		}

		std::string copy_book;//строка с инфой о книге
		std::string arr[5];//массив с инфой о книге
		std::string name = "";//копия для точечной ины о книге
		int j = 0;

		std::string UDK_copy;//УДК
		std::string author_copy;//автор
		std::string name_copy;//название
		int year_copy;//год издания
		std::string year_copy_str;
		int quantity_copy;//количесво экземпляров
		std::string quantity_copy_str;
		
		

		while(std::getline(file, copy_book))
		{
			for (int i = 0; i < copy_book.size(); ++i)
			{	
				if (copy_book[i] == ',')
				{
					if(j < 4)
					{
						arr[j] = name;
						name = "";
					}

					i++;
					i++;
					j++;
				}

				name = name + copy_book[i];
			}

			if(j < 5)
			{
				arr[j] = name;
				name = "";
				j = 0;
			}
			
			UDK_copy = arr[0];
			author_copy = arr[1];
			name_copy = arr[2];
			year_copy_str = arr[3];
			year_copy = std::stoi(year_copy_str);
			quantity_copy_str = arr[4];
			quantity_copy = std::stoi(quantity_copy_str);

			list.push_back(UDK_copy, author_copy, name_copy, year_copy, quantity_copy);

		}

		file.close();
	}

	bool isNumeric(std::string const &str)
	{
    	auto it = str.begin();
    	while (it != str.end() && std::isdigit(*it)) {
        	it++;
    	}
    	return !str.empty() && it == str.end();
	}	

	void add_book(std::string  UDK, std::string author, std::string name, int year)//добавить книгу
	{
		int quantity = 1;
		for (int i = 0; i < list.size; ++i)
		{
			if (UDK == list[i]->UDK && author == list[i]->author && name == list[i]->name && year == list[i]->year)
			{
				(list[i]->quantity)++;
				return;
			}
		}

		list.push_back(UDK, author, name, year, quantity);	
	}

	void take_book(std::string  UDK, std::string author, std::string name, int year)//забирать книгу
	{
		for (int i = 0; i < list.size; ++i)
		{
			if (UDK == list[i]->UDK && author == list[i]->author && name == list[i]->name && year == list[i]->year)
			{
				if(list[i]->quantity > 0) 
				{
					(list[i]->quantity)--;
				}else
				{
					std::cout << "\nПоследний экземпляр книги уже забрали!\nНажмите Enter, чтобы вернуться в главное меню\n";
					std::cin.get();
					return;
				}

				std::cout << "\nЭкземпляр книги успешно удален!\nНажмите Enter, чтобы вернуться в главное меню\n";
				std::cin.get();
				//std::cin.get();

				return;
			}
		}

		std::cout << "\nТакой книги нет!\nНажмите Enter, чтобы вернуться в главное меню\n";
		std::cin.get();
		//std::cin.get();
	}

	void start()//вывод возможностей программы
	{
		while(true)
		{
			system("clear");
			std::cout << "\t\tВозможности этой программы:\n";
			std::cout << "1-Взять книгу.\n";
			std::cout << "2-Добавить книгу.\n";
			std::cout << "3-Показать все книги.\n";
			std::cout << "4-Выйти из программы.\n\n";
			std::cout << "Укажите нужный вам пункт: ";

			std::cin >> index;

			if(index == "1")
			{
				system("clear");

				std::string  UDK;
				std::string author;
				std::string name;
				std::string year_str;
				int year;
				int quantity;

				std::cout << "\t\tУкажите данные книги:\n";
				getline(std::cin, UDK);
				std::cout << "УДК: ";
				getline(std::cin, UDK);
				std::cout << "\nАвтор: ";
				getline(std::cin, author);
				std::cout << "\nНазвание: ";
				getline(std::cin, name);
				std::cout << "\nГод издания: ";
				getline(std::cin, year_str);
				if (isNumeric(year_str))
				{
					year = std::stoi(year_str);
				}else
				{
					std::cout << "Это не число!\n";
					std::cout << "\nДанные книги не удалены.Чтобы продолжить нажмите Enter.\n";
					std::cin.get();
					//std::cin.get();
					continue;
				}
				take_book(UDK, author, name, year);

			}else if(index == "2")
			{
				system("clear");
				std::string  UDK;
				std::string author;
				std::string name;
				std::string year_str;
				int year;
				int quantity;

				std::cout << "\t\tУкажите данные книги:\n";
				getline(std::cin, UDK);
				std::cout << "УДК: ";
				getline(std::cin, UDK);
				std::cout << "\nАвтор: ";
				getline(std::cin, author);
				std::cout << "\nНазвание: ";
				getline(std::cin, name);
				std::cout << "\nГод издания: ";
				getline(std::cin, year_str);
				if (isNumeric(year_str))
				{
					year = std::stoi(year_str);
				}else
				{
					std::cout << "Это не число!\n";
					std::cout << "\nДанные книги не добавлены.Чтобы продолжить нажмите Enter.\n";
					std::cin.get();
					//std::cin.get();
					continue;
				}
				add_book(UDK, author, name, year);
				
				std::cout << "\n\nДанные успешно добавлены.Чтобы продолжить нажмите Enter.\n";
				std::cin.get();
				//std::cin.get();

			}else if(index == "3")
			{
				system("clear");
				std::cout <<"\t\tВесь список книг\n\n";
				list.print();
				std::cout << "\n\nЧтобы продолжить нажмите Enter.\n";
				std::cin.get();
				std::cin.get();
			}else if(index == "4")
			{
				break;
				//exit(1);
			}
		}
	}
};

bool isNumeric(std::string const &str)
{
    auto it = str.begin();
    while (it != str.end() && std::isdigit(*it)) {
        it++;
    }
    return !str.empty() && it == str.end();
}

int main()
{
	Interface interface;//создаем объект, чтобы включить интерфейс
	interface.start();//стартовый метод для работы с программой через интерфейс

	return 0;
}