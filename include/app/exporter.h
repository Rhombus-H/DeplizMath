#pragma once

#include <optional>
#include <string>

namespace sf { class RenderWindow; }

// экспорт скриншота в png
class Exporter {
public:
    // возвращает ошибку если не удалось, nullopt если ок
    static std::optional<std::string> save(const sf::RenderWindow& window,
                                           const std::string& filename);
};