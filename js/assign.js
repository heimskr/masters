let arr = [1, [2], "3"];
let x;
arr = [1, ["2"], 3];
arr[1][0] = x = 42;
print(arr, x);
