
#pragma once

#include <vector>
#include "json/json.h"
#include "Error.h"

namespace brain {
	class Sample {
	public:
		Sample()
			: _error(Error::maxError)
		{};

		Sample(const std::vector<long double>& in, const std::vector<long double>& expectedOut)
			: _in(in)
			, _expectedOut(expectedOut)
			, _error(Error::maxError)
		{};

		long double getError();

	public:
		std::vector<long double> _in;
		std::vector<long double> _expectedOut;
		std::vector<long double> _out;
		std::vector<long double> _bestOut;
		long double _error;
	};

	class Samples {
	public:
		static void addSample(const Sample& sample);
		static void addSample(const std::vector<long double>& in, const std::vector<long double>& expectedOut);

		static long double getError();

		static void parceFromJson(const Json::Value& dataJson);
		static void toJson(Json::Value& dataJson);

	public:
		static long double _error;
		static std::vector<Sample> _samples;
	};
}
