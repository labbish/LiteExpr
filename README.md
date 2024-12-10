# LiteExpr
A very simple math expression parsing lib.

## Speed
Averagely, it's 90% faster than [exprtk](https://github.com/ArashPartow/exprtk).

## Function
Parse & calculate math expressions in string.

Now supports:
* numbers (support `.` as decimal point and `_` as connection)
* operator: `+`, `-`, `*`, `/`, `^`
* parentheses: `(` and `)`
* variables (can use letters and underline as names; case sensitive)
* univariate functions:
  * `abs`, `floor`, `sqr`, `sqrt`
  * `exp`, `ln`, `log`
  * `sin`, `cos`, `tan`, `csc`, `sec`, `cot`
  * `arcsin`, `arccos`, `arctan`, `arccsc`, `arcsec`, `arccot`
  * `sinh`, `cosh`, `tanh`
  * `arcsinh`, `arccosh`, `arctanh`

## Requirements
* Language Standard: C++20
* Supporting lib: [labbish/StableVector](https://github.com/labbish/StableVector)

## Usage
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

```cpp
#include <cstdio>
#include "Calculator.h"
using namespace labbish::mathExpr;
int main(){
	for (double x = 0; x < 1; x += 0.1) {
		Expression expr("sqr(sin(x))+sqr(cos(x))");
		Variables vars;
		vars.addVar("x", x);
		Calculate calculate(vars);
		std::optional<double> ans = calculate(expr);
		if (ans != std::nullopt) printf("sqr(sin(%lf))+sqr(cos(%lf))=%lf\n", x, x, *ans);
		else printf("null\n");
	}
	return 0;
}
```
