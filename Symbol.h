#pragma once

#include <string>
#include <variant>

namespace labbish {
	namespace mathExpr {
		using NumberType = double;
		using OperatorType = char;
		struct VariableType :public std::string {};
		struct FunctionType :public std::string {};

		struct Symbol :public std::variant<std::monostate, NumberType, OperatorType, VariableType, FunctionType> {
			Symbol() : std::variant<std::monostate, NumberType, OperatorType, VariableType, FunctionType>() {
				this->clear();
			}
			template <typename T>
			Symbol(T value) : std::variant<std::monostate, NumberType, OperatorType, VariableType, FunctionType>() {
				static_assert(std::disjunction_v<std::is_same<T, NumberType>, std::is_same<T, OperatorType>,
					std::is_same<T, VariableType>, std::is_same<T, FunctionType>>,
					"Type T must be one of the Symbol's valid types");
				this->emplace<T>(value);
			}
			template <typename T>
			constexpr const bool isType() const noexcept {
				return std::holds_alternative<T>(*this);
			}
			constexpr const bool isEmpty() const noexcept {
				return isType<std::monostate>();
			}
			constexpr void clear() noexcept {
				this->emplace<std::monostate>();
			}
			template <typename T>
			constexpr T& get() {
				static_assert(std::disjunction_v<std::is_same<T, NumberType>, std::is_same<T, OperatorType>,
					std::is_same<T, VariableType>, std::is_same<T, FunctionType>>,
					"Type T must be one of the Symbol's valid types");
				return std::get<T>(*this);
			}
			template <typename T>
			constexpr const T& get() const {
				static_assert(std::disjunction_v<std::is_same<T, NumberType>, std::is_same<T, OperatorType>,
					std::is_same<T, VariableType>, std::is_same<T, FunctionType>>,
					"Type T must be one of the Symbol's valid types");
				return std::get<T>(*this);
			}
		};
	}
}