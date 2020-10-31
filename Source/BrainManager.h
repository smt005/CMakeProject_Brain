
#pragma once

#include <string>

namespace brain {
	class BrainManager
	{
	public:
		static void init();
		static void update();
		static void enterCommand();
		static void load(const std::string& fileName);
		static void save(const std::string& fileName);
		static void print(const std::string& text);
		static void printSamples();
		static void printError();

		static void setPause(const bool state);
		static void setMutation(const long double& value);

	public:
		static std::string defaultFileName;

	private:
		static void generateGeneration();
	};
}
