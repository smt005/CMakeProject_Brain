
#pragma once

#include <vector>

namespace brain {
	class Error
	{
	public:
		static long double error—alculation(const std::vector<long double>& out, const std::vector<long double>& expectedOut);

	public:
		static long double maxError;
	};
}
