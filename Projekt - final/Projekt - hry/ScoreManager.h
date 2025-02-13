#pragma once
#include <fstream>
#include <string>
#include <iostream>

class ScoreManager {
public:
    static int loadHighscore(const std::string& filename) {
        std::ifstream file(filename);
        int highscore = 0;
        if (file.is_open()) {
            file >> highscore;
            file.close();
        }
        return highscore;
    }

    static void saveHighscore(const std::string& filename, int score) {
        int currentHighscore = loadHighscore(filename);
        if (score > currentHighscore) {
            std::ofstream file(filename);
            if (file.is_open()) {
                file << score;
                file.close();
            }
        }
    }
};


