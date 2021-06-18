#pragma once

#include <iostream>
#include "Common/Help.h"

namespace neuralNetwork {
	typedef double long TypeValue;

	void infoWeights();

	//...
	struct Sample {
		std::vector<TypeValue> in;
		std::vector<TypeValue> out;
		std::vector<TypeValue> error;

		Sample() {}
		Sample(const std::vector<TypeValue>& _in, const std::vector<TypeValue>& _out) : in(_in), out(_out) {}
	};

	struct NN {
		TypeValue alpha = 0.5;
		TypeValue errorValue = 0.0001;
		int typeSigmoid = 0;

		std::vector<size_t> config;
		std::vector<std::vector<std::vector<TypeValue>>> weights; // [слои [веса слоя [веса к нейронам]]]
		std::vector<Sample> samples;
	};

	NN net;

	//...
	void init() {

		//...
		std::string configName = "22_t"; net.typeSigmoid = 0;
		//std::string configName = "22"; net.typeSigmoid = 3;
		//std::string configName = "432";
		bool testWeight = false;

		//...
		if (configName == "22_t") {
			net.config = { 2, 2 };
			net.samples.emplace_back(Sample({ 1.0, 0.0 }, { 0.0, 1.0 }));
			net.samples.emplace_back(Sample({ 0.0, 1.0 }, { 1.0, 0.0 }));
		}
		else if (configName == "22") {
			net.config = { 2, 2 };

			net.samples.emplace_back(Sample({ 0.0, 0.0 }, { 0.0, 0.0 }));
			net.samples.emplace_back(Sample({ 1.0, 0.0 }, { 0.0, 1.0 }));
			net.samples.emplace_back(Sample({ 0.0, 1.0 }, { 1.0, 0.0 }));
			net.samples.emplace_back(Sample({ 1.0, 1.0 }, { 1.0, 1.0 }));
		}
		else if (configName == "432") {
			net.config = { 4, 3, 2 };

			net.samples.emplace_back(Sample({ 1.0, 1.0, 0.0, 0.0 }, { 1.0, 0.0 }));
			net.samples.emplace_back(Sample({ 0.0, 0.0, 1.0, 1.0 }, { 0.0, 1.0 }));
		}

		//...
		if (net.config.size() <= 1) {
			return;
		}

		size_t countWeightsLayer = net.config.size() - 1;
		if (countWeightsLayer < 1) {
			return;
		}

		net.weights.resize(countWeightsLayer);
		for (size_t iL = 0; iL < countWeightsLayer; ++iL) {
			net.weights[iL].resize(net.config[iL]);
			size_t countNextlayer = net.config[iL + 1];

			for (size_t iN = 0; iN < net.weights[iL].size(); ++iN) {
				net.weights[iL][iN].resize(countNextlayer);

				if (!testWeight) {
					for (size_t i = 0; i < net.weights[iL][iN].size(); ++i) {
						net.weights[iL][iN][i] = help::random(0.1, 0.9);
					}
				}
				else {
					for (size_t i = 0; i < net.weights[iL][iN].size(); ++i) {
						net.weights[iL][iN][i] = 0.0;
					}
				}
			}
		}

		if (testWeight) {
			if (configName == "22") {
				if (net.typeSigmoid == 0) {
					net.weights[0][0][1] = 1.0;
					net.weights[0][1][0] = 1.0;
				}
				else if (net.typeSigmoid == 1) {
					TypeValue k = 1;
					net.weights[0][0][1] = 1.0 * k;
					net.weights[0][1][0] = 1.0 * k;
				}
				else if (net.typeSigmoid == 2) {
					TypeValue k = 10;
					net.weights[0][0][1] = 1.0 * k;
					net.weights[0][1][0] = 1.0 * k;
				}
				else if (net.typeSigmoid == 3) {
					TypeValue k = 10;
					net.weights[0][0][1] = 1.0 * k;
					net.weights[0][1][0] = 1.0 * k;
				}
			}
			else if (configName == "432") {
				if (net.typeSigmoid == 0) {
					net.weights[0][0][0] = 1.0;
					net.weights[0][1][1] = 1.0;

					net.weights[1][0][0] = 0.5;
					net.weights[1][1][0] = 0.5;

					//...
					net.weights[0][2][2] = 0.5;
					net.weights[0][3][2] = 0.5;

					net.weights[1][2][1] = 1.0;
				}
				else if (net.typeSigmoid == 1) {
					TypeValue k = 1;

					net.weights[0][0][0] = 1.0 * k;
					net.weights[0][1][1] = 1.0 * k;

					net.weights[1][0][0] = 0.5 * k;
					net.weights[1][1][0] = 0.5 * k;

					//...
					net.weights[0][2][2] = 0.5 * k;
					net.weights[0][3][2] = 0.5 * k;

					net.weights[1][2][1] = 1.0 * k;
				}
				else if (net.typeSigmoid == 2) {
					TypeValue k = 10;

					net.weights[0][0][0] = 1.0 * k;
					net.weights[0][1][1] = 1.0 * k;

					net.weights[1][0][0] = 0.5 * k;
					net.weights[1][1][0] = 0.5 * k;

					//...
					net.weights[0][2][2] = 0.5 * k;
					net.weights[0][3][2] = 0.5 * k;

					net.weights[1][2][1] = 1.0 * k;
				}
				else if (net.typeSigmoid == 3) {
					TypeValue k = 10;

					net.weights[0][0][0] = 1.0 * k;
					net.weights[0][1][1] = 1.0 * k;

					net.weights[1][0][0] = 0.5 * k;
					net.weights[1][1][0] = 0.5 * k;

					//...
					net.weights[0][2][2] = 0.5 * k;
					net.weights[0][3][2] = 0.5 * k;

					net.weights[1][2][1] = 1.0 * k;
				}
			}
		}

		//...
		infoWeights();
	}

	void query(Sample& sample) {
		std::string infoStr;

		//...
		if (net.config[0] != sample.in.size()) {
			return;
		}

		std::vector<TypeValue> result;
		result = sample.in;

		size_t countL = net.weights.size();

		for (size_t iL = 0; iL < countL; ++iL) {
			std::vector<TypeValue> out;
			out.resize(net.config[iL + 1]);

			size_t countN = net.weights[iL].size();

			for (size_t iN = 0; iN < countN; ++iN) {

				size_t countW = net.weights[iL][iN].size();
				for (size_t iW = 0; iW < countW; ++iW) {
					out[iW] += result[iN] * net.weights[iL][iN][iW];
				}
			}

			result = out;
			for (auto& value : result) {
				value = help::sigmoida(value, net.typeSigmoid);
			}
		}

		size_t count = result.size();
		sample.error.resize(count);
		for (size_t i = 0; i < result.size(); ++i) {
			sample.error[i] = sample.out[i] - result[i];
		}

		infoStr += "query\n";

		{
			infoStr += "in:  [";
			for (auto& value : sample.in) {
				infoStr += std::to_string(value) + " ";
			}
			infoStr += "]\n";
		}

		{
			infoStr += " o:  [";
			for (auto& value : sample.out) {
				infoStr += std::to_string(value) + " ";
			}
			infoStr += "]\n";
		}

		{
			infoStr += " t:  [";
			for (auto& value : result) {
				infoStr += std::to_string(value) + " ";
			}
			infoStr += "]\n";
		}

		std::cout << infoStr << std::endl;
	}

	void query() {
		std::string infoStr = "query\n";

		for (auto& sample : net.samples) {
			query(sample);
		}

		std::cout << infoStr << std::endl;
	}

	void train(const int count) {

		for (Sample& sample : net.samples)
		{
		}
	}

	void info() {
		std::cout << "neuralNetwork::info" << std::endl;
	}

	void infoWeights() {
		std::string infoStr = "weights\n";

		for (size_t iL = 0; iL < net.weights.size(); ++iL) {
			for (size_t iN = 0; iN < net.weights[iL].size(); ++iN) {
				infoStr += "[";

				for (size_t iW = 0; iW < net.weights[iL][iN].size(); ++iW) {
					infoStr += std::to_string(net.weights[iL][iN][iW]) + " ";
				}

				infoStr += "]\n";
			}
			infoStr += "\n";
		}

		std::cout << infoStr << std::endl;
	}
};
