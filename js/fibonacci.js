function bad_fibo(n) {
	if (n <= 1)
		return n;
	return bad_fibo(n - 2) + bad_fibo(n - 1);
}

print(bad_fibo(10));
