#pragma once
#include "SpaceObject.hpp"

namespace mt
{
	class Enemy : public SpaceObject // Класс Enemy(наследованный от SpaceObject)
	{
	public:
		Enemy() = default;  // Конструктор по-умолчанию
		virtual ~Enemy() {} // Виртуальный деструктор


		void SpecialAbility() override
		{
			std::cout << "Special" << std::endl;
		}

	};
}
