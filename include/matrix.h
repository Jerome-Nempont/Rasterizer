#pragma once
#include "vector.h"

namespace aline {
	template <typename T, std::size_t M, std::size_t N>
    class Matrix
    {
    private:
        Vector<Vector<T,N>,M> matrix;
	public:
		Matrix() 
		{
			matrix = Vector<Vector<T,N>,M>();
		}

		Matrix(std::initializer_list<Vector<T, N>> list)
        {
            if (list.size() > M)
            {
                throw std::runtime_error("ERROR CONSTRUCTOR MATRIX");
            }
            std::size_t i = 0;
            matrix = Vector<Vector<T,N>,M>();
            for (const auto elt : list)
            {
                matrix[i] = elt;
                ++i;
            }
        }

		Matrix(const Matrix<T, M, N> & list){
            std::size_t i;
            matrix = Vector<Vector<T, N>,M>();
            for (i = 0; i < N; ++i)
            {
                matrix[i] = list[i];
            }
        }

		Vector<T,N> at(std::size_t index){
            if (index < M)
                return matrix[index];
            else
                throw std::runtime_error("ERROR AT MATRIX");
        }
        T at(std::size_t i, std::size_t j){
            if (i < N && j < M)
                return matrix[i][j];
            else
                throw std::runtime_error("ERROR AT MATRIX2");
        }

		Vector<T, N> operator[](std::size_t i) const {
            return matrix[i];
        }
        Vector<T, N> &operator[](std::size_t i)
        {
            if (i < M)
                return matrix[i];
            else
                throw std::runtime_error("ERROR AT MATRIX3");
        }

        Matrix<T, M, N> &operator+=(const Matrix<T, M, N> & vec) {
            std::size_t i;
            for (i = 0; i < N; ++i)
            {
                matrix[i] += vec[i];
            }
            return *this;
        }

        Matrix<T, M, N> &operator=(const Matrix<T, M, N> & vec) {
            std::size_t i;
            for (i = 0; i < N; ++i)
            {
                matrix[i] = vec[i];
            }
            return *this;
        }    
    };

	using Mat44r = Matrix<real, 4, 4>;

	template <typename T, std::size_t M, std::size_t N>
    bool nearly_equal(const Matrix<T, M, N> &mat1, const Matrix<T, M, N> &mat2)
    {
        std::size_t i;
        for (i = 0; i < M; ++i)
        {
            if (!nearly_equal(mat1[i],mat2[i]))
            {
                return false;
            }
        }
        return true;
    }

	template <typename T, std::size_t M, std::size_t N>
    Matrix<T, M, N> inverse(const Matrix<T, M, N> &matrix)
    {
        if (M != N)
        {
            return (matrix / 0);
        }
        Matrix<T, M, N> matrix_cpy = Matrix<T, M, N>(matrix);
        Matrix<T, M, N> matrix_id = Matrix<T, M, N>();

        for (std::size_t i = 0; i < M; ++i)
        {
            matrix_id[i][i] = 1;
        }
        T max;
        std::size_t pos;
        for (std::size_t j = 0; j < N; ++j)
        {
            max = matrix_cpy[j][j];
            for (std::size_t i = j; i < M; ++i)
            {
                if (abs(matrix_cpy[i][j]) >= abs(max))
                {
                    max = matrix_cpy[i][j];
                    pos = i;
                }
            }
            if(max == 0){
                return (matrix/0);
            }
            if(pos != j){
                swap(matrix_cpy[pos],matrix_cpy[j]);
                swap(matrix_id[pos],matrix_id[j]);
            }
            matrix_cpy[j] = matrix_cpy[j]/matrix_cpy[j][j];
            matrix_id[j] = matrix_id[j]/matrix_cpy[j][j];

            for(std::size_t r = 0;r < M;++r){
                if(r != j){
                    matrix_cpy[r] = matrix_cpy[r] + matrix_cpy[j] * (-matrix_cpy[r][j]);
                    matrix_id[r] = matrix_id[r] + matrix_id[j] * (-matrix_cpy[r][j]);
                }
            }
        }
        return matrix_id;
    }

	template <typename T, std::size_t M, std::size_t N>
    bool isnan(const Matrix<T, M, N> &matrix)
    {
        std::size_t i;
        for (i = 0; i < M; ++i)
        {
            if (isnan(matrix[i]))
            {
                return true;
            }
        }
        return false;
    }

    template <typename T, std::size_t M, std::size_t N>
    bool operator==(const Matrix<T, M, N> &mat1, const Matrix<T, M, N> &mat2)
    {
        std::size_t i;
        for (i = 0; i < M; ++i)
        {
            if (mat1[i] != mat2[i])
            {
                return false;
            }
        }
        return true;
    }

    template <typename T, std::size_t M, std::size_t N>
    bool operator!=(const Matrix<T, M, N> &mat1, const Matrix<T, M, N> &mat2)
    {
        return !(mat1 == mat2);
    }

    template <typename T, std::size_t M, std::size_t N>
    std::ostream &operator<<(std::ostream &out, const Matrix<T, M, N> &mat)
    {
        out << to_string(mat);
        return out;
    }

    template <typename T, std::size_t M, std::size_t N>
    Matrix<T, M, N> operator+(const Matrix<T, M, N> &mat1, const Matrix<T, M, N> &mat2)
    {
        Matrix<T,M,N> mat3 = Matrix<T,M,N>();
        std::size_t i;
        for(i = 0; i < M; ++i){
            mat3[i] = mat1[i] + mat2[i];
        }
    return mat3;
    }

	template <typename T, std::size_t M, std::size_t N>
    Matrix<T, M, N> operator-(const Matrix<T, M, N> &mat)
    {
        return mat*(T(-1));
    }

    template <typename T, std::size_t M, std::size_t N>
    Matrix<T, M, N> operator-(const Matrix<T, M, N> &mat1, const Matrix<T, M, N> &mat2)
    {
        Matrix<T, M, N> mat3 = Matrix<T, M, N>();
        std::size_t i;
        for (i = 0; i < M; ++i)
        {
            mat3[i] = mat1[i] - mat2[i];
        }
        return mat3;
    }

    template <typename T, std::size_t M, std::size_t N>
    Matrix<T, M, N> operator*(const T &scalar, const Matrix<T, M, N> &mat1)
    {
        Matrix<T, M, N> mat = Matrix<T, M, N>();
        std::size_t i;
        for (i = 0; i < M; ++i)
        {
            mat[i] = mat1[i] * scalar;
        }
        return mat;
    }

    template <typename T, std::size_t M, std::size_t N>
    Matrix<T, M, N> operator*(const Matrix<T, M, N> &mat1, const T &scalar)
    {
        return scalar * mat1;
    }

    template <typename T, std::size_t M, std::size_t N>
    Vector<T, M> operator*(const Matrix<T, M, N> &mat, const Vector<T, N> &vec1)
    {
        Vector<T, M> vec = Vector<T, M>();
        std::size_t i;
        for(i = 0; i < M;++i){
            T tmp = T();
            for(std::size_t j = 0; j <N; ++j){
                tmp += mat[i][j] * vec1[j];
            }
            vec[i] = tmp;
        }
        return vec;
    }

    template <typename T, std::size_t M, std::size_t N,std::size_t O>
    Matrix<T, M, O> operator*(const Matrix<T, M, N> &mat1, const Matrix<T, N, O> &mat2)
    {
        Matrix<T, M, O> mat = Matrix<T, M, O>();
        for(std::size_t i = 0; i < M;++i){
            for(std::size_t j = 0; j < O; ++j){
                T tmp = T();
                for (std::size_t x = 0; x < N; ++x){
                    tmp += mat1[i][x] * mat2[x][j];
                }
                mat[i][j] = tmp;
            }
        }
        return mat;
    }

    template <typename T, std::size_t M, std::size_t N>
    Matrix<T, M, N> operator/(const Matrix<T, M, N> &m, const T &s)
    {
        return m*(1/s);
    }

    template <typename T, std::size_t M, std::size_t N>
    std::string to_string(const Matrix<T, M, N> &mat)
    {
        std::stringstream str;
        str << "(" << mat[0];
        for (std::size_t i = 1; i < M; ++i)
        {
            str << ", " << mat[i];
        }
        str << ")";
        return str.str();
    }

    template <typename T, std::size_t M, std::size_t N>
    Matrix<T, N, M> transpose(const Matrix<T, M, N> &mat1)
    {
        Matrix<T, N, M> mat = Matrix<T, N, M>();
        std::size_t i;
        std::size_t j;
        for(i = 0; i < M; ++i){
            for(j = 0; j < N; ++j){
                mat[j][i] = mat1[i][j];
            }
        }
        return mat;
    }
}