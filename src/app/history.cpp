#include "app/history.h"

History::History() {}

void History::push(const std::vector<std::string>& expressions) {
    std::unique_ptr<Snapshot> snap = std::make_unique<Snapshot>(expressions);
    undo_stack.push_back(std::move(snap));

    // новое действие сбрасывает redo
    redo_stack.clear();
}

std::optional<std::vector<std::string>> History::undo() {
    if (undo_stack.empty()) {
        return std::nullopt;
    }

    // берем последний snapshot из undo
    std::unique_ptr<Snapshot> snap = std::move(undo_stack.back());
    undo_stack.pop_back();

    // копируем expressions для возврата
    std::vector<std::string> result = snap->expressions;

    // перемещаем в redo stack
    redo_stack.push_back(std::move(snap));

    return result;
}

std::optional<std::vector<std::string>> History::redo() {
    if (redo_stack.empty()) {
        return std::nullopt;
    }

    std::unique_ptr<Snapshot> snap = std::move(redo_stack.back());
    redo_stack.pop_back();

    std::vector<std::string> result = snap->expressions;
    undo_stack.push_back(std::move(snap));

    return result;
}

bool History::canUndo() const {
    return undo_stack.size() > 0;
}

bool History::canRedo() const {
    return redo_stack.size() > 0;
}

void History::clear() {
    undo_stack.clear();
    redo_stack.clear();
}
