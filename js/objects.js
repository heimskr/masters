let d = 100;

let obj = {
	a: 42,
	b: {
		c: 64
	},
	d,
	f: function(x) { return x + this.a; }
};

console.log(obj.f(obj.b.c));
