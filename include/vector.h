#pragma once
#include <array>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

namespace aline {

	template <typename T, std::size_t N>
	class Vector {
	private:
		std::array<T, N> tab;
	public:

		Vector()
        {
            tab = std::array<T, N>();
        }

		Vector(std::initializer_list<T> list){
            if(list.size() > N){
                throw std::runtime_error("ERROR CONSTRUCTOR");
            }
            std::size_t i = 0;
            tab = std::array<T, N>();
            for (const auto elt : list)
            {
                tab[i] = elt;
                ++i;
            }
        } 

		Vector(const Vector<T,N> & list){
            std::size_t i;
            tab = std::array<T, N>();
            for(i = 0; i < N;++i){
                tab[i] = list[i];
            }
        }

		T at(std::size_t index) const
        {
            if(index < N)
                return tab[index];
            else
                throw std::runtime_error("ERROR AT");
        }

		T operator[](std::size_t index) const
        {
            return tab[index];
        }
        T &operator[](size_t index) {
            if (index < N)
                return tab[index];
            else
                throw std::runtime_error("ERROR OPERATOR");
        }
        Vector<T, N> &operator+=(const Vector<T,N> & vec) {
            std::size_t i;
            for(i = 0; i < N;++i){
                tab[i] += vec[i];
            }
            return *this;
        }
        Vector<T, N> &operator=(const Vector<T,N> & vec) {
            std::size_t i;
            for(i = 0; i < N;++i){
                tab[i] = vec[i];
            }
            return *this;
        }
    };

	using uint = unsigned int;
	using real = double;
	using Vec2i = Vector<int, 2>;
	using Vec3i = Vector<int, 3>;
	using Vec2r = Vector<real, 2>;
	using Vec3r = Vector<real, 3>;
	using Vec4r = Vector<real, 4>;

	template <typename T, std::size_t N>
	Vector<T, N> cross(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		if (N < 3)
			throw std::runtime_error("ERROR CROSS");

		T u1 = v1[1] * v2[2] - v1[2] * v2[1];
		T u2 = v1[2] * v2[0] - v1[0] * v2[2];
		T u3 = v1[0] * v2[1] - v1[1] * v2[0];

		return Vector<T, N> { u1, u2, u3 };
	}

	template <class T, std::size_t N>
    T dot(const Vector<T, N> &vec1, const Vector<T, N> &vec2)
    {
        T calcul = T();
        std::size_t i;
        for(i = 0; i < N;++i){
            calcul += (vec1[i]*vec2[i]);
        }
        return calcul;
    }

    template <class T, std::size_t N>
    bool isnan(const Vector<T, N> &vec)
    {
        std::size_t i;
        for(i = 0; i<N;++i){
            if(std::isnan(vec[i])){
                return true;
            }
        }
        return false;
    }

    template <class T, std::size_t N>
    bool is_unit(const Vector<T, N> &vec)
    {
        return round(norm(vec)) == 1;
    }

	template <class T, std::size_t N>
    bool nearly_equal(const Vector<T, N> &vec1, const Vector<T, N> &vec2)
    {
        const T epsilon = std::numeric_limits<T>::epsilon();
        for (std::size_t i = 0; i < N; ++i)
        {
            T dif = std::abs(vec1[i] - vec2[i]);
            T max = std::max(vec1[i], vec2[i]);
            if (dif / max > epsilon)
                return false;
        }
        return true;
    }

    template <class T, std::size_t N>
    T norm(const Vector<T, N> &vec)
    {
        return sqrt(sq_norm(vec));
    }

    template <class T, std::size_t N>
    bool operator==(const Vector<T, N> &vec1, const Vector<T, N> &vec2)
    {
        std::size_t i;
        for(i =0;i<N;++i){
            if (vec1[i] != vec2[i])
                return false;
        }
        return true;
    }

    template <class T, std::size_t N>
    bool operator!=(const Vector<T, N> &v1, const Vector<T, N> &v2)
    {
        return !(v1 == v2);
    }

    template <class T, std::size_t N>
    std::ostream & operator<<(std::ostream &out, const Vector<T, N> &v)
    {
        out << to_string(v);
        return out;
    }

    template <class T, std::size_t N>
    Vector<T, N> operator+(const Vector<T, N> &vec1, const Vector<T, N> &vec2)
    {
        Vector<T,N> vec3 = Vector<T,N>();
        for (std::size_t i = 0; i < N; ++i)
        {
            vec3[i] = vec1[i] + vec2[i];
        }
        return vec3;
    }

    template <class T, std::size_t N>
    Vector<T, N> operator-(const Vector<T, N> &vec)
    {
        return vec*(T(-1));
    }

    template <class T, std::size_t N>
    Vector<T, N> operator-(const Vector<T, N> &vec1, const Vector<T, N> &vec2)
    {
        Vector<T, N> vec3 = Vector<T, N>();
        for (std::size_t i = 0; i < N; ++i)
        {
            vec3[i] = vec1[i] - vec2[i];
        }
        return vec3;
    }

    template <class T, std::size_t N>
    Vector<T, N> operator*(const T &scalar, const Vector<T, N> &vec1)
    {
        Vector<T, N> vec = Vector<T, N>();
        for (std::size_t i = 0; i < N; ++i)
        {
            vec[i] = vec1[i] * scalar;
        }
        return vec;
    }

    template <class T, std::size_t N>
    Vector<T, N> operator*(const Vector<T, N> &vec, const T &scalar)
    {
        return scalar * vec;
    }

    template <class T, std::size_t N>
    Vector<T, N> operator*(const Vector<T, N> &vec1, const Vector<T, N> &vec2)
    {
        Vector<T, N> vec3 = Vector<T, N>();
        for (std::size_t i = 0; i < N; ++i)
        {
            vec3[i] = vec1[i] * vec2[i];
        }
        return vec3;
    }

    template <class T, std::size_t N>
    Vector<T, N> operator/(const Vector<T, N> &v, const T &s)
    {
        return v*(1/s);
    }

	template <class T, std::size_t N>
    T sq_norm(const Vector<T, N> &vec)
    {
        std::size_t i;
        T calcul = T();
        for(i = 0;i<N;++i){
            calcul += vec[i]*vec[i];
        }
        return calcul;
    }

    template <class T, std::size_t N>
    std::string to_string(const Vector<T, N> &vec)
    {
        std::stringstream str;
        str << "(" << vec[0];
        for (std::size_t i = 1; i < N; ++i)
        {
            str << ", " << vec[i];
        }
        str << ")";
        return str.str();
    }

    template <class T, std::size_t N>
    Vector<T, N> unit_vector(const Vector<T, N> &v)
    {
        return (v/norm(v));
    }

	template <typename T, std::size_t N>
	T dist(const Vector<T, N>& v1, const Vector<T, N>& v2) {
		T result = T();
		for (std::size_t i = 0; i < N; ++i)
			result += (T)( std::pow(v1[i] - v2[i], 2) );
		return std::sqrt(result);
	}
}
