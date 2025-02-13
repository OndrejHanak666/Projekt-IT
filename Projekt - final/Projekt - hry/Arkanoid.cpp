#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>;
#include <fstream>
using namespace sf;

void saveGame(float x, float y, bool blockState[], int n) {
    std::ofstream file("Arkanoid_data/savegame.txt");
    if (!file) return;

    file << x << " " << y << "\n"; // Uložení pozice míèe
    for (int i = 0; i < n; i++) {
        file << blockState[i] << " "; // Uložení stavu blokù
    }
    file.close();
}

bool loadGame(float& x, float& y, bool blockState[], int n) {
    std::ifstream file("Arkanoid_data/savegame.txt");
    if (!file) return false;

    file >> x >> y;
    for (int i = 0; i < n; i++) {
        file >> blockState[i];
    }
    file.close();
    return true;
}


void resetgame(float& x, float& y, float& dx, float& dy, Sprite& sPaddle, Sprite block[], int& n, Texture& t1, bool blockState[]) {
    x = 300, y = 300;
    dx = 6, dy = 5;
    sPaddle.setPosition(300, 440);

    if (!loadGame(x, y, blockState, n)) {
        for (int i = 0; i < n; i++) blockState[i] = true;
    }

    int index = 0;
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++) {
            if (blockState[index]) {
                block[index].setTexture(t1);
                block[index].setPosition(i * 43, j * 20);
            }
            else {
                block[index].setPosition(-100, 0);
            }
            index++;
        }
}


void playArkanoid()
{

    srand(time(0));

    RenderWindow app(VideoMode(520, 450), "Arkanoid!");
    app.setFramerateLimit(60);

    Texture t1, t2, t3, t4;
    t1.loadFromFile("image/block01.png");
    t2.loadFromFile("image/background03.jpg");
    t3.loadFromFile("image/ball.png");
    t4.loadFromFile("image/paddle.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    sPaddle.setPosition(300, 440);

    Sprite block[1000];
    bool blockState[1000] = { true };
    int n = 100;
    float dx = 6, dy = 5;
    float x = 300, y = 300;

    resetgame(x, y, dx, dy, sPaddle, block, n, t1, blockState);

    Font font;
    font.loadFromFile("font/arial.ttf");

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


    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                app.close();
                remove("Arkanoid_data/savegame.txt");
            }
                

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Escape)
                {
                    saveGame(x, y, blockState, n);
                    return; 
                }
            }

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
                    remove("Arkanoid_data/savegame.txt");
                    resetgame(x, y, dx, dy, sPaddle, block, n, t1, blockState);
                    


                }

                if (ballWaiting && e.key.code == Keyboard::Space)
                {
                    ballWaiting = false; // Míè zaène padat
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
            if (!ballWaiting) // Míè se zaène hýbat až po stisknutí mezerníku
            {
                x += dx;
                for (int i = 0; i < n; i++) {
                    if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds())) {
                        block[i].setPosition(-100, 0);
                        blockState[i] = false;
                        dx = -dx;
                    }
                }

                y += dy;
                for (int i = 0; i < n; i++) {
                    if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds())) {
                        block[i].setPosition(-100, 0);
                        blockState[i] = false;
                        dy = -dy;
                    }
                }

                if (x < 0 || x > 520) dx = -dx;
                if (y < 0) dy = -dy;

                if (y > 450) // Když míè spadne pod okraj
                {
                    isGameOver = true;
                }

                if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds())) dy = -(rand() % 5 + 2);
            }

            if (Keyboard::isKeyPressed(Keyboard::Right) && sPaddle.getPosition().x + sPaddle.getGlobalBounds().width < 520)
                sPaddle.move(8, 0);

            if (Keyboard::isKeyPressed(Keyboard::Left) && sPaddle.getPosition().x > 0)
                sPaddle.move(-8, 0);


                // Kontrola, zda jsou všechny bloky pryè = výhra
            bool allBlocksDestroyed = true;
            for (int i = 0; i < n; i++) 
            {
                if (blockState[i])
                {
                    allBlocksDestroyed = false;
                    break;
                }
            }
            if (allBlocksDestroyed)
            {
                std::cout << "All blocks destroyed! Setting isWin = true" << std::endl;
                isWin = true;
            }

        }



        sBall.setPosition(x, y);

        

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


   
}


