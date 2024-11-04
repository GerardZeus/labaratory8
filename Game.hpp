#pragma once
#include <string>
#include <cmath>
#include "Ship.hpp"
#include "SpaceObject.hpp"
#include "Asteroid.hpp"
#include "Komet.hpp"

namespace mt
{
	const float pi = acos(-1); // Константа пи
	class Game
	{
		int m_width; // Длина окна
		int m_height; // Ширина окна
		std::string m_capture; // Заголовок окна
		mt::SpaceObject** m_c; // Массив астероидов
		mt::SpaceObject** m_enemys; // Массив врагов
		bool* m_a_col; // Массив для проверки коллизий астероидов
		bool* m_e_col; // Массив для проверки коллизий врагов
		int m_n; // Количество астероидов
		int m_k; // Количество врагов
		sf::RenderWindow m_window; // Объкт для отрисовки окна
		sf::Texture m_textureBackground; // Текстура фона
		sf::Sprite m_spriteBackground; // Спрайт фона
		Ship m_ship; // Объект класса корбль

	public:
		Game(int width, int height, const std::string& capture) // Конструктор инициализирует длину, ширину и заголовок окна
		{
			m_width = width; // Устанавливаем дину окна
			m_height = height; // Устанавливае ширину окна
			m_capture = capture; // Устанавливаем заголовок окна 
		}

		bool Setup(int n) // Метод для настройки объектов
		{
			m_n = n; // Устанавливаем количество астероидов
			m_k = n; // Устанавливаем количество врагов
			m_window.create(sf::VideoMode(m_width, m_height), m_capture); // Создание окна
			if (!m_textureBackground.loadFromFile("assets\\background.jpg")) // Загрузка фона
			{
				std::cout << "Error while loading background.jpg" << std::endl;
				return false; // Ошибка, если фон не загружается
			}
			m_spriteBackground.setTexture(m_textureBackground); // Установка текстуры фона
			if (!m_ship.Setup(100, 100)) // Расстояние спавна корабля от левого верхнего угла
				return false;

			srand(time(0)); // Генератор сулчайных чисел 

			m_c = new mt::SpaceObject* [m_n]; // Выделение памяти для массива астероидов
			m_a_col = new bool[m_n]; // Выделение памяти для массива проверки коллизий астероидов
			m_enemys = new mt::SpaceObject* [m_k]; // Выделение памяти для массива врагов
			m_e_col = new bool[m_k]; // Выделение памяти для массива проверки коллизий врагов

			for (int i = 0; i < m_n; i++) // Цикл, настраивающий астероиды
			{
				bool validPosition = false; //Переменная для проверки позиции
				int x, y;
				int radius = 50; // Радиус астероида

				while (!validPosition) // Пока не найдена подходящая позиция
				{
					x = rand() % (m_width - 2 * radius) + radius; // Случайная координата X
					y = rand() % (m_height - 2 * radius) + radius; // Случайная координата Y

					float distToShip = sqrt(pow(m_ship.X() - x, 2) + pow(m_ship.Y() - y, 2)); // Расстояние от корабля до астероида
					if (distToShip < m_ship.Radius() + radius + 50) // Если астероид слишком близко к кораблю
						continue; // Пропускаем итерацию

					validPosition = true; // Позиция допустима
					for (int j = 0; j < i; j++) // Проверка на пересечение с уже существующими астероидами
					{
						float distToAsteroid = sqrt(pow(m_c[j]->X() - x, 2) + pow(m_c[j]->Y() - y, 2)); // Расстояние между астероидами
						if (distToAsteroid < m_c[j]->R() + radius + 10) // Если астероиды пересекаются
						{
							validPosition = false; // Позиция не подходит
							break; // Выходи из цикла
						}
					}
				}

				m_c[i] = new mt::Asteroid(); // Создание нового астероида
				m_c[i]->Setup(x, y, 5, 0, "assets\\asteroid.png", 0.3f); // Инициализация астероида
				m_a_col[i] = true;
			}
			// Тоже самое для врагов(enemy)
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

		~Game() // Деструктор
		{
			for (int i = 0; i < m_n; i++) // Освобождение памяти для астероидов
				delete m_c[i];
			delete[] m_c; // Освобождение массива астероидов
			delete[] m_a_col; // Освобождение массива коллизий астероидов

			for (int i = 0; i < m_k; i++) // Освобождение памяти для врагов
				delete m_enemys[i];
			delete[] m_enemys;  // Освобождение массива врагов
			delete[] m_e_col; // Освобождение массива коллизий врагов
		}
		
		void CheckBulletCollisionWithCircles() // Метод для проверки столконовения пули с астероидом
		{
			for (int i = 0; i < m_n; i++)
			{
				if (m_a_col[i] && m_ship.BulletAvailable()) // Если астероид активен и пуля доступна
				{
					float dx = m_ship.GetBullet().getPosition().x - m_c[i]->X();
					float dy = m_ship.GetBullet().getPosition().y - m_c[i]->Y();
					float distance = sqrt(dx * dx + dy * dy); // Вычисляем расстояние между пулей и астероидом

					if (distance < m_c[i]->R() + m_ship.GetBullet().getRadius()) // Если расстояние меньши суммы радиусов
					{
						m_a_col[i] = false; // Деактивируем астероид 
						m_ship.BulletReset(); // Перезарежаем пулю
					}
				}
			}
			// Тоже самое для врагов
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
		bool CheckCollisionWithCircles() // Метод для проверки коллизии корабля и объекта
		{
			for (int i = 0; i < m_n; i++)
			{
				if (m_a_col[i]) // Если астероид активен
				{
					float dist = sqrt(pow(m_ship.X() - m_c[i]->X(), 2) + pow(m_ship.Y() - m_c[i]->Y(), 2)); // Вычисляем расстояние между кораблём и астероидом
					if (dist < m_ship.Radius() + m_c[i]->R()) // Если расстояние меньши суммы радиусов
					{
						m_ship.RepelFrom(m_c[i]->X(), m_c[i]->Y()); // Отталкиваем корабль от астероида
						return true;  
					}
				}
			}
			return false;
			//Тоже самое для врагов
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
		void LifeCycle() // Метод для отображения игры
		{
			sf::Clock clock; // Создаём объект класса для clock для отслеживания времени
			float fps = 100; // Частота кадров
			//int fpsTextViewCnt = 0;

			while (m_window.isOpen()) // Пока открыто окно
			{
				sf::Event event;
				while (m_window.pollEvent(event)) // Обрабатывать события 
				{
					if (event.type == sf::Event::Closed) // Если событие - закрыть окно
						m_window.close(); // Закрываем онно
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // Кнопка "вверх" увеличивает скорость корабля
				{
					m_ship.setVelocity(0.003);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // Конопка "вниз" уменьшает скорость корабля
				{
					m_ship.setVelocity(-0.008);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // Кнопка "влеово" поворачивает корабль влево
				{
					m_ship.Rotate(-1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // Кнопка "вправо" поворачивает корабль вправо
				{
					m_ship.Rotate(1);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) // Кнорпка "пробел" стреляет 
				{
					if (!m_ship.BulletAvailable()) // Если плуя не активна
						m_ship.Attack(); // Тогда можео стрелять
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) // Кнопка "R" перезарядка и деактвация летящей пули
				{
					m_ship.BulletReset();
				}


				// Эта часть кода предназначена для установления фиксированной(почти) частоты кадров, чтобы на разных компьютерах скорость корабля была одинаквой
				float dt = clock.getElapsedTime().asSeconds();
				if (1 / fps > dt)
				{
					sf::Time t = sf::seconds(1 / fps - dt);
					sf::sleep(t);
				}
				dt = clock.getElapsedTime().asSeconds();
				clock.restart();

				m_ship.Move(dt); // Вызов функции перемещения корабля

				for (int i = 0; i < m_n; i++) //Перемещение каждого астероида
				{
					m_c[i]->Move(dt);
				}
				for (int i = 0; i < m_k; i++) //Перемещение каждого врага
					m_enemys[i]->Move(dt);

				m_ship.UpdateBulletPosition(dt, m_width, m_height); // Обновление позиции пули
				CheckBulletCollisionWithCircles(); // Проверка столкновения пули с астероидом

				m_window.clear(); // Очистка окна
				m_window.draw(m_spriteBackground); // Отображение фона
				m_window.draw(m_ship.Get()); // Отображение корабля

				for (int i = 0; i < m_n; i++) // Отображение астероидов
				{
					if (m_a_col[i]) 
						m_window.draw(m_c[i]->Get());
				}
				for (int i = 0; i < m_k; i++)// Отображение врагов
				{
					if (m_e_col[i])
						m_window.draw(m_enemys[i]->Get());
				}

				if (CheckCollisionWithCircles()) // Проверка коллизии между кораблём и астероидом
					m_ship.ReverSMove(); // Обратное движение

				if (m_ship.BulletAvailable()) // Отображение пули, если она доступна
					m_window.draw(m_ship.GetBullet());

				m_window.display();
			}
		}
	};
}

