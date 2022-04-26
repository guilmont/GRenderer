#pragma once

#include <unordered_map>

#include "core.h"

namespace GRender {

class  Fonts {
public:
    Fonts(void) = default;
    ~Fonts(void) = default;

    void loadDefaultFonts(void);
    void loadFont(const std::string &fontname, const std::string &path, float size);
    void setDefault(const std::string &name);

    void swap(const std::string& name1, const std::string& name2);

    void text(const std::string &txt, const std::string &type);

    void push(const std::string &fontname);
    void pop(void);

private:
    std::string assets;
    std::unordered_map<std::string, ImFont *> mFonts;
};

} // namespace GRender