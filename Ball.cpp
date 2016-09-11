
#include "Ball.h"

#include <cmath>
#include <random>

#include "Game.h"
#include "Paddle.h"

namespace {
    std::random_device rd;
    std::mt19937 gen(rd());
}

Ball::Ball(int x, int y) {
    // Ball status.
    status_ = READY;

    // Ball position.
    x_ = x;
    y_ = y;

    // Ball movement.
    dx_ = 0;
    dy_ = 0;

    bounce_ = false;
    speed_ = 8;
    angle_ = 0.0f;
    hits_ = 0;
    predictedY_ = 0;
}

void Ball::launchBall() {
    std::uniform_int_distribution<int> dir(0, 1);

    // Direction towards either paddle, -1 or 1
    int direction = 1 + (-2) * (dir(gen)%2);

    // Random angle to start, -60 - 60 degrees
    std::uniform_int_distribution<int> ang(-60, 60);
    angle_ = ang(gen);

    // Speed on both axis
    dx_ = (int) (direction * speed_ * std::cos(angle_ * M_PI/180.0f));
    dy_ = (int) (speed_ * std::sin(angle_ * M_PI/180.0f));

    status_ = LAUNCHED;
}

void Ball::bouncesOff(Paddle *paddle) {
    if (paddle == nullptr) return;

    hits_++;

    // Where did the ball come from? If it was left paddle, ball must travel right (+1)
    int direction = (paddle->getX() < SCREEN_WIDTH/2) ? 1 : -1;

    // Calculation for ball's new trajectory
    // Closer to the edges give larger dy
    int relativeY = (y_ - paddle->getY() + BALL_DIAMETER);
    angle_ = (2.14f * relativeY - 75.0f);

    // Speed on both axis
    dx_ = (int) (direction * speed_ * std::cos(angle_ * M_PI/180.0f));
    dy_ = (int) (speed_ * std::sin(angle_ * M_PI/180.0f));
}

void Ball::updateSpeed() {
    // Increment ball speed for every 6 hits.
    if (hits_ == 5) {
        speed_++;
        hits_ = 0;
    }
}

bool Ball::wallCollision() {
    return (y_ + dy_ < 0) || (y_ + BALL_DIAMETER + dy_ >= SCREEN_HEIGHT);
}

bool Ball::collidesWith(Paddle *paddle) {
    if (paddle->getX() < SCREEN_WIDTH/2) {
        // Check if collision with left paddle occurs in next frame
        return !(x_ > paddle->getX() + PADDLE_WIDTH || x_ < paddle->getX() ||
                 !(y_ + BALL_DIAMETER > paddle->getY() && y_ <= paddle->getY() + PADDLE_HEIGHT));
    } else {
        // Check if collision with right paddle occurs in next frame.
        return !(x_ + BALL_DIAMETER < paddle->getX() || x_ > paddle->getX() + PADDLE_WIDTH ||
                 !(y_ + BALL_DIAMETER > paddle->getY() && y_ <= paddle->getY() + PADDLE_HEIGHT));
    }
}

// Reset ball to initial state.
void Ball::reset() {
    x_ = SCREEN_WIDTH/2 - BALL_DIAMETER/2;
    y_ = SCREEN_HEIGHT/2;

    // Ball is fixed.
    dx_ = 0;
    dy_ = 0;
    status_ = READY;

    // Speed and hit counter are reset to their initial positions.
    speed_ = 8;
    hits_ = 0;
}
