
#include "Neuron.h"

using namespace brain;

void Neuron::action()
{
	long double value = activationFunction();

	for (Synaps& synaps : _synapses) {
		synaps.action(value);
	}

	_value = 0.0;
}

long double Neuron::activationFunction() {
	long double value = _value;
	return value;
}
