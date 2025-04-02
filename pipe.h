#ifndef PIPE_H
#define PIPE_H

#include "graphics.h"
#include <vector>
#include <SDL.h>
#include <cstdlib>

const int PIPE_WIDTH = 100;
const int PIPE_HEIGHT = 340;
const int GAP_HEIGHT = 180;
const int PIPE_COUNT = 3;
const int PIPE_SPACING = 200;
const int MIN_PIPE_SPACING = 250;
const int MAX_PIPE_SPACING = 450;

struct Pipe {
    int x;
    int y;
    int z;
    bool passed; // Đánh dấu xem ống đã được vượt qua chưa

    Pipe(int startX) {
        z = 30 + rand() % 50;
        x = startX;
        y = z + rand() % (SCREEN_HEIGHT - GAP_HEIGHT - PIPE_HEIGHT);
        passed = false; // Ban đầu chưa được vượt qua
    }

    void scroll(double v) {
        x -= v;
        if (x + PIPE_WIDTH < 0) {
            x = SCREEN_WIDTH + PIPE_SPACING;
            z = 30 + rand() % 200;
            y = z + rand() % (SCREEN_HEIGHT - GAP_HEIGHT - PIPE_HEIGHT + 30);
            passed = false; // Đặt lại trạng thái khi ống quay lại
        }
    }

    void render(SDL_Renderer* renderer, SDL_Texture* PipeTexture) {
        SDL_Rect topPipe = { x, y - GAP_HEIGHT * 2, PIPE_WIDTH, PIPE_HEIGHT };
        SDL_Rect bottomPipe = { x, y + GAP_HEIGHT, PIPE_WIDTH, PIPE_HEIGHT };
        SDL_RenderCopyEx(renderer, PipeTexture, NULL, &topPipe, 0, NULL, SDL_FLIP_VERTICAL);
        SDL_RenderCopy(renderer, PipeTexture, NULL, &bottomPipe);
    }
};
void generatePipes(std::vector<Pipe>& pipes) {
    pipes.clear();
    int startX = SCREEN_WIDTH;
    for (int i = 0; i < PIPE_COUNT; i++) {
        pipes.emplace_back(startX);
        pipes[i].passed = false; // Đảm bảo tất cả ống chưa được vượt qua
        startX += MIN_PIPE_SPACING + rand() % (MAX_PIPE_SPACING - MIN_PIPE_SPACING + 1);
    }
}
void updatePipes(std::vector<Pipe>& pipes, double v) {
    for (auto& pipe : pipes) {
        pipe.scroll(v);
    }

    if (!pipes.empty() && pipes.front().x + PIPE_WIDTH < 0) {
        Pipe movedPipe = pipes.front();
        pipes.erase(pipes.begin());
        movedPipe.x = pipes.back().x + PIPE_SPACING;
        movedPipe.y = rand() % (SCREEN_HEIGHT - GAP_HEIGHT - PIPE_HEIGHT + 30 );
        movedPipe.passed = false; // Reset trạng thái của ống khi di chuyển
        pipes.push_back(movedPipe);
    }
}
void renderPipes(SDL_Renderer* renderer, SDL_Texture* PipeTexture, std::vector<Pipe>& pipes){
    for (auto& pipe : pipes) {
        pipe.render(renderer, PipeTexture);
    }
}

#endif // PIPE_H
