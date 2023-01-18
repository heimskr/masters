function bad_fibo(n) {
	if (n <= 1)
		return n;
	return bad_fibo(n - 2) + bad_fibo(n - 1);
}

console.log(bad_fibo(10));
