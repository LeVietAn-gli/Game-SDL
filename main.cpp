#include "graphics.h"
#include "defs.h"
#include "pipe.h"
#include "logic.h"
#include "background.h"

using namespace std;

bool quit = false;
bool intro = true;
int stages = 0;

int main(int argc, char *argv[]) {
    Graphics graphics;
    ScrollingBackground background;

    SDL_Event e;
    int x = 150;
    int y = SCREEN_HEIGHT/2;
    int score = 0;
    bool resume = false;
    double v = 6;

    logicBird logic;
    graphics.init();
    graphics.setIcon(WINDOW_ICON_PATH);

    // Load font
    TTF_Font* font = graphics.loadFont(FONT_PATH, TEXT_SIZE_BIG);
    SDL_Color colorRed = {255, 0, 0, 255};

    //Text intro
    SDL_Texture* textTextureIntro = graphics.renderText("FLAPPY  RED  BIRD", font, colorRed);
    int textWidth, textHeight;
    SDL_Texture* textTextureguide = graphics.renderText("Press to play", font, colorRed);
    SDL_QueryTexture(textTextureIntro, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textIntro;
    textIntro.x = (SCREEN_WIDTH - textWidth) / 2;  // Căn giữa theo chiều ngang
    textIntro.y = (SCREEN_HEIGHT - textHeight) / 2; // Căn giữa theo chiều dọc
    textIntro.w = textWidth;
    textIntro.h = textHeight;

    //Text Lost
    SDL_Texture* textTextureLost = graphics.renderText("YOU LOST!", font, colorRed);
    int textW, textH;
    SDL_QueryTexture(textTextureLost, NULL, NULL, &textW, &textH);
    SDL_Rect textLost;
    textLost.x = (SCREEN_WIDTH - textW) / 2;  // Căn giữa theo chiều ngang
    textLost.y = (SCREEN_HEIGHT - textH) / 2; // Căn giữa theo chiều dọc
    textLost.w = textW;
    textLost.h = textH - 10;

    //Text guide play again
    SDL_Texture* textTextureAgain = graphics.renderText("Press to play again", font, colorRed);
    int textWa, textHa;
    SDL_QueryTexture(textTextureAgain, NULL, NULL, &textWa, &textHa);
    SDL_Rect textAgain;
    textLost.x = (SCREEN_WIDTH - textW) / 2;  // Căn giữa theo chiều ngang
    textLost.y = (SCREEN_HEIGHT - textH) / 2 + 30 ; // Căn giữa theo chiều dọc
    textLost.w = textWa;
    textLost.h = textHa;

    // Load âm thanh
    Mix_Music *gMusic = graphics.loadMusic(MUSIC_THEME_PATH);
    graphics.play(gMusic);
    Mix_Chunk *gJump = graphics.loadSound(SOUND_JUMP);
    Mix_Chunk *gLost = graphics.loadSound(SOUND_LOST);
    Mix_Chunk *gScore = graphics.loadSound(SOUND_PASSED);

    // Load hình ảnh
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));
    Sprite bird;
    SDL_Texture* birdTexture = graphics.loadTexture(BIRD_IMAGE);
    SDL_Texture* pipeTexture = graphics.loadTexture(PIPE_IMAGE);
    bird.init(birdTexture, BIRD_FRAMES, BIRD_CLIPS);

    // Tạo danh sách ống
    vector<Pipe> pipes;
    generatePipes(pipes);

    while (!quit) {
        //intro
        while(intro){
            graphics.prepareScene();
            background.scroll(1);
            graphics.render(background);
            SDL_RenderCopy(graphics.renderer, textTextureIntro, NULL, &textIntro);
            graphics.presentScene();

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                } else if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) || e.type == SDL_MOUSEBUTTONDOWN){
                    intro = false;
                }
            }
        }
        //Intro
        if (!intro && !resume && stages == 0){
            graphics.prepareScene();
            background.scroll(1);
            graphics.render(background);
            SDL_RenderCopy(graphics.renderer, textTextureguide, NULL, &textIntro);
            graphics.presentScene();
        }

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            } else if ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) || e.type == SDL_MOUSEBUTTONDOWN) {
                if (!resume) {
                    stages++;
                    resume = true;
                    SDL_Delay(100);
                    y = 200;
                    pipes.clear();
                    generatePipes(pipes);
                    score = 0; // Reset score khi bắt đầu lại
                    v = 6;
                    graphics.play(gJump);
                } else {
                    graphics.play(gJump);
                    logic.space(y);
                }
            }
        }

        if (resume) {
            if (y < 0) y = 0;
            logic.fall(y);

            // Kiểm tra va chạm
            if (checkCollision(x, y, birdRadius, pipes) || y >= SCREEN_HEIGHT - 70) {
                resume = false;
                graphics.play(gLost);
            }

            // Cập nhật điểm số
            if(updateScore(score, x + 53, pipes)) graphics.play(gScore);
            if(score%10 == 0 && score !=0 && v < 10) v = v + 0.02;

            // Cập nhật chim và ống
            bird.tick();
            updatePipes(pipes, v);

            // Vẽ lại màn hình
            graphics.prepareScene();
            background.scroll(1);
            graphics.render(background);
            renderPipes(graphics.renderer, pipeTexture, pipes);
            graphics.render(x, y, bird, logic.getAngle());

            renderScore(graphics, font, score); // Hiển thị điểm số
            if (!resume){
                SDL_RenderCopy(graphics.renderer, textTextureLost, NULL, &textIntro);
                graphics.presentScene();
            }
            graphics.presentScene();

            SDL_Delay(10);
        }
    }

    // Giải phóng tài nguyên
    if (gMusic != nullptr) Mix_FreeMusic(gMusic);
    if (gJump != nullptr) Mix_FreeChunk(gJump);
    if (gLost != nullptr) Mix_FreeChunk(gLost);
    TTF_CloseFont(font);
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    graphics.quit();

    return 0;
}
