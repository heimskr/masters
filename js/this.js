let this = 42;

function foo() {
	console.log(this);
};

{
	let this = 64;
	foo();
}
