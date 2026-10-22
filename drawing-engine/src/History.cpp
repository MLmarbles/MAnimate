#include "History.hpp"

void History::save(const Canvas& canvas)
{
    undoStack.push_back(canvas);
    redoStack.clear();
}

void History::undo(Canvas& canvas)
{
    if (undoStack.empty()) {
        return;
    }
    Canvas poppedCanvas = undoStack.back();
    undoStack.pop_back();
    redoStack.push_back(canvas);
    canvas = poppedCanvas;
}

void History::redo(Canvas& canvas)
{
    if (redoStack.empty()) {
        return;
    }
    Canvas poppedCanvas = redoStack.back();
    redoStack.pop_back();
    undoStack.push_back(canvas);
    canvas = poppedCanvas;
}   