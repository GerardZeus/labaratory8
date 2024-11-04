#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Bullet.hpp"

namespace mt
{

	class Ship // ��������� ������
	{
		const int m_bulletR = 10; // ������ ����
		float m_v_bullet = 700; //�������� ����


		float m_x, m_y; // ���������� ������� 
		float m_alpha; // ���� �������� ������� 
		float m_v; // �������� �������
		sf::Texture m_textureShip; // ������ ������ ��� �������� �������� �������
		sf::Sprite m_spriteShip; // ������ ������ ��� ����������� �������� ������� 

		Bullet bullet; // ������ ������ Bullet


	public:
		Ship() = default; // ����������� ��-���������
		Ship(float x, float y) // ����������� �������������� ���������� �������
		{
			m_x = x;
			m_y = y;
		}

		bool Setup(float x, float y) // ������ ��� ��������� ��������� ���������� �������
		{
			m_x = x; // ������������� ���������� X
			m_y = y; // ������������� ���������� Y
			m_alpha = 0; // ������������� ���� ��������
			m_v = 0; // ������������� ��������

			if (!m_textureShip.loadFromFile("assets\\Starship.png")) // ��������� �������� �������
			{
				std::cout << "Error while loading Starship.png" << std::endl;
				return false; // ���� �������� �� ������ ���������, �� ������� ������
			}
			m_spriteShip.setTexture(m_textureShip); // ������������� �������� ��� �������

			m_spriteShip.setScale(0.2f, 0.2f); // ������������� ������� �������
			m_spriteShip.setOrigin(m_textureShip.getSize().x / 2, m_textureShip.getSize().y / 2); // ������������� ����� ������� � ����������� ��� �����(���������� � ����� ������� ���� �������)
			m_spriteShip.setPosition(m_x, m_y); // ������������� ���������� �������
			m_spriteShip.setRotation(m_alpha); // ������������� ���� �������� �������
		}

		void Attack() // ����� ��� �������� 
		{
			if (!bullet.Available()) // ���� ���� �� �������
			{
				float alphaRad = acos(-1) * m_alpha / 180; // �������������� � �������
				float bullet_vx = (m_v + m_v_bullet) * cos(alphaRad); // ���������� �������� ���� �� X
				float bullet_vy = (m_v + m_v_bullet) * sin(alphaRad); // ���������� �������� ���� �� Y
				bullet.Setup(m_x, m_y, m_bulletR, bullet_vx, bullet_vy); // ������������� ���������, ������ � �������� ����
				bullet.Available(true); // ��������� ���� � �������� ���������
			}
		}
		void UpdateBulletPosition(float dt, float windowWidth, float windowHeight) // ����� ��� ���������� ������� ����
		{
			if (bullet.Available()) // ���� ���� �������
			{
				bullet.Move(dt); // ������ ��� ��������

				if (bullet.X() < 0 || bullet.X() > windowWidth || bullet.Y() < 0 || bullet.Y() > windowHeight) // ���� ���� ����� �� ������� ����
				{
					bullet.Available(false); // �� ��� ��������������
				}
			}
		}

		void setVelocity(float dv) // ������� ��� ��������� �������� �������
		{
			m_v += dv; // ����������� �������� �� �������� ������ v
		}

		void Move(float dt) // ����� ��� ����������� �������
		{
			bullet.Move(dt); // ����������� ����

			float alphaRad = acos(-1) * m_alpha / 180; // ����������� ������� � �������
			m_x += m_v * cos(alphaRad); // ��������� ���������� X �� ������ �������� ���� ��������
			m_y += m_v * sin(alphaRad); // ��������� ���������� Y �� ������ �������� ���� ��������
			m_spriteShip.setPosition(m_x, m_y); //������������� ����� ���������� �������
		}

		void Rotate(float dalpha) // ������� ��� �������� �������
		{
			m_alpha += dalpha; // �������� �������� �� �������� ������ alpha
			m_spriteShip.setRotation(m_alpha); // ������������� ����� ���� �������
		}

		sf::Sprite Get() // ������ ��� ��������� �������
		{
			return m_spriteShip;
		}
		float X() const { return m_x; } // ������ ��� ��������� X �������
		float Y() const { return m_y; } // ������ ��� ��������� Y �������
		float Radius() const { return m_spriteShip.getGlobalBounds().width / 2.5; } // ������ ��� ��������� ������� �������

		void ReverSMove() // ������� ��� ��������� �������� �������
		{
			float alphaRad = acos(-1) * m_alpha / 180; // ����������� ������� � �������
			m_x -= m_v * cos(alphaRad); // ��������� ���������� X (� �������� �������)
			m_y -= m_v * sin(alphaRad); // ��������� ���������� Y (� �������� �������)
			m_spriteShip.setPosition(m_x, m_y); // ������������� ����� ���������� �������
		}
		void RepelFrom(float cx, float cy, float repel = 0.3f) // ������� ��� ������������ ������� �� ���������
		{
			float dx = m_x - cx;
			float dy = m_y - cy;
			float dist = sqrt(dx * dx + dy * dy); // ��������� ���������� �� ���������

			if (dist != 0) // ���� ���������� �� ����� 0, �� ��������� �������
			{
				m_x += (dx / dist) * repel; // ��������� ���������� X
				m_y += (dy / dist) * repel; // ��������� ���������� Y
				m_spriteShip.setPosition(m_x, m_y); // ������������� ����� ���������� �������
			}
		}
		bool BulletAvailable() // ����� ��� �������� ����������� ����
		{
			return bullet.Available();
		}

		void BulletReset() // ����� ��� ������ ���������� ����
		{
			bullet.Available(false);
		}

		sf::CircleShape GetBullet() // ������ ��� ��������� ����
		{
			return bullet.Get();
		}
	};
}

