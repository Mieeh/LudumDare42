#pragma once

#include<iostream>
#include<math.h>

namespace bear { namespace core {

	template<typename T>
	struct Vector2 {
		T x;
		T y;

		Vector2(T a_x, T a_y) : x(a_x), y(a_y) { }
		Vector2() : x(0), y(0) { }

		T magnitude() {
			return std::sqrt(std::pow<T, int>(x, 2) + std::pow<T, int>(y, 2));
		}
		Vector2<T> normalize() {
			return Vector2<T>(x / this->magnitude(), y / this->magnitude());
		}

		static T distance(Vector2<T> &a_V1, Vector2<T> &a_V2) {
			return std::sqrt(std::pow<T, int>(a_V2.x - a_V1.x, 2) + std::pow<T, int>(a_V2.y - a_V1.y, 2));
		}

		// Operator overloading
		friend std::ostream & operator<<(std::ostream &os, const Vector2<T> &right) {
			os << "(" << right.x << ", " << right.y << ")";
			return os;
		}
		Vector2<T> operator-(const Vector2<T> right) {
			return Vector2<T>(this->x - right.x, this->y - right.y);
		}
		Vector2<T> operator+(const Vector2<T> right) {
			return Vector2<T>(this->x + right.x, this->y + right.y);
		}
		Vector2<T> operator*(const Vector2<T> right) {
			return Vector2<T>(this->x * right.x, this->y * right.y);
		}
		Vector2<T> operator*(const T right) {
			return Vector2<T>(this->x * right, this->y * right);
		}
		Vector2<T> operator/(const T right) {
			return Vector2<T>(this->x / right, this->y / right);
		}
		Vector2<T>& operator+=(const Vector2<T>& right) {
			this->x += right.x;
			this->y += right.y;
			return *this;
		}
		Vector2<T>& operator-=(const Vector2<T>& right) {
			this->x -= right.x;
			this->y -= right.y;
			return *this;
		}
		Vector2<T>& operator*=(const Vector2<T>& right) {
			this->x *= right.x;
			this->y *= right.y;
			return *this;
		}
	};
	
	// Define some default templates
	typedef Vector2<int> Vector2i;
	static bool operator<(const Vector2i& left, const Vector2i& right) {
		if (right.x > left.x)
			return true;
	}

	typedef Vector2<float> Vector2f;
	typedef Vector2<unsigned int> Vector2ui;
	typedef Vector2<double> Vector2d;
}
}