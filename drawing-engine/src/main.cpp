#include "Canvas.hpp"
#include "Color.hpp"
#include "Drawing.hpp"
#include "ImageWriter.hpp"

#include <iostream>
#include <SDL3/SDL.h>

#include "BrushTool.hpp"
#include <EraserTool.hpp>
#include "History.hpp"
#include <LayerStack.hpp>

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

    LayerStack layerStack(800, 600);

    layerStack.addLayer();
    layerStack.addLayer();

    layerStack.getLayer(0).getCanvas().clear(
        Color{255, 255, 255, 255}.pack()
    );

    layerStack.getLayer(1).getCanvas().clear(
        Color{0, 0, 0, 0}.pack()
    );

    int activeLayerIndex = 0;
    
    Color white{255, 255, 255, 255};
    Color red{255, 0, 0, 255};
    //canvas.clear(white.pack());

    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        static_cast<int>(800),
        static_cast<int>(600)
    );

    if (texture == nullptr) {
        std::cerr << "Texture creation failed: " << SDL_GetError() << "\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    BrushTool brush;
    EraserTool eraser;
    Tool* activeTool = nullptr;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                if (event.key.key == SDLK_1) {
                    activeLayerIndex = 0;
                }

                if (event.key.key == SDLK_2) {
                    activeLayerIndex = 1;
                }

                if (event.key.key == SDLK_LEFTBRACKET) {
                    layerStack.getLayer(activeLayerIndex).setOpacity(-0.1);
                }

                if (event.key.key == SDLK_RIGHTBRACKET) {
                    layerStack.getLayer(activeLayerIndex).setOpacity(0.1);
                }

                if (event.key.key == SDLK_V && activeLayerIndex != 0) {
                    layerStack.getLayer(activeLayerIndex).toggleVisible();
                }

                SDL_SetWindowTitle(
                    window,
                    ("MAnimate - Layer " + std::to_string(activeLayerIndex)).c_str()
                );
            }
            
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                Canvas& canvas = layerStack.getLayer(activeLayerIndex).getCanvas();
                if (event.button.button == SDL_BUTTON_LEFT) {
                    activeTool = &brush;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    activeTool = &eraser;
                }

                if (activeTool != nullptr) {
                    layerStack.getLayer(activeLayerIndex).getHistory().save(canvas);
                    activeTool->onMouseDown(canvas, static_cast<int>(event.button.x), static_cast<int>(event.button.y));
                }
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                Canvas& canvas = layerStack.getLayer(activeLayerIndex).getCanvas();
                if (activeTool != nullptr) {
                    activeTool->onMouseUp(canvas, static_cast<int>(event.button.x), static_cast<int>(event.button.y));
                    activeTool = nullptr;
                }
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                Canvas& canvas = layerStack.getLayer(activeLayerIndex).getCanvas();
                if (activeTool != nullptr) {
                    activeTool->onMouseMove(canvas, static_cast<int>(event.motion.x), static_cast<int>(event.motion.y));
                }
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                bool ctrl = (event.key.mod & SDL_KMOD_CTRL) != 0;

                if (ctrl && event.key.key == SDLK_Z) {
                    Layer& layer = layerStack.getLayer(activeLayerIndex);
                    layer.getHistory().undo(layer.getCanvas());
                }

                if (ctrl && event.key.key == SDLK_Y) {
                    Layer& layer = layerStack.getLayer(activeLayerIndex);
                    layer.getHistory().redo(layer.getCanvas());
                }
            }
        }

        Canvas composite = layerStack.composite();

        SDL_UpdateTexture(
            texture,
            nullptr,
            composite.getPixels().data(),
            static_cast<int>(800 * sizeof(std::uint32_t))
        );

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