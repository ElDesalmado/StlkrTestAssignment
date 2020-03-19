
#include "Container.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <clocale>

constexpr char sort_names = '1',
sort_surnames = '2',
sort_id = '3';

bool SortName(const Person& p1, const Person& p2);
bool SortSurname(const Person& p1, const Person& p2);
bool SortID(const Person& p1, const Person& p2);

std::function<bool(const Person&, const Person&)> GetSorting(char sort);

int main(int argc, const char **argv)
{
	std::setlocale(LC_ALL, "Russian");

	Container container;

	if (argc == 2)
	{
		std::filesystem::path sourcePath{ argv[1] };
		if (!std::filesystem::exists(sourcePath))
		{
			std::cerr << "Error: " << sourcePath << " does not exist!" << std::endl;
			return -1;
		}

		std::ifstream file{ sourcePath, std::ifstream::in };
		file.imbue(std::locale("Russian"));
		if (!file)
		{
			std::cerr << "Error: Failed to open the source file" << std::endl;
			return -1;
		}

		std::string name, 
			surname;
		size_t id;
		
		while (!(file >> std::ws).eof())
		{
			file >> name >> surname >> id;
			if (file.rdstate() == std::istream::failbit)
			{
				std::cerr << "Error: Invalid file format!" << std::endl;
				return -1;
			}
			container.Insert(Person(name, surname, id));
		}
	}
	else
	{
		std::cout << "Для использования исходного файла с данными:\n"
			"./ConsoleSort *Путь к файлу с данными*\n\n"
			"Демонстрация. Исходные данные:" << std::endl;

		container = Container(Person("Иванов", "Иван", 3515454),
			Person("Сидоров", "Иван", 3515422),
			Person("Сидоров", "Олег", 3515423),
			Person("Петров", "Иван", 3515455));

		
		for (size_t i = 0; i != container.Size(); ++i)
			std::cout << container[i].Name() << " " <<
				container[i].Surname() << " " <<
				container[i].ID() <<
				std::endl;
				
		std::cout << std::endl;
	}
	
	ProxyContainer proxy{ container };

	char in;
	do
	{
		std::cout << "Для сортировки по именам введите " << sort_names <<
			", по фамилиям - введите " << sort_surnames <<
			", по ID - введите " << sort_id << ".\nДля выхода введите q или Q." << std::endl;
		std::cout << ">> ";
		std::cin >> in;

		// clear the buffer
		std::cin.seekg(0, std::ios::end);
		std::cin.clear();

		switch (in)
		{
		case sort_names:
			proxy.SetSorting(GetSorting(in));
			for (size_t i = 0; i != proxy.Size(); ++i)
			{
				std::cout << proxy[i].Name() << " " <<
					proxy[i].Surname() << " " <<
					proxy[i].ID() << 
					std::endl;
			}
			break;
		case sort_surnames:
			proxy.SetSorting(GetSorting(in));
			for (size_t i = 0; i != proxy.Size(); ++i)
			{
				std::cout << proxy[i].Surname() << " " <<
					proxy[i].Name() << " " <<
					proxy[i].ID() << 
					std::endl;
			}
			break;
		case sort_id:
			proxy.SetSorting(GetSorting(in));
			for (size_t i = 0; i != proxy.Size(); ++i)
			{
				std::cout << proxy[i].ID() << " " <<
					proxy[i].Surname() << " " <<
					proxy[i].Name() <<
					std::endl;
			}
			break;
		default:
			break;
		}

	} while (in != 'q' &&
		in != 'Q');
		
	return 0;
}


bool SortName(const Person& p1, const Person& p2)
{
	size_t id1 = p1.ID(),
		id2 = p2.ID();

	return std::tie(p1.Name(), p1.Surname(), id1) <
		std::tie(p2.Name(), p2.Surname(), id2);
}

bool SortSurname(const Person& p1, const Person& p2)
{
	// default order
	return p1 < p2;
}

bool SortID(const Person& p1, const Person& p2)
{
	size_t id1 = p1.ID(),
		id2 = p2.ID();

	return std::tie(id1, p1.Surname(), p1.Name()) <
		std::tie(id2, p2.Surname(), p2.Name());
}

std::function<bool(const Person&, const Person&)> GetSorting(char sort)
{
	switch (sort)
	{
	case sort_names:
		return &SortName;
	case sort_surnames:
		return &SortSurname;
	case sort_id:
		return &SortID;
	default:
		throw std::invalid_argument("Sorting option is not supported");
	}
}