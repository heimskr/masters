let a = [[1]];
let b = [a[0]];
let c = a[0];
let d = a[0][0];

console.log(a, b, c, d);
a[0][0] = 4;
console.log(a, b, c, d);

let n0 = 10;
let n1 = n0;
console.log(n0, n1);
++n0;
console.log(n0, n1);
n1 = 20;
console.log(n0, n1);
