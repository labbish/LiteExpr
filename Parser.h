#pragma once

#include "Symbol.h"
#include "StableVector.h"

namespace labbish {
	namespace mathExpr {
		struct Expression {
			StableVector<Symbol> symbols{};
			bool isOperator(const char& c) {
				for (const char& o : "+-*/^()") if (c == o) return true;
				return false;
			}
			bool isDigit(const char& c) {
				for (const char& d : "0123456789.") if (c == d) return true;
				return false;
			}
			bool isConnector(const char& c) {
				return c == '_';
			}
			bool isIdentifier(const char& c) {
				return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
			}
			double toNum(const char& c) { //No check
				return c - '0';
			}
			Expression(StableVector<Symbol> symbols) :symbols(symbols) {}
			Expression(std::string expression) {
				expression.erase(std::remove_if(expression.begin(), expression.end(), [](int c) {
					return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f' || c == '\v';
					}), expression.end());
				Symbol buffer;
				int decimal_count = 0;
				for (std::string::const_iterator it = expression.begin(); it != expression.end(); ++it) {
					if (!isDigit(*it)) decimal_count = 0;
					if (isOperator(*it)) {
						if (*it == '(' && buffer.isType<VariableType>()) {
							symbols.push_back(Symbol(FunctionType(buffer.get<VariableType>())));
							buffer.clear();
						}
						else {
							if (!buffer.isEmpty()) {
								symbols.push_back(buffer);
							}
							symbols.push_back(OperatorType(*it));
							buffer.clear();
						}
					}
					else if (isDigit(*it)) {
						if (!(buffer.isEmpty() || buffer.isType<NumberType>())) {
							symbols.push_back(buffer);
							buffer.clear();
						}
						if (!buffer.isType<NumberType>()) {
							buffer = Symbol(toNum(*it));
						}
						else {
							if (*it == '.') {
								decimal_count = 1;
							}
							else {
								if (decimal_count == 0) buffer = 10 * buffer.get<NumberType>() + toNum(*it);
								else {
									buffer = buffer.get<NumberType>() + toNum(*it) / pow(10, decimal_count);
									decimal_count++;
								}
							}
						}
					}
					else if (isConnector(*it)) {
						if (buffer.isType<NumberType>());
						else if (buffer.isType<VariableType>()) buffer = VariableType(buffer.get<VariableType>() + *it);
					}
					else if (isIdentifier(*it)) {
						if (!(buffer.isEmpty() || buffer.isType<VariableType>())) {
							symbols.push_back(buffer);
							buffer.clear();
						}
						if (!buffer.isType<VariableType>()) buffer = VariableType(std::string(1, *it));
						else buffer = VariableType(buffer.get<VariableType>() + *it);
					}
				}
				if (!buffer.isEmpty()) symbols.push_back(buffer);
			}
		};
	}
}