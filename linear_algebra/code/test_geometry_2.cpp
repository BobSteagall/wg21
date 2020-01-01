#include "linear_algebra.hpp"
#include <cassert>

namespace STD_LA {
USING_STD

class float2_engine
{
public:
	using engine_category = mutable_vector_engine_tag;
	using element_type    = float;
	using value_type      = float;
	using reference       = float&;
	using pointer         = float*;
	using const_reference = float const&;
	using const_pointer   = float const*;
	using iterator        = detail::vector_iterator<float2_engine>;
	using const_iterator  = detail::vector_const_iterator<float2_engine>;
	using difference_type = ptrdiff_t;
	using size_type      = int_fast32_t;
	using size_type       = int_fast32_t;

	using is_fixed_size   = true_type;
	using is_resizable    = false_type;

	using is_column_major = true_type;
	using is_dense        = true_type;
	using is_rectangular  = true_type;
	using is_row_major    = true_type;

public:
	constexpr float2_engine();
	template<class U>
	constexpr float2_engine(std::initializer_list<U> list);
	constexpr float2_engine(float, float) noexcept;

	constexpr float2_engine(float2_engine&&) noexcept = default;
	constexpr float2_engine(float2_engine const&) = default;

	constexpr float2_engine& operator =(float2_engine&&) noexcept = default;
	constexpr float2_engine& operator =(float2_engine const&) = default;

	constexpr const_reference   operator ()(size_type i) const;
	constexpr const_iterator    begin() const noexcept;
	constexpr const_iterator    end() const noexcept;

	constexpr size_type     capacity() const noexcept;
	constexpr size_type    elements() const noexcept;
	constexpr size_type     size() const noexcept;

	constexpr reference     operator ()(size_type i);
	constexpr iterator      begin() noexcept;
	constexpr iterator      end() noexcept;

	constexpr void  swap(float2_engine& rhs) noexcept;
	constexpr void  swap_elements(size_type i, size_type j) noexcept;

private:
	float   ma_elems[2];
};

template<class U>
constexpr
float2_engine::float2_engine(initializer_list<U> list)
{
	assert(list.size() == 2);
	auto    iter = list.begin();
	ma_elems[0] = *iter++;
	ma_elems[1] = *iter;
}

constexpr typename float2_engine::const_reference
float2_engine::operator ()(size_type i) const
{
	assert(i < 2 && i >= 0);
	return ma_elems[i];
}

class float22_engine
{
public:
	using engine_category = mutable_matrix_engine_tag;
	using element_type    = float;
	using value_type      = float;
	using reference       = float&;
	using pointer         = float*;
	using const_reference = float const&;
	using const_pointer   = float const*;
	using difference_type = ptrdiff_t;
	using size_type       = int_fast32_t;
	using size_type       = int_fast32_t;
	using size_tuple      = std::tuple<size_type, size_type>;

	using is_fixed_size   = true_type;
	using is_resizable    = false_type;

	using is_column_major = false_type;
	using is_dense        = true_type;
	using is_rectangular  = true_type;
	using is_row_major    = true_type;

	using column_view_type    = matrix_column_view<float22_engine>;
	using row_view_type       = matrix_row_view<float22_engine>;
	using transpose_view_type = matrix_transpose_view<float22_engine>;

public:
	constexpr float22_engine();
	template<class U>
	constexpr float22_engine(std::initializer_list<U> list);
	constexpr float22_engine(float22_engine&&) noexcept = default;
	constexpr float22_engine(float22_engine const&) = default;

	constexpr float22_engine& operator =(float22_engine&&) noexcept = default;
	constexpr float22_engine& operator =(float22_engine const&) = default;

	constexpr const_reference   operator ()(size_type i, size_type j) const;

	constexpr size_type    columns() const noexcept;
	constexpr size_type    rows() const noexcept;
	constexpr size_tuple    size() const noexcept;

	constexpr size_type     column_capacity() const noexcept;
	constexpr size_type     row_capacity() const noexcept;
	constexpr size_tuple    capacity() const noexcept;

	constexpr reference     operator ()(size_type i, size_type j);

	constexpr void      assign(float22_engine const& rhs);
	template<class ET2>
	constexpr void      assign(ET2 const& rhs);

	constexpr void      swap(float22_engine& rhs) noexcept;
	constexpr void      swap_columns(size_type j1, size_type j2) noexcept;
	constexpr void      swap_rows(size_type i1, size_type i2) noexcept;

private:
	float   ma_elems[4];
};

template<class U>
constexpr
float22_engine::float22_engine(initializer_list<U> list)
{
	assert(list.size() == 4);
	auto    iter = list.begin();
	ma_elems[0] = *iter++;
	ma_elems[1] = *iter++;
	ma_elems[2] = *iter++;
	ma_elems[3] = *iter;
}

constexpr typename float22_engine::const_reference
float22_engine::operator ()(size_type i, size_type j) const
{
	assert(i < 2 && i >= 0);
	assert(j < 2 && j >= 0);
	return ma_elems[(i * 2) + j];
}

// Addition - vector

template<>
struct matrix_addition_engine_traits<matrix_operation_traits, float2_engine, float2_engine>
{
	using element_type_1 = float;
	using element_type_2 = float;
	using element_type   = float;
	using engine_type    = float2_engine;
};

template<>
auto
matrix_addition_traits<matrix_operation_traits, vector<float2_engine>, vector<float2_engine>>::add
(vector<float2_engine> const& v1, vector<float2_engine> const& v2) -> result_type
{
	PrintOperandTypes<result_type>("float2 addition_traits", v1, v2);

	return result_type{ v1(0) + v2(0), v1(1) + v2(1) };
}

// Addition - matrix

template<>
struct matrix_addition_engine_traits<matrix_operation_traits, float22_engine, float22_engine>
{
	using element_type_1 = float;
	using element_type_2 = float;
	using element_type   = float;
	using engine_type    = float22_engine;
};

template<>
auto
matrix_addition_traits<matrix_operation_traits, matrix<float22_engine>, matrix<float22_engine>>::add
(matrix<float22_engine> const& m1, matrix<float22_engine> const& m2) -> result_type
{
	PrintOperandTypes<result_type>("float22 addition_traits", m1, m2);

	return result_type{ m1(0, 0) + m2(0, 0), m1(0, 1) + m2(0, 1), m1(1, 0) + m2(1, 0), m1(1, 1) + m2(1, 1) };
}

// Subtraction - vector

template<>
struct matrix_subtraction_engine_traits<matrix_operation_traits, float2_engine, float2_engine>
{
	using element_type_1 = float;
	using element_type_2 = float;
	using element_type   = float;
	using engine_type    = float2_engine;
};

template<>
auto
matrix_subtraction_traits<matrix_operation_traits, vector<float2_engine>, vector<float2_engine>>::subtract
(vector<float2_engine> const& v1, vector<float2_engine> const& v2) -> result_type
{
	PrintOperandTypes<result_type>("float2 subtraction_traits", v1, v2);

	return result_type{ v1(0) - v2(0), v1(1) - v2(1) };
}

// Subtraction - matrix

template<>
struct matrix_subtraction_engine_traits<matrix_operation_traits, float22_engine, float22_engine>
{
	using element_type_1 = float;
	using element_type_2 = float;
	using element_type   = float;
	using engine_type    = float22_engine;
};

template<>
auto
matrix_subtraction_traits<matrix_operation_traits, matrix<float22_engine>, matrix<float22_engine>>::subtract
(matrix<float22_engine> const& m1, matrix<float22_engine> const& m2) -> result_type
{
	PrintOperandTypes<result_type>("float22 subtraction_traits", m1, m2);

	return result_type{ m1(0, 0) - m2(0, 0), m1(0, 1) - m2(0, 1), m1(1, 0) - m2(1, 0), m1(1, 1) - m2(1, 1) };
}

// Negation - vector

template<>
struct matrix_negation_engine_traits<matrix_operation_traits, float2_engine>
{
	using element_type = float;
	using engine_type  = float2_engine;
};

template<>
auto
matrix_negation_traits<matrix_operation_traits, vector<float2_engine>>::negate
(vector<float2_engine> const& v) -> result_type
{
	PrintOperandTypes<result_type>("float2 negation_traits", v);

	return result_type{ -v(0), -v(1) };
}

// Negation - matrix

template<>
struct matrix_negation_engine_traits<matrix_operation_traits, float22_engine>
{
	using element_type = float;
	using engine_type  = float22_engine;
};

template<>
auto
matrix_negation_traits<matrix_operation_traits, matrix<float22_engine>>::negate
(matrix<float22_engine> const& m) -> result_type
{
	PrintOperandTypes<result_type>("float22 negation_traits", m);

	return result_type{ -m(0, 0), -m(0, 1), -m(1, 0), -m(1, 1) };
}

// Multiplication - vector * scalar

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float2_engine, detail::element_tag<float>>
{
	using element_type = float;
	using engine_type  = float2_engine;
};

template<>
auto
matrix_multiplication_traits<matrix_operation_traits, vector<float2_engine>, float>::multiply
(vector<float2_engine> const& v, float const& s) -> result_type
{
	PrintOperandTypes<result_type>("float2 multiplication_traits (v*s)", v, s);

	return result_type{ v(0) * s, v(1) * s };
}

// Multiplication - matrix * scalar

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float22_engine, detail::element_tag<float>>
{
	using element_type = float;
	using engine_type  = float22_engine;
};

template<>
auto
matrix_multiplication_traits<matrix_operation_traits, matrix<float22_engine>, float>::multiply
(matrix<float22_engine> const& m, float const& s) -> result_type
{
	PrintOperandTypes<result_type>("float22 multiplication_traits (m*s)", m, s);

	return result_type{ m(0, 0) * s, m(0, 1) * s, m(1, 0) * s, m(1, 1) * s };
}

// Multiplication - scalar * vector

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, detail::element_tag<float>, float2_engine>
{
	using element_type = float;
	using engine_type  = float2_engine;
};

template<>
auto
matrix_multiplication_traits<matrix_operation_traits, float, vector<float2_engine>>::multiply
(float const& s, vector<float2_engine> const& v) -> result_type
{
	PrintOperandTypes<result_type>("float2 multiplication_traits (s*v)", s, v);

	return v * s;
}

// Multiplication - scalar * matrix

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, detail::element_tag<float>, float22_engine>
{
	using element_type = float;
	using engine_type  = float22_engine;
};

template<>
auto
matrix_multiplication_traits<matrix_operation_traits, float, matrix<float22_engine>>::multiply
(float const& s, matrix<float22_engine> const& m) -> result_type
{
	PrintOperandTypes<result_type>("float22 multiplication_traits (s*m)", s, m);

	return m * s;
}

// Multiplication - vector * vector

template<>
auto
matrix_multiplication_traits<matrix_operation_traits, vector<float2_engine>, vector<float2_engine>>::multiply
(vector<float2_engine> const& v1, vector<float2_engine> const& v2) -> result_type
{
	PrintOperandTypes<result_type>("float2 multiplication_traits (v*v)", v1, v2);

	return result_type{ (v1(0) * v2(0)) + (v1(1) * v2(1)) };
}

// Multiplication - vector * matrix

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float2_engine, float22_engine>
{
	using element_type = float;
	using engine_type  = float2_engine;
};

template<>
auto
matrix_multiplication_traits<matrix_operation_traits, vector<float2_engine>, matrix<float22_engine>>::multiply
(vector<float2_engine> const& v, matrix<float22_engine> const& m) -> result_type
{
	PrintOperandTypes<result_type>("float22 multiplication_traits (v*m)", v, m);

	return result_type{ (v(0) * m(0, 0)) + (v(1) * m(1, 0)), (v(0) * m(0, 1)) + (v(1) * m(1, 1)) };
}

// Multiplication - matrix * vector

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float22_engine, float2_engine>
{
	using element_type = float;
	using engine_type  = float2_engine;
};

template<>
auto
matrix_multiplication_traits<matrix_operation_traits, matrix<float22_engine>, vector<float2_engine>>::multiply
(matrix<float22_engine> const& m, vector<float2_engine> const& v) -> result_type
{
	PrintOperandTypes<result_type>("float22 multiplication_traits (m*v)", m, v);

	return result_type{ (m(0, 0) * v(0)) + (m(0, 1) * v(1)), (m(1, 0) * v(0)) + (m(1, 1) * v(1)) };
}

// Multiplication - matrix * matrix

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float22_engine, float22_engine>
{
	using element_type = float;
	using engine_type  = float22_engine;
};

template<>
auto
matrix_multiplication_traits<matrix_operation_traits, matrix<float22_engine>, matrix<float22_engine>>::multiply
(matrix<float22_engine> const& m1, matrix<float22_engine> const& m2) -> result_type
{
	PrintOperandTypes<result_type>("float22 multiplication_traits (m*m)", m1, m2);

	return result_type{ (m1(0, 0) * m2(0, 0)) + (m1(0, 1) * m2(1, 0)), (m1(0, 0) * m2(0, 1)) + (m1(0, 1) * m2(1, 1)),
		                (m1(1, 0) * m2(0, 0)) + (m1(1, 1) * m2(1, 0)), (m1(1, 0) * m2(0, 1)) + (m1(1, 1) * m2(1, 1)) };
}

} // STD_LA

STD_LA::vector<STD_LA::float2_engine> v1{ 0.0f, 1.1f };
STD_LA::vector<STD_LA::float2_engine> v2{ 2.2f, 3.3f };
STD_LA::matrix<STD_LA::float22_engine> m1{ 4.4f, 5.5f, 6.6f, 7.7f };
STD_LA::matrix<STD_LA::float22_engine> m2{ 8.8f, 9.9f, 10.1f, 11.11f };

void
TestGroup501()
{
	PRINT_FNAME();


	auto v3 = v1 + v2;
	auto m3 = m1 + m2;
	auto v4 = v1 - v2;
	auto m4 = m1 - m2;
	auto v5 = -v1;
	auto m5 = -m1;
	auto v6 = v1 * 2.0f;
	auto m6 = m1 * 3.0f;
	auto v7 = 4.0f * v2;
	auto m7 = 5.0f * m2;
	auto v8 = v1 * v2;
	auto v9 = v1 * m1;
	auto v10 = m2 * v2;
	auto m8 = m1 * m2;
}

void TestGroup502()
{
	std::cout << v1 * v2 << "\n";
}

void
TestGroup50()
{
	TestGroup501();
	TestGroup502();
}
