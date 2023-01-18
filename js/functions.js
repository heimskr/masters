function double(x) {
	return x * 2;
}

console.log(double(42));
console.log((function(x) { return !x; })(false));
console.log((x => -x)(64));
