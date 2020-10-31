
#include "BrainManager.h"
#include "Brain.h"
#include "Samples.h"
#include "Error.h"

#include <vector>
#include <iostream>

using namespace brain;

std::string BrainManager::defaultFileName = "temp_brain.json";

BrainPtr bestBrainPtr;
long double errorBestBrain = Error::maxError;

size_t countItems = 1000;
long double kMutation = 1.0;
long double errorPoint = 0.1;

bool statePause = true;
size_t generation = 0;

void BrainManager::init() {
	std::vector<int> layers = { 2, 2 };
	bestBrainPtr = std::make_shared<Brain>(layers);

	Samples::addSample({ 0.0, 0.0 }, { 0.0, 0.0 });
	Samples::addSample({ 1.0, 0.0 }, { -1.0, 0.0 });
	Samples::addSample({ 0.0, 1.0 }, { 0.0, 1.0 });
	Samples::addSample({ 1.0, 1.0 }, { -1.0, 1.0 });

	/*std::vector<int> layers = { 3, 4 };
	bestBrainPtr = std::make_shared<Brain>(layers);

	Samples::addSample({ 0.0, 1.0, 0.0 }, { 1.0, 0.0, 0.0, 0.0 });
	Samples::addSample({ 1.0, 0.0, 1.0 }, { -1.0, 0.0, 0.0, 0.0 });
	Samples::addSample({ 1.0, 1.0, 0.0 }, { 0.0, -1.0, 0.0, 0.0 });
	Samples::addSample({ 0.0, 1.0, 1.0 }, { 0.0, 1.0, 0.0, 0.0 });
	Samples::addSample({ 1.0, 1.0, 1.0 }, { 0.0, 0.0, 0.0, 1.0 });*/

	print("START");
};

void BrainManager::update() {
	if (statePause) {
		return;
	}

	generateGeneration();
}

void BrainManager::enterCommand() {
	std::cout << "Enter command..." << std::endl;

	char textChar[256];
	textChar[0] = '\0';

	std::cin >> textChar;

	std::string textStr(textChar);

	if (!textStr.empty()) {
		if (textStr[0] == 's') {
			save("Brain\\" + textStr + ".json");
		}
		else if (textStr[0] == 'l') {
			load("Brain\\" + textStr + ".json");
		}
		else if (textStr[0] == 'n') {
			setPause(false);
		}
	}
}

void BrainManager::load(const std::string& fileName) {
	BrainPtr brainPtr = Brain::load("Brain\\" + fileName + ".json");
	if (!brainPtr) {
		print("Brain: " + fileName + " loaded FAIL.");
		return;
	}

	bestBrainPtr = brainPtr;
	errorBestBrain = Error::maxError;
	kMutation = 1.0;
	errorPoint = 0.1;

	print("Brain: " + fileName + " is loaded.");

	printError();
}

void BrainManager::save(const std::string& fileName) {
	if (!bestBrainPtr) {
		print("Brain: " + fileName + " saved FAIL.");
		return;
	}

	Brain::save(*bestBrainPtr.get(), fileName);
	print("Brain: " + fileName + " is saved.");
}

void BrainManager::print(const std::string& text) {
	std::cout << text << std::endl;
}

void BrainManager::printSamples() {
	std::stringstream textStream;

	for (auto& samle : Samples::_samples) {
		auto& in = samle._in;
		auto& out = samle._out;

		size_t countInValue = in.size();
		size_t countOutValue = out.size();
		size_t countValue = countOutValue > countInValue ? countOutValue : countInValue;

		textStream << std::endl << "// error: " << samle._error << std::endl;

		for (size_t index = 0; index < countValue; ++index) {
			// in
			if (index < countInValue) {
				textStream << '[' << in[index] << "]\t->\t";
			}
			else {
				textStream << "     \t->\t";
			}

			// out
			if (index < countOutValue) {
				textStream << '[' << out[index] << "]";
			}

			textStream << std::endl;
		}
	}

	print(textStream.str());
}

void BrainManager::printError() {
	if (!bestBrainPtr) {
		return;
	}

	Brain &brain = *bestBrainPtr.get();
	std::stringstream textStream;

	for (auto& sample : Samples::_samples) {
		brain.setIn(sample._in);
		brain.action();
		brain.getOut(sample._out);
	}

	textStream << "//--------------------------" << std::endl;
	std::string errorStr = brain._errorValue == Error::maxError ? "not_error" : std::to_string(brain._errorValue);
	textStream << "Gen: " << brain._generation << ",\tnum: " << brain._generationNumber << ",\tmut: " << brain._mutation << ",\tERROR: " << errorStr << std::endl;

	/*for (auto& samle : Samples::_samples) {
		auto& in = samle._in;
		auto& out = samle._out;

		size_t countInValue = in.size();
		size_t countOutValue = out.size();
		size_t countValue = countOutValue > countInValue ? countOutValue : countInValue;

		textStream << std::endl << "// error: " << samle._error << std::endl;

		for (size_t index = 0; index < countValue; ++index) {
			// in
			if (index < countInValue) {
				textStream << '[' << in[index] << "]\t->\t";
			}
			else {
				textStream << "     \t->\t";
			}

			// out
			if (index < countOutValue) {
				textStream << '[' << out[index] << "]";
			}

			textStream << std::endl;
		}
	}*/

	print(textStream.str());

	printSamples();
}

int iPoint = 0;
int countPoint = 0;

void BrainManager::generateGeneration() {
	if (!bestBrainPtr) {
		init();
	}

	if (!bestBrainPtr) {
		return;
	}

	errorBestBrain = Error::maxError;
	BrainPtr bestCurrebtGenerationBrainPtr;

	for (size_t index = 0; index < countItems; ++index) {
		long double mutation = kMutation / (index + 1);

		BrainPtr brainPtr = bestBrainPtr->copy(mutation);
		
		if (!brainPtr) {
			continue;
		}

		for (auto& sample : Samples::_samples) {
			brainPtr->setIn(sample._in);
			brainPtr->action();
			brainPtr->getOut(sample._out);
		}

		long double errorValue = Samples::getError();
		brainPtr->_errorValue = errorValue;
		brainPtr->_generationNumber = index;
		brainPtr->_generation = generation;
		brainPtr->_mutation = mutation;

		if (errorValue < errorBestBrain) {
			errorBestBrain = errorValue;
			bestCurrebtGenerationBrainPtr = brainPtr;
		}
	}

	++generation;

	if (iPoint > 100) {
		std::cout << '.';
		iPoint = 0;

		if (countPoint > 100) {
			std::cout << std::endl;
			countPoint = 0;
		}
		++countPoint;
	}
	++iPoint;

	if (bestCurrebtGenerationBrainPtr) {
		bestBrainPtr = bestCurrebtGenerationBrainPtr;
		
		std::string errorStr = bestBrainPtr->_errorValue == Error::maxError ? "not_error" : std::to_string(bestBrainPtr->_errorValue);
		std::cout << "Gen: " << bestBrainPtr->_generation << ",\tnum: " << bestBrainPtr->_generationNumber << ",\tmut: " << bestBrainPtr->_mutation << ",\tERROR: " << errorStr << std::endl;

		if (bestBrainPtr->_errorValue < errorPoint) {
			errorPoint = errorPoint * 0.1;
			kMutation = kMutation * 0.1;
			setPause(true);
		}
	}
}

void BrainManager::setPause(const bool state) {
	if (statePause != state) {
		if (state) {
			print("\nPAUSE\n");
		}
		else {
			print("\nLEARNING\n");
		}

		if (state) {
			printError();
		}
	}

	statePause = state;
}

void BrainManager::setMutation(const long double& value) {
	kMutation = value;
}
