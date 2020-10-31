
#include "Synaps.h"
#include "Neuron.h"

using namespace brain;

void Synaps::action(long double value) {
	_neuron->addValue(value * _weight);
}
