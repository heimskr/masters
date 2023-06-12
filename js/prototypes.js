let obj = {a: 42, b: 64};
print(obj.toString() === "[object Object]");

let str = "foobar";
print(str.toString() === "foobar");
print(str.length === 6, str.length !== 5);
print(str.substr(2, 2) === "ob", str.substring(1, 4) === "oob");
print(str.charAt(1) === "o", str.charAt() === "f", str.charAt("3") === "b", str.charAt(NaN) === "f");
print(str.charAt(Infinity) === "f", str.charAt(-Infinity) === "f");
print(str.concat(" ", "quux", "!") === "foobar quux!", str.concat(" ", 'quux', "!") !== "foobar");
print(str.endsWith("bar"), !str.endsWith("foo"), "foo".endsWith("o"), "foo".endsWith("o", 2));
print(!"foo".endsWith("o", "b"), !"foob".endsWith("o", "b"), !"foobobobobobob".endsWith("o", "b"));
print(!"foobobobobobob".endsWith(), "undefined".endsWith(), !"".endsWith());
print("".endsWith(""), "".endsWith("", NaN), "1".endsWith("", NaN), "2".endsWith("", NaN));
print(!"2".endsWith("2", NaN), !"2".endsWith("2", 0), "2".endsWith("2", 1), "2".endsWith("2", 2));
print("".endsWith(""), "".endsWith("", NaN), "".endsWith("", Infinity));
print(String.fromCharCode(100) === "d", String.fromCharCode(101) !== "d",
      String.fromCharCode(100, 101, 97, 100, "98", 101, 101, 102) === "deadbeef", String.fromCharCode() === "",
      !String.fromCharCode());
print(!"foo".includes(), !"".includes(), "".includes(""), "foo".includes(""), "foo".includes("fo"));
print("foo".indexOf("") === 0, "foo".indexOf() === -1, "undefined".indexOf() === 0);
print("undefinedundefined".indexOf(undefined, 1) === 9, "foo".indexOf("f") === 0, "foo".indexOf("o") === 1);
print("foo".indexOf("o", 1) === 1, "fobo".indexOf("o", 2) === 3, "foo".indexOf("o", 3) === -1);