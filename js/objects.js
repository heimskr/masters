let d = 100;

let obj = {
	a: 42,
	b: {
		c: 64
	},
	d,
	f: function(x) { return x + this.a + this.d + 1 + 2 + 3; }
};

print(obj.f(obj.b.c));
this.obj = obj;
