#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace mt
{

	class SpaceObject //Базовый класс для космических объектов
	{
	protected:
		float m_x, m_y; //Переменные для координат 
		float m_vx, m_vy; // Векторы скорости по X и Y 
		float m_r; // Переменная для радиуса
		sf::Texture m_textureShip; // Объект класса для хранения текстуры объектов
		sf::Sprite m_spriteShip; // Объект класса для отображения текстуры объектов
		bool m_active;

	public:
		SpaceObject() = default; // Конструктор по-умолчанию
		virtual ~SpaceObject() {} // Виртуальный деструктор

		bool Setup(float x, float y, float vx, float vy, 
			const std::string& texturePath, float scaleFactor) // Метод настройки объекта с координатами, скоростью, текстурой и масштабом
		{
			m_x = x; // Устанавливаем координаты X
			m_y = y; // Устанавливаем координаты Y
			m_vx = vx; // Устанавливаем скорости по оси X
			m_vy = vy; // Устанавливаем скорости по оси Y

			if (!m_textureShip.loadFromFile(texturePath)) // Загрузка текстуры 
			{
				std::cout << "Error while loading " << texturePath << std::endl;
				return false; // Если текстуру не удаётся загрузить, то вывести ошибку
			}
			m_spriteShip.setTexture(m_textureShip); // Устанавливаем текстуру для объекта

			m_spriteShip.setScale(scaleFactor, scaleFactor); //Устанавливаем размеры объекта
			m_spriteShip.setOrigin(m_textureShip.getSize().x / 2, m_textureShip.getSize().y / 2); // Устанавливаем центр объекта в центральной его точке(изничально в левом врехнем углу спрайта)
			m_spriteShip.setPosition(m_x, m_y); // Устанавливаем координаты объекта

			// Вычисление радиуса объекта на основе размеров текстуры
			if (m_textureShip.getSize().x > m_textureShip.getSize().y)
				m_r = scaleFactor * m_textureShip.getSize().x / 2;
			else
				m_r = scaleFactor * m_textureShip.getSize().y / 2;
		}

		sf::Sprite Get()
		{
			return m_spriteShip; // Геттер для получения объекта
		}
		float X() { return m_x; } // Геттер для получения X объекта
		float Y() { return m_y; } // Геттер для получения Y объекта
		float R() { return m_r; } // Геттер для получения радиуса объекта
		void SetActive(bool active) { m_active = active; }  // Устанвливаем статус активности объекта
		bool IsActive() const { return m_active; } // Проверяем статус активности объекта
		void Move(float dt) // Метод для перемещения объекта
		{
			m_x += m_vx * dt; // Обновляем координату X 
			m_y += m_vy * dt; // Обновляем координату Y
			m_spriteShip.setPosition(m_x, m_y); //Устанавливаем новые координаты объекта
		}

		virtual void SpecialAbility() = 0; //Виртуальный метод для специальной способности объекта
	};
}