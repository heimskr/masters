function Foo(a) {
	this.b = 100 + a;
};

console.log(new Foo(64));
console.log(Foo(64));
