#include "decl.h"
#include <iostream>
#include <Windows.h>

int main()
{
	using std::cout;
	using std::cin;
	using std::endl;

	const int def_navy_lim = 10;	// default navy limit - ������ �������� ������� �� ���������
	int ready = 0;					// ������� �������, ������� � ���� (������������ �������)
	int player_num = 0;				// ���������� �������
	int turn = 0;					// ������ ������� (����������, ������� ������ ������ ������)
	int dialog = 0;					// ���������� ��� �������������� ��������� � �������������
	ship* buffer1 = nullptr;		// ����� ��� �������� ������ � ��������� �������
	bool game_over = false;			// ���� ��������� ����



	cout << " Enter an amount of players. " << endl;	// ������������� ���������� �������
	player_num = get_with_lim(2, 2);					// ����������� ���������� ���� ���������� (���� ��� ����� ������ �����)

	player* player_pool = new player[player_num];		// ���������������� ������� �������
	map* player_maps = new map[player_num];				// ���������������� ������� ���� �������
	system("cls");										// ����� ���������

	
	while (!game_over)																// ����� ���������� �������� ������� ����
	{																				
		if(ready != player_num)														// ���� ����������� ��������
		{
			cout << " Player " << ready + 1 << "'s turn." << endl;					// ������������, ������� ������ ������ �������
			player_maps[ready].draw();												// �������������� ����� i-���� ������
			cout << endl;

			if (player_pool[ready].need_to_place())									// ���� � ������ ��� ���� ������������� �������
			{
				dialog = get_with_lim(0, def_navy_lim - 1);							// ����� ��������, ����� ������� �� ��������� �� ���� ������

				system("cls");														// ����� �����������
				cout << " Player " << ready + 1 << "'s turn." << endl;				//
				player_maps[ready].draw();											//
				
				int placement_state = 0;											// �������� ���������� ��� ���������� �������� ������ ���������� ��������
				placement_state = player_pool[ready].placement(dialog, &buffer1);	// � �� ����������� ��������������� ������
				while (placement_state)												// (��������� � �������� - ��. � ������ placement ������ player)
				{
					system("cls");													// ����� �����������
					cout << " Player " << ready + 1 << "'s turn." << endl;			//
					player_maps[ready].draw();										//

					if (placement_state == 2)										// ���� ������������ ��� ����� ��� ������������� �������
					{
						player_pool[ready].need_to_place();							// �������� ��������� ������ ��������������� ��������
						dialog = get_with_lim(0, def_navy_lim - 1);					// � ����� ������ �����

						system("cls");												// ����������� �����
						cout << " Player " << ready + 1 << "'s turn." << endl;		//
						player_maps[ready].draw();									//
					}

					placement_state = player_pool[ready].placement(dialog, &buffer1);	// �������� ����������� ������ ����������
																						// ��� ���� ����������� ����� ������������ �������
				}
																					// ���� ������� ����������� ������,
				player_maps[ready].save_ship(*buffer1);								// �� ����� ������������ ������� ������������ 
																					// ��� ���������� ����� ������� �� �����.
			}
			else																	// ����� ����� ��������� ��� �������
			{
				player_maps[ready].game_start();									// ����� ����� ������ ������������� � ������� ����� (��. � ������)
				ready++;															// ������� ������� ������� ������������� �� 1.
			}
		}
		else																		// � ����� ���������� ���� �������� ����
		{
			buffer1 = nullptr;														// ����� ������ � ������� ���������
			int x = 0,																// ���������� ��� �������� ���������
				y = 0;																// 
			int enemy_index = (turn + 1) % 2;										// "������ ����������"
			int shoot_state = 0;													// ���������� ��� �������� ������� ������ shoot ������ map

			system("cls");															// ����� �����������
			cout << " Player " << turn + 1 << "'s turn." << endl;
			player_maps[enemy_index].draw();

			shoot_state = player_maps[enemy_index].shoot(x, y);						// ��������������� ������ �����������
			while (shoot_state)														// (��������� � �������� - ��. ����� shoot ������ map)
			{
				system("cls");														// ����� �����������
				cout << " Player " << turn + 1 << "'s turn." << endl;
				player_maps[enemy_index].draw();

				if (shoot_state == 2)												// ���� ����� ����� � ������� ����������
				{
					player_pool[enemy_index].search_by_pos(x, y, &buffer1);			// ����� �������, ������� ��� ��������
					buffer1->deck_damage(x, y);										// �������� ��������� ������� (��� ������ �� ����������� x � y)
					buffer1->state_update();										// �������� ����� ������ �������

					if (!buffer1->check_alive())									// ���� ������� ��� ��������
					{
						cout << " The ship of player " << enemy_index + 1 << " has been sunk!" << endl; // ��������� ������ �� ����
						cin.get();
						player_maps[enemy_index].save_ship(*buffer1);				// ���������� �� ����� ����������� �������
					}

					if (player_pool[enemy_index].is_defeat())						// ���� ��� ������� ���������� ���������
					{
						system("cls");				
						cout << endl << " Player " << turn + 1 << " won the game!" << endl;		// ��������� ������ � ������
						game_over = true;														// �������� ���� ��������� ����
						break;
					}

					system("cls");													// �������� �����
					cout << " Player " << turn + 1 << "'s turn." << endl;
					player_maps[enemy_index].draw();
				}

				shoot_state = player_maps[enemy_index].shoot(x, y);					// ��������� ������ ������ shoot
			}

			if (turn + 1 == player_num)												// �������� ��� ���������� ������
				turn = 0;
			else
				turn++;
		}

		cin.get();
		system("cls");
	}


	return 0;
}