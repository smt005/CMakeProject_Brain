#pragma once

#include <vector>

#include "Synaps.h"

namespace brain {
	class Brain;

	class Neuron {
		friend Brain;

	public:
		Neuron()
			: _iLayer(-1)
			, _iIndex(-1)
			,_value(0.0)
		{}
		
		virtual ~Neuron() {}
		
		inline void addSynaps(Neuron* neuron, const long double weight = 1.0) {
			_synapses.emplace_back(neuron, weight);
		}
		
		inline void addValue(const double value) {
			_value += value;
		}
		
		void action();		
		long double activationFunction();
		
	private:
		int _iLayer;
		int _iIndex;

		long double _value;
		std::vector<Synaps> _synapses;
		
	};
}
