/* Copyright Colton Sullivan
* Basic asteroids arcade game movement for an entity
* made in c++ with the sdl2 libraries
* 
* Ongoing free time project
* 
* 2022-2023
*/

#include <SDL.h>
#include <cmath>
#include <iostream>

const int WIDTH = 1024;
const int HEIGHT = 768;
const int SPEED = 5;
const int ROTATION_SPEED = 10;

void HandleEvents(bool& running, int& x, int& y, double& angle, double& xVel, double& yVel)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_LEFT:
                angle -= ROTATION_SPEED;
                break;
            case SDLK_RIGHT:
                angle += ROTATION_SPEED;
                break;
            case SDLK_UP:
                xVel = sin(angle * 3.14159265 / 180) * SPEED;
                yVel = -cos(angle * 3.14159265 / 180) * SPEED;
                break;
            case SDLK_DOWN:
                xVel = -sin(angle * 3.14159265 / 180) * SPEED;
                yVel = cos(angle * 3.14159265 / 180) * SPEED;
                break;
            default:
                break;
            }
        }
    }
    // Deceleration
    double deceleration = 0.99;
    xVel *= deceleration;
    yVel *= deceleration;
    x += xVel;
    y += yVel;
    angle = fmod(angle, 360.0);

    if (x > WIDTH) x = 0;
    if (x < 0) x = WIDTH;
    if (y > HEIGHT) y = 0;
    if (y < 0) y = HEIGHT;
}

void Draw(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle)
{
    SDL_Rect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 32;
    srcRect.h = 32;

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = 32;
    destRect.h = 32;

    SDL_RenderClear(renderer);
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, angle, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;
    int x = 320, y = 240;
    double angle = 0;
    double xVel = 0, yVel = 0;
    bool running = true;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << ("Error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    surface = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    Uint32 time = SDL_GetTicks();
    Uint32 frameTime;
    Uint32 fixedTimeStep = 1000 / 30; // 30 fps
    while (running)
    {
        frameTime = SDL_GetTicks() - time;
        time = SDL_GetTicks();
        HandleEvents(running, x, y, angle, xVel, yVel);
        Draw(renderer, texture, x, y, angle);
        if (frameTime < fixedTimeStep)
            SDL_Delay(fixedTimeStep - frameTime);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
