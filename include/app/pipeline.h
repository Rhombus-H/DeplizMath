#pragma once

#include <memory>
#include <string>
#include <vector>
#include "core/ast.h"

// одна запись в списке графиков
struct PlotItem {
    std::string expression;
    ParseResult parsed;
    bool visible;
    std::size_t color_index;

    PlotItem(std::string expr, ParseResult result, std::size_t colorIdx)
        : expression(std::move(expr)),
          parsed(std::move(result)),
          visible(true),
          color_index(colorIdx) {}

    // нельзя копировать (потому что unique_ptr внутри)
    PlotItem(const PlotItem&) = delete;
    PlotItem& operator=(const PlotItem&) = delete;
    PlotItem(PlotItem&&) noexcept = default;
    PlotItem& operator=(PlotItem&&) noexcept = default;
};

// pipeline manages список функций для отрисовки
class Pipeline {
public:
    Pipeline() = default;

    bool add(const std::string& expression);
    void remove(std::size_t index);
    void toggle(std::size_t index);
    bool update(std::size_t index, const std::string& expression);
    void clear();

    const std::vector<std::unique_ptr<PlotItem>>& items() const { return m_items; }
    std::size_t size() const { return m_items.size(); }
    bool empty() const { return m_items.empty(); }

    // для сохранения/восстановления
    std::vector<std::string> getExpressions() const;
    void restoreFromExpressions(const std::vector<std::string>& exprs);

private:
    std::vector<std::unique_ptr<PlotItem>> m_items;
    std::size_t nextColorIdx = 0;
};