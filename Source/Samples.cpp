
#include "Samples.h"

using namespace brain;

// Samples
long double Samples::_error;
std::vector<Sample> Samples::_samples;

// Sample

long double Sample::getError() {
	long double error = Error::error—alculation(_out, _expectedOut);
	if (error < _error) {		
		_bestOut = _out;
	}

	_error = error;
	return _error;
}

// Samples
void Samples::addSample(const Sample& sample) {
	_samples.push_back(sample);
}

void Samples::addSample(const std::vector<long double>& in, const std::vector<long double>& expectedOut) {
	_samples.emplace_back(in, expectedOut);
}

long double Samples::getError() {
	_error = 0.0;

	for (auto& sample : _samples) {
		long double error = sample.getError();
		if (error == Error::maxError) {
			_error = Error::maxError;
			return _error;
		}

		_error += error;
	}

	return _error;
}

void Samples::parceFromJson(const Json::Value& dataJson) {
	if (dataJson.empty() || !dataJson.isArray()) {
		return;
	}

	size_t countSamples = dataJson.size();
	_samples.resize(countSamples);

	for (size_t index = 0; index < countSamples; ++index) {
		const Json::Value& sampleJson = dataJson[index];
		Sample& sample = _samples[index];

		auto parceArray = [](const Json::Value& arrayJson, std::vector<long double>& array) {
			if (arrayJson.isArray()) {
				for (const auto& valueJson : arrayJson) {
					const std::string& valueStr = valueJson.isString() ? valueJson.asString() : "0.0";
					long double value = std::stod(valueStr);
					array.push_back(value);
				}
			}
		};

		parceArray(sampleJson["in"], sample._in);
		parceArray(sampleJson["expOut"], sample._expectedOut);
	}
}

void Samples::toJson(Json::Value& dataJson) {
	for (Sample& sample : _samples) {
		Json::Value sampleJson;

		Json::Value inJson;
		for (auto& value : sample._in) {
			inJson.append(std::to_string(value));
		}

		Json::Value expectedOutJson;
		for (auto& value : sample._expectedOut) {
			expectedOutJson.append(std::to_string(value));
		}

		Json::Value outJson;
		for (auto& value : sample._out) {
			outJson.append(std::to_string(value));
		}

		sampleJson["in"] = inJson;
		sampleJson["expOut"] = expectedOutJson;
		sampleJson["out"] = outJson;

		dataJson.append(sampleJson);
	}
}
