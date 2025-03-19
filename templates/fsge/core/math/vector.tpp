#pragma once

namespace fsge {

	template <typename T>
	Vector<T>::Vector() {
		m_m = nullptr;
		m_size = 0;
	}

	template <typename T>
	Vector<T>::Vector(unsigned char size) {
		if (size > 4) {
			m_m = new T[size - 4];
		} else {
			m_m = nullptr;
		}
		m_size = size;
	}

	template <typename T>
	void Vector<T>::copy(Vector<T> v) {
		if (m_size != v.m_size) {
			delete m_m;
			m_size = v.m_size;
			m_m = new T[m_size];
		}
		for (unsigned int i = 0; i < 4; i++) {
			m_b[i] = v[i];
		}
		for (unsigned int i = 4; i < m_size; i++) {
			m_m[i - 4] = v[i];
		}
	}

	template <typename T>
	Vector<T>::Vector(T x, T y) {
		m_m = nullptr;
		m_size = 2;
		m_b[0] = x;
		m_b[1] = y;
		m_b[2] = 0.0;
		m_b[3] = 0.0;
	}

	template <typename T>
	Vector<T>::Vector(T x, T y, T z) {
		m_m = nullptr;
		m_size = 3;
		m_b[0] = x;
		m_b[1] = y;
		m_b[2] = z;
		m_b[3] = 0.0;
	}
	template <typename T>
	Vector<T>::Vector(T x, T y, T z, T w) {
		m_m = nullptr;
		m_size = 4;
		m_b[0] = x;
		m_b[1] = y;
		m_b[2] = z;
		m_b[3] = w;
	}
	template <typename T>
	Vector<T>::~Vector() {
		delete m_m;
	}

	template <typename T>
	bool Vector<T>::is_empty() const {
		return m_size == 0;
	}

	template <typename T>
	T Vector<T>::dot(Vector<T> v) const {
		T x = m_b[0] * v[0];
		for (unsigned char i = 1; i < 4; i++) {
			x += m_b[i] * v[i];
		}
		unsigned char m = std::min(m_size, v.get_size());
		for (unsigned char i = 4; i < m ; i++) {
			x += m_m[i - 4]*v[i];
		}
		return x;
	}

	template <typename T>
	Vector<T> Vector<T>::product(Vector<T> v) const {
		if (m_size != 3 || m_size != v.get_size()) {
			// fsgeError_error(fsgeMatrixOperationSizeError, "Vector Product with other than 3-dim vectors", __FILE__, __LINE__);
		}
		Vector<T> n(m_size);
		n[0] = m_b[1] * v[2] - m_b[2] * v[1];
		n[1] = m_b[2] * v[0] - m_b[0] * v[2];
		n[2] = m_b[0] * v[1] - m_b[1] * v[0];
		return n;
	}

	template <typename T>
	void Vector<T>::scale(T k) {
		for (unsigned char i = 0; i < 4; i++) {
			m_b[i] *= k;
		}
		for (unsigned char i = 4; i < m_size; i++) {
			m_m[i - 4] *= k;
		}
	}

	template <typename T>
	void Vector<T>::display() const {
		std::cout << "[";
		for (unsigned char i = 0; i < std::min((unsigned char)4, m_size); i++) {
			std::cout << " " << m_b[i] << " ";
		}
		for (unsigned char i = 4; i < m_size; i++) {
			std::cout << " " << m_m[i-4] << " ";
		}
		std::cout << "]" << std::endl;
	}

	template <typename T>
	unsigned char Vector<T>::get_size() const {
		return m_size;
	}

	template <typename T>
	void Vector<T>::normalize() {
		float l = length();
		for (unsigned char i = 0; i < 4; i++) {
			m_b[i] /= l;
		}
		for (unsigned char i = 0; i + 4 < m_size; i++) {
			m_m[i] /= l;
		}
	}

	template <typename T>
	double Vector<T>::length() const {
		T x = 0.0;
		for (unsigned char i = 0; i < std::min((unsigned char)4, m_size); i++) {
			x += m_b[i]*m_b[i];
		}
		for (unsigned char i = 0; i + 4 < m_size; i++) {
			x += m_m[i]*m_m[i];
		}
		return sqrt(x);
	}

	template <typename T>
	T &Vector<T>::operator[](unsigned char i) {
		return (i < 4) ? m_b[i] : m_m[i - 4];
	}

	template <typename T>
	Vector<T> Vector<T>::operator+(Vector<T> v) {
		Vector<T> n(std::min(m_size, v.m_size));
		for (unsigned char i = 0; i < 4; i++) {
			n.m_b[i] = m_b[i] + v.m_b[i];
		}
		for (unsigned char i = 0; i + 4 < n.m_size; i++) {
			n.m_m[i] = m_m[i] + v.m_m[i];
		}
		return n;
	}

	template <typename T>
	void Vector<T>::operator+=(Vector<T> x) {
		unsigned char i;
		for (i = 0; i < 4; i++) {
			m_b[i] += x[i];
		}
		if (m_size > x.get_size()) {
			
			for (; i < x.get_size(); i++) {
				m_m[i - 4] += x[i - 4];
			}
			for (; i < m_size; i++) {
				m_m[i - 4] = 0.0;
			}
		} else {
			for (; i < x.get_size(); i++) {
				m_m[i-4] += x[i-4];
			}
		}
	}

	template <typename T>
	Vector<T> Vector<T>::operator+(T x) {
		Vector<T> n(m_size);
		for (unsigned char i = 0; i < 4; i++) {
			n.m_b[i] = m_b[i] + x;
		}
		for (unsigned char i = 0; i + 4 < n.m_size; i++) {
			n.m_m[i] = m_m[i] + x;
		}
		return n;
	}

	template <typename T>
	Vector<T> Vector<T>::operator-(T x) {
		Vector<T> n(m_size);
		for (unsigned char i = 0; i < 4; i++) {
			n.m_b[i] = m_b[i] - x;
		}
		for (unsigned char i = 0; i + 4 < n.m_size; i++) {
			n.m_m[i] = m_m[i] - x;
		}
		return n;
	}

	template <typename T>
	Vector<T> Vector<T>::operator-() const {
		Vector<T> n(m_size);
		for (unsigned char i = 0; i < 4; i++) {
			n.m_b[i] = -m_b[i];
		}
		for (unsigned char i = 0; i + 4 < n.m_size; i++) {
			n.m_m[i] = -m_m[i];
		}
		return n;
	}

	template <typename T>
	Vector<T> Vector<T>::operator-(Vector<T> v) {
		Vector<T> n(std::min(m_size, v.m_size));
		for (unsigned char i = 0; i < 4; i++) {
			n.m_b[i] = m_b[i] - v.m_b[i];
		}
		for (unsigned char i = 0; i + 4 < n.m_size; i++) {
			n.m_m[i] = m_m[i] - v.m_m[i];
		}
		return n;
	}

	template <typename T>
	void Vector<T>::operator-=(Vector<T> x) {
		unsigned char i;
		for (i = 0; i < 4; i++) {
			m_b[i] -= x[i];
		}
		if (m_size > x.get_size()) {
			
			for (; i < x.get_size(); i++) {
				m_m[i - 4] -= x[i - 4];
			}
			for (; i < m_size; i++) {
				m_m[i - 4] = 0.0;
			}
		} else {
			for (; i < x.get_size(); i++) {
				m_m[i-4] -= x[i-4];
			}
		}
	}
	template <typename T>
	void Vector<T>::operator-=(T x) {
		unsigned char i;
		for (i = 0; i < 4; i++) {
			m_b[i] -= x;
		}
		for (; i < m_size; i++) {
			m_m[i-4] -= x;
		}
	}

	template <typename T>
	Vector<T> Vector<T>::operator*(T x) {
		Vector<T> n(m_size);
		for (unsigned char i = 0; i < 4; i++) {
			n.m_b[i] = m_b[i] * x;
		}
		for (unsigned char i = 0; i + 4 < n.m_size; i++) {
			n.m_m[i] = m_m[i] * x;
		}
		return n;
	}

	template <typename T>
	Vector<T> Vector<T>::operator*(Vector<T> v) {
		Vector<T> n(std::min(m_size, v.m_size));
		for (unsigned char i = 0; i < 4; i++) {
			n.m_b[i] = m_b[i] * v.m_b[i];
		}
		for (unsigned char i = 0; i + 4 < n.m_size; i++) {
			n.m_m[i] = m_m[i] * v.m_m[i];
		}
		return n;
	}

	template <typename T>
	Vector<T> Vector<T>::operator/(T x) {
		Vector<T> n(m_size);
		for (unsigned char i = 0; i < 4; i++) {
			n.m_b[i] = m_b[i] / x;
		}
		for (unsigned char i = 0; i + 4 < n.m_size; i++) {
			n.m_m[i] = m_m[i] / x;
		}
		return n;
	}

	template <typename T>
	Vector<T> Vector<T>::operator/(Vector<T> v) {
		Vector<T> n(std::min(m_size, v.m_size));
		for (unsigned char i = 0; i < 4; i++) {
			n.m_b[i] = m_b[i] / v.m_b[i];
		}
		for (unsigned char i = 0; i + 4 < n.m_size; i++) {
			n.m_m[i] = m_m[i] / v.m_m[i];
		}
		return n;
	}

	template <typename T>
	void Vector<T>::operator*=(T x) {
		unsigned char i;
		for (i = 0; i < 4; i++) {
			m_b[i] *= x;
		}
		for (; i < m_size; i++) {
			m_m[i-4] *= x;
		}
	}

	template <typename T>
	void Vector<T>::operator*=(Vector<T> x) {
		unsigned char i;
		for (i = 0; i < 4; i++) {
			m_b[i] *= x[i];
		}
		if (m_size > x.get_size()) {
			
			for (; i < x.get_size(); i++) {
				m_m[i - 4] *= x[i - 4];
			}
			for (; i < m_size; i++) {
				m_m[i - 4] = 0.0;
			}
		} else {
			for (; i < x.get_size(); i++) {
				m_m[i-4] *= x[i-4];
			}
		}
	}

	template <typename T>
	void Vector<T>::operator/=(T x) {
		unsigned char i;
		for (i = 0; i < 4; i++) {
			m_b[i] /= x;
		}
		for (; i < m_size; i++) {
			m_m[i-4] /= x;
		}
	}

	template <typename T>
	void Vector<T>::operator/=(Vector<T> x) {
		unsigned char i;
		for (i = 0; i < 4; i++) {
			m_b[i] /= x[i];
		}
		if (m_size > x.m_size) {
			
			for (; i < x.m_size; i++) {
				m_m[i - 4] /= x[i - 4];
			}
			for (; i < m_size; i++) {
				m_m[i - 4] = 0.0;
			}
		} else {
			for (; i < x.m_size; i++) {
				m_m[i-4] /= x[i-4];
			}
		}
	}

	template <typename T>
	bool Vector<T>::operator!=(Vector<T> v) {
		return !((*this) == v);
	}

	template <typename T>
	bool Vector<T>::operator==(Vector<T> v) {
		if (v.m_size != m_size) {
			return false;
		}
		unsigned char i;
		for (i = 0; i < 4; i++) {
			if (m_b[i] != v[i]) {
				return false;
			}
		}
		for (; i < m_size; i++) {
			if (m_m[i-4] != v[i]) {
				return false;
			}
		}
		return true;
	}

	template <typename T>
	Vector<T> Vector<T>::operator&(Vector<T> v) {
		Vector<T> n(m_size + v.m_size);
		unsigned char i;
		for (i = 0; i < 4; i++) {
			n[i] = m_b[i];
		}
		for (; i < m_size; i++) {
			n[i] = m_m[i-4];
		}
		for (unsigned char i = 0; i < v.m_size; i++)
		{
			n[i + m_size] = v[i];
		}
		return n;
	}

	template <typename T>
	void Vector<T>::operator&=(Vector<T> x) {
		unsigned char n_size = m_size + x.m_size;
		unsigned char i;
		if (n_size > 4) {
			T *v = new T[n_size - 4];
			for (i = 0; i < 4; i++) {
				m_b[i] = x[i];
			}
			for (; i < m_size; i++) {
				v[i-4] = m_m[i-4];
			}
			for (i = 0; i < x.m_size; i++)
			{
				v[i + m_size-4] = x[i];
			}
			if (m_size > 4) {
				delete[] m_m;
			}
			m_m = v;
		} else {
			for (i = x.m_size; i < 4; i++) {
				m_b[i] = x[i - x.m_size];
			}
		}
		m_size = n_size;
	}

	template <typename T>
	Vector<T> Vector<T>::operator&(T x) {
		Vector<T> n(m_size + 1);
		unsigned char i;
		for (i = 0; i < 4; i++) {
			n[i] = m_b[i];
		}
		for (; i < m_size; i++) {
			n[i] = m_m[i-4];
		}
		n[m_size] = x;
		return n;
	}

	template <typename T>
	void Vector<T>::operator&=(T x) {
		if (m_size + 1 > 4) {
			T *v = new T[m_size + 1];
			for (unsigned char i = 4; i + 4 < m_size; i++) {
				v[i] = m_m[i];
			}
			v[m_size - 4] = x;
			if (m_size > 4) {
				delete[] m_m;
			}
			m_m = v;
		} else {
			m_b[m_size] = x;
		}
		m_size++;
	}
}