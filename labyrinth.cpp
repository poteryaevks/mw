﻿
#include <iostream>
#include <cstring>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <iterator>


using namespace std;

struct graph_node
{
	int num;
	int weight;
	bool go_here;
};

void add_node(list<graph_node> &nodes, int n, bool go);
void check(list<list<graph_node>> g, int kirk, stack<int> &w_t_g);

int get_num(pair<int, int> p, int size_c);
char* calc_dir(int from, int to);

void add_neighbours(string* map, int size_r, int size_c, pair<int, int> cr, list<graph_node> &nodes);
void add(string* m, int size_r, int size_c, int i, int j, list<list<graph_node>> &g);
stack<int> find_way(list<list<graph_node>> &g, int start, int finish);

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/

int main()
{
	int R; // number of rows.
	int C; // number of columns.
	int A; // number of rounds between the time the alarm countdown is activated and the time the alarm goes off.
	cin >> R >> C >> A; cin.ignore();

	list<list<graph_node>> graph; //граф
	pair<int, int> room_coordinate;
	pair<int, int> init_coordinate;

	bool found = false;
	bool back = false;

	// game loop
	while (1) {
		string map[30]; //карта 
		stack<int> where_to_go;

		int KR; // row where Kirk is located.
		int KC; // column where Kirk is located.
		cin >> KR >> KC; cin.ignore();

		for (int i = 0; i < R; i++)
		{
			string ROW; // C of the characters in '#.TC?' (i.e. one line of the ASCII maze).
			cin >> ROW; cin.ignore();
			map[i] = ROW;
		}

		if (KR == room_coordinate.first
			&& KC == room_coordinate.second)
			back = true;

		//graph init
		for (int i = 0; i < R; i++)
		{
			for (int j = 0; j < C; j++)
			{
				if (map[i][j] != '#'
					&& map[i][j] != '?')
				{

					if (map[i][j] == 'C')
					{
						//room
						found = true;
						room_coordinate.first = i;
						room_coordinate.second = j;
					}

					//init pos
					if (map[i][j] == 'T')
					{
						init_coordinate.first = i;
						init_coordinate.second = j;
					}
					add(map, R, C, i, j, graph);
				}
			}
		}
		check(graph, get_num({ KR, KC }, C), where_to_go);

		if (back)
		{
			int go_to = get_num(init_coordinate, C);
			int go_from = get_num({ KR, KC }, C);
			stack<int> st = find_way(graph, go_from, go_to);
			char *c = calc_dir(go_from, st.top());
			cout << c;
		}
		else if (found && where_to_go.size() == 0)
		{
			int go_to = get_num(room_coordinate, C);
			int go_from = get_num({ KR, KC }, C);
			stack<int> st = find_way(graph, go_from, go_to);
			char *c = calc_dir(go_from, st.top());
			cout << c;
		}
		else
		{
			int go_to = where_to_go.top();
			int go_from = get_num({ KR, KC }, C);
			stack<int> st = find_way(graph, go_from, go_to);
			char *c = calc_dir(go_from, st.top());
			cout << c;
		}
		cout << endl;
		graph.clear();
	}
}



void add(string* m, int size_r, int size_c, int i, int j, list<list<graph_node>> &g)
{
	list<graph_node> temp_node;
	int temp_num = get_num({ i, j }, size_c);
	bool go = false;

	if ((i != 0 && m[i - 1][j] == '?')
		|| (j != 0 && m[i][j - 1] == '?')
		|| (j != size_c && m[i][j + 1] == '?')
		|| (i != size_r && m[i + 1][j] == '?'))
	{
		go = true;
	}
	add_node(temp_node, temp_num, go);
	add_neighbours(m, size_r, size_c, { i, j }, temp_node);
	g.push_back(temp_node);
}


void check(list<list<graph_node>> g, int kirk, stack<int> &w_t_g)
{
	for (list<list<graph_node>> ::iterator it_g = g.begin(); it_g != g.end(); it_g++)
	{
		list<graph_node> ::iterator it_n = (*it_g).begin();
		if ((*it_n).go_here)
		{
			stack<int> st;
			int go_from = kirk;
			st = find_way(g, go_from, (*it_n).num);
			if (st.size() != 0)
			{
				w_t_g.push((*it_n).num);
			}
		}
	}
}


void add_node(list<graph_node> &nodes, int n, bool go)
{
	graph_node temp_elem;
	temp_elem.go_here = go;
	temp_elem.num = n;
	nodes.push_back(temp_elem);
}


void add_neighbours(string* map, int size_r, int size_c, pair<int, int> cr, list<graph_node> &nodes)
{
	if (cr.first != 0)
	{
		//UP
		if (map[cr.first - 1][cr.second] == '.'
			|| map[cr.first - 1][cr.second] == 'C'
			|| map[cr.first - 1][cr.second] == 'T')
		{
			pair<int, int> temp_cr(cr.first - 1, cr.second);
			int temp_num = get_num(temp_cr, size_c);
			add_node(nodes, temp_num, false);
		}
	}
	if (cr.second != 0)
	{
		//LEFT
		if (map[cr.first][cr.second - 1] == '.'
			|| map[cr.first][cr.second - 1] == 'C'
			|| map[cr.first][cr.second - 1] == 'T')
		{
			pair<int, int> temp_cr(cr.first, cr.second - 1);
			int temp_num = get_num(temp_cr, size_c);
			add_node(nodes, temp_num, false);
		}
	}
	if (cr.second < size_c - 1)
	{
		//RIGHT
		if (map[cr.first][cr.second + 1] == '.'
			|| map[cr.first][cr.second + 1] == 'C'
			|| map[cr.first][cr.second + 1] == 'T')
		{
			pair<int, int> temp_cr(cr.first, cr.second + 1);
			int temp_num = get_num(temp_cr, size_c);
			add_node(nodes, temp_num, false);
		}
	}
	if (cr.first < size_r - 1)
	{
		//DOWN
		if (map[cr.first + 1][cr.second] == '.'
			|| map[cr.first + 1][cr.second] == 'C'
			|| map[cr.first + 1][cr.second] == 'T')
		{
			pair<int, int> temp_cr(cr.first + 1, cr.second);
			int temp_num = get_num(temp_cr, size_c);
			add_node(nodes, temp_num, false);
		}
	}
}


stack<int> find_way(list<list<graph_node>> &g, int start, int finish)
{
	stack<int> way;

	int temp_weight = 0;
	int current_num = start;

	map <int, int> num_weight = { { current_num, temp_weight } };

	int counter = 0;
	int exp = 1;
	queue<int> for_counter;
	queue<int> que;

	list<list<graph_node>> ::iterator it_g = g.begin();
	(*(*it_g).begin()).weight = temp_weight;
	temp_weight++;
	do
	{
		int temp_num = (*(*it_g).begin()).num;
		if (current_num == temp_num)
		{
			exp--;
			for (list<graph_node> ::iterator it_n = (*it_g).begin(); it_n != (*it_g).end(); it_n++)
			{
				map <int, int> ::iterator it;
				it = num_weight.find((*it_n).num);

				if (it == (num_weight.end()))
				{
					//заносим в map
					num_weight.insert(make_pair((*it_n).num, temp_weight));
					(*it_n).weight = temp_weight;
					que.push((*it_n).num);
					counter++;
				}
				else
					(*it_n).weight = it->second;
			}

			if (counter)
				for_counter.push(counter);

			if (exp == 0 && !que.empty())
			{
				exp = for_counter.front();
				for_counter.pop();
				temp_weight++;
			}

			//если очередь пуста - обход графа закончен
			if (que.empty())
			{
				map <int, int> ::iterator it;
				it = num_weight.find(finish);
				if (it == (num_weight.end()))
				{
					return way;
				}
				break;
			}

			//если не пуста - берем соседа из очереди
			current_num = que.front();
			que.pop();

			//сброс счетчика
			counter = 0;

			//сброс итератора
			it_g = g.begin();
		}
		else
		{
			//если не нашли, то идем дальше 
			++it_g;
		}
	} while (it_g != g.end());


	current_num = finish;
	way.push(finish);
	it_g = g.begin();
	do
	{
		int temp_num = (*(*it_g).begin()).num;
		if (current_num == temp_num)
		{
			list<graph_node> ::iterator it_n = (*it_g).begin();
			int min = (*it_n).weight;
			it_n++;
			int temp;
			for (it_n; it_n != (*it_g).end(); it_n++)
			{
				if ((*it_n).weight < min)
				{
					min = (*it_n).weight;
					temp = (*it_n).num;
				}
			}
			way.push(temp);

			if (min == 0)
			{
				way.pop();
				return way;
			}
			it_g = g.begin();
			current_num = temp;
		}

		else
		{
			//если не нашли, то идем дальше 
			++it_g;
		}
	} while (it_g != g.end());
}



char* calc_dir(int from, int to)
{
	if (from + 30 == to)
		return "DOWN";
	else if (from - 30 == to)
		return "UP";
	else if (from + 1 == to)
		return "RIGHT";
	else if (from - 1 == to)
		return "LEFT";
	else
		return "error";
}


int get_num(pair<int, int> p, int size_c)
{
	return (p.first * size_c + p.second + 1);
}


