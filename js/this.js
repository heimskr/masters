let this = 42;

function foo(x) {
	console.log(this);
	return x * 2;
};

{
	let this = 64;
	console.log("foo(1) ==", foo(1));
}

console.log("foo.apply({x: 100}, [2]) ==", foo.apply({x: 100}, [2]));
console.log("foo.call(null, 4) ==", foo.call(null, 4));
