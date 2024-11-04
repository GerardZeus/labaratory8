#pragma once
#include <string>
#include <cmath>
#include "Ship.hpp"
#include "SpaceObject.hpp"
#include "Asteroid.hpp"
#include "Komet.hpp"

namespace mt
{
	const float pi = acos(-1); // ��������� ��
	class Game
	{
		int m_width; // ����� ����
		int m_height; // ������ ����
		std::string m_capture; // ��������� ����
		mt::SpaceObject** m_c; // ������ ����������
		mt::SpaceObject** m_enemys; // ������ ������
		bool* m_a_col; // ������ ��� �������� �������� ����������
		bool* m_e_col; // ������ ��� �������� �������� ������
		int m_n; // ���������� ����������
		int m_k; // ���������� ������
		sf::RenderWindow m_window; // ����� ��� ��������� ����
		sf::Texture m_textureBackground; // �������� ����
		sf::Sprite m_spriteBackground; // ������ ����
		Ship m_ship; // ������ ������ ������

	public:
		Game(int width, int height, const std::string& capture) // ����������� �������������� �����, ������ � ��������� ����
		{
			m_width = width; // ������������� ���� ����
			m_height = height; // ������������ ������ ����
			m_capture = capture; // ������������� ��������� ���� 
		}

		bool Setup(int n) // ����� ��� ��������� ��������
		{
			m_n = n; // ������������� ���������� ����������
			m_k = n; // ������������� ���������� ������
			m_window.create(sf::VideoMode(m_width, m_height), m_capture); // �������� ����
			if (!m_textureBackground.loadFromFile("assets\\background.jpg")) // �������� ����
			{
				std::cout << "Error while loading background.jpg" << std::endl;
				return false; // ������, ���� ��� �� �����������
			}
			m_spriteBackground.setTexture(m_textureBackground); // ��������� �������� ����
			if (!m_ship.Setup(100, 100)) // ���������� ������ ������� �� ������ �������� ����
				return false;

			srand(time(0)); // ��������� ��������� ����� 

			m_c = new mt::SpaceObject* [m_n]; // ��������� ������ ��� ������� ����������
			m_a_col = new bool[m_n]; // ��������� ������ ��� ������� �������� �������� ����������
			m_enemys = new mt::SpaceObject* [m_k]; // ��������� ������ ��� ������� ������
			m_e_col = new bool[m_k]; // ��������� ������ ��� ������� �������� �������� ������

			for (int i = 0; i < m_n; i++) // ����, ������������� ���������
			{
				bool validPosition = false; //���������� ��� �������� �������
				int x, y;
				int radius = 50; // ������ ���������

				while (!validPosition) // ���� �� ������� ���������� �������
				{
					x = rand() % (m_width - 2 * radius) + radius; // ��������� ���������� X
					y = rand() % (m_height - 2 * radius) + radius; // ��������� ���������� Y

					float distToShip = sqrt(pow(m_ship.X() - x, 2) + pow(m_ship.Y() - y, 2)); // ���������� �� ������� �� ���������
					if (distToShip < m_ship.Radius() + radius + 50) // ���� �������� ������� ������ � �������
						continue; // ���������� ��������

					validPosition = true; // ������� ���������
					for (int j = 0; j < i; j++) // �������� �� ����������� � ��� ������������� �����������
					{
						float distToAsteroid = sqrt(pow(m_c[j]->X() - x, 2) + pow(m_c[j]->Y() - y, 2)); // ���������� ����� �����������
						if (distToAsteroid < m_c[j]->R() + radius + 10) // ���� ��������� ������������
						{
							validPosition = false; // ������� �� ��������
							break; // ������ �� �����
						}
					}
				}

				m_c[i] = new mt::Asteroid(); // �������� ������ ���������
				m_c[i]->Setup(x, y, 5, 0, "assets\\asteroid.png", 0.3f); // ������������� ���������
				m_a_col[i] = true;
			}
			// ���� ����� ��� ������(enemy)
			for (int i = 0; i < m_k; i++)
			{
				bool validPosition = false;
				int x, y;
				int radius = 50;

				while (!validPosition)
				{
					x = rand() % (m_width - 2 * radius) + radius;
					y = rand() % (m_height - 2 * radius) + radius;

					float distToShip = sqrt(pow(m_ship.X() - x, 2) + pow(m_ship.Y() - y, 2));
					if (distToShip < m_ship.Radius() + radius + 50)
						continue;

					validPosition = true;
					for (int j = 0; j < i; j++)
					{
						float distToKomet = sqrt(pow(m_enemys[j]->X() - x, 2) + pow(m_enemys[j]->Y() - y, 2));
						if (distToKomet < m_enemys[j]->R() + radius + 10)
						{
							validPosition = false;
							break;
						}
					}
				}

				m_enemys[i] = new mt::Enemy();
				m_enemys[i]->Setup(x, y, -5, 0, "assets\\enemy.png", 0.3f);
				m_e_col[i] = true;
			}
			return true;
		}

		~Game() // ����������
		{
			for (int i = 0; i < m_n; i++) // ������������ ������ ��� ����������
				delete m_c[i];
			delete[] m_c; // ������������ ������� ����������
			delete[] m_a_col; // ������������ ������� �������� ����������

			for (int i = 0; i < m_k; i++) // ������������ ������ ��� ������
				delete m_enemys[i];
			delete[] m_enemys;  // ������������ ������� ������
			delete[] m_e_col; // ������������ ������� �������� ������
		}
		
		void CheckBulletCollisionWithCircles() // ����� ��� �������� ������������� ���� � ����������
		{
			for (int i = 0; i < m_n; i++)
			{
				if (m_a_col[i] && m_ship.BulletAvailable()) // ���� �������� ������� � ���� ��������
				{
					float dx = m_ship.GetBullet().getPosition().x - m_c[i]->X();
					float dy = m_ship.GetBullet().getPosition().y - m_c[i]->Y();
					float distance = sqrt(dx * dx + dy * dy); // ��������� ���������� ����� ����� � ����������

					if (distance < m_c[i]->R() + m_ship.GetBullet().getRadius()) // ���� ���������� ������ ����� ��������
					{
						m_a_col[i] = false; // ������������ �������� 
						m_ship.BulletReset(); // ������������ ����
					}
				}
			}
			// ���� ����� ��� ������
			for (int i = 0; i < m_k; i++)
			{
				if (m_e_col[i] && m_ship.BulletAvailable())
				{
					float dx = m_ship.GetBullet().getPosition().x - m_enemys[i]->X();
					float dy = m_ship.GetBullet().getPosition().y - m_enemys[i]->Y();
					float distance = sqrt(dx * dx + dy * dy);

					if (distance < m_enemys[i]->R() + m_ship.GetBullet().getRadius())
					{
						m_e_col[i] = false;
						m_ship.BulletReset();
					}
				}
			}
		}
		bool CheckCollisionWithCircles() // ����� ��� �������� �������� ������� � �������
		{
			for (int i = 0; i < m_n; i++)
			{
				if (m_a_col[i]) // ���� �������� �������
				{
					float dist = sqrt(pow(m_ship.X() - m_c[i]->X(), 2) + pow(m_ship.Y() - m_c[i]->Y(), 2)); // ��������� ���������� ����� ������� � ����������
					if (dist < m_ship.Radius() + m_c[i]->R()) // ���� ���������� ������ ����� ��������
					{
						m_ship.RepelFrom(m_c[i]->X(), m_c[i]->Y()); // ����������� ������� �� ���������
						return true;  
					}
				}
			}
			return false;
			//���� ����� ��� ������
			for (int i = 0; i < m_k; i++)
			{
				if (m_e_col[i])
				{
					float dist = sqrt(pow(m_ship.X() - m_enemys[i]->X(), 2) + pow(m_ship.Y() - m_enemys[i]->Y(), 2));
					if (dist < m_ship.Radius() + m_enemys[i]->R())
					{
						m_ship.RepelFrom(m_enemys[i]->X(), m_enemys[i]->Y());
						return true;
					}
				}
			}
			return false;
		}
		void LifeCycle() // ����� ��� ����������� ����
		{
			sf::Clock clock; // ������ ������ ������ ��� clock ��� ������������ �������
			float fps = 100; // ������� ������
			//int fpsTextViewCnt = 0;

			while (m_window.isOpen()) // ���� ������� ����
			{
				sf::Event event;
				while (m_window.pollEvent(event)) // ������������ ������� 
				{
					if (event.type == sf::Event::Closed) // ���� ������� - ������� ����
						m_window.close(); // ��������� ����
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // ������ "�����" ����������� �������� �������
				{
					m_ship.setVelocity(0.003);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // ������� "����" ��������� �������� �������
				{
					m_ship.setVelocity(-0.008);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // ������ "������" ������������ ������� �����
				{
					m_ship.Rotate(-1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // ������ "������" ������������ ������� ������
				{
					m_ship.Rotate(1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) // ������� "������" �������� 
				{
					if (!m_ship.BulletAvailable()) // ���� ���� �� �������
						m_ship.Attack(); // ����� ����� ��������
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) // ������ "R" ����������� � ���������� ������� ����
				{
					m_ship.BulletReset();
				}


				// ��� ����� ���� ������������� ��� ������������ �������������(�����) ������� ������, ����� �� ������ ����������� �������� ������� ���� ���������
				float dt = clock.getElapsedTime().asSeconds();
				if (1 / fps > dt)
				{
					sf::Time t = sf::seconds(1 / fps - dt);
					sf::sleep(t);
				}
				dt = clock.getElapsedTime().asSeconds();
				clock.restart();

				m_ship.Move(dt); // ����� ������� ����������� �������

				for (int i = 0; i < m_n; i++) //����������� ������� ���������
				{
					m_c[i]->Move(dt);
				}
				for (int i = 0; i < m_k; i++) //����������� ������� �����
					m_enemys[i]->Move(dt);

				m_ship.UpdateBulletPosition(dt, m_width, m_height); // ���������� ������� ����
				CheckBulletCollisionWithCircles(); // �������� ������������ ���� � ����������

				m_window.clear(); // ������� ����
				m_window.draw(m_spriteBackground); // ����������� ����
				m_window.draw(m_ship.Get()); // ����������� �������

				for (int i = 0; i < m_n; i++) // ����������� ����������
				{
					if (m_a_col[i]) 
						m_window.draw(m_c[i]->Get());
				}
				for (int i = 0; i < m_k; i++)// ����������� ������
				{
					if (m_e_col[i])
						m_window.draw(m_enemys[i]->Get());
				}

				if (CheckCollisionWithCircles()) // �������� �������� ����� ������� � ����������
					m_ship.ReverSMove(); // �������� ��������

				if (m_ship.BulletAvailable()) // ����������� ����, ���� ��� ��������
					m_window.draw(m_ship.GetBullet());

				m_window.display();
			}
		}
	};
}

