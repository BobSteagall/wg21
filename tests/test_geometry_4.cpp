namespace STD_LA {
USING_STD

class float4_engine
{
public:
	using engine_category = writable_vector_engine_tag;
	using element_type    = float;
	using value_type      = float;
	using reference       = float&;
	using pointer         = float*;
	using const_reference = float const&;
	using const_pointer   = float const*;
	using iterator        = detail::vector_iterator<float4_engine>;
	using const_iterator  = detail::vector_const_iterator<float4_engine>;
	using difference_type = size_t;
	using index_type       = int_fast32_t;

	static constexpr bool   is_fixed_size   = true;
	static constexpr bool   is_resizable    = false;

	static constexpr bool   is_column_major = true;
	static constexpr bool   is_dense        = true;
	static constexpr bool   is_rectangular  = true;
	static constexpr bool   is_row_major    = true;

public:
	constexpr float4_engine();
	template<class U>
	constexpr float4_engine(std::initializer_list<U> list);
	constexpr float4_engine(float, float) noexcept;

	constexpr float4_engine(float4_engine&&) noexcept = default;
	constexpr float4_engine(float4_engine const&) = default;

	constexpr float4_engine& operator =(float4_engine&&) noexcept = default;
	constexpr float4_engine& operator =(float4_engine const&) = default;

	constexpr const_reference   operator ()(index_type i) const;
	constexpr const_iterator    begin() const noexcept;
	constexpr const_iterator    end() const noexcept;

	constexpr index_type     capacity() const noexcept;
	constexpr index_type     size() const noexcept;
	constexpr index_type     size() const noexcept;

	constexpr reference     operator ()(index_type i);
	constexpr iterator      begin() noexcept;
	constexpr iterator      end() noexcept;

	constexpr void  swap(float4_engine& rhs) noexcept;
	constexpr void  swap_elements(index_type i, index_type j) noexcept;

private:
	float   ma_elems[4];
};

template<class U>
constexpr
float4_engine::float4_engine(initializer_list<U> list)
{
	assert(list.size() == 4);
	auto    iter = list.begin();
	ma_elems[0] = *iter++;
	ma_elems[1] = *iter++;
	ma_elems[2] = *iter++;
	ma_elems[3] = *iter;
}

constexpr typename float4_engine::const_reference
float4_engine::operator ()(index_type i) const
{
	assert(i < 4 && i >= 0);
	return ma_elems[i];
}

class float44_engine
{
public:
	using engine_category = writable_matrix_engine_tag;
	using element_type    = float;
	using value_type      = float;
	using reference       = float&;
	using pointer         = float*;
	using const_reference = float const&;
	using const_pointer   = float const*;
	using difference_type = size_t;
	using index_type       = int_fast32_t;
	using index_tuple      = std::tuple<index_type, index_type>;

	static constexpr bool   is_fixed_size   = true;
	static constexpr bool   is_resizable    = false;

	static constexpr bool   is_column_major = false;
	static constexpr bool   is_dense        = true;
	static constexpr bool   is_rectangular  = true;
	static constexpr bool   is_row_major    = true;

	using column_view_type    = matrix_column_engine<float44_engine>;
	using row_view_type       = matrix_row_engine<float44_engine>;
	using transpose_view_type = matrix_transpose_engine<float44_engine>;

public:
	constexpr float44_engine();
	template<class U>
	constexpr float44_engine(std::initializer_list<U> list);
	constexpr float44_engine(float44_engine&&) noexcept = default;
	constexpr float44_engine(float44_engine const&) = default;

	constexpr float44_engine& operator =(float44_engine&&) noexcept = default;
	constexpr float44_engine& operator =(float44_engine const&) = default;

	constexpr const_reference   operator ()(index_type i, index_type j) const;

	constexpr index_type     columns() const noexcept;
	constexpr index_type     rows() const noexcept;
	constexpr index_tuple    size() const noexcept;

	constexpr index_type     column_capacity() const noexcept;
	constexpr index_type     row_capacity() const noexcept;
	constexpr index_tuple    capacity() const noexcept;

	constexpr reference     operator ()(index_type i, index_type j);

	constexpr void      assign(float44_engine const& rhs);
	template<class ET2>
	constexpr void      assign(ET2 const& rhs);

	constexpr void      swap(float44_engine& rhs) noexcept;
	constexpr void      swap_columns(index_type j1, index_type j2) noexcept;
	constexpr void      swap_rows(index_type i1, index_type i2) noexcept;

private:
	float   ma_elems[16];
};

template<class U>
constexpr
float44_engine::float44_engine(initializer_list<U> list)
{
	assert(list.size() == 16);
	auto    iter = list.begin();
	ma_elems[0] = *iter++;
	ma_elems[1] = *iter++;
	ma_elems[2] = *iter++;
	ma_elems[3] = *iter++;
	ma_elems[4] = *iter++;
	ma_elems[5] = *iter++;
	ma_elems[6] = *iter++;
	ma_elems[7] = *iter++;
	ma_elems[8] = *iter++;
	ma_elems[9] = *iter++;
	ma_elems[10] = *iter++;
	ma_elems[11] = *iter++;
	ma_elems[12] = *iter++;
	ma_elems[13] = *iter++;
	ma_elems[14] = *iter++;
	ma_elems[15] = *iter;
}

constexpr typename float44_engine::const_reference
float44_engine::operator ()(index_type i, index_type j) const
{
	assert(i < 4 && i >= 0);
	assert(j < 4 && j >= 0);
	return ma_elems[(i * 4) + j];
}

// Addition - vector

template<>
struct matrix_addition_engine_traits<matrix_operation_traits, float4_engine, float4_engine>
{
	using element_type_1 = float;
	using element_type_2 = float;
	using element_type   = float;
	using engine_type    = float4_engine;
};

template<>
auto
matrix_addition_arithmetic_traits<matrix_operation_traits, vector<float4_engine>, vector<float4_engine>>::add
(vector<float4_engine> const& v1, vector<float4_engine> const& v2) -> result_type
{
	PRINT_OP_TYPES(result_type, "float4 addition_arithmetic_traits", v1, v2);

	return result_type{ v1(0) + v2(0), v1(1) + v2(1), v1(2) + v2(2), v1(3) + v2(3) };
}

// Addition - matrix

template<>
struct matrix_addition_engine_traits<matrix_operation_traits, float44_engine, float44_engine>
{
	using element_type_1 = float;
	using element_type_2 = float;
	using element_type   = float;
	using engine_type    = float44_engine;
};

template<>
auto
matrix_addition_arithmetic_traits<matrix_operation_traits, matrix<float44_engine>, matrix<float44_engine>>::add
(matrix<float44_engine> const& m1, matrix<float44_engine> const& m2) -> result_type
{
	PRINT_OP_TYPES(result_type, "float44 addition_arithmetic_traits", m1, m2);

	return result_type{ m1(0, 0) + m2(0, 0), m1(0, 1) + m2(0, 1), m1(0, 2) + m2(0, 2), m1(0, 3) + m2(0, 3),
						m1(1, 0) + m2(1, 0), m1(1, 1) + m2(1, 1), m1(1, 2) + m2(1, 2), m1(1, 3) + m2(1, 3),
						m1(2, 0) + m2(2, 0), m1(2, 1) + m2(2, 1), m1(2, 2) + m2(2, 2), m1(2, 3) + m2(2, 3),
						m1(3, 0) + m2(3, 0), m1(3, 1) + m2(3, 1), m1(3, 2) + m2(3, 2), m1(3, 3) + m2(3, 3) };
}

// Subtraction - vector

template<>
struct matrix_subtraction_engine_traits<matrix_operation_traits, float4_engine, float4_engine>
{
	using element_type_1 = float;
	using element_type_2 = float;
	using element_type   = float;
	using engine_type    = float4_engine;
};

template<>
auto
matrix_subtraction_arithmetic_traits<matrix_operation_traits, vector<float4_engine>, vector<float4_engine>>::subtract
(vector<float4_engine> const& v1, vector<float4_engine> const& v2) -> result_type
{
	PRINT_OP_TYPES(result_type, "float4 subtraction_arithmetic_traits", v1, v2);

	return result_type{ v1(0) - v2(0), v1(1) - v2(1), v1(2) - v2(2), v1(3) - v2(3) };
}

// Subtraction - matrix

template<>
struct matrix_subtraction_engine_traits<matrix_operation_traits, float44_engine, float44_engine>
{
	using element_type_1 = float;
	using element_type_2 = float;
	using element_type   = float;
	using engine_type    = float44_engine;
};

template<>
auto
matrix_subtraction_arithmetic_traits<matrix_operation_traits, matrix<float44_engine>, matrix<float44_engine>>::subtract
(matrix<float44_engine> const& m1, matrix<float44_engine> const& m2) -> result_type
{
	PRINT_OP_TYPES(result_type, "float44 subtraction_arithmetic_traits", m1, m2);

	return result_type{ m1(0, 0) - m2(0, 0), m1(0, 1) - m2(0, 1), m1(0, 2) - m2(0, 2), m1(0, 3) - m2(0, 3),
						m1(1, 0) - m2(1, 0), m1(1, 1) - m2(1, 1), m1(1, 2) - m2(1, 2), m1(1, 3) - m2(1, 3),
						m1(2, 0) - m2(2, 0), m1(2, 1) - m2(2, 1), m1(2, 2) - m2(2, 2), m1(2, 3) - m2(2, 3),
						m1(3, 0) - m2(3, 0), m1(3, 1) - m2(3, 1), m1(3, 2) - m2(3, 2), m1(3, 3) - m2(3, 3) };
}

// Negation - vector

template<>
struct matrix_negation_engine_traits<matrix_operation_traits, float4_engine>
{
	using element_type = float;
	using engine_type  = float4_engine;
};

template<>
auto
matrix_negation_arithmetic_traits<matrix_operation_traits, vector<float4_engine>>::negate
(vector<float4_engine> const& v) -> result_type
{
	PRINT_OP_TYPES(result_type, "float4 negation_arithmetic_traits", v);

	return result_type{ -v(0), -v(1), -v(2), -v(3) };
}

// Negation - matrix

template<>
struct matrix_negation_engine_traits<matrix_operation_traits, float44_engine>
{
	using element_type = float;
	using engine_type  = float44_engine;
};

template<>
auto
matrix_negation_arithmetic_traits<matrix_operation_traits, matrix<float44_engine>>::negate
(matrix<float44_engine> const& m) -> result_type
{
	PRINT_OP_TYPES(result_type, "float44 negation_arithmetic_traits", m);

	return result_type{ -m(0, 0), -m(0, 1), -m(0, 2), -m(0, 3),
						-m(1, 0), -m(1, 1), -m(1, 2), -m(1, 3),
						-m(2, 0), -m(2, 1), -m(2, 2), -m(2, 3),
						-m(3, 0), -m(3, 1), -m(3, 2), -m(3, 3) };
}

// Multiplication - vector * scalar

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float4_engine, scalar_engine<float>>
{
	using element_type = float;
	using engine_type  = float4_engine;
};

template<>
auto
matrix_multiplication_arithmetic_traits<matrix_operation_traits, vector<float4_engine>, float>::multiply
(vector<float4_engine> const& v, float const& s) -> result_type
{
	PRINT_OP_TYPES(result_type, "float4 multiplication_arithmetic_traits (v*s)", v, s);

	return result_type{ v(0) * s, v(1) * s, v(2) * s, v(3) * s };
}

// Multiplication - matrix * scalar

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float44_engine, scalar_engine<float>>
{
	using element_type = float;
	using engine_type  = float44_engine;
};

template<>
auto
matrix_multiplication_arithmetic_traits<matrix_operation_traits, matrix<float44_engine>, float>::multiply
(matrix<float44_engine> const& m, float const& s) -> result_type
{
	PRINT_OP_TYPES(result_type, "float44 multiplication_arithmetic_traits (m*s)", m, s);

	return result_type{ m(0, 0) * s, m(0, 1) * s, m(0, 2) * s, m(0, 3) * s,
						m(1, 0) * s, m(1, 1) * s, m(1, 2) * s, m(1, 3) * s,
						m(2, 0) * s, m(2, 1) * s, m(2, 2) * s, m(2, 3) * s,
						m(3, 0) * s, m(3, 1) * s, m(3, 2) * s, m(3, 3) * s };
}

// Multiplication - scalar * vector

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, scalar_engine<float>, float4_engine>
{
	using element_type = float;
	using engine_type  = float4_engine;
};

template<>
auto
matrix_multiplication_arithmetic_traits<matrix_operation_traits, float, vector<float4_engine>>::multiply
(float const& s, vector<float4_engine> const& v) -> result_type
{
	PRINT_OP_TYPES(result_type, "float4 multiplication_arithmetic_traits (s*v)", s, v);

	return v * s;
}

// Multiplication - scalar * matrix

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, scalar_engine<float>, float44_engine>
{
	using element_type = float;
	using engine_type  = float44_engine;
};

template<>
auto
matrix_multiplication_arithmetic_traits<matrix_operation_traits, float, matrix<float44_engine>>::multiply
(float const& s, matrix<float44_engine> const& m) -> result_type
{
	PRINT_OP_TYPES(result_type, "float44 multiplication_arithmetic_traits (s*m)", s, m);

	return m * s;
}

// Multiplication - vector * vector

template<>
auto
matrix_multiplication_arithmetic_traits<matrix_operation_traits, vector<float4_engine>, vector<float4_engine>>::multiply
(vector<float4_engine> const& v1, vector<float4_engine> const& v2) -> result_type
{
	PRINT_OP_TYPES(result_type, "float4 multiplication_arithmetic_traits (v*v)", v1, v2);

	return result_type{ (v1(0) * v2(0)) + (v1(1) * v2(1)) + (v1(2) * v2(2)) + (v1(3) * v2(3)) };
}

// Multiplication - vector * matrix

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float4_engine, float44_engine>
{
	using element_type = float;
	using engine_type  = float4_engine;
};

template<>
auto
matrix_multiplication_arithmetic_traits<matrix_operation_traits, vector<float4_engine>, matrix<float44_engine>>::multiply
(vector<float4_engine> const& v, matrix<float44_engine> const& m) -> result_type
{
	PRINT_OP_TYPES(result_type, "float44 multiplication_arithmetic_traits (v*m)", v, m);

	return result_type{ (v(0) * m(0, 0)) + (v(1) * m(1, 0)) + (v(2) * m(2, 0)) + (v(3) * m(3, 0)),
						(v(0) * m(0, 1)) + (v(1) * m(1, 1)) + (v(2) * m(2, 1)) + (v(3) * m(3, 1)),
						(v(0) * m(0, 2)) + (v(1) * m(1, 2)) + (v(2) * m(2, 2)) + (v(3) * m(3, 2)),
						(v(0) * m(0, 3)) + (v(1) * m(1, 3)) + (v(2) * m(2, 3)) + (v(3) * m(3, 3)) };
}

// Multiplication - matrix * vector

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float44_engine, float4_engine>
{
	using element_type = float;
	using engine_type  = float4_engine;
};

template<>
auto
matrix_multiplication_arithmetic_traits<matrix_operation_traits, matrix<float44_engine>, vector<float4_engine>>::multiply
(matrix<float44_engine> const& m, vector<float4_engine> const& v) -> result_type
{
	PRINT_OP_TYPES(result_type, "float44 multiplication_arithmetic_traits (m*v)", m, v);

	return result_type{ (m(0, 0) * v(0)) + (m(0, 1) * v(1)) + (m(0, 2) * v(2)) + (m(0, 3) * v(3)),
						(m(1, 0) * v(0)) + (m(1, 1) * v(1)) + (m(1, 2) * v(2)) + (m(1, 3) * v(3)),
						(m(2, 0) * v(0)) + (m(2, 1) * v(1)) + (m(2, 2) * v(2)) + (m(2, 3) * v(3)),
						(m(3, 0) * v(0)) + (m(3, 1) * v(1)) + (m(3, 2) * v(2)) + (m(3, 3) * v(3)) };
}

// Multiplication - matrix * matrix

template<>
struct matrix_multiplication_engine_traits<matrix_operation_traits, float44_engine, float44_engine>
{
	using element_type = float;
	using engine_type  = float44_engine;
};

template<>
auto
matrix_multiplication_arithmetic_traits<matrix_operation_traits, matrix<float44_engine>, matrix<float44_engine>>::multiply
(matrix<float44_engine> const& m1, matrix<float44_engine> const& m2) -> result_type
{
	PRINT_OP_TYPES(result_type, "float22 multiplication_arithmetic_traits (m*m)", m1, m2);

	return result_type{ (m1(0, 0) * m2(0, 0)) + (m1(0, 1) * m2(1, 0)) + (m1(0, 2) * m2(2, 0)) + (m1(0, 3) * m2(3, 0)), (m1(0, 0) * m2(0, 1)) + (m1(0, 1) * m2(1, 1)) + (m1(0, 2) * m2(2, 1)) + (m1(0, 3) * m2(3, 1)), (m1(0, 0) * m2(0, 2)) + (m1(0, 1) * m2(1, 2)) + (m1(0, 2) * m2(2, 2)) + (m1(0, 3) * m2(3, 2)), (m1(0, 0) * m2(0, 3)) + (m1(0, 1) * m2(1, 3)) + (m1(0, 2) * m2(2, 3)) + (m1(0, 3) * m2(3, 3)),
						(m1(1, 0) * m2(0, 0)) + (m1(1, 1) * m2(1, 0)) + (m1(1, 2) * m2(2, 0)) + (m1(1, 3) * m2(3, 0)), (m1(1, 0) * m2(0, 1)) + (m1(1, 1) * m2(1, 1)) + (m1(1, 2) * m2(2, 1)) + (m1(1, 3) * m2(3, 1)), (m1(1, 0) * m2(0, 2)) + (m1(1, 1) * m2(1, 2)) + (m1(1, 2) * m2(2, 2)) + (m1(1, 3) * m2(3, 2)), (m1(1, 0) * m2(0, 3)) + (m1(1, 1) * m2(1, 3)) + (m1(1, 2) * m2(2, 3)) + (m1(1, 3) * m2(3, 3)),
						(m1(2, 0) * m2(0, 0)) + (m1(2, 1) * m2(1, 0)) + (m1(2, 2) * m2(2, 0)) + (m1(2, 3) * m2(3, 0)), (m1(2, 0) * m2(0, 1)) + (m1(2, 1) * m2(1, 1)) + (m1(2, 2) * m2(2, 1)) + (m1(2, 3) * m2(3, 1)), (m1(2, 0) * m2(0, 2)) + (m1(2, 1) * m2(1, 2)) + (m1(2, 2) * m2(2, 2)) + (m1(2, 3) * m2(3, 2)), (m1(2, 0) * m2(0, 3)) + (m1(2, 1) * m2(1, 3)) + (m1(2, 2) * m2(2, 3)) + (m1(2, 3) * m2(3, 3)),
						(m1(3, 0) * m2(0, 0)) + (m1(3, 1) * m2(1, 0)) + (m1(3, 2) * m2(2, 0)) + (m1(3, 3) * m2(3, 0)), (m1(3, 0) * m2(0, 1)) + (m1(3, 1) * m2(1, 1)) + (m1(3, 2) * m2(2, 1)) + (m1(3, 3) * m2(3, 1)), (m1(3, 0) * m2(0, 2)) + (m1(3, 1) * m2(1, 2)) + (m1(3, 2) * m2(2, 2)) + (m1(3, 3) * m2(3, 2)), (m1(3, 0) * m2(0, 3)) + (m1(3, 1) * m2(1, 3)) + (m1(3, 2) * m2(2, 3)) + (m1(3, 3) * m2(3, 3)) };
}

} // STD_LA

STD_LA::vector<STD_LA::float4_engine> v1{ 0.0f, 1.1f, 2.2f, 3.3f };
STD_LA::vector<STD_LA::float4_engine> v2{ 2.2f, 3.3f, 4.4f, 5.5f };
STD_LA::matrix<STD_LA::float44_engine> m1{ 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 10.1f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f, 17.17f, 18.18f, 19.19f };
STD_LA::matrix<STD_LA::float44_engine> m2{ 8.8f, 9.9f, 10.1f, 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f, 17.17f, 18.18f, 19.19f, 20.20f, 21.21f, 22.22f, 23.23f };

void
TestGroup701()
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

void TestGroup702()
{
	std::cout << v1 * v2 << "\n";
}

void
TestGroup70()
{
	TestGroup701();
	TestGroup702();
}
