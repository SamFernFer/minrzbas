#ifndef FENTON_OTTER_HPP
#define FENTON_OTTER_HPP

#include <acs/Animal.hpp>
#include <iostream>

namespace Animals {
	
	class Otter : public ACS::Animal {
		virtual bool Moo() override {
			std::cout << "Moo!" << std::endl;
			return true;
		}
	};
}
#endif