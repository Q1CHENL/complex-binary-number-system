// boundaries for input value
// unsigned
#define UMAX_VALUE_STR "340282366920938463463374607431768211455"
// signed
#define MAX_VALUE_STR "170141183460469231731687303715884105727"
#define MIN_VALUE_STR "-170141183460469231731687303715884105728"

// for tests
#define REAL_MIN_STR "-15987178197214944733"
#define REAL_MAX_STR "8608480567731124087"
#define IMAG_MIN_STR "-17216961135462248174"
#define IMAG_MAX_STR "13527612320720337851"
#define RESULT_REAL_MIN_STR "285570300082669926200635945060385871574"
#define RESULT_REAL_MAX_STR "145454031350518794186305224353187196269"
#define RESULT_IMAG_MIN_STR "241533758481136713281846289980980573621"
#define RESULT_IMAG_MAX_STR "121434099567864314412419957946238851931"

extern const char* usage_msg;

void print_int128(__int128 x);
void print_uint128(unsigned __int128 x);
__int128 str_to_int128(char* s);
unsigned __int128 str_to_uint128(char* s);
void print_uint128_as_binary(unsigned __int128 x);
void parse_complex_number_arg(char* complex_arg, __int128* real, __int128* imag);
char* extract_complex_number_arg(int argc, char** argv);
void boundary_check(char* real, char* imag);
void print_usage(const char* progname);
void print_complex_number(__int128 real, __int128 imag);
int generate_random_int(int min, int max);
unsigned __int128 random_number(int length);