#include <gtest/gtest.h>
#include "app/pipeline.h"

TEST(PipelineTest, AddValid) {
    Pipeline p;
    EXPECT_TRUE(p.add("y = x^2"));
    EXPECT_EQ(p.size(), 1u);
    EXPECT_EQ(p.items()[0]->expression, "y = x^2");
}

TEST(PipelineTest, AddInvalid) {
    Pipeline p;
    EXPECT_FALSE(p.add(""));
    EXPECT_EQ(p.size(), 0u);
}

TEST(PipelineTest, AddBareExpressionFails) {
    Pipeline p;
    // without = must return 'false'
    EXPECT_FALSE(p.add("x^2"));
    EXPECT_EQ(p.size(), 0u);
}

TEST(PipelineTest, AddMultiple) {
    Pipeline p;
    EXPECT_TRUE(p.add("y = x"));
    EXPECT_TRUE(p.add("y = x^2"));
    EXPECT_TRUE(p.add("y = sin(x)"));
    EXPECT_EQ(p.size(), 3u);
}

TEST(PipelineTest, AddEquation) {
    Pipeline p;
    EXPECT_TRUE(p.add("x^2 + y^2 = 1"));
    EXPECT_EQ(p.size(), 1u);
}

TEST(PipelineTest, Remove) {
    Pipeline p;
    p.add("y = x");
    p.add("y = x^2");
    p.add("y = sin(x)");
    p.remove(1);
    EXPECT_EQ(p.size(), 2u);
    EXPECT_EQ(p.items()[0]->expression, "y = x");
    EXPECT_EQ(p.items()[1]->expression, "y = sin(x)");
}

TEST(PipelineTest, Toggle) {
    Pipeline p;
    p.add("y = x");
    EXPECT_TRUE(p.items()[0]->visible);
    p.toggle(0);
    EXPECT_FALSE(p.items()[0]->visible);
    p.toggle(0);
    EXPECT_TRUE(p.items()[0]->visible);
}

TEST(PipelineTest, Update) {
    Pipeline p;
    p.add("y = x");
    EXPECT_TRUE(p.update(0, "y = x^2"));
    EXPECT_EQ(p.items()[0]->expression, "y = x^2");
}

TEST(PipelineTest, UpdateInvalid) {
    Pipeline p;
    p.add("y = x");
    EXPECT_FALSE(p.update(0, ""));
    EXPECT_EQ(p.items()[0]->expression, "y = x");
}

TEST(PipelineTest, Clear) {
    Pipeline p;
    p.add("y = x");
    p.add("y = x^2");
    p.clear();
    EXPECT_EQ(p.size(), 0u);
    EXPECT_TRUE(p.empty());
}

TEST(PipelineTest, GetExpressions) {
    Pipeline p;
    p.add("y = x");
    p.add("y = sin(x)");
    std::vector<std::string> exprs = p.getExpressions();
    ASSERT_EQ(exprs.size(), 2u);
    EXPECT_EQ(exprs[0], "y = x");
    EXPECT_EQ(exprs[1], "y = sin(x)");
}

TEST(PipelineTest, Restore) {
    Pipeline p;
    p.add("y = x");
    p.add("y = x^2");
    std::vector<std::string> saved = p.getExpressions();
    p.clear();
    p.restoreFromExpressions(saved);
    EXPECT_EQ(p.size(), 2u);
    EXPECT_EQ(p.items()[0]->expression, "y = x");
}

TEST(PipelineTest, RejectBadInequality) {
    Pipeline p;
    // x < 1 does not contain y - must be rejected
    EXPECT_FALSE(p.add("x < 1"));
    EXPECT_EQ(p.size(), 0u);
}
