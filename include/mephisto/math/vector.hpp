#pragma once
#ifndef _MEPHISTO_VECTOR_H_INCLUDED
#define _MEPHISTO_VECTOR_H_INCLUDED

#include <iostream>
#include <math.h>

#include <mephisto/math/utils.hpp>

namespace mephisto {

	// typedef int v4sf __attribute__ ((mode(V4SF))); // vector of four single floats

	template <typename T>
	class Vector
	{
	private:
		T m_b[4]; // for little vectors -> faster : allow not to copy and reallocate all the time
		T *m_m; // for big vectors
		unsigned char m_size;
	public:
		Vector();
		Vector(unsigned char size);
		Vector(T x, T y);
		Vector(T x, T y, T z);
		Vector(T x, T y, T z, T w);
		// Vector(Vector<T> &v);
		~Vector();

		void copy(Vector<T> v);

		T dot(Vector<T> v) const;

		Vector<T> product(Vector<T> v) const;

		void scale(T k);

		void display() const;

		unsigned char get_size() const;

		bool is_empty() const;

		void normalize();
		double length() const;

		T &operator[](unsigned char i);

		Vector<T> operator+(T v);
		void operator+=(T v);
		Vector<T> operator+(Vector<T> v);
		void operator+=(Vector<T> v);

		Vector<T> operator*(T x);
		void operator*=(T x);
		Vector<T> operator*(Vector<T> x);
		void operator*=(Vector<T> x);

		Vector<T> operator-(T x);
		Vector<T> operator-() const;
		void operator-=(T v);
		Vector<T> operator-(Vector<T> v);
		void operator-=(Vector<T> v);

		Vector<T> operator/(T x);
		void operator/=(T x);
		Vector<T> operator/(Vector<T> x);
		void operator/=(Vector<T> x);

		bool operator!=(Vector<T> v);
		bool operator==(Vector<T> v);


		void operator&=(Vector<T> x);
		void operator&=(T x);
		Vector<T> operator&(Vector<T> v);
		Vector<T> operator&(T x);
	
		template <typename T2>
		operator Vector<T2>() const {
			Vector<T2> n(m_size);
			for (unsigned char i = 0; i < m_size; i++) {
				n[i] = (T2)m_m[i];
			}
			return n;
		}

	};
}

#include <mephisto/math/vector.tpp>

#endif