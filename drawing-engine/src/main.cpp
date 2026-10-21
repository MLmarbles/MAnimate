#include "Canvas.hpp"
#include "Color.hpp"
#include "Drawing.hpp"
#include "ImageWriter.hpp"

#include <iostream>
#include <SDL3/SDL.h>

int main()
{
    bool running = true;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL failed: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "MAnimate",
        256,
        256,
        0
    );

    if (window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        nullptr
    );

    if (renderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Canvas canvas(256, 256);
    Color white{255, 255, 255, 255};
    Color red{255, 0, 0, 255};
    canvas.clear(white.pack());

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        static_cast<int>(canvas.getWidth()),
        static_cast<int>(canvas.getHeight())
    );

    if (texture == nullptr) {
        std::cerr << "Texture creation failed: " << SDL_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool drawing = false;
    float prevX = 0.0f;
    float prevY = 0.0f;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                drawing = true;
                prevX = event.button.x;
                prevY = event.button.y;
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                drawing = false;
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                if (drawing) {
                    drawBrushStroke(
                        canvas,
                        prevX,
                        prevY,
                        event.motion.x,
                        event.motion.y,
                        5,
                        red.pack()
                    );

                    prevX = event.motion.x;
                    prevY = event.motion.y;
                }
            }

                SDL_UpdateTexture(
                    texture,
                    nullptr,
                    &canvas.getPixels()[0],
                    static_cast<int>(canvas.getWidth() * sizeof(std::uint32_t))
                );
        }

        SDL_RenderClear(renderer);

        SDL_RenderTexture(
            renderer,
            texture,
            nullptr,
            nullptr
        );

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}