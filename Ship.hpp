#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Bullet.hpp"

namespace mt
{

	class Ship // Объвление класса
	{
		const int m_bulletR = 10; // Радиус пули
		float m_v_bullet = 700; //Скорость пули


		float m_x, m_y; // Координаты коробля 
		float m_alpha; // Угол поворота коробля 
		float m_v; // Скорость коробля
		sf::Texture m_textureShip; // Объект класса для хранения текстуры корабля
		sf::Sprite m_spriteShip; // Объект класса для отображения текстуры корабля 

		Bullet bullet; // Объект класса Bullet


	public:
		Ship() = default; // Конструктор по-умолчанию
		Ship(float x, float y) // Конструктор инициализирует координаты корбаля
		{
			m_x = x;
			m_y = y;
		}

		bool Setup(float x, float y) // Фунция для установки начальных параметров корабля
		{
			m_x = x; // Устанавливаем координату X
			m_y = y; // Устанавливаем координату Y
			m_alpha = 0; // Устанавливаем угол поворота
			m_v = 0; // Устанавливаем скорость

			if (!m_textureShip.loadFromFile("assets\\Starship.png")) // Загружаем текстуру корабля
			{
				std::cout << "Error while loading Starship.png" << std::endl;
				return false; // Если текстуру не удаётся загрузить, то вывести ошибку
			}
			m_spriteShip.setTexture(m_textureShip); // Устанавливаем текстуру для корабля

			m_spriteShip.setScale(0.2f, 0.2f); // Устанавливаем размеры корабля
			m_spriteShip.setOrigin(m_textureShip.getSize().x / 2, m_textureShip.getSize().y / 2); // Устанавливаем центр корабля в центральной его точке(изничально в левом верхнем углу спрайта)
			m_spriteShip.setPosition(m_x, m_y); // Устанавливаем координаты корабля
			m_spriteShip.setRotation(m_alpha); // Устанавливаем угол поворота корабля
		}

		void Attack() // Метод для стрельбы 
		{
			if (!bullet.Available()) // Если пуля не активна
			{
				float alphaRad = acos(-1) * m_alpha / 180; // Преобразование в радианы
				float bullet_vx = (m_v + m_v_bullet) * cos(alphaRad); // Вычисление скорости пули по X
				float bullet_vy = (m_v + m_v_bullet) * sin(alphaRad); // Вычисление скорости пули по Y
				bullet.Setup(m_x, m_y, m_bulletR, bullet_vx, bullet_vy); // Устанавливаем коодинаты, радиус и скорость пули
				bullet.Available(true); // Установка пули в активное состояние
			}
		}
		void UpdateBulletPosition(float dt, float windowWidth, float windowHeight) // Метод для обновления позиции пули
		{
			if (bullet.Available()) // Если пуля активна
			{
				bullet.Move(dt); // Значит она движется

				if (bullet.X() < 0 || bullet.X() > windowWidth || bullet.Y() < 0 || bullet.Y() > windowHeight) // Если пуля вышла за границы окна
				{
					bullet.Available(false); // То она деактивируется
				}
			}
		}

		void setVelocity(float dv) // Функция для изменения скорости корабля
		{
			m_v += dv; // Увеличивает скорость на заданную дельта v
		}

		void Move(float dt) // Метод для перемещения корабля
		{
			bullet.Move(dt); // Перемещение пули

			float alphaRad = acos(-1) * m_alpha / 180; // Преобразуем градусы в радианы
			m_x += m_v * cos(alphaRad); // Обновляем координату X на основе скорости угла поворота
			m_y += m_v * sin(alphaRad); // Обновляем координату Y на основе скорости угла поворота
			m_spriteShip.setPosition(m_x, m_y); //Устанавливаем новые координаты корабля
		}

		void Rotate(float dalpha) // Функция для поворота корабля
		{
			m_alpha += dalpha; // Изменяет скорость на заданную дельта alpha
			m_spriteShip.setRotation(m_alpha); // Устанавливаем новый угол корабля
		}

		sf::Sprite Get() // Геттер для получения корабля
		{
			return m_spriteShip;
		}
		float X() const { return m_x; } // Геттер для получения X корабля
		float Y() const { return m_y; } // Геттер для получения Y корабля
		float Radius() const { return m_spriteShip.getGlobalBounds().width / 2.5; } // Геттер для получения радиуса корабля

		void ReverSMove() // Функция для обратного движения корабля
		{
			float alphaRad = acos(-1) * m_alpha / 180; // Преобразуем градусы в радианы
			m_x -= m_v * cos(alphaRad); // Обновляем координату X (в обратную сторону)
			m_y -= m_v * sin(alphaRad); // Обновляем координату Y (в обратную сторону)
			m_spriteShip.setPosition(m_x, m_y); // Устанавливаем новые координаты корабля
		}
		void RepelFrom(float cx, float cy, float repel = 0.3f) // Функция для отталкивания корабля от астероида
		{
			float dx = m_x - cx;
			float dy = m_y - cy;
			float dist = sqrt(dx * dx + dy * dy); // Вычисляем расстояние до астероида

			if (dist != 0) // Если расстояние не равно 0, то пермещаем корабль
			{
				m_x += (dx / dist) * repel; // Обновляем координату X
				m_y += (dy / dist) * repel; // Обновляем координату Y
				m_spriteShip.setPosition(m_x, m_y); // Устанавливаем новые координаты корабля
			}
		}
		bool BulletAvailable() // Метод для проверки доступности пули
		{
			return bullet.Available();
		}

		void BulletReset() // Метод для сброса актинвости пули
		{
			bullet.Available(false);
		}

		sf::CircleShape GetBullet() // Геттер для получения пули
		{
			return bullet.Get();
		}
	};
}

