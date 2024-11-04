#pragma once
#include "SpaceObject.hpp"


namespace mt
{
	class Asteroid : public SpaceObject // ����� Asteroid(������������� �� SpaceObject)
	{
	public:
		Asteroid() = default; // ����������� ��-���������
		virtual ~Asteroid() {} // ����������� ����������

		void SpecialAbility() override
		{
			std::cout << "Special" << std::endl;
		}
	};
}
