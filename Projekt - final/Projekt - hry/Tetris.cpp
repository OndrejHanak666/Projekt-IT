#include "Tetris.h"
#include "ScoreManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <string>
#include <fstream>

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point
{
    int x, y;
} a[4], b[4];

int figures[7][4] =
{
    1,3,5,7,    // I
    2,4,5,7,    // Z
    3,5,4,6,    // S
    3,5,4,7,    // T
    2,3,5,7,    // L
    3,5,7,6,    // J
    2,3,4,5     // O
};

void saveGameState(const std::string& filename, int score, int colorNum, int field[M][N], Point a[4]) {
    std::ofstream file(filename);
    if (!file) return;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            file << field[i][j] << " ";
        }
        file << "\n";
    }

    for (int i = 0; i < 4; i++) {
        file << a[i].x << " " << a[i].y << "\n";
    }

    file << score << " " << colorNum << "\n";
    file.close();
}


bool loadGameState(const std::string& filename, int& score, int& colorNum, int field[M][N], Point a[4]) {
    std::ifstream file(filename);
    if (!file) return false;

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            file >> field[i][j];
        }
    }

    for (int i = 0; i < 4; i++) {
        file >> a[i].x >> a[i].y;
    }

    file >> score >> colorNum;
    file.close();
    return true;
}

void resetGame(int& score, int& colorNum, int field[M][N], Point a[4])
{
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            field[i][j] = 0;

    score = 0;
    colorNum = 1;

    int n = rand() % 7;
    for (int i = 0; i < 4; i++) 
    {
        a[i].x = figures[n][i] % 2;
        a[i].y = figures[n][i] / 2;
    }
}


bool check()
{
    for (int i = 0; i < 4; i++)
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return 0;
        else if (field[a[i].y][a[i].x])
            return 0;

    return 1;
};

void playTetris()
{
    srand(time(0));

    RenderWindow window(VideoMode(320, 480), "Tetris!");
    window.setFramerateLimit(60);

    Texture t1, t2, t3;
    t1.loadFromFile("image/tiles.png");
    t2.loadFromFile("image/background.png");
    t3.loadFromFile("image/frame.png");


    Sprite s(t1), background(t2), frame(t3);

    int dx = 0;
    bool rotate = 0;
    int colorNum = 1;
    float timer = 0, delay = 0.3;

    int score = 0;
    bool gameOver = false;

    if (loadGameState("Tetris_data/savegame.txt", score, colorNum, field, a))
    {
        std::cout << "Game loaded successfully!" << std::endl;
    }
    else 
    {
        std::cout << "No saved game found, starting new game." << std::endl;
        resetGame(score, colorNum, field, a);
    }


    int highscore = ScoreManager::loadHighscore("Tetris_data/highscore_tetris.txt");

    // Naètení fontu pro zobrazení textu
    Font font;
    font.loadFromFile("font/arial.ttf");
        

    Text textScore, textHighscore;
    textScore.setFont(font);
    textHighscore.setFont(font);
    textScore.setCharacterSize(20);
    textHighscore.setCharacterSize(20);
    textScore.setFillColor(Color::Black);
    textHighscore.setFillColor(Color::Black);
    textScore.setPosition(10, 400);
    textHighscore.setPosition(10, 420);

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                remove("Tetris_data/savegame.txt");
                resetGame(score, colorNum, field, a);
                window.close();                
            }
                

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Escape)
                {
                    saveGameState("Tetris_data/savegame.txt", score, colorNum, field, a);
                    return; // Opustí funkci a vrátí se do hlavní nabídky
                }
            }

            if (gameOver)
            {

                if (e.type == Event::KeyPressed && e.key.code == Keyboard::R) {
                    // Restart hry
                    resetGame(score, colorNum, field, a);
                    if (score > highscore)
                    {
                        highscore = score;
                        ScoreManager::saveHighscore("Tetris_data/highscore_tetris.txt", highscore);
                    }
                    score = 0;
                    gameOver = false;
                }
                continue;
            }

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Up)
                    rotate = true;
                else if (e.key.code == Keyboard::Left)
                    dx = -1;
                else if (e.key.code == Keyboard::Right)
                    dx = 1;
        }

        if (e.key.code == Keyboard::Down)
            delay = 0.05;

        for (int i = 0; i < 4; i++) // move
        {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check())
            for (int i = 0; i < 4; i++)
                a[i] = b[i];

        if (rotate) // rotate
        {
            Point p = a[1]; // centre of rotation
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check())
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
        }

        // tick
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
            if (!check())
            {
                for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }

                // Kontrola na Game Over (bloky dosáhly vrcholu)
                for (int i = 0; i < 4; i++) {
                    if (a[i].y == 0 && field[a[i].y][a[i].x] != 0) {
                        gameOver = true;
                        if (score > highscore)
                        {
                            highscore = score;
                            ScoreManager::saveHighscore("highscore_tetris.txt", highscore);         // aktualizace highscore
                        } 
                    }
                }
            }
            timer = 0;
        }

        // check lines
        int K = M - 1;
        for (int i = M - 1; i > 0; i--)
        {
            int count = 0;
            for (int j = 0; j < N; j++)
            {
                if (field[i][j]) count++;
                field[K][j] = field[i][j];
            }
            if (count == N) // øádek je plný
            {
                score += 100;  // pøidáme skóre za odstranìný øádek
            }
            if (count < N) K--;
        }

        dx = 0; rotate = 0; delay = 0.3;

        window.clear(Color::White); // draw
        window.draw(background);

        // Zobrazení polí
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (field[i][j] == 0) continue;
                s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                s.setPosition(j * 18, i * 18);
                s.move(28, 31); // offset
                window.draw(s);
            }

        // Zobrazení aktuální figury
        for (int i = 0; i < 4; i++)
        {
            s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            s.setPosition(a[i].x * 18, a[i].y * 18);
            s.move(28, 31); // offset
            window.draw(s);
        }

        window.draw(frame);

        // Zobrazení skóre a highscore
        textScore.setString("Score: " + std::to_string(score));
        textHighscore.setString("Highscore: " + std::to_string(highscore));
        window.draw(textScore);
        window.draw(textHighscore);

        // Zobrazení Game Over obrazovky
        if (gameOver)
        {
            Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER\nPress R to Restart");
            gameOverText.setCharacterSize(30);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setPosition(40, 200);
            window.clear(Color::Black);
            window.draw(gameOverText);

        }

        window.display();
    }

    
}

