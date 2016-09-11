#include <cstdlib>
#include <cmath>
#include "Paddle.h"
#include "Ball.h"
#include "Game.h"

Paddle::Paddle(int x, int y) {
    x_ = x;
    y_ = y;
}

int Paddle::getX() const {
    return x_;
}

int Paddle::getY() const {
    return y_;
}

void Paddle::setY(int y) {
    y_ = y;

    // Paddle shouldn't be allowed to go above or below the screen.
    if (y_ < 0) {
        y_ = 0;
    }
    else if (y_ + PADDLE_HEIGHT > SCREEN_HEIGHT) {
        y_ = SCREEN_HEIGHT - PADDLE_HEIGHT;
    }
}

void Paddle::addToY(int y) {
    y_ += y;

    // Paddle shouldn't be allowed to go above or below the screen.
    if (y_ < 0)
        y_ = 0;
    else if (y_ + PADDLE_HEIGHT > SCREEN_HEIGHT)
        y_ = SCREEN_HEIGHT - PADDLE_HEIGHT;
}

// Imprecise prediction of ball position on the y-axis.
int Paddle::predict(Ball *ball) {
    // Find slope.
    float slope = static_cast<float>(ball->y_ - ball->y_ + ball->dy_) /
                  (ball->x_ - ball->x_ +ball->dx_);

    // Distance between ball and paddle.
    int paddleDistance = ball->x_ - x_;

    // Prediction without taking into consideration upper and
    // bottom wall collisions.
    int predictedY = (int) std::abs(slope * -(paddleDistance) + ball->y_);

    // Calculate number of reflexions.
    int numberOfReflexions = predictedY / SCREEN_HEIGHT;

    // Predictions taking into consideration upper and bottom
    // wall collisions.

    // Even number of reflexions.
    if (numberOfReflexions % 2 == 0)
        predictedY = predictedY % SCREEN_HEIGHT;

        // Odd number of reflexions.
    else
        predictedY = SCREEN_HEIGHT - (predictedY % SCREEN_HEIGHT);

    return predictedY;
}

// Basic AI movement.
void Paddle::AI(Ball *ball) {
    // Ball on the left 3/5th side of the screen and going left.
    if (ball->x_ < SCREEN_WIDTH * 3/5 && ball->dx_ < 0) {
        // Follow the ball.
        if (y_ + (PADDLE_HEIGHT - BALL_DIAMETER)/2 < ball->predictedY_ - 2) {
            addToY(ball->speed_/8 * 5);
        }
        else if (y_ + (PADDLE_HEIGHT - BALL_DIAMETER)/2 > ball->predictedY_ + 2) {
            addToY(-(ball->speed_/8 * 5));
        }
        // Ball is anywhere on the screen but going right.
    } else if (ball->dx_ >= 0) {
        // Left paddle slowly moves to the center.
        if (y_ + PADDLE_HEIGHT / 2 < SCREEN_HEIGHT/2) {
            addToY(2);
        }
        else if (y_ + PADDLE_HEIGHT / 2 > SCREEN_HEIGHT/2) {
            addToY(-2);
        }
    }
}
