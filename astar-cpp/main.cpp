
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>

#include "math.h"

typedef unsigned int uint;

uint deletes = 0;
uint allocs = 0;

void operator delete(void *ptr)
{
	deletes++;
	free(ptr);
}

void *operator new(size_t size)
{
	allocs++;
	return malloc(size);
}

enum class MapTile
{
	Empty = ' ',
	Blocked = '|',
	Start = 'S',
	End = 'E',
	Path = '#',
	None = '?',
};

struct Map
{
	Vec2<int> size;
	MapTile *map;
	bool allowDiagonal = false;

	Map(Vec2<int> sz)
	{
		size = sz;
		map = new MapTile[sz.width * sz.height];
		for (int i = 0; i < sz.width * sz.height; i++)
			map[i] = MapTile::Empty;

		set(sz.width >> 1, 0, MapTile::Start);
		set(sz.width - 1, sz.height - 1, MapTile::End);
	}

	~Map()
	{
		delete[] map;
	}

	MapTile get(int x, int y)
	{
		return map[mapIndex(x, y)];
	}

	MapTile get(Vec2<int> v)
	{
		return get(v.x, v.y);
	}

	void set(int x, int y, MapTile tile)
	{
		map[mapIndex(x, y)] = tile;
	}

	void set(Vec2<int> v, MapTile tile)
	{
		set(v.x, v.y, tile);
	}

	int mapIndex(int x, int y)
	{
		return y * size.x + x;
	}

	int mapIndex(Vec2<int> v)
	{
		return mapIndex(v.x, v.y);
	}

	Vec2<int> find(MapTile tile)
	{
		for (int i = 0; i < size.x * size.y; i++)
			if (get(indexMap(i)) == tile)
				return indexMap(i);
		return Vec2<int>{};
	}

	std::vector<Vec2<int>> findAll(MapTile tile)
	{
		std::vector<Vec2<int>> tiles;
		for (int i = 0; i < size.x * size.y; i++)
			if (get(indexMap(i)) == tile)
				tiles.push_back(indexMap(i));
		return tiles;
	}

	Vec2<int> indexMap(int index)
	{
		int y = index / size.x;
		int x = index - (y * size.x);
		return Vec2<int>{x, y};
	}

	std::vector<Vec2<int>> getSurrounding(Vec2<int> v)
	{
		if (allowDiagonal)
			return std::vector<Vec2<int>>{
				Vec2<int>{0, -1} + v,
				Vec2<int>{0, 1} + v,
				Vec2<int>{-1, 0} + v,
				Vec2<int>{1, 0} + v,
				Vec2<int>{-1, -1} + v,
				Vec2<int>{1, -1} + v,
				Vec2<int>{-1, 1} + v,
				Vec2<int>{1, 1} + v};
		else
			return std::vector<Vec2<int>>{
				Vec2<int>{0, -1} + v,
				Vec2<int>{0, 1} + v,
				Vec2<int>{-1, 0} + v,
				Vec2<int>{1, 0} + v};
	}

	void print()
	{
		for (int y = 0; y < size.height; y++)
		{
			for (int x = 0; x < size.width; x++)
				std::cout << (char)get(x, y);
			std::cout << std::endl;
		}
	}

	static Map *loadFromString(std::string input)
	{
		size_t mapWidth = (input.find('\n') == std::string::npos) ? input.size() : input.find('\n');
		size_t newLine;
		while ((newLine = input.find('\n')) != std::string::npos)
			input.replace(newLine, 1, "");
		size_t mapHeight = input.size() / mapWidth;
		Map *map = new Map(Vec2<int>{(int)mapWidth, (int)mapHeight});
		for (int i = 0; i < input.size(); i++)
			map->map[i] = (MapTile)input[i];
		return map;
	}
};

struct AStarNode
{
	Vec2<int> loc;
	AStarNode *parent = nullptr;
	int g = 0;
	int h = 0;

	int getF()
	{
		return g + h;
	}
};

std::vector<Vec2<int>> unwindPath(AStarNode *currentNode)
{

	std::vector<Vec2<int>> path = {currentNode->loc};
	AStarNode *node = currentNode;
	do
		path.push_back(node->loc);
	while (node = node->parent);
	return path;
}

std::vector<Vec2<int>> aStar(Map *map, const size_t maxIters = 0)
{
	std::vector<AStarNode *> openNodes;
	std::vector<AStarNode *> closedNodes;
	std::vector<Vec2<int>> pathFound;

	size_t iters = 0;
	Vec2<int> endPos = map->find(MapTile::End);

	openNodes.push_back(new AStarNode{map->find(MapTile::Start)});

	while (openNodes.size() > 0)
	{
		AStarNode *currentNode = openNodes[0];
		size_t currentIndex = 0;
		for (size_t i = 0; i < openNodes.size(); i++)
			if (openNodes[i]->getF() < currentNode->getF())
			{
				currentNode = openNodes[i];
				currentIndex = i;
			}
		openNodes.erase(openNodes.begin() + currentIndex);
		closedNodes.push_back(currentNode);

		iters++;
		if (iters >= maxIters && maxIters != 0)
		{
			std::cout << "Giving up; too many iterations." << std::endl;
			pathFound = unwindPath(currentNode);
			break;
		}

		if (currentNode->loc == endPos)
		{
			pathFound = unwindPath(currentNode);
			break;
		}

		for (Vec2<int> newPos : map->getSurrounding(currentNode->loc))
		{
			if (newPos.x < 0 || newPos.y < 0 || newPos.x >= map->size.width || newPos.y >= map->size.height)
				continue;
			if (map->get(newPos) == MapTile::Blocked)
				continue;
			AStarNode *child = new AStarNode{newPos, currentNode};
			bool pass = false;
			for (AStarNode *closed : closedNodes)
				if (child->loc == closed->loc)
					pass = true;

			if (pass)
			{
				delete child;
				continue;
			}

			child->g = currentNode->g + 1;
			child->h = ((child->loc.x - endPos.x) * (child->loc.x - endPos.x)) +
					   ((child->loc.y - endPos.y) * (child->loc.y - endPos.y));

			for (AStarNode *openNode : openNodes)
				if (child->loc == openNode->loc && child->g > openNode->g)
					pass = true;

			if (pass)
			{
				delete child;
				continue;
			}

			openNodes.push_back(child);
		}
	}

	for (auto node : closedNodes)
		delete node;
	for (auto node : openNodes)
		delete node;

	return pathFound;
}

int main(int argc, char *argv[])
{
	std::ifstream ifs("../../maze.txt");
	std::string content((std::istreambuf_iterator<char>(ifs)),
						(std::istreambuf_iterator<char>()));
	ifs.close();
	Map *map = Map::loadFromString(content);
	map->allowDiagonal = true;
	auto start = clock();
	for (auto elem : aStar(map))
		map->set(elem, MapTile::Path);
	auto end = (clock() - start) / CLOCKS_PER_SEC;
	map->print();
	std::cout << "Algorithm completed in " << end << "ms" << std::endl;
	delete map;

	std::cout << "Allocs: " << allocs << std::endl
			  << "Deletes: " << deletes << std::endl;
}