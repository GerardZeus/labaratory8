#include "SpaceObject.hpp"
#include "Game.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    mt::Game game(1920, 1080, "Game!!!"); //Создаём объект класса с заданной длиной, шириной и заголовком.
    if (!game.Setup(3)) //Создаём игру с 3 астероидами и 3 врагами
        return -1;

    game.LifeCycle(); // Запускаем функцию отображения всего происходящего


    return 0;
}