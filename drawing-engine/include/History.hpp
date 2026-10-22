#pragma once

#include "canvas.hpp"
#include <vector>

class History {
    private:
        std::vector<Canvas> undoStack;
        std::vector<Canvas> redoStack;

    public:
        void save(const Canvas& canvas);
        void undo(Canvas& canvas);
        void redo(Canvas& canvas);
};