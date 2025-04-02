#ifndef _DEFS__H
#define _DEFS__H
#define WINDOW_TITLE "Flappy Red Bird v1.0"
#define WINDOW_ICON_PATH "assets\\icon_redbird.png"

#define BACKGROUND_IMG "assets\\background.png"
#define BACKGROUND_GAMEOVER "assets\\background.png"

#define FONT_PATH "assets\\MouldyCheeseRegular-WyMWG.ttf"

#define MUSIC_THEME_PATH "assets\\C418 - Moog City 2.wav"
#define SOUND_JUMP "assets\\jump.wav"
#define SOUND_LOST "assets\\lost.wav"
#define SOUND_PASSED "assets\\passed.wav"

#define PIPE_IMAGE "assets\\pipe.png"
#define BIRD_IMAGE "assets\\flappy.png"

#define TEXT_SIZE_BIG 30
#define TEXT_SIZE_SMALL 16

enum class GameStates {
    Intro,
    Level,
    Lost,
    Quit,
    Null
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 550;

const int BIRD_CLIPS[][4] = {
    {0, 0, 182, 168},
    {181, 0, 182, 168},
    {364, 0, 182, 168},
    {547, 0, 182, 168},
    {728, 0, 182, 168},

    {0, 170, 182, 168},
    {181, 170, 182, 168},
    {364, 170, 182, 168},
    {547, 170, 182, 168},
    {728, 170, 182, 168},

    {0, 340, 182, 168},
    {181, 340, 182, 168},
    {364, 340, 182, 168},
    {547, 340, 182, 168},
};
const int BIRD_FRAMES = sizeof(BIRD_CLIPS)/sizeof(int)/4;

#endif
