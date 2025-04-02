#ifndef _LOGIC__H
#define _LOGIC__H

#include <cmath>
#include "graphics.h"
#include <SDL_mixer.h>

const int birdRadius = 17 ;

struct logicBird {
    double velocity = 0;   // Vận tốc rơi
    double gravity = 0.5;  // Gia tốc rơi
    double jumpPower = -10; // Lực nhảy
    double angle = 0;      // Góc xoay của chim

    void fall(int &y) {
        velocity += gravity; // Tăng tốc độ rơi
        y += (int)velocity;  // Cập nhật vị trí

        if (velocity > 0) {
            angle += 2;  //tăng góc để cúi xuống
        } else {
            angle -= 3;  //giảm góc để ngửa lên
        }

        // Giới hạn góc quay từ -15 đến 30
        if (angle < -15) angle = -15;
        if (angle > 30) angle = 30;
    }

    void space(int &y) {
        velocity = jumpPower; // Khi nhảy, đặt lại vận tốc
        angle = -15  ;        // Ngửa đầu lên khi nhảy
    }

    double getAngle() {
        return angle; // Trả về góc xoay để vẽ chim
    }
};

bool checkCollision(int birdX, int birdY, int birdRadius, const std::vector<Pipe>& pipes) {
    int birdCenterX = birdX + 54;
    int birdCenterY = birdY + 48;

    for (const auto& pipe : pipes) {
        int pipeTopX = pipe.x;
        int pipeTopY = 0;
        int pipeBottomX = pipe.x;
        int pipeBottomY = pipe.y + GAP_HEIGHT;

        // Kiểm tra va chạm với phần ống
        if (birdCenterX + birdRadius > pipeTopX && birdCenterX - birdRadius < pipeTopX + PIPE_WIDTH &&
            birdCenterY - birdRadius < pipe.y) {
            return true;
        }

        // Kiểm tra va chạm với phần dưới của ống
        if (birdCenterX + birdRadius > pipeBottomX && birdCenterX - birdRadius < pipeBottomX + PIPE_WIDTH &&
            birdCenterY + birdRadius > pipeBottomY) {
            return true;
        }
    }
    return false;
}

bool updateScore(int& score, int birdX, std::vector<Pipe>& pipes) {
    bool flag;
    for (auto& pipe : pipes) {
        if (birdX > pipe.x + PIPE_WIDTH && !pipe.passed) {
            score++;
            pipe.passed = true; // Đánh dấu ống đã được vượt qua
            flag = pipe.passed;
        }
    }
    return flag;
}

void renderScore(Graphics& graphics, TTF_Font* font, int score, int scoreX, int scoreY) {
    SDL_Color color = {255, 255, 255, 255}; // Màu trắng
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Texture* scoreTexture = graphics.renderText(scoreText.c_str(), font, color);

    if (scoreTexture) {
        graphics.renderTexture(scoreTexture, scoreX, scoreY);
        SDL_DestroyTexture(scoreTexture);
    }
}

#endif //_LOGIC__H
