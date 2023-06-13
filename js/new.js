function Foo(a) {
	this.b = 100 + a;
};

let foo = new Foo(64);
console.log(foo, Foo(64));

Foo.prototype.negate = function() {
	return -this.b;
};

console.log(Foo.prototype);
console.log(foo.negate());
