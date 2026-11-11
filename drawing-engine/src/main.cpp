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

    if (!SDL_SetRenderVSync(renderer, 1)) {
        std::cerr << "VSync failed: " << SDL_GetError() << '\n';
    }

    LayerStack layerStack(800, 600);

    layerStack.addLayer();
    layerStack.addLayer();

    layerStack.getLayer(0).getCanvas().clear(
        Color{0, 0, 0, 0}.pack()
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

    bool needsRedraw = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                if (event.key.key == SDLK_1) {
                    activeLayerIndex = 0;
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_2) {
                    activeLayerIndex = 1;
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_LEFTBRACKET) {
                    layerStack.getLayer(activeLayerIndex).setOpacity(-0.1);
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_RIGHTBRACKET) {
                    layerStack.getLayer(activeLayerIndex).setOpacity(0.1);
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_V) {
                    layerStack.getLayer(activeLayerIndex).toggleVisible();
                    needsRedraw = true;
                }

                bool ctrl = (event.key.mod & SDL_KMOD_CTRL) != 0;

                if (ctrl && event.key.key == SDLK_UP) {
                    int newIndex = activeLayerIndex + 1;

                    if (newIndex < 2) {
                        layerStack.moveLayer(activeLayerIndex, newIndex);
                        activeLayerIndex = newIndex;
                        needsRedraw = true;
                    }
                }

                if (ctrl && event.key.key == SDLK_DOWN) {
                    int newIndex = activeLayerIndex - 1;

                    if (newIndex >= 0) {
                        layerStack.moveLayer(activeLayerIndex, newIndex);
                        activeLayerIndex = newIndex;
                    }
                    needsRedraw = true;
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
                    needsRedraw = true;
                }
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                Canvas& canvas = layerStack.getLayer(activeLayerIndex).getCanvas();
                if (activeTool != nullptr) {
                    activeTool->onMouseUp(canvas, static_cast<int>(event.button.x), static_cast<int>(event.button.y));
                    activeTool = nullptr;
                    needsRedraw = true;
                }
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                Canvas& canvas = layerStack.getLayer(activeLayerIndex).getCanvas();
                if (activeTool != nullptr) {
                    activeTool->onMouseMove(canvas, static_cast<int>(event.motion.x), static_cast<int>(event.motion.y));
                    needsRedraw = true;
                }
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                bool ctrl = (event.key.mod & SDL_KMOD_CTRL) != 0;

                if (ctrl && event.key.key == SDLK_Z) {
                    Layer& layer = layerStack.getLayer(activeLayerIndex);
                    layer.getHistory().undo(layer.getCanvas());
                    needsRedraw = true;
                }

                if (ctrl && event.key.key == SDLK_Y) {
                    Layer& layer = layerStack.getLayer(activeLayerIndex);
                    layer.getHistory().redo(layer.getCanvas());
                    needsRedraw = true;
                }
            }
        }

        if (needsRedraw) {
            Canvas composite = layerStack.composite();

            Canvas displayCanvas(800, 600);
            displayCanvas.clear(Color{255, 255, 255, 255}.pack());

            for (std::size_t y = 0; y < composite.getHeight(); ++y) {
                for (std::size_t x = 0; x < composite.getWidth(); ++x) {
                    Color top = Color::unpack(composite.getPixel(x, y));
                    Color bottom = Color::unpack(displayCanvas.getPixel(x, y));

                    Color result = Color::blend(top, bottom, 1.0f);

                    displayCanvas.setPixel(x, y, result.pack());
                }
            }

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