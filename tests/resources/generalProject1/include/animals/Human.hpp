#ifndef FENTON_HUMAN_HPP
#define FENTON_HUMAN_HPP

#include <vegetables/Cabbage.hpp>
#include <acs/Animal.hpp>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

namespace Animals {
	
	class Special {
		std::string specialID = "ERROR";
		virtual Special& retrieve() = 0;
		virtual std::string getID();
	};
	class Intermediate : public Special {
		std::string intermediateID = "None";
		Special& retrieve() override;
		std::string getID() override;
	};
	class Human : public ACS::Animal, public virtual Intermediate {
		Special& retrieve() final;

		virtual bool Moo() override {
			std::cout << "Ouch!" << std::endl;
			return false;
		}
	};
}
#endif