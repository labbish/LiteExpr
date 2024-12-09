# LiteExpr
A very simple math expression parsing lib.

## Function
Parse & calculate math expressions in string.

Now supports:
* numbers (support . as decimal point and _ as connection)
* operator: +, -, *, /, ^
* parentheses: ( and )
* variables (can use letters and underline as names; case sensitive)
* univariate functions: abs (will soon be more!)

## Usage
Needs supporting lib: [labbish/StableVector](https://github.com/labbish/StableVector).

Demo:

```cpp
#include <cstdio>
#include "Calculator.h"
using namespace labbish::mathExpr;
int main(){
	Expression expr("abs(1+(2+4)/3-6)*5^x");
	Variables vars;
	vars.addVar("x", 1);
	Calculate calculate(vars);
	std::optional<double> ans = calculate(expr);
	if (ans != std::nullopt) printf("%lf\n", *ans);
	else printf("null\n");
	return 0;
}
```
