a + b 
a - b
a * b 
a / b
-a

term = number x ^ number

//
expr = expr1 expr_r

expr_r = + expr1 expr_r
expr_r = - expr1 expr_r
expr_r = e

expr1 = expr2 expr1_r

expr1_r = * expr2 expr1_r
expr1_r = / expr2 expr1_r
expr1_r = e

expr2 = -expr2
expr2 = (expr)
expr2 = number
expr2 = power
