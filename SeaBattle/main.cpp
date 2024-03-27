#include "decl.h"
#include <iostream>
#include <Windows.h>

int main()
{
	using std::cout;
	using std::cin;
	using std::endl;

	const int def_navy_lim = 10;	// default navy limit - размер флотилий игроков по умолчанию
	int ready = 0;					// Счётчик игроков, готовых к игре (расставивших корабли)
	int player_num = 0;				// Количество игроков
	int turn = 0;					// Индекс очереди (показывает, очередь какого игрока пришла)
	int dialog = 0;					// Переменная для взаимодействия программы и пользователей
	ship* buffer1 = nullptr;		// Буфер для хранения данных о некотором корабле
	bool game_over = false;			// Флаг окончания игры



	cout << " Enter an amount of players. " << endl;	// Запрашивается количество игроков
	player_num = get_with_lim(2, 2);					// Принимается защищённый ввод количества (пока что можно только вдвоём)

	player* player_pool = new player[player_num];		// Инициализируются объекты игроков
	map* player_maps = new map[player_num];				// Инициализируются объекты карт игроков
	system("cls");										// Экран очищается

	
	while (!game_over)																// Здесь начинается основной игровой цикл
	{																				
		if(ready != player_num)														// Этап расстановки кораблей
		{
			cout << " Player " << ready + 1 << "'s turn." << endl;					// Показывается, очередь какого игрока настала
			player_maps[ready].draw();												// Отрисовывается карта i-того игрока
			cout << endl;

			if (player_pool[ready].need_to_place())									// Пока у игрока ещё есть неразмещённые корабли
			{
				dialog = get_with_lim(0, def_navy_lim - 1);							// Игрок выбирает, какой корабль он установит на поле сейчас

				system("cls");														// Экран обновляется
				cout << " Player " << ready + 1 << "'s turn." << endl;				//
				player_maps[ready].draw();											//
				
				int placement_state = 0;											// Создаётся переменная для сохранения сигналов метода размещения кораблей
				placement_state = player_pool[ready].placement(dialog, &buffer1);	// В неё сохраняется соответствующий сигнал
				while (placement_state)												// (Подробнее о сигналах - см. в методе placement класса player)
				{
					system("cls");													// Экран обновляется
					cout << " Player " << ready + 1 << "'s turn." << endl;			//
					player_maps[ready].draw();										//

					if (placement_state == 2)										// Если пользователь ввёл номер уже поставленного корабля
					{
						player_pool[ready].need_to_place();							// Повторно выводится список нерасставленных кораблей
						dialog = get_with_lim(0, def_navy_lim - 1);					// и игрок делает выбор

						system("cls");												// Обновляется экран
						cout << " Player " << ready + 1 << "'s turn." << endl;		//
						player_maps[ready].draw();									//
					}

					placement_state = player_pool[ready].placement(dialog, &buffer1);	// Повторно сохраняется сигнал размещения
																						// При этом сохраняется адрес размещённого корабля
				}
																					// Если корабль размещается удачно,
				player_maps[ready].save_ship(*buffer1);								// то адрес размещённого корабля используется 
																					// для сохранения этого корабля на карте.
			}
			else																	// Когда игрок разместил все корабли
			{
				player_maps[ready].game_start();									// Карта этого игрока переключается в игровой режим (см. в методе)
				ready++;															// Счётчик готовых игроков увеличивается на 1.
			}
		}
		else																		// А здесь начинается этап основной игры
		{
			buffer1 = nullptr;														// Буфер данных о корабле очищается
			int x = 0,																// Переменные для хранения координат
				y = 0;																// 
			int enemy_index = (turn + 1) % 2;										// "Индекс противника"
			int shoot_state = 0;													// Переменная для хранения сигнала метода shoot класса map

			system("cls");															// Экран обновляется
			cout << " Player " << turn + 1 << "'s turn." << endl;
			player_maps[enemy_index].draw();

			shoot_state = player_maps[enemy_index].shoot(x, y);						// Соответствующий сигнал сохраняется
			while (shoot_state)														// (Подробнее о сигналах - см. метод shoot класса map)
			{
				system("cls");														// Экран обновляется
				cout << " Player " << turn + 1 << "'s turn." << endl;
				player_maps[enemy_index].draw();

				if (shoot_state == 2)												// Если игрок попал в корабль противника
				{
					player_pool[enemy_index].search_by_pos(x, y, &buffer1);			// Найти корабль, который был повреждён
					buffer1->deck_damage(x, y);										// Изменить состояние корабля (его палубы по координатам x и y)
					buffer1->state_update();										// Обновить общий статус корабля

					if (!buffer1->check_alive())									// Если корабль был потоплен
					{
						cout << " The ship of player " << enemy_index + 1 << " has been sunk!" << endl; // Уведомить игрока об этом
						cin.get();
						player_maps[enemy_index].save_ship(*buffer1);				// Отобразить на карте потопленный корабль
					}

					if (player_pool[enemy_index].is_defeat())						// Если все корабли противника потоплены
					{
						system("cls");				
						cout << endl << " Player " << turn + 1 << " won the game!" << endl;		// Уведомить игрока о победе
						game_over = true;														// Изменить флаг окончания игры
						break;
					}

					system("cls");													// Обновить экран
					cout << " Player " << turn + 1 << "'s turn." << endl;
					player_maps[enemy_index].draw();
				}

				shoot_state = player_maps[enemy_index].shoot(x, y);					// Сохранить сигнал метода shoot
			}

			if (turn + 1 == player_num)												// Передать ход следующему игроку
				turn = 0;
			else
				turn++;
		}

		cin.get();
		system("cls");
	}


	return 0;
}