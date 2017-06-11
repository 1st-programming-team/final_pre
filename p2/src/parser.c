#include "parser.h"

#define assert__parser_error(s) \
    do {\
        puts("Syntax Error!");\
        if (*s) puts(s);\
        token_list->index = token_list->size;\
        ret_ty_data ret = { 1 };\
        return ret;\
    } while(0)

#define assert_parser_error(s) \
    assert__parser_error(s)

#define empty() \
	(token_list->index >= token_list->size)

#define current() \
	(token_list->list[token_list->index])

#define next() \
	(token_list->index++)

#define force_get(T, p, f) \
    do {\
        T##_data dummy = (f)(token_list);\
        if (dummy.error_code) {\
            ret_ty_data ret = { dummy.error_code };\
            return ret;\
        }\
        memcpy(&(p), &dummy.data, sizeof(ret_ty));\
    } while (0)

#define stringlize_(str) \
    #str

#define stringlize(str) \
    stringlize_(str)

#define stringcat_(a, b) \
    a##b

#define stringcat(a, b) \
    stringcat_(a, b)

#define force__token(token_ty, str) \
    do {\
        if (current() != token_ty) \
            assert_parser_error(str);\
        else next();\
    } while (0)

#define force_token(token_ty, str) \
    force__token(token_ty, str)

#define return_succ(n) \
    do {\
        ret_ty_data dummy = { 0, n };\
        return dummy;\
    } while (0)

#define def_getter_helper(...) \
        __VA_ARGS__\
    }

#define def_getter(T, name, ...) \
    T##_data get_##name (__VA_ARGS__ tokens *token_list) {\
        typedef T ret_ty;\
        typedef T##_data ret_ty_data;\
        def_getter_helper

static int_data get_number(tokens *);
static int_data get_number1(tokens *);

static def_getter(int, number2)
(
	if (!empty()) {
		switch (current()) {
			case TOK_S_BRAK_O: {	// (expr)
				next();
				int p;
				force_get(int, p, get_number);
				force_token(TOK_S_BRAK_C, "expected )");
				return_succ(p);
			} break;
			case TOK_SUB: {
				next();
				int p;
				force_get(int, p, get_number1);
				return_succ(-p);
			} break;
			case TOK_WORD: {
				return_succ(1);
			} break;
			default: {
				int p = current();
				if (p < 0) assert_parser_error("expected number");
				next();
				return_succ(p);
			}
		}
	} else {
		assert_parser_error("unexpected eof");
	}
)

static def_getter(int, number1_r, int lhs,)
(
	if (empty()) {// e
		return_succ(lhs);
	}
	switch (current()) {
		case TOK_MUL: {		// * expr1 expr_r
			next();
			int p;
			force_get(int, p, get_number2);
			return get_number1_r(lhs * p, token_list);
		} break;
		case TOK_DIV: {		// * expr1 expr_r
			next();
			int p;
			force_get(int, p, get_number2);
			return get_number1_r(lhs / p, token_list);
		} break;
		default: {			// e
			return_succ(lhs);
		}
	}
)

static def_getter(int, number1)
(
    int p;
	force_get(int, p, get_number2);
	return get_number1_r(p, token_list);
)

static def_getter(int, number_r, int lhs,)
(
	if (empty()) {// e
		return_succ(lhs);
	}
	switch (current()) {
		case TOK_ADD: {		// + expr1 expr_r
			next();
			int p;
			force_get(int, p, get_number1);
			return get_number_r(lhs + p, token_list);
		} break;
		case TOK_SUB: {		// - expr1 expr_r
			next();
			int p;
			force_get(int, p, get_number1);
			return get_number_r(lhs - p, token_list);
		} break;
		default: {			// e
			return_succ(lhs);
		}
	}
)

static def_getter(int, number)
(
    int p;
	force_get(int, p, get_number1);
	return get_number_r(p, token_list);
)

def_getter(poly, expr2)
(
	if (!empty()) {
		switch (current()) {
			case TOK_S_BRAK_O: {	// (expr)
				next();
				poly p;
				force_get(poly, p, get_expr);
				force_token(TOK_S_BRAK_C, "expected )");
				//#mod syntax
				if (!empty() && (current() == TOK_WORD || current() == TOK_S_BRAK_O)) {
                    poly pp;
                    force_get(poly, pp, get_expr2);
                    mul_poly(p, pp);
                    dispose_poly(pp);
				}
				//#mod syntax
				return_succ(p);
			} break;
			case TOK_SUB: {
				next();
				poly p;
				force_get(poly, p, get_expr2);
				reverse_poly(p);
				return_succ(p);
			} break;
			case TOK_WORD: {
			    next();
                if (!empty()) {
                    if (current() == TOK_POW) {
                        next();
                        int p;
                        force_get(int, p, get_number2);
                        //#mod syntax
                        poly pl = make_poly(1, p);
                        if (!empty() && current() == TOK_S_BRAK_O) {
                            poly pp;
                            force_get(poly, pp, get_expr2);
                            mul_poly(pl, pp);
                            dispose_poly(pp);
                        }
                        //#mod syntax
                        return_succ(pl);
                    }
                }
                //#mod syntax
                poly pl = make_poly(1, 1);
				if (!empty() && current() == TOK_S_BRAK_O) {
                    poly pp;
                    force_get(poly, pp, get_expr2);
                    mul_poly(pl, pp);
                    dispose_poly(pp);
				}
				//#mod syntax
                return_succ(pl);
			} break;
			default: {
			    if (current() < 0) {
                    assert_parser_error("unexpected token");
			    } else {
                    int a = current();
                    next();
                    //#mod syntax
                    poly p = make_poly(a, 0);
                    if (!empty() && (current() == TOK_WORD || current() == TOK_S_BRAK_O)) {
                        poly pp;
                        force_get(poly, pp, get_expr2);
                        mul_poly(p, pp);
                        dispose_poly(pp);
                    }
                    //#mod syntax
                    return_succ(p);
			    }
			}
		}
	} else {
		assert_parser_error("expected poly");
	}
)

static def_getter(poly, expr1_r, poly lhs,)
(
	if (empty()) {// e
		return_succ(lhs);
	}
	switch (current()) {
		case TOK_MUL: {		// * expr1 expr_r
			next();
			poly p;
			force_get(poly, p, get_expr2);
			mul_poly(lhs, p);
			dispose_poly(p);
			return get_expr1_r(lhs, token_list);
		} break;
		case TOK_DIV: {		// / expr1 expr_r
			next();
			poly p;
			force_get(poly, p, get_expr2);
			int err_code = div_poly(lhs, p);
			dispose_poly(p);
			switch (err_code) {
			case 0xFF: {
			    assert_parser_error("div by 0");
			} break;
			case 0x1: {
                assert_parser_error("no common divider");
			} break;
			case 0xF: {
                assert_parser_error("this calculator is too weak to handle this problem, QwQ");
			} break;
			default: {
                return get_expr1_r(lhs, token_list);
			}
			}
		} break;
		default: {			// e
			return_succ(lhs);
		}
	}
)

def_getter(poly, expr1)
(
    poly p;
	force_get(poly, p, get_expr2);
	poly_data pp = get_expr1_r(p, token_list);
	if (pp.error_code) return pp;
	if (pp.data != p) {
		dispose_poly(p);
	}
	return_succ(pp.data);
)

static def_getter(poly, expr_r, poly lhs,)
(
	if (empty()) {// e
		return_succ(lhs);
	}
	switch (current()) {
		case TOK_ADD: {		// + expr1 expr_r
			next();
			poly p;
			force_get(poly, p, get_expr1);
			add_poly(lhs, p);
			dispose_poly(p);
			return get_expr_r(lhs, token_list);
		} break;
		case TOK_SUB: {		// - expr1 expr_r
			next();
			poly p;
			force_get(poly, p, get_expr1);
			sub_poly(lhs, p);
			dispose_poly(p);
			return get_expr_r(lhs, token_list);
		} break;
		default: {			// e
			return_succ(lhs);
		}
	}
)

def_getter(poly, expr)
(
    poly p;
    force_get(poly, p, get_expr1);
    poly_data pp = get_expr_r(p, token_list);
    if (pp.error_code) return pp;
    if (pp.data != p) {
        dispose_poly(p);
    }
    return pp;
)
