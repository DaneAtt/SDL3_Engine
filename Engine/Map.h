#pragma once
#include "nlohmann/json.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include "SDL3/SDL.h"
#include "EngineAPI.h"
#include "JSONTemplate.h"

using json = nlohmann::json;

class ENGINE_API Map : public JSONTemplate
{
public:
    struct TileData {
        int srcX, srcY;
        int xPos, yPos;
        SDL_FlipMode flip;
    };

    Map(const char* path, int scale);
    ~Map();

    void loadJSONC() override;
    std::vector<Map::TileData>& searchMap(std::string name);
    void createEntities(std::string texID, std::string name, int group, int offsetX, int offsetY);
    void createColliders(std::string name, int group, int offsetX, int offsetY);
    int getMapWidth() const { return mapWidth * 32 * Scale; }
    int getMapHeight() const { return mapHeight * 48 * Scale; }


private:
    std::vector<TileData> tileVec;
    std::map<std::string, std::vector<TileData>> mapLayers;
    int Scale;
    int mapWidth;
    int mapHeight;
};