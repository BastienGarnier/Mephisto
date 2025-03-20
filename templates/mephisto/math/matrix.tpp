#pragma once

namespace mephisto {
	template <typename T>
	Matrix<T>::Matrix() {
		m_m = nullptr;
		m_glmat = nullptr;
	}
	
	template <typename T>
	bool Matrix<T>::is_empty() const {
		return m_m == nullptr;
	}
	
	template <typename T>
	Matrix<T>::~Matrix() {
		if (m_glmat != nullptr) {
			delete[] m_glmat;
		}
	}
	
	template <typename T>
	void Matrix<T>::display() const {
		for (unsigned char i = 0; i < m_l; i++) {
			std::cout << "[";
			for (unsigned char j = 0; j < m_c; j++) {
				std::cout << " " << m_m[i * m_c + j] << " ";
			}
			std::cout << "]" << std::endl;
		}
	}
	
	template <typename T>
	Matrix<T>::Matrix(unsigned char rows, unsigned char columns) {
		m_m = new T[rows * columns];
		for (unsigned short int i = 0; i < rows * columns; i++) {
			m_m[i] = 0.0;
		}
		m_l = rows;
		m_c = columns;
		m_glmat = nullptr;
	}
	
	template <typename T>
	Matrix<T>::Matrix(unsigned char s) {
		m_glmat = nullptr;
		m_m = new T[s * s];
		for (unsigned short int i = 0; i < s * s; i++) {
			m_m[i] = 0.0;
		}
		m_l = s;
		m_c = s;
	}
	
	template <typename T>
	Matrix<T>::Matrix(const Matrix<T> &m) {
		m_glmat = nullptr;
		m_l = m.m_l;
		m_c = m.m_c;
		m_m = new T[m_c * m_l];
		for (unsigned short int i = 0; i < m_l * m_c; i++) {
			m_m[i] = m.m_m[i];
		}
	}
	
	template <typename T>
	bool Matrix<T>::is_square() const {
		return m_l == m_c;
	}
	
	template <typename T>
	unsigned short int Matrix<T>::get_dim() const {
		return m_l * m_l;
	}
	
	template <typename T>
	unsigned char Matrix<T>::get_rows() const {
		return m_l;
	}
	
	template <typename T>
	unsigned char Matrix<T>::get_cols() const {
		return m_c;
	}

	template <typename T>
	float* Matrix<T>::get_glmatrix() {
		return m_m;
	}

	template <>
	inline float* Matrix<double>::get_glmatrix() {
		
		if (m_glmat == nullptr) {
			m_glmat = new float[m_l * m_c];
		}
		for (unsigned short int i = 0; i < m_l*m_c; i++) {
			m_glmat[i] = static_cast<float>(m_m[i]);
		}
		return m_glmat;
	}
	
	template <typename T>
	void Matrix<T>::scale(T k) {
		m_m[m_l*m_l - 1] /= k;
	}
	
	template <typename T>
	Matrix<T> Matrix<T>::scaling(T k) {
		Matrix m = Matrix<T>::identity(4);
		m(3, 3) = 1/k;
		return m;
	}
	
	template <typename T>
	void Matrix<T>::translate(Vector<T> v) {
		
		for (int i = 0; i < m_c - 1; i++) {
			m_m[m_c*(m_l - 1) + i] += v[i];
		}
	}

	template <typename T>
	void fswap(T *x, T *y) {
		if (x == y) {
			return;
		}
		T t = *x;
		*x = *y;
		*y = t;
	}
	
	template <typename T>
	void Matrix<T>::transpose() {
		for (unsigned char i = 0; i < m_l; i++) {
			for (unsigned char j = 0; j < (m_c >> 1); j++) {
				fswap(&m_m[m_c*i + j], &m_m[m_l*j + i]);
			}
		}
	}

	template <typename T>
	T* multiply_in(Matrix<T> a, Matrix<T> b) {
		T *t = new T[a.get_rows() * b.get_cols()];
		for (unsigned char i = 0; i < a.get_rows(); i++) {
			for (unsigned char j = 0; j < b.get_cols(); j++) {
				T x = 0.0;
				for (unsigned char k = 0; k < a.get_cols(); k++) {
					x += a(i, k)*b(k, j);
				}
				t[i * b.get_cols() + j] = x;
			}
		}
		return t;
	}
	
	template <typename T>
	void Matrix<T>::multiply(Matrix<T> m) {
		if (m_c != m.get_rows()) {
			// mephistoError_error(mephistoMatrixOperationSizeError, "Multiply", __FILE__, __LINE__);
		}
		T* t = multiply_in(*this, m);
		delete[] m_m;
		m_m = t;
		m_c = m.m_c;
	}
	
	template <typename T>
	void Matrix<T>::add(Matrix<T> m) {
		if (m_c != m.get_cols() || m_l != m.get_rows()) {
			// mephistoError_error(mephistoMatrixOperationSizeError, "Add In", __FILE__, __LINE__);
		}

		for (unsigned short int i = 0; i < m_l * m_c; i++) {
			m_m[i] += m.m_m[i];
		}
	}
	
	template <typename T>
	void Matrix<T>::pow(uint32_t n) {
		
	}
	
	template <typename T>
	Matrix<T> Matrix<T>::rotation(T angle, Vector<T> axis) {
		Matrix<T> m(4);
		T c = cos(angle);
		T s = sin(angle);
		m.m_m[0] = c + axis[0]*axis[0]*(1 - c);
		m.m_m[1] = axis[0]*axis[1]*(1 - c) - axis[2]*s;
		m.m_m[2] = axis[2]*axis[0]*(1 - c) + axis[1]*s;

		m.m_m[4] = axis[1]*axis[0]*(1 - c) + axis[3]*s;
		m.m_m[5] = c + axis[1]*axis[1]*(1 - c);
		m.m_m[6] = axis[1]*axis[2]*(1 - c) - axis[0]*s;

		m.m_m[8] = axis[2]*axis[0]*(1 - c) - axis[1]*s;
		m.m_m[9] = axis[2]*axis[1]*(1 - c) + axis[0]*s;
		m.m_m[10] = c + axis[2]*axis[2]*(1 - c);
		m.m_m[15] = 1;
		return m;
	}
	
	template <typename T>
	Matrix<T> Matrix<T>::translation(Vector<T> v) {
		if (v.get_size() != 3) {
			// mephistoError_error(mephistoMatrixOperationSizeError, "Translation", __FILE__, __LINE__);
		}

		Matrix<T> m(4);
		m(0, 0) = 1;
		m(1, 1) = 1;
		m(2, 2) = 1;
		m(3, 3) = 1;
		m(3, 0) = v[0];
		m(3, 1) = v[1];
		m(3, 2) = v[2];
		return m;
	}
	
	template <typename T>
	Matrix<T> Matrix<T>::identity(unsigned char n) {
		Matrix<T> m(n);
		for (unsigned char i = 0; i < n; i++) {
			m(i, i) = 1;
		}
		return m;
	}
	
	template <typename T>
	Matrix<T> Matrix<T>::perspective_projection(T fovY, T aspect_ratio, T n) {
		T angle = tan(-fovY/2);

		T r = n * angle;
		T t = r / aspect_ratio;

		Matrix<T> m(4);
		m(0, 0) = n/r;
		m(1, 1) = n/t;
		m(2, 2) = -1;
		m(3, 2) = -2*n;
		m(2, 3) = -1;
		return m;
	}
	
	template <typename T>
	T Matrix<T>::get(unsigned char i, unsigned char j) const {
		return m_m[i * m_c + j];
	}
	
	template <typename T>
	void Matrix<T>::set(unsigned char i, unsigned char j, T x) {
		m_m[i * m_c + j] = x;
	}
}