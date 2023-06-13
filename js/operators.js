let a = 40;
console.log(a);
console.log(a += 2);
console.log(a + 2);
console.log(a - 2);
console.log(a * 2);
console.log(a % 2);
console.log(a / 2);
console.log(a ** 2);

let x = 0;

function foo() {
	++x;
	return true;
};

console.log("foo() && foo() is", foo() && foo());
console.log("x is now", x);
