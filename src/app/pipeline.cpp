#include "app/pipeline.h"
#include "parser/parser.h"
#include "evaluator/evaluator.h"
#include "core/errors.h"
#include <variant>

bool Pipeline::add(const std::string& expression) {
    try {
        Parser p(expression);
        ParseResult result = p.parse();

        // доп. валидация после парсинга
        // 1) если функция (y = ...) но rhs содержит y -> переделать в уравнение
        ParsedFunction* funcPtr = std::get_if<ParsedFunction>(&result);
        if (funcPtr != nullptr) {
            bool rhsHasY = Evaluator::containsVar(*funcPtr->expr, "y");
            if (rhsHasY) {
                // y = f(x,y) -> уравнение: lhs=y, rhs=f(x,y)
                ParsedEquation eq;
                eq.lhs = makeVariable("y");
                eq.rhs = std::move(funcPtr->expr);
                result = std::move(eq);
            }
        }

        // 2) если неравенство, проверяем что есть y
        ParsedInequality* ineqPtr = std::get_if<ParsedInequality>(&result);
        if (ineqPtr != nullptr) {
            bool lhsHasY = Evaluator::containsVar(*ineqPtr->lhs, "y");
            bool rhsHasY = Evaluator::containsVar(*ineqPtr->rhs, "y");
            if (!lhsHasY && !rhsHasY) {
                // нет y -> ошибка (иначе крашит)
                return false;
            }
        }

        std::unique_ptr<PlotItem> newItem = std::make_unique<PlotItem>(
            expression, std::move(result), nextColorIdx);

        m_items.push_back(std::move(newItem));
        nextColorIdx++;
        return true;
    } catch (const MathPlotError& err) {
        return false;
    }
}

void Pipeline::remove(std::size_t index) {
    if (index < m_items.size()) {
        m_items.erase(m_items.begin() + (long)index);
    }
}

void Pipeline::toggle(std::size_t index) {
    if (index >= m_items.size()) return;

    bool currentlyVisible = m_items[index]->visible;
    m_items[index]->visible = !currentlyVisible;
}

bool Pipeline::update(std::size_t index, const std::string& expression) {
    if (index >= m_items.size()) {
        return false;
    }

    try {
        Parser p(expression);
        ParseResult result = p.parse();

        // та же валидация что и в add
        ParsedFunction* funcPtr = std::get_if<ParsedFunction>(&result);
        if (funcPtr != nullptr) {
            bool rhsHasY = Evaluator::containsVar(*funcPtr->expr, "y");
            if (rhsHasY) {
                ParsedEquation eq;
                eq.lhs = makeVariable("y");
                eq.rhs = std::move(funcPtr->expr);
                result = std::move(eq);
            }
        }

        ParsedInequality* ineqPtr = std::get_if<ParsedInequality>(&result);
        if (ineqPtr != nullptr) {
            bool lhsHasY = Evaluator::containsVar(*ineqPtr->lhs, "y");
            bool rhsHasY = Evaluator::containsVar(*ineqPtr->rhs, "y");
            if (!lhsHasY && !rhsHasY) {
                return false;
            }
        }

        std::size_t oldColor = m_items[index]->color_index;
        m_items[index] = std::make_unique<PlotItem>(expression, std::move(result), oldColor);
        return true;
    } catch (const MathPlotError& err) {
        return false;
    }
}

void Pipeline::clear() {
    m_items.clear();
    nextColorIdx = 0;
}

std::vector<std::string> Pipeline::getExpressions() const {
    std::vector<std::string> result;
    for (std::size_t i = 0; i < m_items.size(); i++) {
        result.push_back(m_items[i]->expression);
    }
    return result;
}

void Pipeline::restoreFromExpressions(const std::vector<std::string>& exprs) {
    clear();
    for (std::size_t i = 0; i < exprs.size(); i++) {
        add(exprs[i]);
    }
}
