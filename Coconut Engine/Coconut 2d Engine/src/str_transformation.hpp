#ifndef STR_TRANSFORMATION_HPP
#define STR_TRANSFORMATION_HPP
// when "text"_std is called, it immidiatly converts the literal into a string
inline std::string operator "" _std(const char* a, size_t s) {
	return std::string(a, s);
}
#endif