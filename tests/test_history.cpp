#include <gtest/gtest.h>
#include "app/history.h"

TEST(HistoryTest, InitiallyEmpty) {
    History h;
    EXPECT_FALSE(h.canUndo());
    EXPECT_FALSE(h.canRedo());
}

TEST(HistoryTest, PushEnablesUndo) {
    History h;
    h.push({"x"});
    EXPECT_TRUE(h.canUndo());
    EXPECT_FALSE(h.canRedo());
}

TEST(HistoryTest, UndoReturnsPrevState) {
    History h;
    h.push({"x"});
    h.push({"x", "x^2"});

    std::optional<std::vector<std::string>> result = h.undo();
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2u);
    EXPECT_EQ((*result)[0], "x");
    EXPECT_EQ((*result)[1], "x^2");
}

TEST(HistoryTest, RedoAfterUndo) {
    History h;
    h.push({"x"});
    h.push({"x", "x^2"});
    h.undo();
    EXPECT_TRUE(h.canRedo());

    std::optional<std::vector<std::string>> result = h.redo();
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2u);
}

TEST(HistoryTest, UndoEmptyReturnsNullopt) {
    History h;
    std::optional<std::vector<std::string>> result = h.undo();
    EXPECT_FALSE(result.has_value());
}

TEST(HistoryTest, RedoEmptyReturnsNullopt) {
    History h;
    std::optional<std::vector<std::string>> result = h.redo();
    EXPECT_FALSE(result.has_value());
}

TEST(HistoryTest, PushClearsRedo) {
    History h;
    h.push({"x"});
    h.push({"x", "x^2"});
    h.undo();
    EXPECT_TRUE(h.canRedo());
    h.push({"sin(x)"});
    EXPECT_FALSE(h.canRedo());
}

TEST(HistoryTest, ClearRemovesAll) {
    History h;
    h.push({"x"});
    h.push({"x^2"});
    h.clear();
    EXPECT_FALSE(h.canUndo());
    EXPECT_FALSE(h.canRedo());
}

TEST(HistoryTest, MultipleUndos) {
    History h;
    h.push({"a"});
    h.push({"b"});
    h.push({"c"});

    std::optional<std::vector<std::string>> r1 = h.undo();
    ASSERT_TRUE(r1.has_value());
    EXPECT_EQ(r1->at(0), "c");

    std::optional<std::vector<std::string>> r2 = h.undo();
    ASSERT_TRUE(r2.has_value());
    EXPECT_EQ(r2->at(0), "b");

    std::optional<std::vector<std::string>> r3 = h.undo();
    ASSERT_TRUE(r3.has_value());
    EXPECT_EQ(r3->at(0), "a");

    EXPECT_FALSE(h.canUndo());
}
