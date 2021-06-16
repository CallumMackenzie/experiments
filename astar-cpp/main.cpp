
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <chrono>
#include <algorithm>
#include <unordered_map>

#include "math.h"
#include "list.h"

typedef unsigned int uint;

uint deletes = 0;
uint allocs = 0;

void operator delete(void *ptr)
{
	++deletes;
	free(ptr);
}

void *operator new(size_t size)
{
	++allocs;
	return malloc(size);
}

struct Pathfinder
{
	struct Node
	{
		Vec2<int> loc;
		Node *parent = nullptr;
		int g = 0;
		int h = 0;

		int getF()
		{
			return g + h;
		}
	};

	struct Map
	{
		enum class Tile
		{
			Empty = ' ',
			Blocked = '|',
			Start = 'S',
			End = 'E',
			Path = '#',
			None = '?',
		};

		Vec2<int> size;
		Tile *map;
		bool allowDiagonal = false;

		Map(Vec2<int> sz)
		{
			size = sz;
			map = new Tile[sz.width * sz.height];
			for (int i = 0; i < sz.width * sz.height; i++)
				map[i] = Tile::Empty;

			set(sz.width >> 1, 0, Tile::Start);
			set(sz.width - 1, sz.height - 1, Tile::End);
		}

		~Map()
		{
			delete[] map;
		}

		inline Tile get(int x, int y)
		{
			return map[mapIndex(x, y)];
		}

		inline Tile get(Vec2<int> v)
		{
			return get(v.x, v.y);
		}

		inline void set(int x, int y, Tile tile)
		{
			map[mapIndex(x, y)] = tile;
		}

		inline void set(Vec2<int> v, Tile tile)
		{
			set(v.x, v.y, tile);
		}

		inline int mapIndex(int x, int y)
		{
			return y * size.x + x;
		}

		inline int mapIndex(Vec2<int> v)
		{
			return mapIndex(v.x, v.y);
		}

		Vec2<int> find(Tile tile)
		{
			for (int i = 0; i < size.x * size.y; i++)
				if (get(indexMap(i)) == tile)
					return indexMap(i);
			return Vec2<int>{};
		}

		std::vector<Vec2<int>> findAll(Tile tile)
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

		inline std::vector<Vec2<int>> getSurrounding(Vec2<int> v)
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

		void setPath(std::vector<Vec2<int>> path)
		{
			for (auto elem : path)
				set(elem, Tile::Path);
		}

		std::vector<Vec2<int>> getPath(size_t maxIters = 0L)
		{
			return Pathfinder().getPath(this, maxIters);
		}

		void getAndSetPath(size_t maxIters = 0L)
		{
			setPath(getPath(maxIters));
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

		static Map *loadFromString(std::string *input)
		{
			if (input->size() == 0)
			{
				std::cerr << "Input map string is empty, using default map." << std::endl;
				Tile stdMap[3] = {Tile::Start, Tile::Empty, Tile::End};
				input->append(std::string((char *)stdMap));
			}
			size_t mapWidth = (input->find('\n') == std::string::npos) ? input->size() : input->find('\n');
			size_t newLine;
			while ((newLine = input->find('\n')) != std::string::npos)
				input->replace(newLine, 1, "");
			size_t mapHeight = input->size() / mapWidth;
			Map *map = new Map(Vec2<int>{(int)mapWidth, (int)mapHeight});
			for (int i = 0; i < input->size(); i++)
				map->map[i] = (Tile)(*input)[i];
			return map;
		}
	};

	static Map *loadMapFromString(std::string *map)
	{
		return Map::loadFromString(map);
	}

	static Map *loadMapFromFile(std::string path)
	{

		std::ifstream ifs(path);
		std::string content((std::istreambuf_iterator<char>(ifs)),
							(std::istreambuf_iterator<char>()));
		ifs.close();
		return loadMapFromString(&content);
	}

	std::vector<Vec2<int>> unwindPath(Node *currentNode)
	{
		std::vector<Vec2<int>> path = {currentNode->loc};
		Node *node = currentNode;
		do
			path.push_back(node->loc);
		while (node = node->parent);
		return path;
	}

	std::vector<Vec2<int>> getPath(Map *map, const size_t maxIters = 0)
	{
		std::vector<Node *> openNodes = {
			new Node{map->find(Map::Tile::Start)}};
		std::make_heap(openNodes.begin(), openNodes.end());
		std::unordered_map<VecHash, bool> closedNodes;
		std::vector<Vec2<int>> pathFound;

		// LkList<Node *> allNodes = LkList<Node *>(openNodes);
		// std::vector<Node *> allNodes = openNodes;
		// auto allNodes = new Node *[map->size.width * map->size.height] { openNodes[0] };
		PtrArray<Node> allNodes = PtrArray<Node>(map->size.width * map->size.height * (map->allowDiagonal ? 4 : 2), openNodes);

		size_t iters = 0;
		Vec2<int> endPos = map->find(Map::Tile::End);

		while (openNodes.size() > 0)
		{
			std::pop_heap(openNodes.begin(), openNodes.end());
			Node *currentNode = openNodes.back();
			openNodes.pop_back();
			closedNodes[Vec2<int>::hash(currentNode->loc)] = true;
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
				if (map->get(newPos) == Map::Tile::Blocked)
					continue;
				Node *child = new Node{newPos, currentNode};
				if (closedNodes.find(Vec2<int>::hash(child->loc)) != closedNodes.end())
				{
					delete child;
					continue;
				}
				child->g = currentNode->g + 1;
				Vec2<int> h = child->loc - endPos;
				h = h * h;
				// child->h = ((child->loc.x - endPos.x) * (child->loc.x - endPos.x)) +
				// 		   ((child->loc.y - endPos.y) * (child->loc.y - endPos.y));
				child->h = h.x + h.y;
				bool pass = false;
				for (auto openNode : openNodes)
					if (child->loc == openNode->loc && child->g > openNode->g)
					{
						pass = true;
						break;
					}

				if (pass)
				{
					delete child;
					continue;
				}
				openNodes.push_back(child);
				std::push_heap(openNodes.begin(), openNodes.end());
				allNodes.push_back(child);
			}
			// delete surrounding;
		}

		// allNodes.clear();
		// for (auto node : allNodes)
		// delete node->data;
		// delete node;
		closedNodes.clear();
		openNodes.clear();

		return pathFound;
	}
};

int main(int argc, char *argv[])
{

	clock_t start;
	auto elapsedTimeMs = [&start]()
	{
		return ((double)(clock() - start) / (double)CLOCKS_PER_SEC) * 1000.0;
	};

	DLkList<double> results;
	const size_t iters = 2;
	for (size_t i = 0; i < iters; i++)
	{
		Pathfinder::Map *map = Pathfinder::loadMapFromFile("../maze2.txt");
		start = clock();
		auto path = map->getPath();
		results.push_back(elapsedTimeMs());
		if (iters <= 2)
		{
			map->setPath(path);
			map->print();
		}
		std::cout << "Algorithm completed in " << results.tail->data << "ms" << std::endl;
		delete map;
	}
	if (results.length > 1)
	{
		double avg = 0;
		double lowest = results[0]->data;
		double highest = results[0]->data;
		for (auto result : results)
		{
			avg += result->data;
			if (result->data < lowest)
				lowest = result->data;
			if (result->data > highest)
				highest = result->data;
		}
		avg /= results.length;
		std::cout << "Average pathfinding time was " << avg << "ms" << std::endl
				  << "Lowest time was " << lowest << "ms" << std::endl
				  << "Highest time was " << highest << "ms" << std::endl;
	}
	results.clear();

	std::cout << "Allocs: " << allocs << std::endl
			  << "Deletes: " << deletes << std::endl;
}