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
print(str.concat(" ", 'quux', "!") !== "foobar");
print(str.endsWith("bar"));
print(!str.endsWith("foo"));
print("foo".endsWith("o"));
print("foo".endsWith("o", 2));
print(!"foo".endsWith("o", "b"));
print(!"foob".endsWith("o", "b"));
print(!"foobobobobobob".endsWith("o", "b"));
print(!"foobobobobobob".endsWith());
print("undefined".endsWith());
print(!"".endsWith());
print("".endsWith(""));
print("".endsWith("", NaN));
print("1".endsWith("", NaN));
print("2".endsWith("", NaN));
print(!"2".endsWith("2", NaN));
print(!"2".endsWith("2", 0));
print("2".endsWith("2", 1));
print("2".endsWith("2", 2));
print("".endsWith(""));
print("".endsWith("", NaN));
print("".endsWith("", Infinity));
print(String.fromCharCode(100) === "d");
print(String.fromCharCode(101) !== "d");
print(String.fromCharCode(100, 101, 97, 100, "98", 101, 101, 102) === "deadbeef");
print(String.fromCharCode() === "");
print(!String.fromCharCode());
