
#include "Container.h"
#include <iostream>
#include <clocale>

int main(int argc, const char **argv)
{
	std::setlocale(LC_ALL, "Russian");

	Container container{ Person("Иванов", "Иван", 3515454),
	Person("Сидоров", "Иван", 3515422),
	Person("Иванов", "Иван", 3515423),
	Person("Иванов", "Иван", 3515455) };



	char in;
	constexpr char sort_names = '1',
		sort_surnames = '2',
		sort_id = '3';

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
		case sort_surnames:
		case sort_id:
		default:
			break;
		}

	} while (in != 'q' &&
		in != 'Q');

	return 0;
}