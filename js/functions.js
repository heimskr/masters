function double(x) {
	return x * 2;
}

print(double(42));
print((function(x) { return !x; })(false));
// print((x => -x)(64));

function quadratic(a, b, c) {
	return (-b + (b**2 - 4 * a * c)**(1/2)) / (2 * a);
}

print("x^2 + 2x + 1 = 0 -> x = " + quadratic(1, 2, 1));
