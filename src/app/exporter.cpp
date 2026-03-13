#include "app/exporter.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

std::optional<std::string> Exporter::save(const sf::RenderWindow& window,
                                           const std::string& filename) {
    try {
        sf::Texture tex;
        unsigned int w = window.getSize().x;
        unsigned int h = window.getSize().y;
        tex.create(w, h);
        tex.update(window);

        sf::Image img = tex.copyToImage();
        bool ok = img.saveToFile(filename);
        if (!ok) {
            return std::string("Failed to save image to ") + filename;
        }
        return std::nullopt; // все ок
    } catch (const std::exception& e) {
        std::string errorMsg = "Export error: ";
        errorMsg += e.what();
        return errorMsg;
    }
}
