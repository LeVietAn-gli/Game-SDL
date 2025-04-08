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
    int highscore = 0;
    bool resume = false;
    bool highsc;
    double v = 6;

    logicBird logic;
    graphics.init();
    graphics.setIcon(WINDOW_ICON_PATH);

    // Load font
    TTF_Font* font = graphics.loadFont(FONT_PATH, TEXT_SIZE);
    SDL_Color colorRed = {255, 0, 0, 255};
    SDL_Color colorWhite ={255, 255, 255, 255};

    //Text intro
    SDL_Texture* textTextureIntro = graphics.renderText("FLAPPY  RED  BIRD", font, colorRed);
    int textWidth, textHeight;
    SDL_Texture* textTextureguide = graphics.renderText("Press to play", font, colorRed);
    SDL_QueryTexture(textTextureIntro, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textIntro;
    textIntro.x = (SCREEN_WIDTH - textWidth) / 2;
    textIntro.y = (SCREEN_HEIGHT - textHeight) / 2;
    textIntro.w = textWidth;
    textIntro.h = textHeight;

    //Text Lost
    SDL_Texture* textTextureLost = graphics.renderText("YOU LOST!", font, colorRed);
    SDL_QueryTexture(textTextureLost, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textLost;
    textLost.x = (SCREEN_WIDTH - textWidth) / 2 - 20;
    textLost.y = (SCREEN_HEIGHT - textHeight) / 2 - 40;
    textLost.w = textWidth + 50;
    textLost.h = textHeight + 5;

    //Text play again
    SDL_Texture* textTextureAgain = graphics.renderText("Press to play again", font, colorWhite);
    SDL_QueryTexture(textTextureLost, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textAgain;
    textAgain.x = (SCREEN_WIDTH - textWidth) / 2 + 8;
    textAgain.y = (SCREEN_HEIGHT - textHeight) / 2 + 30;
    textAgain.w = textWidth;
    textAgain.h = textHeight - 5;

    // Load music
    Mix_Music *gMusic = graphics.loadMusic(MUSIC_THEME_PATH);
    graphics.play(gMusic);
    Mix_Chunk *gJump = graphics.loadSound(SOUND_JUMP);
    Mix_Chunk *gLost = graphics.loadSound(SOUND_LOST);
    Mix_Chunk *gScore = graphics.loadSound(SOUND_PASSED);

    // Load texture
    background.setTexture(graphics.loadTexture(BACKGROUND_IMG));
    Sprite bird;
    SDL_Texture* birdTexture = graphics.loadTexture(BIRD_IMAGE);
    SDL_Texture* pipeTexture = graphics.loadTexture(PIPE_IMAGE);
    bird.init(birdTexture, BIRD_FRAMES, BIRD_CLIPS);

    // Creat pipes
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
                    intro = false;
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
                    if (highscore < score) highscore = score;
                    score = 0;
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

            // checkCollision
            if (checkCollision(x, y, birdRadius, pipes) || y >= SCREEN_HEIGHT - 70) {
                resume = false;
                graphics.play(gLost);
            }

            // update score
            if(updateScore(score, x + 53, pipes)) graphics.play(gScore);
            if(score%10 == 0 && score !=0 && v < 10) v = v + 0.02;

            // update bird and pos pipe, v pipe
            bird.tick();
            updatePipes(pipes, v);

            // render screen
            graphics.prepareScene();
            background.scroll(1);
            graphics.render(background);
            renderPipes(graphics.renderer, pipeTexture, pipes);
            graphics.render(x, y, bird, logic.getAngle());

            renderScore(graphics, font, score, 20, 20); //show score on the left top
            renderhighScore(graphics, font, highscore, 20, 50);

            // Show Lost
            if (!resume){
                SDL_RenderCopy(graphics.renderer, textTextureLost, NULL, &textLost);
                renderScore(graphics, font, score, SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2 - 18);
                SDL_RenderCopy(graphics.renderer, textTextureAgain, NULL, &textAgain);
                graphics.presentScene();
            }
            graphics.presentScene();

            SDL_Delay(10);
        }
    }

    // liberate
    if (gMusic != nullptr) Mix_FreeMusic(gMusic);
    if (gJump != nullptr) Mix_FreeChunk(gJump);
    if (gLost != nullptr) Mix_FreeChunk(gLost);
    TTF_CloseFont(font);
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    graphics.quit();

    return 0;
}
