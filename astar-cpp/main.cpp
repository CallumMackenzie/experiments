
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <chrono>
#include <algorithm>
#include <unordered_map>

#include "math.h"

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
		vec2i loc;
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

		vec2i size;
		Tile *map;
		bool allowDiagonal = false;

		Map(vec2i sz)
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

		inline Tile get(vec2i v)
		{
			return get(v.x, v.y);
		}

		inline void set(int x, int y, Tile tile)
		{
			map[mapIndex(x, y)] = tile;
		}

		inline void set(vec2i v, Tile tile)
		{
			set(v.x, v.y, tile);
		}

		inline int mapIndex(int x, int y)
		{
			return y * size.x + x;
		}

		inline int mapIndex(vec2i v)
		{
			return mapIndex(v.x, v.y);
		}

		vec2i find(Tile tile)
		{
			for (int i = 0; i < size.x * size.y; i++)
				if (get(indexMap(i)) == tile)
					return indexMap(i);
			return vec2i{};
		}

		std::vector<vec2i> findAll(Tile tile)
		{
			std::vector<vec2i> tiles;
			for (int i = 0; i < size.x * size.y; i++)
				if (get(indexMap(i)) == tile)
					tiles.push_back(indexMap(i));
			return tiles;
		}

		vec2i indexMap(int index)
		{
			int y = index / size.x;
			int x = index - (y * size.x);
			return vec2i{x, y};
		}

		inline std::vector<vec2i> getSurrounding(vec2i v)
		{
			if (allowDiagonal)
				return std::vector<vec2i>{
					vec2i{0, -1} + v,
					vec2i{0, 1} + v,
					vec2i{-1, 0} + v,
					vec2i{1, 0} + v,
					vec2i{-1, -1} + v,
					vec2i{1, -1} + v,
					vec2i{-1, 1} + v,
					vec2i{1, 1} + v};
			else
				return std::vector<vec2i>{
					vec2i{0, -1} + v,
					vec2i{0, 1} + v,
					vec2i{-1, 0} + v,
					vec2i{1, 0} + v};
		}

		void setPath(std::vector<vec2i> path)
		{
			for (auto elem : path)
				set(elem, Tile::Path);
		}

		inline std::vector<vec2i> getPath(size_t maxIters = 0L, vec2i startPos = vec2i{-1}, vec2i endPos = vec2i{-1})
		{
			return Pathfinder::getPath(this, maxIters, startPos, endPos);
		}

		inline void getAndSetPath(size_t maxIters = 0L, vec2i startPos = vec2i{-1}, vec2i endPos = vec2i{-1})
		{
			setPath(getPath(maxIters, startPos, endPos));
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
			Map *map = new Map(vec2i{(int)mapWidth, (int)mapHeight});
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

	static inline std::vector<vec2i> unwindPath(Node *currentNode)
	{
		std::vector<vec2i> path = {currentNode->loc};
		auto node = currentNode;
		do
			path.push_back(node->loc);
		while (node = node->parent);
		return path;
	}

	static inline bool shouldPass(std::vector<Node *> *openNodes, std::unordered_map<vec_hash, bool> *closedNodes,
								  int child_g, vec2i *newPos, Map *map)
	{
		if (newPos->x < 0 || newPos->y < 0 || newPos->x >= map->size.width || newPos->y >= map->size.height)
			return true;
		if (map->get(*newPos) == Map::Tile::Blocked)
			return true;
		if (closedNodes->find(vec2i::hash(*newPos)) != closedNodes->end())
			return true;
		for (int i = openNodes->size() - 1; i > 0; i--)
			if (*newPos == (*openNodes)[i]->loc && child_g > (*openNodes)[i]->g)
				return true;
		return false;
	}

	static std::vector<vec2i> getPath(Map *map, const size_t maxIters = 0,
									  vec2i startPos = vec2i{-1}, vec2i endPos = vec2i{-1})
	{
		if (startPos.x < 0)
			startPos = map->find(Map::Tile::Start);
		if (endPos.x < 0)
			endPos = map->find(Map::Tile::End);
		std::vector<Node *> openNodes = {
			new Node{startPos}};
		std::make_heap(openNodes.begin(), openNodes.end());
		std::unordered_map<vec_hash, bool> closedNodes;
		std::vector<vec2i> pathFound;
#ifdef PATH_CAP_ITERS
		size_t iters = 0;
#endif
		std::vector<Node *> allNodes = openNodes;

		while (openNodes.size() > 0)
		{
			std::pop_heap(openNodes.begin(), openNodes.end());
			auto currentNode = openNodes.back();
			openNodes.pop_back();
			closedNodes[vec2i::hash(currentNode->loc)] = true;
#ifdef PATH_CAP_ITERS
			iters++;
			if (iters >= maxIters && maxIters != 0)
			{
				std::cout << "Giving up; too many iterations." << std::endl;
				pathFound = unwindPath(currentNode);
				break;
			}
#endif
			if (currentNode->loc == endPos)
			{
				pathFound = unwindPath(currentNode);
				break;
			}
			for (vec2i newPos : map->getSurrounding(currentNode->loc))
			{
				int child_g = currentNode->g + 1;
				if (shouldPass(&openNodes, &closedNodes, child_g, &newPos, map))
					continue;

				auto child = new Node{newPos, currentNode, child_g, (newPos - endPos).pow2().sum()};
				allNodes.push_back(child);
				openNodes.push_back(child);
				std::push_heap(openNodes.begin(), openNodes.end());
			}
		}

		for (auto node : allNodes)
			delete node;

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

	std::vector<double> results;
	vec2i mapSize;
	const size_t iters = 1;
	for (size_t i = 0; i < iters; i++)
	{
		Pathfinder::Map *map = Pathfinder::loadMapFromFile("../../maze3.txt");
		mapSize = map->size;
		start = clock();
		auto path = map->getPath(0UL); //, vec2i{1, 1}, vec2i{88, 59});
		results.push_back(elapsedTimeMs());
		if (iters <= 2)
		{
			map->setPath(path);
			map->print();
		}
		// std::cout << "Algorithm completed in " << results.tail->data << "ms" << std::endl;
		delete map;
	}
	if (results.size() > 1)
	{
		double avg = 0;
		double lowest = results[0];
		double highest = results[0];
		for (auto result : results)
		{
			avg += result;
			if (result < lowest)
				lowest = result;
			if (result > highest)
				highest = result;
		}
		avg /= results.size();
		std::cout << "Average pathfinding time (" << mapSize.x << "x" << mapSize.y << ") was " << avg << "ms" << std::endl
				  << "Lowest time was " << lowest << "ms" << std::endl
				  << "Highest time was " << highest << "ms" << std::endl;
	}
	results.clear();

	std::cout << "Allocs: " << allocs << std::endl
			  << "Deletes: " << deletes << std::endl;
}