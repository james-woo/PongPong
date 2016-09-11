#include "Game.h"

#include "Ball.h"
#include "Paddle.h"

Game::Game(int argc, char* argv[]) {
    // Intialize SDL.
    SDL_Init(SDL_INIT_EVERYTHING);

    // Don't show cursor.
    SDL_ShowCursor(0);

    // Create window and renderer_.
    window_ = SDL_CreateWindow("Pong",
                               SDL_WINDOWPOS_UNDEFINED,  // Centered window.
                               SDL_WINDOWPOS_UNDEFINED,  // Centered window.
                               SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN);

    renderer_ = SDL_CreateRenderer(window_, 
                                   -1, 
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // Instantiate objects
    ball_ = new Ball(SCREEN_WIDTH/2 - BALL_DIAMETER/2, SCREEN_HEIGHT/2 - BALL_DIAMETER/2);
    leftPaddle_ = new Paddle(40, SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2);
    rightPaddle_ = new Paddle(SCREEN_WIDTH - (40 + PADDLE_WIDTH), SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2);

    // Controllers.
    if (argc == 2) {
        if ( strcmp(argv[1], "keyboard") == 0 ) {
            controller_ = keyboard;
        } else {
            controller_ = mouse;
        }
    } else {
        controller_ = mouse;  // Default controller.
    }

    // Scores
    leftScore_ = 0;
    rightScore_ = 0;
    leftScoreChanged_ = true;
    rightScoreChanged_ = true;

    // Game status_.
    exit_ = false;
}

Game::~Game() {
    // Destroy renderer_ and window.
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    // Shuts down SDL.
    SDL_Quit();
}

void Game::execute() {
    while (!exit_) {
        input();
        update();
        render();
        SDL_Delay(10);
    }
}

void Game::input() {
    // Handle events.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Track mouse movement.
        if (event.type == SDL_MOUSEMOTION) {
            SDL_GetMouseState(&mouseX_, &mouseY_);
        }

        // Clicking 'x' or pressing F4.
        if (event.type == SDL_QUIT) {
            exit_ = true;
        }

        // Pressing a key.
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                // Pressing ESC exits from the game.
                case SDLK_ESCAPE:
                    exit_ = true;
                    break;

                // Pressing space will launch the ball if it isn't
                // already launched.
                case SDLK_SPACE:
                    if (ball_->status_ == ball_->READY) {
                        ball_->status_ = ball_->LAUNCH;
                    }
                    break;
            }
        }
    }
}

// Update game values.
void Game::update() {
    // Paddle movement.
    if (controller_ == mouse) {
        // Right paddle follows the player's mouse on the y-axis.
        rightPaddle_->setY(mouseY_);
    }

    // AI paddle movement.
    leftPaddle_->AI(ball_);

    // Launch ball_.
    if (ball_->status_ == ball_->READY) {
        return;
    } else if (ball_->status_ == ball_->LAUNCH) {
        ball_->launchBall();
        ball_->predictedY_ = leftPaddle_->predict(ball_);
    }

    // Update ball_ speed.
    ball_->updateSpeed();

    // Collision.
    if (ball_->collidesWith(leftPaddle_)) {
        ball_->bouncesOff(leftPaddle_);
    } else if (ball_->collidesWith(rightPaddle_)) {
        ball_->bouncesOff(rightPaddle_);
        // Predict ball_ position on the y-axis.
        ball_->predictedY_ = leftPaddle_->predict(ball_);
    }

    // Upper and bottom walls collision.
    if (ball_->wallCollision()) {
        ball_->dy_ *= -1;  // Reverse ball_ direction on y-axis.
    }

    // Update ball_ coordinates.
    ball_->x_ += ball_->dx_;
    ball_->y_ += ball_->dy_;

    // Ball goes out.
    if (ball_->x_ > SCREEN_WIDTH || ball_->x_ < 0) {
        // Change score.
        if (ball_->x_ > SCREEN_WIDTH) {
            leftScore_++;
            leftScoreChanged_ = true;
        } else {
            rightScore_++;
            rightScoreChanged_ = true;
        }
        ball_->reset();
    }
}

// Render objects on screen.
void Game::render() {
    // Clear screen (background color).
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);  // Dark grey.
    SDL_RenderClear(renderer_);

    // Paddle color.
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);

    // Render filled paddle.
    SDL_Rect paddle1 = { leftPaddle_->getX(),
                         leftPaddle_->getY(),
                         PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_RenderFillRect(renderer_, &paddle1);

    // Render filled paddle.
    SDL_Rect paddle2 = { rightPaddle_->getX(),
                         rightPaddle_->getY(),
                         PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_RenderFillRect(renderer_, &paddle2);

    // Render ball_.
    SDL_Rect pongBall = { ball_->x_, ball_->y_, BALL_DIAMETER, BALL_DIAMETER };
    SDL_RenderFillRect(renderer_, &pongBall);


    // Render text indicating the winner.
    if (leftScore_ == 5) {
        if (ball_->status_ == ball_->LAUNCHED) {
            leftScore_ = 0;
            rightScore_ = 0;
            leftScoreChanged_ = true;
            rightScoreChanged_ = true;
        }
    } else if (rightScore_ == 5) {
        if (ball_->status_ == ball_->LAUNCHED) {
            leftScore_ = 0;
            rightScore_ = 0;
            leftScoreChanged_ = true;
            rightScoreChanged_ = true;
        }
    }

    // Swap buffers.
    SDL_RenderPresent(renderer_);
}

