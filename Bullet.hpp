#pragma once
#include <SFML/Graphics.hpp>

namespace mt
{

	class Bullet //Объявление класса
	{
		float m_r; // Радиус пули
		float m_x, m_y; // Координаты пули
		float m_vx, m_vy; // Векторы скорости пули
		sf::CircleShape m_shape; // Объект класса для отображения пули

		bool m_available = false; // Булевая переменная для обзначенности доступности пули

	public:
		Bullet() = default; // Конструктор по-умолчанию

		Bullet(float x, float y, float r, float vx, float vy) // Конструктор инициализирует коодинаты, радиус и векторы скорости пули
		{
			Setup(x, y, r, vx, vy);
		}

		void Setup(float x, float y, float r, float vx, float vy) // Метод настройки параметров пули
		{
			m_x = x; // Устанавливаем координату X
			m_y = y; // Устанавливаем координату Y
			m_r = r; // Устанавливаем радиус
			m_vx = vx; // Устанавливаем вектор скорости по X
			m_vy = vy; // Устанавливаем вектор скорости по Y
			m_shape.setOrigin(m_r, m_r); // Устанавливаем начало координат в центр пули
			m_shape.setRadius(m_r); // Устанавливаем радиус пули
			m_shape.setPosition(m_x, m_y); // Устанавливаем координаты пули
			int color_r = 255; // Красный цвет
			int color_g = 0;
			int color_b = 0;
			m_shape.setFillColor(sf::Color::Color(color_r, color_b, color_g, 255)); // Устанавливаем красный цвет пули
		}

		sf::CircleShape Get() // Геттер для получения пули
		{
			return m_shape;
		}

		void X(float value) { m_x = value; m_shape.setPosition(m_x, m_y); } //Сеттер для установки координаты X пули с обнолением её позиции
		void Y(float value) { m_y = value; m_shape.setPosition(m_x, m_y); } //Сеттер для установки координаты Y пули с обнолением её позиции
		float X() { return m_x; } // Геттер для получения X 
		float Y() { return m_y; } // Геттер для получения Y
		float R() { return m_r; } // Геттер для получения радиуса
		float Vx() { return m_vx; } // Геттер для получения скорости по X
		float Vy() { return m_vy; } // Геттер для получения скорости по Y

		void Move(float dt) // Метод для перемещения пули
		{
			m_x += m_vx * dt; // Обновление координаты X
			m_y += m_vy * dt; // Обновление координаты Y
			m_shape.setPosition(m_x, m_y); // Обновления позиции пули
		}

		bool Available() { return m_available; } // Проверка доступности пули
		void Available(bool value) { m_available = value; } // Установка доступности пули
	};
}