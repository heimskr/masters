let obj = {};
for (let i = 0; i < 1e6; ++i)
	obj[i + ""] = i;
console.log(obj[1]);
