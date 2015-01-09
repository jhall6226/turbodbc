#include <pydbc/column.h>
#include <sqlext.h>

namespace pydbc {

column::column() = default;

column::~column() = default;

field column::get_field() const
{
	return do_get_field();
}


long_column::long_column(cpp_odbc::statement & statement, std::size_t one_based_index) :
		buffer_(sizeof(long), cached_rows)
{
	statement.bind_column(one_based_index, SQL_C_SBIGINT, buffer_);
}

field long_column::do_get_field() const
{
	auto value_ptr = reinterpret_cast<long const *>(buffer_[0].data_pointer);
	return {*value_ptr};
}


namespace {
	std::size_t const maximum_string_size = 1024;
}

string_column::string_column(cpp_odbc::statement & statement, std::size_t one_based_index) :
		buffer_(maximum_string_size, cached_rows)
{
	statement.bind_column(one_based_index, SQL_CHAR, buffer_);
}

field string_column::do_get_field() const
{
	return {std::string(buffer_[0].data_pointer)}; // unixodbc stores null-terminated strings
}

}