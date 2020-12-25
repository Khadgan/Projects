//Implementaion of A* and D* pathfinding algorithms
//on 3-dimensional grid
//"It works" version, without proper project structure

#include <Windows.h>
#include <ctime>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <list>

using namespace std;

//static const int width = 10, height = 10, depth = 10;
//static const int width = 25, height = 25, depth = 25;
//static const int width = 50, height = 50, depth = 50;
static const int width = 100, height = 100, depth = 100;

float delta = 1;

struct Point
{
	float pointX;
	float pointY;
	float pointZ;
};


struct CubeData 
{
	float cost, diagCost;
};

class Cube 
{
public:
	Point center;
	CubeData data;
	float arcCost, pathCost;
	bool isOpen;
	Cube *previous;
	//vector<Cube*> neighbours;

	Cube()
	{
		isOpen = false;
	}

	~Cube()
	{
		//neighbours.clear();
		previous = NULL;
		delete previous;
	}

	bool operator < (const Cube& rCube) const
	{
		return pathCost < rCube.pathCost;
	}

	Point getCenter() const
	{
		return center;
	}

	friend bool operator == (const Cube& lCube, const Cube& rCube)
	{
		if ((lCube.getCenter().pointX == rCube.getCenter().pointX) && (lCube.getCenter().pointY == rCube.getCenter().pointY) && (lCube.getCenter().pointZ == rCube.getCenter().pointZ))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

Cube ***cube;

vector<Cube*> getNeighbours(int i, int j, int k)
{
	vector<Cube*> neighbours;
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int z = -1; z <= 1; z++)
			{
				if (((i + x)>-1) && ((j + y) > -1) && ((k + z) > -1) && ((i + x)<width) && ((j + y) < height) && ((k + z) < depth))
				{
					if ((cube[i + x][j + y][k + z].getCenter().pointX != cube[i][j][k].getCenter().pointX) || (cube[i + x][j + y][k + z].getCenter().pointY != cube[i][j][k].getCenter().pointY) || (cube[i + x][j + y][k + z].getCenter().pointZ != cube[i][j][k].getCenter().pointZ))
					{
						if ((abs(cube[i + x][j + y][k + z].getCenter().pointX - cube[i][j][k].getCenter().pointX) <= delta) && (abs(cube[i + x][j + y][k + z].getCenter().pointY - cube[i][j][k].getCenter().pointY) <= delta) && (abs(cube[i + x][j + y][k + z].getCenter().pointZ - cube[i][j][k].getCenter().pointZ) <= delta))
						{
							neighbours.push_back(&cube[i + x][j + y][k + z]);
						}
					}
				}
			}
		}
	}
	return neighbours;
}

struct CubeFind
{
	CubeFind(Cube* rCube) : c(rCube) {}
	Cube *c;
	bool operator()(Cube* lCube) const
	{
		return lCube == c;
	}
};

class CubeBuilder 
{	
	int cWidth, cHeight, cDepth;

public:
	float cDelta;
	CubeBuilder(int width, int height, int depth, float delta)
	{
		cDelta = delta;
		cWidth = width;
		cHeight = height;
		cDepth = depth;
	}
	Cube createCube(int i, int j, int k, int cost)
	{
		Cube result;
		float z = i;
		float y = j;
		float x = k;
		result.center = { x, y, z };
		if (cost > 6)
		{
			cost = 1;
		}
		else
		{
			cost = 10;
		}
		result.data.cost = cost;
		result.data.diagCost = cost + 0.414;
		return result;
	}
};

float cDistance(Cube *start, Cube *finish)
{
	float dist;

	dist = powf((start->getCenter().pointX - finish->getCenter().pointX), 2) + powf((start->getCenter().pointY - finish->getCenter().pointY), 2) + powf((start->getCenter().pointZ - finish->getCenter().pointZ), 2);

	dist = sqrtf(dist);

	return dist;
}

void setPath(Cube *current, list<Cube*> *closed)
{
	vector<Cube*> neighb = getNeighbours(current->getCenter().pointX, current->getCenter().pointY, current->getCenter().pointZ);
	for (int i = 0; i < neighb.size(); i++)
	{
		if (neighb[i]->data.diagCost < 9)
		{
			if (cDistance(current, neighb[i]) > delta)
			{
				neighb[i]->arcCost = current->data.cost + neighb[i]->data.diagCost;
			}
			else
			{
				neighb[i]->arcCost = current->data.cost + neighb[i]->data.cost;
			}
			if ((find_if(closed->begin(), closed->end(), CubeFind(neighb[i])) != closed->end()) == false)
			{
				if ((neighb[i]->getCenter().pointX != current->getCenter().pointX) || (neighb[i]->getCenter().pointY != current->getCenter().pointY) || (neighb[i]->getCenter().pointZ != current->getCenter().pointZ))
				{
					neighb[i]->previous = current;
				}
			}
		}
	}
}

vector<Cube*> make_path(Cube *start, Cube *cube)
{
	vector<Cube*> path;

	int v=0;

	path.push_back(cube);

	//printf("%.1f, %.1f, %.1f\n", cube->getCenter().pointX, cube->getCenter().pointY, cube->getCenter().pointZ);

	while ((cube->previous) && (cube != start) && (cube != cube->previous->previous))
	{
		cube = cube->previous;
		path.push_back(cube);
		v++;
		printf("%i) %.1f, %.1f, %.1f\n", v, cube->getCenter().pointX, cube->getCenter().pointY, cube->getCenter().pointZ);
	}

	//printf("%f, %f, %f\n", start->getCenter().pointX, start->getCenter().pointY, start->getCenter().pointZ);

	return path;
}

vector<Cube*> aStar(Cube *start, Cube *finish)
{
	int n=0;
	bool isFirst = true;
	vector<Cube*> neighb;
	list<Cube*> open;
	open.push_back(finish);
	start->isOpen = true;
	list<Cube*> closed;
	start->data.cost = 0;
	start->data.diagCost = 0;
	finish->data.cost = 0;
	finish->data.diagCost = 0;
	start->pathCost = start->data.cost + cDistance(start, finish);
	Cube *current, *next;
	while (!open.empty())
	{
		open.sort();
		current = open.front();
		n++;
		system("CLS");
		printf("%i\n", n);
		if ((find_if(closed.begin(), closed.end(), CubeFind(start)) != closed.end()) == true)
		{
			//if ((current->getCenter().pointX == finish->getCenter().pointX) && (current->getCenter().pointY == finish->getCenter().pointY) && (current->getCenter().pointZ == finish->getCenter().pointZ))
			//{
				open.clear();
				closed.clear();
				return make_path(finish, current);
			//}
		}
		open.pop_front();
		current->isOpen = false;	
		setPath(current, &closed);
		closed.push_back(current);
		list<Cube*> openNeighb;
		neighb = getNeighbours(current->getCenter().pointX, current->getCenter().pointY, current->getCenter().pointZ);
		for (int i = 0; i < neighb.size(); i++)
		{
			if ((find_if(closed.begin(), closed.end(), CubeFind(neighb[i])) != closed.end()) == false)
			{
				if (neighb[i]->data.diagCost < 9)
				{
					openNeighb.push_back(neighb[i]);
					neighb[i]->pathCost = neighb[i]->arcCost + cDistance(neighb[i], finish);
					if ((find_if(open.begin(), open.end(), CubeFind(neighb[i])) != open.end()) == false)
					{
						neighb[i]->isOpen = true;
						open.push_back(neighb[i]);
					}
					else
					{
						openNeighb.sort();
						next = openNeighb.front();
						if (neighb[i]->data.cost < next->data.cost)
						{
							next = neighb[i];
							next->previous = current;
						}
					}
				}
				else
				{
					neighb[i]->isOpen = false;
					closed.push_back(neighb[i]);
				}
			}
		}
		openNeighb.clear();
	}
	open.clear();
	closed.clear();
	printf("no path\n");
	return make_path(start, start);

}

Cube* make_path_dStar(Cube *start, Cube *finish, list<Cube*> *open, list<Cube*> *closed)
{
	Cube *current, *next;
	vector<Cube*> neighb;
	open->sort();
	if (open->empty())
	{
		printf("no path\n");
		return start;
	}
	current = open->front();
	/*if ((current->getCenter().pointX == start->getCenter().pointX) && (current->getCenter().pointY == start->getCenter().pointY) && (current->getCenter().pointZ == start->getCenter().pointZ))
	{
		return *current;
	}*/
	open->pop_front();
	current->isOpen = false;
	setPath(current, closed);
	closed->push_back(current);
	list<Cube*> openNeighb;
	neighb = getNeighbours(current->getCenter().pointX, current->getCenter().pointY, current->getCenter().pointZ);
	for (int i = 0; i < neighb.size(); i++)
	{
		if ((find_if(closed->begin(), closed->end(), CubeFind(neighb[i])) != closed->end()) == false)
		{
			if (neighb[i]->data.diagCost < 9)
			{
				openNeighb.push_back(neighb[i]);
				neighb[i]->pathCost = neighb[i]->arcCost + cDistance(neighb[i], finish);
				if ((find_if(open->begin(), open->end(), CubeFind(neighb[i])) != open->end()) == false)
				{
					neighb[i]->isOpen = true;
					open->push_back(neighb[i]);
				}
				else
				{
					openNeighb.sort();
					next = openNeighb.front();
					if (neighb[i]->data.cost < next->data.cost)
					{
						next = neighb[i];
						next->previous = current;
					}
				}
			}
			else
			{
				neighb[i]->isOpen = false;
				closed->push_back(neighb[i]);
			}
		}
	}
	openNeighb.clear();
	if (open->empty())
	{
		printf("no path\n");
		return start;
	}
	open->sort();
	current = open->front();
	return current;
}

Cube* changeCost(Cube *move, Cube *neighb, float newCost, list<Cube*> open, list<Cube*> closed)
{
	neighb->data.cost = newCost;
	if ((find_if(closed.begin(), closed.end(), CubeFind(neighb)) != closed.end() == true))
	{
		open.push_back(move);
	}
	open.sort();
	return open.front();
}

int newCost()
{
	if ((rand() % 10 + 1) <= 3)
	{
		return rand() % 19 - 9;
	}
	else
	{
		return 0;
	}
}

Cube* dStar(Cube *start, Cube *finish)
{
	int n=0;
	Cube *move, *min;
	vector<Cube*> neighb;
	list<Cube*> open;
	open.push_back(finish);
	finish->isOpen = true;
	list<Cube*> closed;
	start->data.cost = 0;
	start->data.diagCost = 0;
	finish->data.cost = 0;
	finish->data.diagCost = 0;
	while (((find_if(closed.begin(), closed.end(), CubeFind(start)) != closed.end()) == false) || (open.empty()))
	{
		min = make_path_dStar(start, finish, &open, &closed);
		n++;
		system("CLS");
		printf("%i\n", n);
		//printf("%f, %f, %f\n", min->getCenter().pointX, min->getCenter().pointY, min->getCenter().pointZ);
	}
	move = start;
	int v = 0;
	while (move != finish)
	{
		neighb.clear();
		neighb = getNeighbours(move->getCenter().pointX, move->getCenter().pointY, move->getCenter().pointZ);
		n++;
		printf("%i   ", n);
		for (int i = 0; i < neighb.size(); i++)
		{
			int x = 0;
			if ((neighb[i]->data.cost + (x = newCost())) != neighb[i]->data.cost)
			{
				min = changeCost(move, neighb[i], neighb[i]->data.cost + x, open, closed);
			}
		}
		while ((min->data.cost < move->data.cost) && (!open.empty()))
		{
			min = make_path_dStar(min, finish, &open, &closed);
			n++;
			printf("\r%i   ", n);
		}
		if (open.empty())
		{
			open.clear();
			closed.clear();
			printf("no path\n");
			return start;
		}
		move = move->previous;
		v++;
		printf("%i) %.1f, %.1f, %.1f\n", v, move->getCenter().pointX, move->getCenter().pointY, move->getCenter().pointZ);
	}
	open.clear();
	closed.clear();
	return move;
}


void main() 
{	
	CubeBuilder cubes(width, height, depth, delta);
	cube = new Cube**[width];

	for (int i = 0; i < width; i++)
	{
		cube[i] = new Cube*[height];

		for (int j = 0; j < height; j++)
		{
			cube[i][j] = new Cube[depth];
		}
	}

	vector<Point> cNeighbours;
	srand(time(0));
	
	for (int i = 0; i<width; i++) 
	{
		for (int j = 0; j<height; j++) 
		{
			for (int k = 0; k<depth; k++) 
			{
				cube[i][j][k] = cubes.createCube(i, j, k, rand()%10+1);
				//Point p = cube[i][j][k].getCenter();
				//printf("%.1f, %.1f, %.1f | ", p.pointX, p.pointY, p.pointZ);
			}
			//printf("\n");
		}
		//printf("\n");
	}
	printf("gen done\n");
	/*for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < depth; k++)
			{
				getNeighbours(i,j,k,cube);
			}
		}
	}
	printf("neighb done\n");*/
	
	clock_t st = clock();
	float et;

	//aStar(&cube[0][0][0], &cube[9][9][9]);
	//aStar(&cube[0][0][0], &cube[24][24][24]);
	//aStar(&cube[0][0][0], &cube[49][49][49]);
	//aStar(&cube[0][0][0], &cube[99][99][99]);

	//dStar(&cube[0][0][0], &cube[9][9][9]);
	//dStar(&cube[0][0][0], &cube[24][24][24]);
	//dStar(&cube[0][0][0], &cube[49][49][49]);
	dStar(&cube[0][0][0], &cube[99][99][99]);
	printf("done\n");

	et = (clock() - st) / CLOCKS_PER_SEC + 0.001 * ((clock() - st) % CLOCKS_PER_SEC);
	printf("%f\n", et);
	/*for (int i = 0; i < cube[8][4][5].neighbours.size(); i++)
	{
		printf("%.1f, %.1f, %.1f \n", cube[8][4][5].neighbours[i]->getCenter().pointX, cube[8][4][5].neighbours[i]->getCenter().pointY, cube[8][4][5].neighbours[i]->getCenter().pointZ);
	}*/

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			delete[] cube[i][j];
		}
		delete[] cube[i];
	}
	delete[] cube;

	system("pause");
}