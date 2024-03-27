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
	decks[0].pos_x = rear_x; // ���� ����� ���������� � �����
	decks[0].pos_y = rear_y; // ������� ���������� ����� ������������� ������ ������ �������
	for (int i = 1; i < deck_amt; i++) // ����� ������ ������
	{
		if (!orientation) // � ����������� �� ���������� �������
		{
			decks[i].pos_x = decks[i - 1].pos_x + 1; // �������� ����������
			decks[i].pos_y = decks[i - 1].pos_y; // �� ������ ��������� ���������� ������
		}
		else
		{
			decks[i].pos_x = decks[i - 1].pos_x;
			decks[i].pos_y = decks[i - 1].pos_y + 1;
		}
 	}

	return;
}

void ship::change_orientation() // ��������� ���������� �������
{
	orientation = !orientation;		// �������� ���������� �� ���������������
	set_pos(this->decks[0].pos_x, this->decks[0].pos_y);	// ����������� ���������� ����� �������
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

void ship::state_update()			// ���������� ������� �������
{
	int destroyed = 0;				// ����������-����������� ��� �������� ����������� ����� �������
	for (int i = 0; i < deck_amt; i++)
	{
		if (decks[i].state == 0)	// ���� ������ ����������
		{
			destroyed++;			// ���������������� �������
		}
	}

	if (destroyed == deck_amt)		// ���� ��� ������ ������� ����������
		state = 0;					// �������� ����� ��������� ������� �� 0 (��������)

	return;
}

bool ship::check_alive()
{
	return state;
}


bool ship::pg_inside()
{
	// std::cout << std::endl << " Checkout if ship is out of playground. " << std::endl;
	for (int i = 0; i < deck_amt; i++) // ��������� ������ ������
	{
		// std::cout << " Deck #" << i + 1 << ": (" << decks[i].pos_x << ";" << decks[i].pos_y << ")" << std::endl;
 		if (decks[i].pos_x >= map::length || decks[i].pos_y >= map::width || decks[i].pos_x < 0 || decks[i].pos_y < 0)
			return false; // ���� � ���������� ������� �� ������� �����, �� ������� ��������� ��� ����
	}
	return true;
}

bool ship::contact_check(const ship& checking) // �������� ��������� ������������ ��������
{
	// std::cout << std::endl << " Checkout if distanse between two ships is very close." << std::endl;
	if(this != &checking)
	{
		int dzone_x1 = checking.decks[0].pos_x - 1, // ������������ ������� �������, ������ ������� ���������� ������ ������� ������
			dzone_y1 = checking.decks[0].pos_y - 1,
			dzone_x2 = checking.decks[checking.deck_amt - 1].pos_x + 1,
			dzone_y2 = checking.decks[checking.deck_amt - 1].pos_y + 1;

		//std::cout << " Cannot place ship in zone (" << char('A' + dzone_x1) << ";" << dzone_y1 + 1 
			//<< ") - (" << char('A' + dzone_x2) << ";" << dzone_y2 + 1 << ")" << std::endl;

		for (int i = 0; i < deck_amt; i++) // �����, ����������� ������ ������ ��������� �������
		{
			if (decks[i].pos_x <= dzone_x2 && decks[i].pos_y <= dzone_y2 && decks[i].pos_x >= dzone_x1 && decks[i].pos_y >= dzone_y1) // ���� ���� ���� ������
				return false;  // �������� � ��������� ����, ������������ false, ��� �� ��� ���������� ������� � ������ ������������
		}
	}
	return true; // ���� �� ����� ����� ������ �� ���� �������, �� ���������� �� ����� ����������� ����������.
}

bool ship::deck_damage(int x, int y)		// ������� ���� ������ ������� � ������������ x � y
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

	// ����������� ������������ �����

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

bool player::need_to_place()				// ����� ������ ������������� ��������
{
	using std::cout;
	using std::endl;

	int acc = 0;		
	for (int i = 0; i < navy_size; i++)		// ��������� ���������� ������������� ��������
	{
		if (!navy[i].placed)
			acc++;
	}

	if (!acc)															// ���� ����� �������� ���
	{
		cout << " All ships are placed!" << endl;						// ��������� ������������ 
		return false;													// ������� ������ 0
	}
	else																// �����
	{
		cout << " Need to place: " << acc << " ships." << endl;			// ������� ����������
		for (int i = 0; i < navy_size; i++)								// � ���������� � ������ �������, 
		{
			if (!navy[i].placed)
			{
				cout << " #" << i << ": " << navy[i].deck_amt << " decks ship." << endl;
			}
		}
		return true;
	}
}

int player::placement(int ship_id, ship** placed_addr)				// ���������� �������
{
	using std::cout;
	using std::cin;
	using std::endl;

	int input = 1;													// ���������� ����������������� �����-������ 
	bool confirmed = false;											// ����������-���� ������������� ����������
	if (navy[ship_id].placed)										// ���� ��������� ������� ��� ��� ��������
	{
		cout << " This ship was placed! " << endl;					// ��������� ������
		cin.get();	
		return 2;													// ������� ������ 2 (��������� ��������� ����� �������)
	}
	cout << endl;
	navy[ship_id].preview();										// ��������������� ������������ �������.
	cout << endl;
	cout << " Chosen ship: " << navy[ship_id].deck_amt << " decks ship" << endl;  // ����� ���������� � �������

	cout << " 1) change orientation" << endl						// ����� ���� ��������� ���������� �������
		<< " 2) set stern coordinates" << endl
		<< " 3) Save ship position" << endl;
	input = get_with_lim(1, 3);										// ������������ �������� ����� �� ����
	switch (input)
	{
		case 1:														// ��� ����� 1 
		{
			navy[ship_id].change_orientation();						// �������� ���������� �������
			return 1;												// ������� ������ 1 (���������� ��������� ���������� �������)
		}
		case 2:														// ��� ����� 2
		{
			int x, y;	
			cout << " S�t coordinate X (A-J)." << endl;				// ��������� ���� ���������
			x = char_with_lim('A', 'A' + map::width - 1);			// ��� �� ������������� ���������������� ���� � ������� �������
			x -= int('A');
			cout << " Enter coordinate Y." << endl;
			y = get_with_lim(1, map::length);
			y--;
			navy[ship_id].set_pos(x, y);							// ��������� �������� ���������� ��� ���������� �������

			return 1;												// ������� ������ 1 (���������� ���������)
		}
		case 3:														// ��� ����� 3
		{
			confirmed = navy[ship_id].pg_inside();					// ���������, �� ������� �� ������� �� ������� �������� ����
			if (!confirmed)											// ���� �������� �� ��������
			{
				cout << endl << " Error! Ship is out of playground! Change its position." << endl;		// ��������� ������������
				cin.get();																					
				return 1;																				// ������� ������ 1
			}

			for (int i = 0; i < navy_size; i++)						// ��������� ��������� ������� �� ����� ������������ ������
			{
				if(navy[i].placed)
				{
					confirmed = navy[ship_id].contact_check(navy[i]);	// �������� �� ���������� ���� �������� ��������
					if (!confirmed)										// ���� ����� ���� �������� ���� �������
						break;											// �� ������������ ������������ � ����������
				}
			}
			if (!confirmed)
			{
				cout << endl << " Error! Ship placed near another one! Change its position." << endl;
				cin.get();
				return 1;
			}

			cout << endl << " The ship was placed! " << endl;			// ���� ������� ������� ������ ��� ��������
			navy[ship_id].placed = 1;									// �������� �������� ����� ���������� ������� �� ������
			(*placed_addr) = &navy[ship_id];							// ��������� ����� ������������ �������
			return 0;													// ������� ������ 0 (��������� ��������� ���������� �������)
		}
	}
}

void player::search_by_pos(int x, int y, ship** wanted) // ����� ������� �� �����������
{
	bool found = false; // ���� ����������

	for (int i = 0; i < navy_size; i++)				// ����������� ��� �������
	{
		for (int j = 0; j < navy[i].deck_amt; j++)	// ����������� ��� ������ �������
		{
			if (navy[i].decks[j].pos_x == x && navy[i].decks[j].pos_y == y)	
			{										// ���� ���������� ���������� ��������� � ������������ ��������� ������
				found = true;						// �������� ����� ���������� ���������� �� ������
				break;
			}
		}
		if (found)
		{
			(*wanted) = &navy[i];					// ��������� ����� ���������� �������
			break;
		}
	}

	return;
}

bool player::is_defeat() // �������� ������ �� ��������
{
	bool defeat = true;
	for (int i = 0; i < navy_size; i++)
	{
		if (navy[i].check_alive())	// ���� � ������ ���� ���� �� ���� ������������� �������
		{
			defeat = false;			// ����� �� ��������� �����������
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

	cout << " Enter coordinate X (A-J)." << endl;		// ������������ ������ ���������� ��������� ������
	x = char_with_lim('A', 'A' + width - 1);			// ���������� ��� �� ������������� � ������� ����� �������
	x -= int('A');
	cout << " Enter coordinate Y (1-10)." << endl;
	y = get_with_lim(1, length);
	y--;


	if (playground[y][x] == '#' || playground[y][x] == 'X') // ���� �������� ���������� ��� ��������� �������� ��� ���������
	{
		cout << " You've already fired into this sector!" << endl;  // ��������� ������ �� ����
		cin.get();
		return 1;													// ������� ������ 1 (��������� ���� ���������)		
	}
	else													// �����, ���� �� ���� ����������� ��� �� ������������ �������
	{
		switch (playground[y][x])							// ���������������� ���������� ������
		{
			case '.':										// ���� ������ ������
			{
				playground[y][x] = '#';						// ��������� � ��������
				cout << " You missed." << endl;				// ��������� ������ � �������
				return 0;									// ������� ������ 0 (�������� ���� ���������� ������)
			}
			case 'U':										// ���� � ���� ������ ��������� ������ �������
			case 'D':
			{
				playground[y][x] = 'X';						// ��������� ������ ���������
				cout << " Nice shot! " << endl;				// ��������� ������ � ��������� � ������� �����
				cin.get();
				return 2;									// ��������� �� ������� ��������� ������ 2 (�������� � ��������� � ������ �������)
			}
		}
	}
}



void map::save_ship(const ship& boat)
{
	if (boat.state)															// E��� ������� �� ��������
	{
		if (boat.orientation)												// � ����������� �� ����������
		{
			for (int j = 0; j < boat.deck_amt; j++) 
			{
				playground[boat.decks[j].pos_y][boat.decks[j].pos_x] = 'U'; // ���������� ��� ������ �������
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
	else									// ���� ������� ��� ��������,
	{
		int painted_x1 = 0,					// ���������� ���������� ���� ������� �������������� ����� ������� ����� � �������
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
				if (playground[j][i] == '.')// ��������� ��� ������ ����� � ����������� �������
				{
					playground[j][i] = '#';
				}
			}
		}

	}

	return;
}

void map::game_start() // ��������� ����� � ����� ����
{
	mode = 1;
	return;
}

void map::draw() // ����������� ����� � �������
{
	using std::cout;
	using std::endl;

	cout << "   ";								// ��������� ������ �������������� ���������
	for (int i = 0; i < width; i++)
	{
		cout << ' ' << char('A' + i) << ' ';
	}
	cout << endl;

	for (int i = 0; i < length; i++)			// ����� ��������� ������������ ���������� � ���������� ������ ������ ����
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
					if (!mode)									// ���� �������� ���� ��� �� �������� (������ ��������� ������� �� �����)
					{
						cout << ' ' << playground[i][j] << ' '; // ����� ������� ������������ �� �����
					}
					else
						cout << " . ";							// �����, ������� ����������
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


int get_with_lim(int bottom, int top)	// ���������� ���� ����� �����
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


char char_with_lim(char bottom, char top) // ���������� ���������� ����
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
