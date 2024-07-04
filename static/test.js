// 简单的JavaScript文件示例

// 打印欢迎消息到控制台
console.log("Welcome to my simple JavaScript file!");

// 定义一个简单的函数，用于计算两个数的和
function addNumbers(a, b) {
    return a + b;
}

// 调用函数并打印结果
let result = addNumbers(5, 3);
console.log("The result of adding 5 and 3 is: " + result);

// 等待页面加载完成后执行代码
document.addEventListener("DOMContentLoaded", (event) => {
    // 获取页面中的按钮元素
    let button = document.getElementById("myButton");

    // 为按钮添加点击事件处理器
    button.addEventListener("click", () => {
        // 获取输入框的值
        let inputValue = document.getElementById("myInput").value;

        // 在页面上显示消息
        let message = "Hello, " + inputValue + "!";
        document.getElementById("message").innerText = message;
    });
});
