
#pragma once

namespace brain {
	class Brain;
	class Neuron;

	class Synaps {
		friend Brain;

	public:
		Synaps()
			: _neuron(nullptr)
			, _weight(0.0)
		{}
		Synaps(Neuron* neuron, const long double weight)
			: _neuron(neuron)
			, _weight(weight)
		{}

		void action(long double value);

	private:
		Neuron* _neuron;
		long double _weight;

	};
}
