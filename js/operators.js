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
console.log(1 in [1, 2, 3], 1 in [0, 0, 0], !(3 in [1, 2]));
