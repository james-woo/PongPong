#ifndef PONG_PADDLE_H
#define PONG_PADDLE_H

class Ball;

const int PADDLE_HEIGHT = 60;
const int PADDLE_WIDTH = 10;

class Paddle {
private:
    // Paddle position
    int x_;
    int y_;

public:
    Paddle(int x, int y);

    int getX() const;
    int getY() const;

    void setX(int x);
    void setY(int y);
    void addToY(int y);
    int predict(Ball* ball);
    void AI(Ball* ball);
};

#endif //PONG_PADDLE_H
