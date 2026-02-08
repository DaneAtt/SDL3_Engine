#include "Map.h"
#include <regex>
#include "Vector2D.h"
#include "ECS.h"
#include "TileComponent.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"

Map::Map(const char* path, int scale) : Scale(scale)
{
	loadMapJSON(path);
}

Map::~Map() {}

void Map::loadMapJSON(const char* path)
{
	if (!mapJ.empty()) return;
	std::ifstream file(path);
	if (!file.is_open()) { std::cout << "Map JSON File not found" << "\n"; return; }
	file >> mapJ;
	file.close();

	for (auto& [name, tiles] : mapJ.items()) {
		try {
			tileVec.clear();
			int x = 0; //current column
			int y = 0; //current row
			int width = tiles["width"];
			int height = tiles["height"];
			mapWidth = width;
			mapHeight = height;
			Vector2D tileSize = { 32,48 };

			for (const auto& tile : tiles["layers"]) {
				std::string tileStr = tile;
				std::regex remove("\\{[hvd]\\}");
				bool hasH = (tileStr.find("{h}") != std::string::npos);
				bool hasV = (tileStr.find("{v}") != std::string::npos);
				SDL_FlipMode flip = SDL_FLIP_NONE;
				if (hasH && hasV) {
					flip = SDL_FLIP_HORIZONTAL_AND_VERTICAL;
				}
				else if (hasH) {
					flip = SDL_FLIP_HORIZONTAL;
				}
				else if (hasV) {
					flip = SDL_FLIP_VERTICAL;
				}

				int tileIndex = std::stoi(std::regex_replace(tileStr, remove, ""));
				int xPos = x * tileSize.x * Scale;
				int yPos = y * tileSize.y * Scale;

				TileData data;
				if (tileIndex > 0 && tileIndex <= 100) 
				{
					data.srcX = ((tileIndex - 1) % 10) * 32;
					data.srcY = ((tileIndex - 1) / 10) * 48;
					data.xPos = xPos;
					data.yPos = yPos;
					data.flip = flip;
					tileVec.push_back(data);
				}

				x++;
				if (x >= width) {
					x = 0;
					y++;
				}
			}
			mapLayers[name] = std::move(tileVec);
		}
		catch (const json::exception& e) {
			std::cout << "Error parsing " << name << ": " << e.what() << "\n";
		}
	}
}

std::vector<Map::TileData>& Map::searchMap(std::string id)
{
	if (mapLayers.find(id) == mapLayers.end()) {
		std::cout << "Warning: Map layer '" << id << "' not found\n";
		static std::vector<TileData> empty;
		return empty;
	}
	return mapLayers[id];
}

void Map::createEntities(std::string texID, std::string id, int group, int offsetX, int offsetY)
{
	if (mapLayers.find(id) == mapLayers.end()) {
		std::cout << "Warning: Map '" << id << "' not found\n";
		return;
	}
	Manager* manager = Engine::getManager();
	for (auto& tileData : mapLayers[id]) {
		auto& tile(manager->addEntity());
		tile.addComponent<TransformComponent>(
			(float)(tileData.xPos + offsetX),
			(float)(tileData.yPos + offsetY),
			32,
			48,
			Scale
		);
		tile.addComponent<TileComponent>(
			tileData.srcX,
			tileData.srcY,
			Scale,
			texID,
			tileData.flip
		);
		tile.addGroup(group);
	}
}

void Map::createColliders(std::string id, int group, int offsetX, int offsetY)
{
	if (mapLayers.find(id) == mapLayers.end()) {
		std::cout << "Warning: Map '" << id << "' not found\n";
		return;
	}

	/*
	std::cout << "Creating colliders for layer: " << id << std::endl;
	std::cout << "Total tiles in layer: " << mapLayers[id].size() << std::endl;

	std::unordered_map<std::string, int> positions;
	for (auto& tileData : mapLayers[id]) {
		std::string key = std::to_string(tileData.xPos) + "," + std::to_string(tileData.yPos);
		positions[key]++;
	}

	int duplicates = 0;
	for (auto& [pos, count] : positions) {
		if (count > 1) {
			std::cout << "  Duplicate at " << pos << " x" << count << std::endl;
			duplicates += (count - 1);
		}
	}
	std::cout << "Unique positions: " << positions.size() << ", Duplicates: " << duplicates << std::endl;
	*/
	Manager* manager = Engine::getManager();
	for (auto& tileData : mapLayers[id]) {
		auto& tile(manager->addEntity());
		tile.addComponent<TransformComponent>(
			(float)(tileData.xPos + offsetX),
			(float)(tileData.yPos + offsetY),
			32,
			48,
			Scale
		);
		tile.addComponent<CollisionComponent>(true);
		tile.addGroup(group);
	}
}