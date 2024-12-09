#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <functional>
#include "Parser.h"

namespace labbish {
	namespace mathExpr {
		class Variables {
			std::map<VariableType, double> variables;
		public:
			friend class Calculate;
			Variables() {}
			void addVar(std::string var, double value) {
				variables.insert_or_assign(VariableType(var), value);
			}
		};
		class Calculate {
		private:
			//erase [start, end] and replace with new_value
			template <typename T>
			void combine_elements(StableVector<T>& vec, size_t start, size_t end, const T& new_value) {
				for (size_t i = start; i <= end; i++) vec.erase(start);
				vec.insert(start, new_value);
			} //TODO: make it faster by deleting duplicate addressing

			//sub-vector in [start, end]
			template <typename T>
			StableVector<T> subvec(const StableVector<T>& vec, size_t start, size_t end) {
				StableVector<T> ans{};
				for (size_t i = start; i <= end; i++) {
					ans.push_back(vec[i]);
				}
				return ans;
			} //TODO: make it faster by deleting duplicate addressing

			std::map<VariableType, double> variables;
			Calculate(std::map<VariableType, double> variables) :variables(variables) {
				functions = {
					{FunctionType("abs"), [](double x)->double {return abs(x); }}
				};
			}
		public:
			std::map<FunctionType, std::function<double(double)>> functions;

			Calculate(Variables vars = Variables()) :Calculate(vars.variables) {}

			std::optional<double> operator()(Expression expression) {
				StableVector<Symbol>& symbols = expression.symbols;
				while (true) {
					bool did = false;
					for (int i = 0; i < symbols.size(); i++) {
						if (symbols[i].isType<VariableType>()) {
							VariableType var = symbols[i].get<VariableType>();
							if (variables.find(var) == variables.end()) return std::nullopt;
							combine_elements(symbols, i, i, Symbol(variables[var]));
						}
					}
					for (int i = 0; i < symbols.size(); i++) {
						if (symbols[i].isType<OperatorType>()) {
							if (symbols[i].get<OperatorType>() == ')') {
								bool hasLeft = false;
								int j;
								for (j = i; j >= 0; j--) {
									if (symbols[j].isType<OperatorType>()) {
										if (symbols[j].get<OperatorType>() == '(') {
											hasLeft = true;
											break;
										}
									}
									else if (symbols[j].isType<FunctionType>()) {
										hasLeft = true;
										break;
									}
								}
								did = true;
								if (!hasLeft) return std::nullopt;
								std::optional<double> subAns = operator()(Expression(subvec(symbols, j + 1, i - 1)));
								if (subAns == std::nullopt) return std::nullopt;
								else {
									if (symbols[j].isType<OperatorType>()) combine_elements(symbols, j, i, Symbol(*subAns));
									else if (symbols[j].isType<FunctionType>()) {
										FunctionType func = symbols[j].get<FunctionType>();
										if (functions.find(func) == functions.end()) return std::nullopt;
										combine_elements(symbols, j, i, Symbol(functions[func](*subAns)));
									}
									break;
								}
							}
						}
					}
					if (!did) break;
				}
				while (true) {
					bool did = false;
					for (int i = symbols.size() - 1; i >= 0; i--) {
						if (0 < i && i < symbols.size() - 1) {
							if (symbols[i].isType<OperatorType>()) {
								if (symbols[i].get<OperatorType>() == '^' && symbols[i - 1].isType<NumberType>() && symbols[i + 1].isType<NumberType>()) {
									combine_elements(symbols, i - 1, i + 1, Symbol(pow(symbols[i - 1].get<NumberType>(), symbols[i + 1].get<NumberType>())));
									did = true;
									break;
								}
							}
						}
					}
					if (!did) break;
				}
				while (true) {
					bool did = false;
					for (int i = 0; i < symbols.size(); i++) {
						if (0 < i && i < symbols.size() - 1) {
							if (symbols[i].isType<OperatorType>()) {
								if (symbols[i].get<OperatorType>() == '*' && symbols[i - 1].isType<NumberType>() && symbols[i + 1].isType<NumberType>()) {
									combine_elements(symbols, i - 1, i + 1, Symbol(symbols[i - 1].get<NumberType>() * symbols[i + 1].get<NumberType>()));
									did = true;
									break;
								}
								else if (symbols[i].get<OperatorType>() == '/' && symbols[i - 1].isType<NumberType>() && symbols[i + 1].isType<NumberType>()) {
									combine_elements(symbols, i - 1, i + 1, Symbol(symbols[i - 1].get<NumberType>() / symbols[i + 1].get<NumberType>()));
									did = true;
									break;
								}
							}
						}
					}
					if (!did) break;
				}
				while (true) {
					bool did = false;
					for (int i = 0; i < symbols.size(); i++) {
						if (0 < i && i < symbols.size() - 1) {
							if (symbols[i].isType<OperatorType>()) {
								if (symbols[i].get<OperatorType>() == '+' && symbols[i - 1].isType<NumberType>() && symbols[i + 1].isType<NumberType>()) {
									combine_elements(symbols, i - 1, i + 1, Symbol(symbols[i - 1].get<NumberType>() + symbols[i + 1].get<NumberType>()));
									did = true;
									break;
								}
								else if (symbols[i].get<OperatorType>() == '-' && symbols[i - 1].isType<NumberType>() && symbols[i + 1].isType<NumberType>()) {
									combine_elements(symbols, i - 1, i + 1, Symbol(symbols[i - 1].get<NumberType>() - symbols[i + 1].get<NumberType>()));
									did = true;
									break;
								}
							}
						}
					}
					if (!did) break;
				}
				if (symbols.size() == 1) if (symbols[0].isType<NumberType>()) return symbols[0].get<NumberType>();
				return std::nullopt;
			}
		};
	}
}