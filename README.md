# LiteExpr
A very simple math expression parsing lib.

## Function
Parse & calculate math expressions in string.

Now supports:
* numbers (support <code>.</code> as decimal point and <code>_</code> as connection)
* operator: <code>+</code>, <code>-</code>, <code>*</code>, <code>/</code>, <code>^</code>
* parentheses: <code>(</code> and <code>)</code>
* variables (can use letters and underline as names; case sensitive)
* univariate functions:
  * <code>abs</code>, <code>floor</code>, <code>sqr</code>, <code>sqrt</code>
  * <code>exp</code>, <code>ln</code>, <code>log</code>
  * <code>sin</code>, <code>cos</code>, <code>tan</code>, <code>csc</code>, <code>sec</code>, <code>cot</code>
  * <code>arcsin</code>, <code>arccos</code>, <code>arctan</code>, <code>arccsc</code>, <code>arcsec</code>, <code>arccot</code>
  * <code>sinh</code>, <code>cosh</code>, <code>tanh</code>
  * <code>arcsinh</code>, <code>arccosh</code>, <code>arctanh</code>


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
