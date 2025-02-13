// Projekt - hry.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Minesweeper.h"
#include "Bejeweled.h"
#include "Tetris.h"
#include "Arkanoid.h"

int main() {
    while (true)
    {
    sf::RenderWindow menu(sf::VideoMode(800, 600), "Hlavni Nabidka");

    sf::Font font;
    if (!font.loadFromFile("font/arial.ttf")) {
        std::cerr << "Nelze nacist font\n";
        return -1;
    }

    sf::Text title("Hlavni Nabidka", font, 50);
    title.setFillColor(sf::Color::Red);
    title.setPosition(200, 50);

    sf::Text option1("1. Minesweeper", font, 30);
    option1.setFillColor(sf::Color::Red);
    option1.setPosition(250, 200);

    sf::Text option2("2. Bejeweled", font, 30);
    option2.setFillColor(sf::Color::Red);
    option2.setPosition(250, 275);

    sf::Text option3("3. Tetris", font, 30);
    option3.setFillColor(sf::Color::Red);
    option3.setPosition(250, 350);

    sf::Text option4("4. Arkanoid", font, 30);
    option4.setFillColor(sf::Color::Red);
    option4.setPosition(250, 425);

    sf::Text exitText("ESC: Ukoncit hru", font, 20);
    exitText.setFillColor(sf::Color::Red);
    exitText.setPosition(250, 550);

    while (menu.isOpen()) {
        sf::Event event;
        while (menu.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                menu.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    menu.close();
                    playMinesweeper(); // Spustí Minesweeper
                }
                else if (event.key.code == sf::Keyboard::Num2) {
                    menu.close();
                    playBejeweled(); // Spustí Bejeweled
                }
                else if (event.key.code == sf::Keyboard::Num3) {
                    menu.close();
                    playTetris(); // Spustí Tetris
                }
                else if (event.key.code == sf::Keyboard::Num4) {
                    menu.close();
                    playArkanoid(); // Spustí Arkanoid
                }
            }
        }

        menu.clear(sf::Color::Black);
        menu.draw(title);
        menu.draw(option1);
        menu.draw(option2);
        menu.draw(option3);
        menu.draw(option4);
        menu.draw(exitText);
        menu.display();
    }

    }
    
}


// Spuštění programu: Ctrl+F5 nebo nabídka Ladit > Spustit bez ladění
// Ladění programu: F5 nebo nabídka Ladit > Spustit ladění

// Tipy pro zahájení práce:
//   1. K přidání nebo správě souborů použijte okno Průzkumník řešení.
//   2. Pro připojení ke správě zdrojového kódu použijte okno Team Explorer.
//   3. K zobrazení výstupu sestavení a dalších zpráv použijte okno Výstup.
//   4. K zobrazení chyb použijte okno Seznam chyb.
//   5. Pokud chcete vytvořit nové soubory kódu, přejděte na Projekt > Přidat novou položku. Pokud chcete přidat do projektu existující soubory kódu, přejděte na Projekt > Přidat existující položku.
//   6. Pokud budete chtít v budoucnu znovu otevřít tento projekt, přejděte na Soubor > Otevřít > Projekt a vyberte příslušný soubor .sln.
