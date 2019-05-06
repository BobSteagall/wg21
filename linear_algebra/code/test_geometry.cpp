#include "linear_algebra.hpp"
#include <cassert>

namespace STD_LA {
USING_STD

class float2_engine
{
public:
	using engine_category = STD_LA::mutable_vector_engine_tag;
	using element_type    = float;
	using value_type      = float;
	using reference       = float&;
	using pointer         = float*;
	using const_reference = float const&;
	using const_pointer   = float const*;
	using iterator        = STD_LA::detail::vector_iterator<float2_engine>;
	using const_iterator  = STD_LA::detail::vector_const_iterator<float2_engine>;
	using difference_type = ptrdiff_t;
	using index_type      = int_fast32_t;
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

	constexpr const_reference   operator ()(index_type i) const;
	constexpr const_iterator    begin() const noexcept;
	constexpr const_iterator    end() const noexcept;

	constexpr size_type     capacity() const noexcept;
	constexpr index_type    elements() const noexcept;
	constexpr size_type     size() const noexcept;

	constexpr reference     operator ()(index_type i);
	constexpr iterator      begin() noexcept;
	constexpr iterator      end() noexcept;

	constexpr void  swap(float2_engine& rhs) noexcept;
	constexpr void  swap_elements(index_type i, index_type j) noexcept;

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
float2_engine::operator ()(index_type i) const
{
	assert(i < 2 && i >= 0);
	return ma_elems[i];
}

class float22_engine
{
public:
	using engine_category = STD_LA::mutable_matrix_engine_tag;
	using element_type    = float;
	using value_type      = float;
	using reference       = float&;
	using pointer         = float*;
	using const_reference = float const&;
	using const_pointer   = float const*;
	using difference_type = ptrdiff_t;
	using index_type      = int_fast32_t;
	using size_type       = int_fast32_t;
	using size_tuple      = std::tuple<size_type, size_type>;

	using is_fixed_size   = true_type;
	using is_resizable    = false_type;

	using is_column_major = false_type;
	using is_dense        = true_type;
	using is_rectangular  = true_type;
	using is_row_major    = true_type;

	using column_view_type    = STD_LA::matrix_column_view<float22_engine>;
	using row_view_type       = STD_LA::matrix_row_view<float22_engine>;
	using transpose_view_type = STD_LA::matrix_transpose_view<float22_engine>;

public:
	constexpr float22_engine();
	template<class U>
	constexpr float22_engine(std::initializer_list<U> list);
	constexpr float22_engine(float22_engine&&) noexcept = default;
	constexpr float22_engine(float22_engine const&) = default;

	constexpr float22_engine& operator =(float22_engine&&) noexcept = default;
	constexpr float22_engine& operator =(float22_engine const&) = default;

	constexpr const_reference   operator ()(index_type i, index_type j) const;

	constexpr index_type    columns() const noexcept;
	constexpr index_type    rows() const noexcept;
	constexpr size_tuple    size() const noexcept;

	constexpr size_type     column_capacity() const noexcept;
	constexpr size_type     row_capacity() const noexcept;
	constexpr size_tuple    capacity() const noexcept;

	constexpr reference     operator ()(index_type i, index_type j);

	constexpr void      assign(float22_engine const& rhs);
	template<class ET2>
	constexpr void      assign(ET2 const& rhs);

	constexpr void      swap(float22_engine& rhs) noexcept;
	constexpr void      swap_columns(index_type j1, index_type j2) noexcept;
	constexpr void      swap_rows(index_type i1, index_type i2) noexcept;

private:
	float   ma_elems[4];
};

template<>
struct matrix_addition_engine_traits<STD_LA::matrix_operation_traits, float2_engine, float2_engine>
{
	using element_type_1 = float;
	using element_type_2 = float;
	using element_type   = float;
	using engine_type    = float2_engine;
};

template<>
inline auto
matrix_addition_traits<matrix_operation_traits, vector<float2_engine, matrix_operation_traits>, vector<float2_engine, matrix_operation_traits>>::add
(vector<float2_engine, matrix_operation_traits> const& v1, vector<float2_engine, matrix_operation_traits> const& v2) -> result_type
{
	PrintOperandTypes<result_type>("float2 addition_traits", v1, v2);

	return result_type{ v1(0) + v2(0), v1(1) + v2(1) };
}

} // STD_LA

void
TestGroup50()
{
	PRINT_FNAME();

	STD_LA::vector<STD_LA::float2_engine, STD_LA::matrix_operation_traits> v1{ 0.0f, 1.1f };
	STD_LA::vector<STD_LA::float2_engine, STD_LA::matrix_operation_traits> v2{ 2.2f, 3.3f };

	auto v3 = v1 + v2;
}