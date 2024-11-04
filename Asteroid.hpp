#pragma once
#include "SpaceObject.hpp"


namespace mt
{
	class Asteroid : public SpaceObject // Класс Asteroid(наследованный от SpaceObject)
	{
	public:
		Asteroid() = default; // Конструктор по-умолчанию
		virtual ~Asteroid() {} // Виртуальный деструктор

		void SpecialAbility() override
		{
			std::cout << "Special" << std::endl;
		}
	};
}
