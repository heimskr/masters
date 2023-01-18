#!/usr/bin/env node

const acorn = require("acorn");
const fs = require("fs");

console.log(JSON.stringify(acorn.parse(fs.readFileSync(0, "utf8"), {
	ecmaVersion: "latest"
})));
