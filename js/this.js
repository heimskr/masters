let this = 42;

function foo() {
	print(this);
};

{
	let this = 64;
	foo();
}
