#include <vegetables/Cabbage.hpp>
#include <acs/Animal.hpp>
#include <iostream>
#include <fstream>
#include <cstdint>

namespace Animals {
	
	class Human : ACS::Animal {
		virtual bool Moo() override {
			std::cout << "Ouch!" << std::endl;
			return false;
		}
	};
}