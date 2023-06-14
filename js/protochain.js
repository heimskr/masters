let first = {
	a: 1,
	b: 2,
	c: 3
};

let second = Object.create(first);
second.a = "one";
second.mutate = function(v) {
	this.d = v;
};

second.b = "two";

let third = Object.create(second);
third.a = "eins";

let obj = Object.create(third);
obj.c = "drei";

console.log(first.a === 1, first.b === 2, first.c === 3);
console.log(second.a === "one", second.b === "two", second.c === 3);
console.log(third.a === "eins", third.b === "two", third.c === 3);
console.log(obj.a === "eins", obj.b === "two", obj.c === "drei");

print();

console.log("first: ", first, first.a, first.b, first.c);
console.log("second:", second, second.a, second.b, second.c);
console.log("third: ", third, third.a, third.b, third.c);
console.log("obj:   ", obj, obj.a, obj.b, obj.c);
console.log("obj.prototype:", obj.prototype);

print();
obj.mutate(42);

console.log("first: ", first, first.a, first.b, first.c);
console.log("second:", second, second.a, second.b, second.c);
console.log("third: ", third, third.a, third.b, third.c);
console.log("obj:   ", obj, obj.a, obj.b, obj.c);
console.log("obj.prototype:", obj.prototype);

print();
