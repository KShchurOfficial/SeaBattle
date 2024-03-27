#ifndef decl
#define decl

#include <climits>

class map;
class player;

class ship
{
private:
	friend map;
	friend player;

	struct deck
	{
		int pos_x;
		int pos_y;
		bool state; // 0 - палуба уничтожена, 1 - палуба цела
	};

	int deck_amt; // кол-во палуб
	deck* decks; // информаци€ о палубах
	bool state; // 0 - корабль потоплен, 1 - корабль цел
	bool placed; // 0 - не размещЄн, 1 - размещЄн;
	bool orientation; // 0 - горизонтально, 1 - вертикально;

public:
	ship();
	ship(int size);
	ship(const ship& orig);
	~ship();

	ship& operator=(const ship& orig);

	void preview();
	void state_update();
	bool check_alive();
	void set_pos(int rear_x, int rear_y);
	void change_orientation();
	bool pg_inside();
	bool contact_check(const ship& checking);
	bool deck_damage(int x, int y);
};


class player
{
private:
	friend map;
	int navy_size;
	ship* navy;

public:
	player();
	player(const player& orig);
	~player();

	bool need_to_place();
	int placement(int ship_id, ship** placed_addr);
	void search_by_pos(int x, int y, ship** wanted);
	bool is_defeat();
};


class map
{
private:
	friend ship;
	friend player;

	enum { length = 10, width = 10 };
	char** playground; // информаци€ о каждой €чейке пол€
	int mode; // 0 - размещение кораблей, 1 - основна€ игра

public:
	map();
	map(const map& orig);
	~map();

	int shoot(int& x, int& y);
	void save_ship(const ship& boat);
	void game_start();
	void draw();

};

int get_with_lim(int bottom = INT_MIN, int top = INT_MAX);
char char_with_lim(char bottom = CHAR_MIN, char top = CHAR_MAX);


#endif