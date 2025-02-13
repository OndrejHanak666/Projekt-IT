#include "Bejeweled.h"
#include "ScoreManager.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <time.h>
#include <sstream>
using namespace sf;

int ts = 54; //tile size
Vector2i offset(48, 24);


struct piece
{
    int x, y, col, row, kind, match, alpha;
    piece() { match = 0; alpha = 255; }
} grid[10][10];


void saveGameState(const std::string& filename, int score, int gameTime)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        file << score << " " << gameTime << "\n";
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                file << grid[i][j].kind << " "
                << grid[i][j].col << " "
                << grid[i][j].row << " "
                << grid[i][j].x << " "
                << grid[i][j].y << " "
                << grid[i][j].match << " ";
        file.close();
    }
}

bool loadGameState(const std::string& filename, int& score, int& gameTime)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        file >> score >> gameTime;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                file >> grid[i][j].kind
                    >> grid[i][j].col
                    >> grid[i][j].row
                    >> grid[i][j].x
                    >> grid[i][j].y
                    >> grid[i][j].match;

                // Zajist�, �e v�echny ozdoby jsou spr�vn� um�st�ny a nespojuj� se
                grid[i][j].match = 0;
            }
        file.close();
        return true;
    }
    return false;
}

void swap(piece p1, piece p2)
{
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);

    grid[p1.row][p1.col] = p1;
    grid[p2.row][p2.col] = p2;
}

void resetGame()
{
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
        {
            grid[i][j].kind = rand() % 3;
            grid[i][j].col = j;
            grid[i][j].row = i;
            grid[i][j].x = j * ts;
            grid[i][j].y = i * ts;
            grid[i][j].match = 0;
        }
}


void playBejeweled()
{
    srand(time(0));

    RenderWindow app(VideoMode(740, 480), "Bejeweled!");
    app.setFramerateLimit(60);

    Texture t1, t2;
    t1.loadFromFile("image/background2.png");
    t2.loadFromFile("image/gems.png");

    Font font;
    font.loadFromFile("font/arial.ttf");

    int highscore = ScoreManager::loadHighscore("Bejeweled_data/highscore_bejeweled.txt");

    Text scoreText("Score: 0", font, 20);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(500, 370);

    Text timerText("Time: 120", font, 20);
    timerText.setFillColor(Color::Black);
    timerText.setPosition(500, 400);

    Text highscoreText("Highcore: 0", font, 20);
    highscoreText.setFillColor(Color::Black);
    highscoreText.setPosition(500, 430);

    Text gameOverText("Game Over!", font, 40);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(200, 200);

    Text startText("Press Enter to Start", font, 30);
    startText.setFillColor(Color::White);
    startText.setPosition(200, 200);

    int score = 0;
    int gameTime = 120; // seconds
    bool isGameOver = false;
    bool inMenu = true;

    if (loadGameState("Bejeweled_data/save_bejeweled.txt", score, gameTime))
    {
        inMenu = false; // Pokud je save dostupn�, pokra�ujeme v ulo�en� h�e
    }


    Clock gameClock;
    Clock timerClock;

    Sprite background(t1), gems(t2);

    

    int x0, y0, x, y;
    int click = 0;
    Vector2i pos;
    bool isSwap = false, isMoving = false;

    piece backup1, backup2;
    
    if (loadGameState("Bejeweled_data/save_bejeweled.txt", score, gameTime))
    {
        inMenu = false;
    }
    else
    {
        resetGame();
    }

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                remove("Bejeweled_data/save_bejeweled.txt");
                app.close();             
            }
                

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Escape)         // U�ivatelsk� n�vrat do hlavn� nab�dky
                {
                    saveGameState("Bejeweled_data/save_bejeweled.txt", score, gameTime);
                    return;
                }
            }

            if (e.type == Event::KeyPressed)
            {
                if (inMenu && e.key.code == Keyboard::Enter)
                {
                    inMenu = false;
                    gameClock.restart();
                    timerClock.restart();
                    score = 0;
                    gameTime = 120;
                    resetGame();
                }

                if (isGameOver && e.key.code == Keyboard::R)
                {
                    isGameOver = false;
                    gameClock.restart();
                    timerClock.restart();
                    if (score > highscore)
                    {
                        highscore = score;
                        ScoreManager::saveHighscore("Bejeweled_data/highscore_bejeweled.txt", highscore);
                    }
                    score = 0;
                    gameTime = 120;
                    resetGame();
                }
            }


            if (!isGameOver && !inMenu && e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left)
                {
                    if (!isSwap && !isMoving) click++;
                    pos = Mouse::getPosition(app) - offset;
                }
        }

        if (inMenu)
        {
            app.clear(Color::Black); // Display black screen
            app.draw(startText);
            app.display();
            continue;
        }

        // mouse click
        if (click == 1)
        {
            x0 = pos.x / ts + 1;
            y0 = pos.y / ts + 1;
        }
        if (click == 2)
        {
            x = pos.x / ts + 1;
            y = pos.y / ts + 1;
            if (abs(x - x0) + abs(y - y0) == 1)
            {
                swap(grid[y0][x0], grid[y][x]);
                isSwap = 1;
                click = 0;
            }
            else click = 1;
        }

        //Match finding
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                if (grid[i][j].kind == grid[i + 1][j].kind)
                    if (grid[i][j].kind == grid[i - 1][j].kind)
                        for (int n = -1; n <= 1; n++) grid[i + n][j].match++;

                if (grid[i][j].kind == grid[i][j + 1].kind)
                    if (grid[i][j].kind == grid[i][j - 1].kind)
                        for (int n = -1; n <= 1; n++) grid[i][j + n].match++;
            }

        //Moving animation
        isMoving = false;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                piece& p = grid[i][j];
                int dx, dy;
                for (int n = 0; n < 4; n++)   // 4 - speed
                {
                    dx = p.x - p.col * ts;
                    dy = p.y - p.row * ts;
                    if (dx) p.x -= dx / abs(dx);
                    if (dy) p.y -= dy / abs(dy);
                }
                if (dx || dy) isMoving = 1;
            }

        //Deleting amimation
        if (!isMoving)
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match)
                        if (grid[i][j].alpha > 10)
                        {
                            grid[i][j].alpha -= 10; isMoving = true;
                        }



        //Get score
        int score1 = 0;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                score1 += grid[i][j].match;

        //Second swap if no match
        if (isSwap && !isMoving)
        {
            if (!score1) swap(grid[y0][x0], grid[y][x]); isSwap = 0;
        }



        //Update grid
        if (!isMoving)
        {
            for (int i = 8; i > 0; i--)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match)
                        for (int n = i; n > 0; n--)
                            if (!grid[n][j].match)
                            {
                                swap(grid[n][j], grid[i][j]);
                                break;
                            };

            for (int j = 1; j <= 8; j++)
                for (int i = 8, n = 0; i > 0; i--)
                    if (grid[i][j].match)
                    {
                        grid[i][j].kind = rand() % 7;
                        grid[i][j].y = -ts * n++;
                        grid[i][j].match = 0;
                        grid[i][j].alpha = 255;
                        score += 10;
                    }

        }

        if (timerClock.getElapsedTime().asSeconds() >= 1)
        {
            gameTime--;
            timerClock.restart();
            if (gameTime <= 0)
            {
                isGameOver = true;
                if (score > highscore)
                {
                    highscore = score;
                    ScoreManager::saveHighscore("Bejeweled_data/highscore_bejeweled.txt", highscore);
                }
            }
        }

        std::stringstream scoreStream;
        scoreStream << "Score: " << score;
        scoreText.setString(scoreStream.str());

        std::stringstream timerStream;
        timerStream << "Time: " << gameTime;
        timerText.setString(timerStream.str());

        std::stringstream highscoreStream;
        highscoreStream << "Highscore: " << highscore;
        highscoreText.setString(highscoreStream.str());




        //////draw///////
        app.draw(background);

        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
            {
                piece p = grid[i][j];
                gems.setTextureRect(IntRect(p.kind * 49, 0, 49, 49));
                gems.setColor(Color(255, 255, 255, p.alpha));
                gems.setPosition(p.x, p.y);
                gems.move(offset.x - ts, offset.y - ts);
                app.draw(gems);
            }
        app.draw(scoreText);
        app.draw(timerText);
        app.draw(highscoreText);

        if (isGameOver)
        {
            app.clear(Color::Black);
            app.draw(gameOverText);
            Text restartText("Press R to Restart", font, 20);
            restartText.setFillColor(Color::White);
            restartText.setPosition(200, 250);
            app.draw(restartText);
        }

        app.display();
    }
 
}

