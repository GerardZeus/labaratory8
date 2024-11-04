#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace mt
{

	class SpaceObject //������� ����� ��� ����������� ��������
	{
	protected:
		float m_x, m_y; //���������� ��� ��������� 
		float m_vx, m_vy; // ������� �������� �� X � Y 
		float m_r; // ���������� ��� �������
		sf::Texture m_textureShip; // ������ ������ ��� �������� �������� ��������
		sf::Sprite m_spriteShip; // ������ ������ ��� ����������� �������� ��������
		bool m_active;

	public:
		SpaceObject() = default; // ����������� ��-���������
		virtual ~SpaceObject() {} // ����������� ����������

		bool Setup(float x, float y, float vx, float vy, 
			const std::string& texturePath, float scaleFactor) // ����� ��������� ������� � ������������, ���������, ��������� � ���������
		{
			m_x = x; // ������������� ���������� X
			m_y = y; // ������������� ���������� Y
			m_vx = vx; // ������������� �������� �� ��� X
			m_vy = vy; // ������������� �������� �� ��� Y

			if (!m_textureShip.loadFromFile(texturePath)) // �������� �������� 
			{
				std::cout << "Error while loading " << texturePath << std::endl;
				return false; // ���� �������� �� ������ ���������, �� ������� ������
			}
			m_spriteShip.setTexture(m_textureShip); // ������������� �������� ��� �������

			m_spriteShip.setScale(scaleFactor, scaleFactor); //������������� ������� �������
			m_spriteShip.setOrigin(m_textureShip.getSize().x / 2, m_textureShip.getSize().y / 2); // ������������� ����� ������� � ����������� ��� �����(���������� � ����� ������� ���� �������)
			m_spriteShip.setPosition(m_x, m_y); // ������������� ���������� �������

			// ���������� ������� ������� �� ������ �������� ��������
			if (m_textureShip.getSize().x > m_textureShip.getSize().y)
				m_r = scaleFactor * m_textureShip.getSize().x / 2;
			else
				m_r = scaleFactor * m_textureShip.getSize().y / 2;
		}

		sf::Sprite Get()
		{
			return m_spriteShip; // ������ ��� ��������� �������
		}
		float X() { return m_x; } // ������ ��� ��������� X �������
		float Y() { return m_y; } // ������ ��� ��������� Y �������
		float R() { return m_r; } // ������ ��� ��������� ������� �������
		void SetActive(bool active) { m_active = active; }  // ������������ ������ ���������� �������
		bool IsActive() const { return m_active; } // ��������� ������ ���������� �������
		void Move(float dt) // ����� ��� ����������� �������
		{
			m_x += m_vx * dt; // ��������� ���������� X 
			m_y += m_vy * dt; // ��������� ���������� Y
			m_spriteShip.setPosition(m_x, m_y); //������������� ����� ���������� �������
		}

		virtual void SpecialAbility() = 0; //����������� ����� ��� ����������� ����������� �������
	};
}