#ifndef SW_COMMAND_INTERFACE_HXX
#define SW_COMMAND_INTERFACE_HXX
#pragma once

#include <functional>
#include <string>
#include <vector>

struct CommandOption {
	std::string label;
	std::string description;
	std::function<void()> action;
};

class CommandInterface {
  public:
	virtual ~CommandInterface() = default;

	std::vector<CommandOption> options;
	int currentIndex = 0;

	bool text_input = false;
	std::wstring text = L"";
	size_t text_cursor = 0;
	std::wstring text_placeholder = L"";

	std::function<void()> action;

  private:
};

#endif // SW_COMMAND_INTERFACE_HXX