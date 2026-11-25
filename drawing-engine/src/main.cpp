#include "Editor.hpp"

#include <iostream>
#include <string>
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
        800,
        600,
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

    if (!SDL_SetRenderVSync(renderer, 1)) {
        std::cerr << "VSync failed: " << SDL_GetError() << '\n';
    }

    Editor editor(800, 600);

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        editor.getWidth(),
        editor.getHeight()
    );

    if (texture == nullptr) {
        std::cerr << "Texture creation failed: " << SDL_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool needsRedraw = true;

    Uint64 lastFrameTime = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                bool ctrl = (event.key.mod & SDL_KMOD_CTRL) != 0;

                if (!ctrl && event.key.key == SDLK_EQUALS) {
                    editor.setFPS(editor.getFPS() + 1);
                }

                if (!ctrl && event.key.key == SDLK_MINUS) {
                    editor.setFPS(editor.getFPS() - 1);
                }

                if (!ctrl && event.key.key == SDLK_SPACE) {
                    editor.togglePlayback();
                    lastFrameTime = SDL_GetTicks();
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_O) {
                    editor.toggleOnionMode();
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_D) {
                    editor.duplicateFrame();
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_F) {
                    editor.addFrame();
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_DOWN) {
                    editor.previousLayer();
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_UP) {
                    editor.nextLayer();
                    needsRedraw = true;
                }
                
                if (event.key.key == SDLK_LEFTBRACKET) {
                    editor.changeLayerOpacity(-0.1f);
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_RIGHTBRACKET) {
                    editor.changeLayerOpacity(0.1f);
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_V) {
                    editor.toggleLayerVisibility();
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_N) {
                    editor.addLayer();
                    needsRedraw = true;
                }

                bool shift = (event.key.mod & SDL_KMOD_SHIFT) != 0;

                if (!ctrl && !shift && event.key.key == SDLK_RIGHT) {
                    editor.nextFrame();
                    needsRedraw = true;
                }

                if (!ctrl && !shift && event.key.key == SDLK_LEFT) {
                    editor.previousFrame();
                    needsRedraw = true;
                }
                
                if (shift && event.key.key == SDLK_RIGHT) {
                    editor.moveFrameRight();
                    needsRedraw = true;
                }

                if (shift && event.key.key == SDLK_LEFT) {
                    editor.moveFrameLeft();
                    needsRedraw = true;
                }

                if (!shift && event.key.key == SDLK_DELETE) {
                    editor.removeCurrentLayer();
                    needsRedraw = true;
                }

                if (shift && event.key.key == SDLK_DELETE && editor.getFrameCount() > 1) {
                    editor.removeCurrentFrame();
                    needsRedraw = true;
                }

                if (ctrl && event.key.key == SDLK_UP) {
                    editor.moveLayerUp();
                    needsRedraw = true;
                }

                if (ctrl && event.key.key == SDLK_DOWN) {
                    editor.moveLayerDown();
                    needsRedraw = true;
                }
            }
            
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT)) {
                editor.onMouseDown(
                    static_cast<int>(event.button.x),
                    static_cast<int>(event.button.y),
                    event.button.button == SDL_BUTTON_RIGHT
                );
                needsRedraw = true;
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                editor.onMouseUp(static_cast<int>(event.button.x), static_cast<int>(event.button.y));
                needsRedraw = true;
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                editor.onMouseMove(static_cast<int>(event.motion.x), static_cast<int>(event.motion.y));
                needsRedraw = true;
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                bool ctrl = (event.key.mod & SDL_KMOD_CTRL) != 0;

                if (ctrl && event.key.key == SDLK_Z) {
                    editor.undo();
                    needsRedraw = true;
                }

                if (ctrl && event.key.key == SDLK_Y) {
                    editor.redo();
                    needsRedraw = true;
                }
            }
        }

        if (editor.isPlaying()) {
            Uint64 now = SDL_GetTicks();
            Uint64 frameDuration = 1000 / editor.getFPS();

            if (now - lastFrameTime >= frameDuration) {
                editor.advancePlaybackFrame();

                needsRedraw = true;
                lastFrameTime = now;
            }
        }

        SDL_SetWindowTitle(
            window,
            (
                "MAnimate - Frame " +
                std::to_string(editor.getCurrentFrameIndex() + 1) +
                "/" +
                std::to_string(editor.getFrameCount()) +
                " - Layer " +
                std::to_string(editor.getActiveLayerIndex()) +
                " - fps " +
                std::to_string(editor.getFPS())
            ).c_str()
        );

        if (needsRedraw) {
            Canvas displayCanvas = editor.render();

            SDL_UpdateTexture(
                texture,
                nullptr,
                displayCanvas.getPixels().data(),
                static_cast<int>(
                    displayCanvas.getWidth() * sizeof(std::uint32_t)
                )
            );
            needsRedraw = false;
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
