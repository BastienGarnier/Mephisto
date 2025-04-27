#pragma once
#ifndef _MEPHISTO_MATRIX_H_INCLUDED
#define _MEPHISTO_MATRIX_H_INCLUDED

#include <iostream>
#include <math.h>

#include <mephisto/math/vector.hpp>
#include <mephisto/math/utils.hpp>

namespace mephisto {
	template <typename T>
	class Matrix
	{
	private:
		T *m_m;
		float *m_glmat;
		unsigned char m_l; // nb lignes
		unsigned char m_c; // nb colonnes
	public:
		Matrix();
		Matrix(unsigned char l, unsigned char c);
		Matrix(unsigned char s);
		Matrix(const Matrix<T> &m);
		~Matrix();

		static Matrix identity(unsigned char n);

		float* get_glmatrix();

		void display() const;
		void scale(T k);
		void translate(Vector<T> v);
		void transpose();
		void multiply(Matrix<T> m);
		void pow(uint32_t n);
		void add(Matrix<T> m);

		bool is_empty() const;
		bool is_square() const;

		unsigned char get_rows() const;
		unsigned char get_cols() const;
		unsigned short int get_dim() const;

		T & operator()(unsigned char i, unsigned char j) {
			return m_m[i * m_c + j];
		}

		T get(unsigned char i, unsigned char j) const;
		void set(unsigned char i, unsigned char j, T x);

		// Some  3D matrix
		static Matrix rotation(T angle, Vector<T> axis);
		static Matrix translation(Vector<T> v);
		static Matrix scaling(T k);
		
		static Matrix perspective_projection(T fov, T aspect_ratio, T near);

		friend Matrix operator*(const Matrix<T> &a, const Matrix<T> &b) {
			Matrix result(a.get_rows(), b.get_cols());
			for (unsigned char i = 0; i < a.get_rows(); i++) {
				for (unsigned char j = 0; j < b.get_cols(); j++) {
					result.m_m[i * b.get_cols() + j] = 0.0;
					for (unsigned char k = 0; k < a.get_cols(); k++) {
						result.m_m[i * b.get_cols() + j] += a.get(i, k)*b.get(k, j);
					}
				}
			}
			return result;
		}

		friend Vector<T> operator*(Matrix<T> a, Vector<T> b) {
			Vector<T> v(b.get_size());

			for (unsigned char i = 0; i < b.get_size(); i++) {
				v[i] = 0.0;
				for (unsigned char j = 0; j < b.get_size(); j++) {
					v[i] += b[j]*a.get(i, j);
				}
			}
			return v;
		}

		friend Vector<T> operator*(Vector<T> b, Matrix<T> a) {
			Vector<T> v(b.get_size());

			for (unsigned char i = 0; i < b.get_size(); i++) {
				v[i] = 0.0;
				for (unsigned char j = 0; j < b.get_size(); j++) {
					v[i] += b[j]*a.get(j, i);
				}
			}
			return v;
		}

	};
	
}

#include <mephisto/math/matrix.tpp>

#endif