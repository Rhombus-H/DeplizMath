#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

struct Snapshot {
    std::vector<std::string> expressions;
    explicit Snapshot(std::vector<std::string> exprs)
        : expressions(std::move(exprs)) {}
};

class History {
public:
    History();

    void push(const std::vector<std::string>& expressions);
    std::optional<std::vector<std::string>> undo();
    std::optional<std::vector<std::string>> redo();

    bool canUndo() const;
    bool canRedo() const;
    void clear();

private:
    std::vector<std::unique_ptr<Snapshot>> undo_stack;
    std::vector<std::unique_ptr<Snapshot>> redo_stack;
};