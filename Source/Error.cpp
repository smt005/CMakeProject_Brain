
#include "Error.h"
#include <limits>

using namespace brain;

// std::numeric_limits<double>::max();
// std::numeric_limits<double>::min();

long double Error::maxError = std::numeric_limits<double>::max();

long double Error::error—alculation(const std::vector<long double>& out, const std::vector<long double>& expectedOut) {
	size_t size = out.size();
	if (size != expectedOut.size()) {
		return maxError;
	}
	
	long double errorValue = 0.0;

	for (size_t index = 0; index < size; ++index) {
		long double outValue = out[index];
		long double expectedOutValue = expectedOut[index];

		long double differenceValue = expectedOutValue - outValue;
		differenceValue = std::abs(differenceValue);
		errorValue += differenceValue;
	}

	return errorValue;
}
