#ifndef TError_h
#define TError_h

class TError
{
	static int last_error_code ;
	map_from_string_to_double_type map_from_error_name_to_value ;

	public:

	TError() ;
	int GetErrorCode(string error_string) ;
} ;

int TError::last_error_code  = 1 ;

TError::TError()
{
	map_from_error_name_to_value["ConfigOrProjectFileDoesNotExist"] = last_error_code++ ;
	map_from_error_name_to_value["BadKeyword"] = last_error_code++ ;
	map_from_error_name_to_value["UndefinedParameter"] = last_error_code++ ;
	map_from_error_name_to_value["UndefinedSetting"] = last_error_code++ ;
}

int TError::GetErrorCode(string error_string)
{
	map_from_string_to_double_type::iterator it ;

	it = map_from_error_name_to_value.find(error_string) ;

	if (it != map_from_error_name_to_value.end()) return (int)map_from_error_name_to_value[error_string] ;
	else return -1 ;
}

#endif // #ifndef TError_h
