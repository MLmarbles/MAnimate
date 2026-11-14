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
#include <algorithm>
#include "Timeline.hpp"

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

    Timeline timeline(800, 600);

    auto getLayerStack = [&timeline]() -> LayerStack& {
        return timeline.getCurrentFrame().getLayerStack();
    };

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
    bool onionMode = true;

    bool isPlaying = false;
    int fps = 12;
    Uint64 lastFrameTime = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
                bool ctrl = (event.key.mod & SDL_KMOD_CTRL) != 0;

                if (!ctrl && event.key.key == SDLK_EQUALS) {
                    if (fps < 60) {
                        fps+=1;
                    }
                }

                if (!ctrl && event.key.key == SDLK_MINUS) {
                    if (fps > 1) {
                        fps-=1;
                    }
                }

                if (!ctrl && event.key.key == SDLK_SPACE) {
                    isPlaying = !isPlaying;
                    lastFrameTime = SDL_GetTicks();
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_O) {
                    onionMode = !onionMode;
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_D) {
                    timeline.duplicateFrame();
                    activeLayerIndex = 0;
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_F) {
                    timeline.addFrame();
                    activeLayerIndex = 0;
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_RIGHT) {
                    timeline.nextFrame();
                    activeLayerIndex = 0;
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_LEFT) {
                    timeline.previousFrame();
                    activeLayerIndex = 0;
                    needsRedraw = true;
                }

                if (!ctrl && event.key.key == SDLK_DOWN) {
                    if (activeLayerIndex > 0) {
                        activeLayerIndex--;
                        needsRedraw = true;
                    }
                }

                if (!ctrl && event.key.key == SDLK_UP) {
                    if (activeLayerIndex < getLayerStack().getLayerIndexes()) {
                        activeLayerIndex++;
                        needsRedraw = true;
                    }
                }
                
                if (event.key.key == SDLK_LEFTBRACKET) {
                    getLayerStack().getLayer(activeLayerIndex).setOpacity(-0.1);
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_RIGHTBRACKET) {
                    getLayerStack().getLayer(activeLayerIndex).setOpacity(0.1);
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_V) {
                    getLayerStack().getLayer(activeLayerIndex).toggleVisible();
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_N) {
                    getLayerStack().addLayer();
                    int layerIndexes = getLayerStack().getLayerIndexes();

                    getLayerStack().getLayer(layerIndexes).getCanvas().clear(
                        Color{0, 0, 0, 0}.pack()
                    );

                    activeLayerIndex = layerIndexes;
                    needsRedraw = true;
                }

                if (event.key.key == SDLK_DELETE) {
                    if (getLayerStack().getLayerIndexes() > 0) {
                        getLayerStack().removeLayer(activeLayerIndex);
                        activeLayerIndex = std::min(
                            activeLayerIndex,
                            getLayerStack().getLayerIndexes()
                        );
                        needsRedraw = true;
                    }
                }

                if (ctrl && event.key.key == SDLK_UP) {
                    int newIndex = activeLayerIndex + 1;

                    if (newIndex <= getLayerStack().getLayerIndexes()) {
                        getLayerStack().moveLayer(activeLayerIndex, newIndex);
                        activeLayerIndex = newIndex;
                        needsRedraw = true;
                    }
                }

                if (ctrl && event.key.key == SDLK_DOWN) {
                    int newIndex = activeLayerIndex - 1;

                    if (newIndex >= 0) {
                        getLayerStack().moveLayer(activeLayerIndex, newIndex);
                        activeLayerIndex = newIndex;
                        needsRedraw = true;
                    }
                }

                SDL_SetWindowTitle(
                    window,
                    (
                        "MAnimate - Frame " +
                        std::to_string(timeline.getCurrentFrameIndex() + 1) +
                        "/" +
                        std::to_string(timeline.getFrameCount()) +
                        " - Layer " +
                        std::to_string(activeLayerIndex) +
                        " - fps " +
                        std::to_string(fps)
                    ).c_str()
                );
            }
            
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                Canvas& canvas = getLayerStack().getLayer(activeLayerIndex).getCanvas();
                if (event.button.button == SDL_BUTTON_LEFT) {
                    activeTool = &brush;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    activeTool = &eraser;
                }

                if (activeTool != nullptr) {
                    getLayerStack().getLayer(activeLayerIndex).getHistory().save(canvas);
                    activeTool->onMouseDown(canvas, static_cast<int>(event.button.x), static_cast<int>(event.button.y));
                    needsRedraw = true;
                }
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                Canvas& canvas = getLayerStack().getLayer(activeLayerIndex).getCanvas();
                if (activeTool != nullptr) {
                    activeTool->onMouseUp(canvas, static_cast<int>(event.button.x), static_cast<int>(event.button.y));
                    activeTool = nullptr;
                    needsRedraw = true;
                }
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                Canvas& canvas = getLayerStack().getLayer(activeLayerIndex).getCanvas();
                if (activeTool != nullptr) {
                    activeTool->onMouseMove(canvas, static_cast<int>(event.motion.x), static_cast<int>(event.motion.y));
                    needsRedraw = true;
                }
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                bool ctrl = (event.key.mod & SDL_KMOD_CTRL) != 0;

                if (ctrl && event.key.key == SDLK_Z) {
                    Layer& layer = getLayerStack().getLayer(activeLayerIndex);
                    layer.getHistory().undo(layer.getCanvas());
                    needsRedraw = true;
                }

                if (ctrl && event.key.key == SDLK_Y) {
                    Layer& layer = getLayerStack().getLayer(activeLayerIndex);
                    layer.getHistory().redo(layer.getCanvas());
                    needsRedraw = true;
                }
            }
        }

        if (isPlaying) {
            Uint64 now = SDL_GetTicks();
            Uint64 frameDuration = 1000 / fps;

            if (now - lastFrameTime >= frameDuration) {
                if (timeline.getCurrentFrameIndex() == timeline.getFrameCount() - 1) {
                    timeline.goToFrame(0);
                } else {
                    timeline.nextFrame();
                }

                activeLayerIndex = std::min(
                    activeLayerIndex,
                    getLayerStack().getLayerIndexes()
                );

                needsRedraw = true;
                lastFrameTime = now;
            }
        }

        if (needsRedraw) {
            Canvas composite = getLayerStack().composite();

            Canvas displayCanvas(800, 600);
            displayCanvas.clear(Color{255, 255, 255, 255}.pack());

            if (onionMode && !isPlaying) {
                Frame* prevptr = timeline.getPreviousFrame();

                if (prevptr != nullptr) {
                    Canvas prevCanvas = prevptr -> getLayerStack().composite();


                    for (std::size_t y = 0; y < prevCanvas.getHeight(); ++y) {
                        for (std::size_t x = 0; x < prevCanvas.getWidth(); ++x) {
                            Color top = Color::unpack(prevCanvas.getPixel(x, y));
                            Color bottom = Color::unpack(displayCanvas.getPixel(x, y));

                            Color result = Color::blend(top, bottom, 0.25f);

                            displayCanvas.setPixel(x, y, result.pack());
                        }
                    }
                }
            }

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