if (typeof gc === "undefined")
	function gc() {};
let str = "";
for (let i = 0; i < 1e6; ++i) {
	str += "foo";
	if (i % 10000 === 0)
		gc();
}
// console.log(str);
