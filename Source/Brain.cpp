
#include "Brain.h"
#include "Error.h"
#include "Samples.h"
#include "Common/Help.h"

using namespace brain;

void Brain::set(const std::vector<int>& layers) {
	// Создание слоёв
	size_t countLayers = layers.size();
	if (countLayers == 0) {
		return;
	}

	_layers.resize(countLayers);

	// Создание нейронов в слоях, параметры нейронов по умолчанию
	for (size_t iLayer = 0; iLayer < countLayers; ++iLayer) {
		size_t countNeurons = layers[iLayer];

		if (countNeurons == 0) {
			_layers.clear();
			return;
		}

		_layers[iLayer].resize(countNeurons);

		for (size_t iNeuron = 0; iNeuron < countNeurons; ++iNeuron) {
			Neuron& neuron = _layers[iLayer][iNeuron];
			neuron._iIndex = iNeuron;
			neuron._iLayer = iLayer;
		}
	}

	// Создание связей
	for (size_t iLayer = 0; iLayer < countLayers; ++iLayer) {
		size_t iNextLayer = iLayer + 1;

		if (iNextLayer < countLayers) {
			size_t countNeurons = _layers[iLayer].size();

			for (size_t iNeuron = 0; iNeuron < countNeurons; ++iNeuron) {
				Neuron& neuron = _layers[iLayer][iNeuron];

				size_t countNextNeurons = _layers[iNextLayer].size();
				for (size_t iNextNeuron = 0; iNextNeuron < countNextNeurons; ++iNextNeuron) {
					neuron.addSynaps(&_layers[iNextLayer][iNextNeuron]);
				}
			}
		}
	}
}

void Brain::set(const Json::Value& dataJson) {

}

BrainPtr Brain::copy(const long double mutation) {
	BrainPtr brainPtr = std::shared_ptr<Brain>(new Brain());
	Brain& brain = *brainPtr.get();

	// Создание слоёв
	size_t countLayers = _layers.size();
	if (countLayers == 0) {
		return nullptr;
	}

	brain._layers.resize(countLayers);

	// Создание нейронов в слоях, параметры нейронов по умолчанию
	for (size_t iLayer = 0; iLayer < countLayers; ++iLayer) {
		size_t countNeurons = _layers[iLayer].size();

		if (countNeurons == 0) {
			brain._layers.clear();
			return nullptr;
		}

		brain._layers[iLayer].resize(countNeurons);

		for (size_t iNeuron = 0; iNeuron < countNeurons; ++iNeuron) {
			Neuron& neuron = brain._layers[iLayer][iNeuron];
			neuron._iIndex = iNeuron;
			neuron._iLayer = iLayer;
		}
	}

	// Создание связей
	for (size_t iLayer = 0; iLayer < countLayers; ++iLayer) {
		size_t iNextLayer = iLayer + 1;

		if (iNextLayer < countLayers) {
			size_t countNeurons = _layers[iLayer].size();
			size_t countNewNeurons = brain._layers[iLayer].size();

			if (countNeurons != countNewNeurons) {
				return nullptr;
			}

			for (size_t iNeuron = 0; iNeuron < countNeurons; ++iNeuron) {
				Neuron& neuron = _layers[iLayer][iNeuron];
				Neuron& newNeuron = brain._layers[iLayer][iNeuron];

				size_t countSynaps = neuron._synapses.size();
				size_t countNextNeurons = _layers[iNextLayer].size();
				size_t countNewNextNeurons = brain._layers[iNextLayer].size();

				if (countSynaps != countNextNeurons || countNextNeurons != countNewNextNeurons) {
					continue;
				}

				for (size_t iNextNeuron = 0; iNextNeuron < countNextNeurons; ++iNextNeuron) {
					long double weight = neuron._synapses[iNextNeuron]._weight;

					if (mutation != 0.0) {						
						long double randomValue = help::random(-mutation, mutation);
						weight = weight + randomValue;
					}

					newNeuron.addSynaps(&brain._layers[iNextLayer][iNextNeuron], weight);
				}
			}
		}
	}

	return brainPtr;
}

void Brain::mutating(const long double mutation) {
	for (auto& neurons : _layers) {
		for (auto& neuron : neurons) {
			for (auto& synaps : neuron._synapses) {
				long double randomValue = help::random(-mutation, mutation);				
				long double weight = synaps._weight;
				weight = weight + (weight * randomValue);
				synaps._weight = weight;
			}
		}
	}

	_mutation = mutation;
}

void Brain::action() {

	size_t countLayers = _layers.size();
	if (countLayers > 1) {
		--countLayers;
	}

	for (size_t iLayer = 0; iLayer < countLayers; ++iLayer) {
		size_t countNeurons = _layers[iLayer].size();

		for (size_t iNeuron = 0; iNeuron < countNeurons; ++iNeuron) {
			Neuron& neuron = _layers[iLayer][iNeuron];
			neuron.action();
		}
	}
}

void Brain::setIn(std::vector<long double>& in) {
	if (_layers.empty()) {
		return;
	}

	for (auto& neurons : _layers) {
		for (auto& neuron : neurons) {
			neuron._value = 0.0;
		}
	}

	auto& neurons = _layers[0];
	size_t countNeurons = neurons.size();

	if (countNeurons != in.size()) {
		return;
	}

	for (size_t iNeuron = 0; iNeuron < countNeurons; ++iNeuron) {
		Neuron& neuron = neurons[iNeuron];
		const long double& value = in[iNeuron];

		neuron._value = value;
	}
}

void Brain::getOut(std::vector<long double>& out) {
	size_t countLayers = _layers.size();
	if (countLayers == 0) {
		return;
	}

	size_t indexLastLayer = countLayers - 1;

	auto& neurons = _layers[indexLastLayer];
	size_t countNeurons = neurons.size();

	if (countNeurons != out.size()) {
		out.resize(countNeurons);
	}

	for (size_t iNeuron = 0; iNeuron < countNeurons; ++iNeuron) {
		Neuron& neuron = neurons[iNeuron];
		out[iNeuron] = neuron._value;
	}
}

// static

BrainPtr Brain::load(const std::string& fileName) {
	BrainPtr brainPtr(new Brain);
	Brain &brain = *brainPtr.get();

	Json::Value brainJson;
	help::loadJson(fileName, brainJson);

	if (brainJson.empty()) {
		return nullptr;
	}

	if (!brainJson["layers"].isArray()) {
		return nullptr;
	}

	// Создание нейронов

	Json::Value& layerJson = brainJson["layers"];
	const size_t countLayers = layerJson.size();
	brain._layers.resize(countLayers);

	for (size_t iLayer = 0; iLayer < countLayers; ++iLayer) {
		//size_t countNeurons = brain._layers[iLayer].size();
		
		size_t countNeurons = layerJson[iLayer].size();

		if (countNeurons == 0) {
			brain._layers.clear();
			return nullptr;
		}

		brain._layers[iLayer].resize(countNeurons);

		for (size_t iNeuron = 0; iNeuron < countNeurons; ++iNeuron) {
			Neuron& neuron = brain._layers[iLayer][iNeuron];
			neuron._iIndex = iNeuron;
			neuron._iLayer = iLayer;
		}
	}

	// Создание связей
	for (size_t iLayer = 0; iLayer < countLayers; ++iLayer) {
		auto& neurons = brain._layers[iLayer];
		const auto& neuronsJson = layerJson[iLayer];
		auto countNeurons = neuronsJson.size();

		for (size_t iN = 0; iN < countNeurons; ++iN) {
			const auto& neuronJson = neuronsJson[iN];
			const double valueActivation = neuronJson["activation"].isDouble() ? neuronJson["activation"].asDouble() : 0.0;

			const double layer = neuronJson["layer"].isInt() ? neuronJson["layer"].asInt() : -1;
			const double index = neuronJson["index"].isInt() ? neuronJson["index"].asInt() : -1;
			if (layer == -1 || index == -1) {
				return nullptr;
			}

			auto& neuron = brain._layers[layer][index];

			auto& synapses = neuron._synapses;
			auto& synapsesJson = neuronJson["synaps"];

			for (size_t iS = 0; iS < synapsesJson.size(); ++iS) {
				const auto& synapseJson = synapsesJson[iS];

				//auto weight = synapseJson["weight"].isDouble() ? synapseJson["weight"].asDouble() : 0.0;
				auto layer = synapseJson["layerNeuron"].isInt() ? synapseJson["layerNeuron"].asInt() : -1;
				auto index = synapseJson["indexNeuron"].isInt() ? synapseJson["indexNeuron"].asInt() : -1;

				const std::string& weightStr = synapseJson["weight"].isString() ? synapseJson["weight"].asString() : "0.0";
				long double weight = std::stod(weightStr);

				if (!(layer >= 0 && index >= 0)) {
					return nullptr;
				}

				Neuron& newSynapsNeuron = brain._layers[layer][index];
				neuron.addSynaps(&newSynapsNeuron, weight);
			}
		}
	}

	Samples::parceFromJson(brainJson["samples"]);

	return brainPtr;
}

void Brain::save(Brain& brain, const std::string& fileName) {
	Json::Value brainJson;
	brainJson["generation"] = brain._generation;
	brainJson["number"] = brain._generationNumber;
	brainJson["mutation"] = std::to_string(brain._mutation);
	brainJson["error"] = std::to_string(brain._errorValue);

	for (auto& layer : brain._layers) {
		brainJson["configLayers"].append(layer.size());
	}

	Json::Value samplesJson;
	Samples::toJson(samplesJson);
	brainJson["samples"] = samplesJson;

	for (size_t iL = 0; iL < brain._layers.size(); ++iL) {
		auto& neurons = brain._layers[iL];

		Json::Value layerJson;

		for (size_t iN = 0; iN < neurons.size(); ++iN) {
			Json::Value neuronJson;
			auto& neuron = neurons[iN];

			neuronJson["layer"] = neuron._iLayer;
			neuronJson["index"] = neuron._iIndex;
			//neuronJson["activation"] = neuron.

			auto& synapses = neurons[iN]._synapses;

			for (size_t iS = 0; iS < synapses.size(); ++iS) {
				auto& synaps = synapses[iS];
				auto& synapsNeyron = synaps._neuron;

				Json::Value synapsJson;

				synapsJson["weight"] = std::to_string(synaps._weight);
				synapsJson["layerNeuron"] = synapsNeyron->_iLayer;
				synapsJson["indexNeuron"] = synapsNeyron->_iIndex;

				neuronJson["synaps"].append(synapsJson);
			}

			layerJson.append(neuronJson);
		}

		brainJson["layers"].append(layerJson);
	}

	help::saveJson(fileName, brainJson);
}