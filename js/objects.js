let d = 100;

let obj = {
	a: 42,
	b: {
		c: 64
	},
	d,
	f(x) { return x + 1000; }
};

console.log(obj.f(obj.b.c));
