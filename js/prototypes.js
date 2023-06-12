let obj = {a: 42, b: 64};
print(obj.toString() === "[object Object]");

let str = "foobar";
print(str.toString() === "foobar");
print(str.length === 6);
print(str.substr(2, 2) === "ob");
print(str.substring(1, 4) === "oob");
print(str.charAt(1) === "o");
print(str.charAt() === "f");
print(str.charAt("3") === "b");
print(str.charAt(NaN) === "f");
print(str.charAt(Infinity) === "f");
print(str.charAt(-Infinity) === "f");
print(str.concat(" ", "quux", "!") === "foobar quux!");
print(str.concat(" ", "quux", "!") !== "foobar");