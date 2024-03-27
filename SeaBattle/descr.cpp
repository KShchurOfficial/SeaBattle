#include "decl.h"
#include <iostream>

// ship

ship::ship()
{
	deck_amt = 0;
	decks = nullptr;
	state = 0;
	placed = 0;
	orientation = 0;
}

ship::ship(int size)
{
	deck_amt = size;
	decks = new deck[deck_amt];
	for(int i = 0; i < deck_amt; i++)
	{
		decks[i].pos_y = -100;
		decks[i].pos_x = -100;
		decks[i].state = 1;
	}
	placed = 0;
	state = 1;
	orientation = 0;
}

ship::ship(const ship& orig)
{
	delete[] decks;
	deck_amt = orig.deck_amt;
	placed = orig.placed;
	state = orig.state;
	orientation = orig.orientation;

	decks = new deck[deck_amt];
	for (int i = 0; i < deck_amt; i++)
	{
		decks[i].pos_y = orig.decks[i].pos_y;
		decks[i].pos_x = orig.decks[i].pos_x;
		decks[i].state = orig.decks[i].state;
	}
}

ship::~ship()
{
	delete[] decks;
}

ship& ship::operator=(const ship& orig)
{
	if (this != &orig)
	{
		delete[] decks;
		deck_amt = orig.deck_amt;
		placed = orig.placed;
		state = orig.state;
		orientation = orig.orientation;

		decks = new deck[deck_amt];
		for (int i = 0; i < deck_amt; i++)
		{
			decks[i].pos_y = orig.decks[i].pos_y;
			decks[i].pos_x = orig.decks[i].pos_x;
			decks[i].state = orig.decks[i].state;
		}
	}
	return *this;
}

void ship::set_pos(int rear_x, int rear_y)
{
	decks[0].pos_x = rear_x; // счёт палуб начинается с кормы
	decks[0].pos_y = rear_y; // поэтому координаты кормы присваиваются первой палубе корабля
	for (int i = 1; i < deck_amt; i++) // далее каждая палуба
	{
		if (!orientation) // в зависимости от ориентации корабля
		{
			decks[i].pos_x = decks[i - 1].pos_x + 1; // получает координаты
			decks[i].pos_y = decks[i - 1].pos_y; // на основе координат предыдущей палубы
		}
		else
		{
			decks[i].pos_x = decks[i - 1].pos_x;
			decks[i].pos_y = decks[i - 1].pos_y + 1;
		}
 	}

	return;
}

void ship::change_orientation() // Изменение ориентации корабля
{
	orientation = !orientation;		// Изменить ориентацию на противоположную
	set_pos(this->decks[0].pos_x, this->decks[0].pos_y);	// Пересчитать координаты палуб корабля
	return;
}

void ship::preview()
{
	using std::cout;
	using std::endl;

	if (orientation)
	{
		for(int i = 0; i < deck_amt; i++)
			cout << " U" << endl;
	}
	else
	{
		cout << ' ';
		for (int i = 0; i < deck_amt; i++)
			cout << 'D';
		cout << endl;
	}
	return;
}

void ship::state_update()			// Обновление статуса корабля
{
	int destroyed = 0;				// Переменная-аккумулятор для подсчёта повреждённых палуб корабля
	for (int i = 0; i < deck_amt; i++)
	{
		if (decks[i].state == 0)	// Если палуба повреждена
		{
			destroyed++;			// Инкрементировать счётчик
		}
	}

	if (destroyed == deck_amt)		// Если все палубы корабля повреждены
		state = 0;					// Изменить общее состояние корабля на 0 (потоплен)

	return;
}

bool ship::check_alive()
{
	return state;
}


bool ship::pg_inside()
{
	// std::cout << std::endl << " Checkout if ship is out of playground. " << std::endl;
	for (int i = 0; i < deck_amt; i++) // проверяем каждую палубу
	{
		// std::cout << " Deck #" << i + 1 << ": (" << decks[i].pos_x << ";" << decks[i].pos_y << ")" << std::endl;
 		if (decks[i].pos_x >= map::length || decks[i].pos_y >= map::width || decks[i].pos_x < 0 || decks[i].pos_y < 0)
			return false; // Если её координаты выходят за границы карты, то корабль находится вне поля
	}
	return true;
}

bool ship::contact_check(const ship& checking) // Проверка взаимного расположения кораблей
{
	// std::cout << std::endl << " Checkout if distanse between two ships is very close." << std::endl;
	if(this != &checking)
	{
		int dzone_x1 = checking.decks[0].pos_x - 1, // определяются границы корабля, внтури которых разместить другой корабль нельзя
			dzone_y1 = checking.decks[0].pos_y - 1,
			dzone_x2 = checking.decks[checking.deck_amt - 1].pos_x + 1,
			dzone_y2 = checking.decks[checking.deck_amt - 1].pos_y + 1;

		//std::cout << " Cannot place ship in zone (" << char('A' + dzone_x1) << ";" << dzone_y1 + 1 
			//<< ") - (" << char('A' + dzone_x2) << ";" << dzone_y2 + 1 << ")" << std::endl;

		for (int i = 0; i < deck_amt; i++) // Далее, проверяется каждая палуба исходного корабля
		{
			if (decks[i].pos_x <= dzone_x2 && decks[i].pos_y <= dzone_y2 && decks[i].pos_x >= dzone_x1 && decks[i].pos_y >= dzone_y1) // Если хоть одна палуба
				return false;  // попадает в запретную зону, возвращается false, что не даёт разместить корабль с такими координатами
		}
	}
	return true; // Если ни одной такой палубы не было найдено, то размещение по таким координатам одобряется.
}

bool ship::deck_damage(int x, int y)		// нанести урон палубе корабля с координатами x и y
{
	for (int i = 0; i < deck_amt; i++)
	{
		if (decks[i].pos_x == x && decks[i].pos_y == y)
		{
			decks[i].state = false;
		}
	}
	return true;
}



// player

player::player()
{
	navy_size = 10;
	navy = new ship[10];

	// стандартная конфигурация флота

	navy[0] = ship(4);
	navy[1] = navy[2] = ship(3);
	navy[3] = navy[4] = navy[5] = ship(2);
	for (int i = 6; i < navy_size; i++)
		navy[i] = ship(1);

}

player::player(const player& orig)
{
	delete[] navy;
	navy_size = orig.navy_size;
	navy = new ship[navy_size];
	for (int i = 0; i < navy_size; i++)
		navy[i] = ship(orig.navy[i].deck_amt);

}

player::~player()
{
	delete[] navy;
}

bool player::need_to_place()				// Вывод списка неразмещённых кораблей
{
	using std::cout;
	using std::endl;

	int acc = 0;		
	for (int i = 0; i < navy_size; i++)		// Посчитать количество неразмещённых кораблей
	{
		if (!navy[i].placed)
			acc++;
	}

	if (!acc)															// Если таких кораблей нет
	{
		cout << " All ships are placed!" << endl;						// Уведомить пользователя 
		return false;													// Вернуть сигнал 0
	}
	else																// Иначе
	{
		cout << " Need to place: " << acc << " ships." << endl;			// Вывести количество
		for (int i = 0; i < navy_size; i++)								// и информацию о каждом корабле, 
		{
			if (!navy[i].placed)
			{
				cout << " #" << i << ": " << navy[i].deck_amt << " decks ship." << endl;
			}
		}
		return true;
	}
}

int player::placement(int ship_id, ship** placed_addr)				// Размещение корабля
{
	using std::cout;
	using std::cin;
	using std::endl;

	int input = 1;													// Переменная пользовательского ввода-вывода 
	bool confirmed = false;											// Переменная-флаг подтверждения размещения
	if (navy[ship_id].placed)										// Если выбранный корабль уже был размещён
	{
		cout << " This ship was placed! " << endl;					// Уведомить игрока
		cin.get();	
		return 2;													// Вернуть сигнал 2 (запросить повторный выбор корабля)
	}
	cout << endl;
	navy[ship_id].preview();										// Предварительная демонстрация корабля.
	cout << endl;
	cout << " Chosen ship: " << navy[ship_id].deck_amt << " decks ship" << endl;  // Вывод информации о корабле

	cout << " 1) change orientation" << endl						// Вывод меню настройки размещения корабля
		<< " 2) set stern coordinates" << endl
		<< " 3) Save ship position" << endl;
	input = get_with_lim(1, 3);										// Пользователь выбирает пункт из меню
	switch (input)
	{
		case 1:														// При вводе 1 
		{
			navy[ship_id].change_orientation();						// Изменить ориентацию корабля
			return 1;												// Вернуть сигнал 1 (продолжить настройку размещения корабля)
		}
		case 2:														// При вводе 2
		{
			int x, y;	
			cout << " Sуt coordinate X (A-J)." << endl;				// Запросить ввод координат
			x = char_with_lim('A', 'A' + map::width - 1);			// Тут же транслировать пользовательский ввод в индексы матрицы
			x -= int('A');
			cout << " Enter coordinate Y." << endl;
			y = get_with_lim(1, map::length);
			y--;
			navy[ship_id].set_pos(x, y);							// Сохранить введённые координаты для выбранного корабля

			return 1;												// Вернуть сигнал 1 (продолжить настройку)
		}
		case 3:														// При вводе 3
		{
			confirmed = navy[ship_id].pg_inside();					// Проверить, не выходит ли корабль за пределы игрового поля
			if (!confirmed)											// Если проверка не пройдена
			{
				cout << endl << " Error! Ship is out of playground! Change its position." << endl;		// Уведомить пользователя
				cin.get();																					
				return 1;																				// Вернуть сигнал 1
			}

			for (int i = 0; i < navy_size; i++)						// Проверить выбранный корабль со всеми размещёнными прежде
			{
				if(navy[i].placed)
				{
					confirmed = navy[ship_id].contact_check(navy[i]);	// Проверка на размещение двух кораблей вплотную
					if (!confirmed)										// Если такая пара кораблей была найдена
						break;											// то пользователю отказывается в размещении
				}
			}
			if (!confirmed)
			{
				cout << endl << " Error! Ship placed near another one! Change its position." << endl;
				cin.get();
				return 1;
			}

			cout << endl << " The ship was placed! " << endl;			// Если корабль успешно прошел все проверки
			navy[ship_id].placed = 1;									// Изменить значение флага размещения корабля на истину
			(*placed_addr) = &navy[ship_id];							// Сохранить адрес размещённого корабля
			return 0;													// Вернуть сигнал 0 (завершить настройку размещения корабля)
		}
	}
}

void player::search_by_pos(int x, int y, ship** wanted) // Поиск корабля по координатам
{
	bool found = false; // Флаг нахождения

	for (int i = 0; i < navy_size; i++)				// Проверяются все корабли
	{
		for (int j = 0; j < navy[i].deck_amt; j++)	// Проверяются все палубы корабля
		{
			if (navy[i].decks[j].pos_x == x && navy[i].decks[j].pos_y == y)	
			{										// Если переданные координаты совпадают с координатами некоторой палубы
				found = true;						// Значение флага нахождения изменяется на истину
				break;
			}
		}
		if (found)
		{
			(*wanted) = &navy[i];					// Сохранить адрес найденного корабля
			break;
		}
	}

	return;
}

bool player::is_defeat() // Проверка игрока на проигрыш
{
	bool defeat = true;
	for (int i = 0; i < navy_size; i++)
	{
		if (navy[i].check_alive())	// Если у игрока есть хотя бы один непотопленный корабль
		{
			defeat = false;			// игрок не считается проигравшим
			break;
		}
	}

	return defeat;
}



// map

map::map()
{
	playground = new char* [length];
	for (int i = 0; i < length; i++)
	{
		playground[i] = new char[width];
		for (int j = 0; j < width; j++)
			playground[i][j] = '.';
	}

	mode = 0;
}

map::map(const map& orig)
{
	for (int i = 0; i < length; i++)
		delete playground[i];
	delete playground;

	playground = new char* [orig.length];
	for (int i = 0; i < orig.length; i++)
	{
		playground[i] = new char[orig.width];
		for (int j = 0; i < orig.width; i++)
			playground[i][j] = orig.playground[i][j];
	}
	mode = orig.mode;
}

map::~map()
{
	for (int i = 0; i < width; i++)
		delete playground[i];
	delete playground;
}



int map::shoot(int& x, int& y)
{
	using std::cout;
	using std::endl;
	using std::cin;	

	cout << " Enter coordinate X (A-J)." << endl;		// Пользователь вводит координаты атакуемой ячейки
	x = char_with_lim('A', 'A' + width - 1);			// Координаты тут же транслируются в индексы ячеек матрицы
	x -= int('A');
	cout << " Enter coordinate Y (1-10)." << endl;
	y = get_with_lim(1, length);
	y--;


	if (playground[y][x] == '#' || playground[y][x] == 'X') // Если введённые координаты уже закрашены решёткой или крестиком
	{
		cout << " You've already fired into this sector!" << endl;  // Уведомить игрока об этом
		cin.get();
		return 1;													// Вернуть сигнал 1 (Повторный ввод координат)		
	}
	else													// Иначе, если по этим координатам ещё не производился выстрел
	{
		switch (playground[y][x])							// Проанализировать содержимое ячейки
		{
			case '.':										// Если ячейка пустая
			{
				playground[y][x] = '#';						// Закрасить её решёткой
				cout << " You missed." << endl;				// Уведомить игрока о промахе
				return 0;									// Вернуть сигнал 0 (передача хода следующему игроку)
			}
			case 'U':										// Если в этой ячейке находится палуба корабля
			case 'D':
			{
				playground[y][x] = 'X';						// Закрасить ячейку крестиком
				cout << " Nice shot! " << endl;				// Уведомить игрока о попадании в корабль врага
				cin.get();
				return 2;									// Отправить во внешнюю программу сигнал 2 (сообщить о попадании в палубу корабля)
			}
		}
	}
}



void map::save_ship(const ship& boat)
{
	if (boat.state)															// Eсли корабль не потоплен
	{
		if (boat.orientation)												// В зависимости от ориентации
		{
			for (int j = 0; j < boat.deck_amt; j++) 
			{
				playground[boat.decks[j].pos_y][boat.decks[j].pos_x] = 'U'; // Отрисовать все палубы корабля
			}
		}
		else
		{
			for (int j = 0; j < boat.deck_amt; j++)
			{
				playground[boat.decks[j].pos_y][boat.decks[j].pos_x] = 'D';
			}
		}
	}
	else									// Если корабль уже потоплен,
	{
		int painted_x1 = 0,					// Определить координаты двух угловых противолежащих ячеек области рядом с кораблём
			painted_y1 = 0,
			painted_x2 = 0,
			painted_y2 = 0;

		if (boat.decks[0].pos_x != 0) 
			painted_x1 = boat.decks[0].pos_x - 1;
		if (boat.decks[0].pos_y != 0)
			painted_y1 = boat.decks[0].pos_y - 1;

		if (boat.decks[boat.deck_amt - 1].pos_x != (map::width - 1))
			painted_x2 = boat.decks[boat.deck_amt - 1].pos_x + 1;
		if (boat.decks[boat.deck_amt - 1].pos_y != (map::length - 1))
			painted_y2 = boat.decks[boat.deck_amt - 1].pos_y + 1;

		for (int i = painted_x1; i <= painted_x2; i++) 
		{
			for (int j = painted_y1; j <= painted_y2; j++)
			{
				if (playground[j][i] == '.')// Закрасить все ячейки рядом с потопленным кораблём
				{
					playground[j][i] = '#';
				}
			}
		}

	}

	return;
}

void map::game_start() // Перевести карту в режим игры
{
	mode = 1;
	return;
}

void map::draw() // Отображение карты в консоли
{
	using std::cout;
	using std::endl;

	cout << "   ";								// Выводится строка горизонтальных координат
	for (int i = 0; i < width; i++)
	{
		cout << ' ' << char('A' + i) << ' ';
	}
	cout << endl;

	for (int i = 0; i < length; i++)			// Затем выводятся вертикальные координаты и содержимое каждой ячейки поля
	{
		if (i + 1 < 100)
			cout << ' ';
		cout << i + 1;
		if (i + 1 < 10)
			cout << ' ';

		for (int j = 0; j < width; j++)
		{
			switch (playground[i][j])
			{
				case 'U':
				case 'D':
				{
					if (!mode)									// Если основная игра ещё не началась (игроки размещают корабли на карте)
					{
						cout << ' ' << playground[i][j] << ' '; // Тогда корабли отображаются на карте
					}
					else
						cout << " . ";							// Иначе, корабли скрываются
					break;
				}
				default:	
				{
					cout << ' ' << playground[i][j] << ' ';
				}
			}
		}
		cout << endl;
	}
}


int get_with_lim(int bottom, int top)	// Защищённый ввод целых чисел
{
	using std::cout;
	using std::cin;
	using std::endl;

	int input = 0;
	bool invalid = true;

	while (invalid)
	{
		cout << " Your input: ";
		cin >> input;
		cin.ignore();

		if (!cin)
		{
			cin.clear();
			cin.ignore();

			cout << " You should enter the number!" << endl;
			continue;
		}

		if (input < bottom || input > top)
		{
			cout << " You should enter the value more than " << bottom;
			if (top != INT_MAX)
				cout << " and less than " << top;
			continue;
		}

		invalid = false;
	}

	return input;
}


char char_with_lim(char bottom, char top) // Защищённый символьный ввод
{
	using std::cout;
	using std::cin;
	using std::endl;

	char input = 0;
	bool invalid = true;

	while (invalid)
	{
		cout << " Your input: ";
		input = cin.get();
		cin.ignore();

		if (!cin)
		{
			cin.clear();

			cout << " You should enter the symbol!" << endl;
			continue;
		}

		if (input < bottom || input > top)
		{
			cout << " You should enter the symbol after " << bottom;
			if (top != CHAR_MAX)
				cout << " and before " << top;
			continue;
		}

		invalid = false;
	}

	return input;
}
