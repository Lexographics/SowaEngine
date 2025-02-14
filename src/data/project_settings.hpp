#ifndef PROJECT_SETTINGS_HPP
#define PROJECT_SETTINGS_HPP
#pragma once

#include <string>
#include <unordered_map>

class ProjectSettings {
  public:
	void Load();
	bool Save();

  public:
	std::string name = "";
	std::string author = "";

	struct {
		struct {
			int width = 1280;
			int height = 720;
		} window;

		struct {
			int width = 1280;
			int height = 720;
		} viewport;
	} rendering;
};

#endif // PROJECT_SETTINGS_HPP