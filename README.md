# DeplizMath — C++23 Graphing Calculator


## Features

- *Expression parsing*: Tokenizer, recursive-descent parser, AST evaluation
- *14 math functions*: sin, cos, tan, asin, acos, atan, log, ln, sqrt, abs, exp, ceil, floor, sign
- *Configurable viewport*: Set bounds via input fields; zoom (mouse wheel) and pan (mouse drag) via ImPlot
- *Multiple graphs*: Add/remove/toggle functions with automatic color assignment
- *Discontinuity handling*: NaN-safe evaluation; automatic line-break at jumps (1/x, tan(x), etc.)
- *Inequality shading*: Supports y > f(x), y < f(x), y >= f(x), y <= f(x), and two-variable forms like x^2+y^2 < 9
- *Undo/Redo*: Full history of expression list changes
- *PNG export*: Screenshot via SFML texture capture

## C++ Features 
`std::unique_ptr`:  AST ownership, Pipeline owns PlotItems, App owns Plotter 

 `std::shared_ptr`  Viewport shared between App, Plotter, GridRenderer

 `std::optional` Tokenizer::peek(), findFunction(), findConstant(), Exporter::save(), History::undo/redo 

`std::variant` + `std::visit` ExprVariant (AST nodes), ParseResult, Evaluator dispatch 

Exception hierarchy 9 classes, 3 levels

`constexpr` PI, E, BUILTIN_FUNCTIONS, findFunction/Constant, ColorPalette, NumericalDerivative::H

# User Guide

### Input Format

All expressions are entered in the format **"something = something"** or **"something > something"**

For inequalities, using y is required (x<1 is not supported)

It is not recommended to write equalities with y on the right side (x = y)

### Control Panel

- *Input field* — enter an expression and press Enter or click the "Add" button
- *Function list* — colored square, visibility checkbox, text, X button to delete
- *Undo/Redo* — undo and redo actions
- *Clear All* — remove all plots
- *Viewport* — manual input of coordinate plane bounds (X Min, X Max, Y Min, Y Max)
- *Reset View* — reset to default view (-10..10 on both axes)
- *Export PNG* — save a screenshot of the window

### Plot Window

- *Zoom* — mouse scroll wheel
- *Pan* — hold right mouse button and drag
- *Axis scaling* — hold and drag along an axis

### Supported Functions and Constants

Sin `y = sin(x)`

Cos `y = cos(x)`

Tan `y = tan(x)`

Arcsin (domain: [-1, 1]) `y = asin(x)`

Arccosin (domain: [-1, 1]) `y = acos(x)`

Arctan `y = atan(x)`

Base-10 logarithm `y = log(x)`

Natural logarithm `y = ln(x)`

Square root `y = sqrt(x)`

Absolute value `y = abs(x)`

Exponential (e^x) `y = exp(x)`

Ceiling `y = ceil(x)`

Floor `y = floor(x)`

Sign (-1, 0 or 1) `y = sign(x)`

Functions can be nested: `y = sin(cos(x))`, `y = sqrt(abs(x))`.

#### Constants:
`pi`: `y = sin(pi * x)`

`e`: `y = e^x`