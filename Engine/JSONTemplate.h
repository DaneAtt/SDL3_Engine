#pragma once
#include "EngineAPI.h"
#include <iostream>
#include <fstream>
#include <set>
#include <nlohmann/json.hpp>

class ENGINE_API JSONTemplate
{
public:
	virtual void loadJSONC() {};

	void loadJSON(const char* path)
	{
		if (alreadyLoaded(path)) return;
		readJSONFile(path);
		loadJSONC();
	}

	bool alreadyLoaded(const char* path)
	{
		if (loadedFiles.find(path) != loadedFiles.end()) {
			return true;  //Only blocks THIS specific file if already loaded
		}
		loadedFiles.insert(path);
		return false;
	}

	void readJSONFile(const char* path)
	{
		std::ifstream file(path);
		if (!file.is_open()) { std::cout << "Animation JSON File not found" << "\n"; return; }
		file >> jsonFile;
		file.close();
		if (jsonFile.empty()) { std::cout << "JSON file is empty!\n"; return; }
	}

protected:
	std::set<std::string> loadedFiles;
	nlohmann::json jsonFile;
};