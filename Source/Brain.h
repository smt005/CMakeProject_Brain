#pragma once

#include <vector>
#include <memory>

#include "json/json.h"
#include "Neuron.h"
#include "Error.h"

namespace brain {
	class Brain;

	typedef std::vector<Neuron> Neurons;
	typedef std::shared_ptr<Brain> BrainPtr;

	class Brain {
	public:
		Brain()
			: _errorValue(Error::maxError)
		{}

		Brain(const std::vector<int>& layers)
			: _errorValue(Error::maxError)
		{
			set(layers);
		}
		Brain(const Json::Value& dataJson) {
			set(dataJson);
		}
		
		void set(const std::vector<int>& layers);
		void set(const Json::Value& dataJson);
		BrainPtr copy(const long double mutation = 0.0);
		void mutating(const long double mutation);

		void action();
		void setIn(std::vector<long double>& in);
		void getOut(std::vector<long double>& out);

	public:
		static bool compareLess(BrainPtr first, BrainPtr second) {
			return first->_errorValue < second->_errorValue ? true : false;
		}

	public:
		static BrainPtr load(const std::string& fileName);
		static void save(Brain& brain, const std::string& fileName);

	public:
		// Информация
		long double _errorValue;
		int _generation;
		int _generationNumber;
		long double _mutation;

	private:
		std::vector<Neurons> _layers;
	};
}
