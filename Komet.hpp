#pragma once
#include "SpaceObject.hpp"

namespace mt
{
	class Enemy : public SpaceObject // ����� Enemy(������������� �� SpaceObject)
	{
	public:
		Enemy() = default;  // ����������� ��-���������
		virtual ~Enemy() {} // ����������� ����������


		void SpecialAbility() override
		{
			std::cout << "Special" << std::endl;
		}

	};
}
