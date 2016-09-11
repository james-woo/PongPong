
#ifndef PONG_GAME_H
#define PONG_GAME_H

#include <SDL2/SDL.h>
#include <string>

class Ball;
class Paddle;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Game {
private:
    // Window and rednerer
    SDL_Window* window_;
    SDL_Renderer* renderer_;

    // Objects
    Ball* ball_;
    Paddle* leftPaddle_;
    Paddle* rightPaddle_;

    // Controllers
    enum Controllers { mouse, keyboard };
    Controllers controller_;
    int mouseX_, mouseY_;

    // Scores
    int leftScore_;
    int rightScore_;

    // Rerender when score changes
    bool leftScoreChanged_;
    bool rightScoreChanged_;

    // Game status
    bool exit_;

public:
    Game(int argc, char* argv[]);
    ~Game();
    void execute();
    void input();
    void update();
    void render();
};


#endif //PONG_GAME_H
