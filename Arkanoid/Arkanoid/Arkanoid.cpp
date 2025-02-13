// Arkanoid.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>;
using namespace sf;


void resetgame(float& x, float& y, float& dx, float& dy, Sprite& sPaddle, Sprite block[], int& n, Texture& t1)
{   
    // Reset míče
    x = 300, y = 300;
    dx = 6, dy = 5;

    // Reset pádla
    sPaddle.setPosition(300, 440);

    // Reset bloků
    n = 0;
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            block[n].setTexture(t1);
            block[n].setPosition(i * 43, j * 20);
            n++;
        }

}


int main()
{
    
        srand(time(0));

        RenderWindow app(VideoMode(520, 450), "Arkanoid!");
        app.setFramerateLimit(60);

        Texture t1, t2, t3, t4;
        t1.loadFromFile("images/block01.png");
        t2.loadFromFile("images/background03.jpg");
        t3.loadFromFile("images/ball.png");
        t4.loadFromFile("images/paddle.png");

        Sprite sBackground(t2), sBall(t3), sPaddle(t4);
        sPaddle.setPosition(300, 440);

        Sprite block[1000];

        Font font;
        font.loadFromFile("fonts/arial.ttf");

        Text gameOverText("Game Over!", font, 40);
        gameOverText.setFillColor(Color::Red);
        gameOverText.setPosition(160, 200);

        Text startText("Press Enter to Start", font, 30);
        startText.setFillColor(Color::White);
        startText.setPosition(150, 200);

        Text ballStartText("Press Space to Start Ball", font, 20);
        ballStartText.setFillColor(Color::Yellow);
        ballStartText.setPosition(160, 250);

        Text winText("You Win!", font, 40);
        winText.setFillColor(Color::Green);
        winText.setPosition(160, 200);

        bool isGameOver = false;
        bool inMenu = true;
        bool ballWaiting = true;
        bool isWin = false;

        int n = 0;
        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                block[n].setTexture(t1);
                block[n].setPosition(i * 43, j * 20);
                n++;
            }

        float dx = 6, dy = 5;
        float x = 300, y = 300;

        while (app.isOpen())
        {
            Event e;
            while (app.pollEvent(e))
            {
                if (e.type == Event::Closed)
                    app.close();

                if (e.type == Event::KeyPressed)
                {
                    if (inMenu && e.key.code == Keyboard::Enter)
                    {
                        inMenu = false;

                    }

                    if ((isGameOver || isWin) && e.key.code == Keyboard::R)
                    {
                        isGameOver = false;
                        ballWaiting = true;
                        isWin = false;
                        resetgame(x, y, dx, dy, sPaddle, block, n, t1);


                    }

                    if (ballWaiting && e.key.code == Keyboard::Space)
                    {
                        ballWaiting = false; // Míč začne padat
                    }
                }
            }

            if (inMenu)
            {
                app.clear(Color::Black); // Display black screen
                app.draw(startText);
                app.display();
                continue;
            }

            if (!isGameOver && !isWin)
            {
                if (!ballWaiting) // Míč se začne hýbat až po stisknutí mezerníku
                {
                    x += dx;
                    for (int i = 0; i < n; i++)
                        if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
                        {
                            block[i].setPosition(-100, 0);
                            dx = -dx;
                        }

                    y += dy;
                    for (int i = 0; i < n; i++)
                        if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
                        {
                            block[i].setPosition(-100, 0);
                            dy = -dy;
                        }

                    if (x < 0 || x > 520) dx = -dx;
                    if (y < 0) dy = -dy;

                    if (y > 450) // Když míč spadne pod okraj
                    {
                        isGameOver = true;
                    }

                    if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds())) dy = -(rand() % 5 + 2);
                }

                if (Keyboard::isKeyPressed(Keyboard::Right) && sPaddle.getPosition().x + sPaddle.getGlobalBounds().width < 520)
                    sPaddle.move(6, 0);

                if (Keyboard::isKeyPressed(Keyboard::Left) && sPaddle.getPosition().x > 0)
                    sPaddle.move(-6, 0);
            }



            sBall.setPosition(x, y);

            // Kontrola, zda jsou všechny bloky pryč = výhra
            bool allBlocksDestroyed = true;
            for (int i = 0; i < n; i++)
            {
                if (block[i].getPosition().x >= 0) // Blok je stále na obrazovce
                {
                    allBlocksDestroyed = false;
                    break;
                }
            }
            if (allBlocksDestroyed)
            {
                isWin = true;
            }

            app.clear();
            app.draw(sBackground);
            app.draw(sBall);
            app.draw(sPaddle);

            for (int i = 0; i < n; i++)
                app.draw(block[i]);


            if (isGameOver)
            {
                app.clear(Color::Black);
                app.draw(gameOverText);
                Text restartText("Press R to Restart", font, 20);
                restartText.setFillColor(Color::White);
                restartText.setPosition(180, 250);
                app.draw(restartText);
            }

            if (isWin)
            {
                app.clear(Color::Black);
                app.draw(winText);
                Text restartText("Press R to Restart", font, 20);
                restartText.setFillColor(Color::White);
                restartText.setPosition(180, 250);
                app.draw(restartText);
            }
                

            if (ballWaiting)
            {
                app.draw(ballStartText); // Zobrazí text "Press Space to Start Ball"
            }

            app.display();
            
            
        }

        
     return 0;
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
