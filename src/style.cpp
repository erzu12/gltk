#include "style.h"

namespace gltk {

void StyleSheet::setStyle(const std::string &name, const Style &style) {
    auto it = styles.find(name);
    if (it != styles.end()) {
        it->second = style; // Update existing style
    } else {
        styles[name] = style; // Add new style
    }
}

Style StyleSheet::getStyle(const std::string &name) const {
    auto it = styles.find(name);
    if (it == styles.end()) {
        throw std::runtime_error("Style with name '" + name + "' does not exist.");
    }
    return it->second;
}

} // namespace gltk
