#define DUCKDB_EXTENSION_MAIN

#include "waddle_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/function/scalar_function.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

void WaddleScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "...........🦆 " + name.GetString());
	});
}

void TestScalarFunc(DataChunk &args, ExpressionState &state, Vector &result) {
	// I think that when executing, this function modifies the address pointed to by result
	// that is the output space, hence its data isn't really an input (I think)

	// args.ToString() returns the number of columns in something, then each argument
	// on my custom function, which I called like select quack('test'), it said 1 column, and test

	// it's not currently clear what state is...
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "args: " + args.ToString());
	});
}

// seems unecessary for now
// inline void WaddleOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
// 	auto &name_vector = args.data[0];
// 	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
// 		return StringVector::AddString(result, "Waddle " + name.GetString() + ", my linked OpenSSL version is " +
// 		                                           OPENSSL_VERSION_TEXT);
// 	});
// }

static void LoadInternal(ExtensionLoader &loader) {
	// Register a scalar function
	auto waddle_scalar_function =
	    ScalarFunction("waddle", {LogicalType::VARCHAR}, LogicalType::VARCHAR, WaddleScalarFun);

	loader.RegisterFunction(waddle_scalar_function);

	// register test my scalar function
	auto test_scalar_function =
		ScalarFunction("quack", {LogicalType::VARCHAR}, LogicalType::VARCHAR, TestScalarFunc);
	loader.RegisterFunction(test_scalar_function);

	// Register another scalar function
	// auto waddle_openssl_version_scalar_function = ScalarFunction("waddle_openssl_version", {LogicalType::VARCHAR},
	//                                                              LogicalType::VARCHAR, WaddleOpenSSLVersionScalarFun);
	// loader.RegisterFunction(waddle_openssl_version_scalar_function);
}

void WaddleExtension::Load(ExtensionLoader &loader) {
	LoadInternal(loader);
}
std::string WaddleExtension::Name() {
	return "waddle";
}

std::string WaddleExtension::Version() const {
#ifdef EXT_VERSION_WADDLE
	return EXT_VERSION_WADDLE;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_CPP_EXTENSION_ENTRY(waddle, loader) {
	duckdb::LoadInternal(loader);
}
}
