let fn = function() { print("This should've been overwritten"); };

{
	let a = 1, b = 2, c = 3 + 2 + 1 + 0;

	fn = function() { print("Sum = " + (a + b + c)); };

	// print(gc() + " GC'd");
}

fn();
print(gc() + " GC'd");
print(gc() + " GC'd");
fn();
