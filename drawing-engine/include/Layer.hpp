#include "Canvas.hpp"
#pragma once
#include "History.hpp"
#include <string>

class Layer{
    private:
        std::string id_;
        Canvas canvas_;
        float opacity_ = 1.0f;
        bool visible_ = true;
        History history_;
        std::string name_ = "Layer";

        bool dirty_ = true;

    public:
        Layer(int width, int height);
        Layer(int width, int height, std::string id);
        const std::string& getId() const;
        Layer duplicate() const;
        
        void setOpacity(float opacity);
        void toggleVisible();
        float getOpacity() const;
        bool isVisible() const;
        void setVisible(bool visible);
        Canvas& getCanvas();
        const Canvas& getCanvas() const;
        History& getHistory();

        const std::string& getName() const;
        void setName(const std::string& name);

        bool isDirty() const;
        void markDirty();
        void markClean();
};
