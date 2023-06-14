let arr = [1, 2,, 3,];
arr[3] = (1, 2, 3, 4);
arr[4] = 5;
arr[6] = arr[4];
print(arr);
print(arr[3]);
print([,,,,1.5,2,2.5,,,]);
print([]);
print([,]);
print([,,]);

arr = [];
console.log("push:", arr.push(42));
console.log(arr);
console.log(arr["push"]);
