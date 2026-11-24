#pragma once
#include "Timeline.hpp"
#include "BrushTool.hpp"
#include "EraserTool.hpp"
#include "Tool.hpp"

class Editor {
    private:
        Timeline timeline_;
        int activeLayerIndex_;
        bool onionMode_;
        bool isPlaying_;
        int fps_;
        BrushTool brush_;
        EraserTool eraser_;
        Tool* activeTool_;

    public:
        Editor(int width, int height);

        void addFrame();
        void duplicateFrame();
        void removeCurrentFrame();
        void nextFrame();
        void previousFrame();
        int getCurrentFrameIndex() const;
        int getFrameCount() const;

        LayerStack& getLayerStack();
        void addLayer();
        void removeCurrentLayer();
        void nextLayer();
        void previousLayer();
        int getActiveLayerIndex() const;

        void changeLayerOpacity(float amount);
        void toggleLayerVisibility();

        Layer& getActiveLayer();
        void moveLayerUp();
        void moveLayerDown();

        void moveFrameLeft();
        void moveFrameRight();

        void undo();
        void redo();

        void onMouseDown(int x, int y, bool useEraser);
        void onMouseMove(int x, int y);
        void onMouseUp(int x, int y);

        void togglePlayback();
        bool isPlaying() const;
        void setFPS(int fps);
        int getFPS() const;
        void advancePlaybackFrame();

        void toggleOnionMode();
        bool isOnionMode() const;      
        
        Canvas render();

        void goToFrame(int index);
        void selectLayer(int index);
        int getLayerCount();
        int getWidth() const;
        int getHeight() const;


};

