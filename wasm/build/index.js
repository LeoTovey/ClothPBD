// import {Module} from './PBD.js'; // Assuming PBD.js is in the same directory as this TypeScript file

// const { Module } = require("webpack");

// Module.onRuntimeInitialized = () => {
//     console.log(Module._simple_add(1, 2));
//     console.log(Module._simple_add(2, 1));
// };


// import Module from './PBD.js'; // Assuming PBD.js is in the same directory as this TypeScript file

// var Module = {}
// Module.onRuntimeInitialized = () => {
//     console.log(Module);
//     console.log(Module._simple_add(1, 2));
//     console.log(Module._simple_add(2, 1));
// };

{/* <script type="text/javascript" src="./PBD.js"></script>
 */}

// 导入你的计算器模块
// importScript("PBD.js");
var Module = {}
console.log(Module)
// var script = document.createElement('script');
// script.src = './PBD.js';
// document.head.appendChild(script);
// import {Module} from "./PBD.js";
// console.log(Module);
// // 当模块加载并准备就绪时执行
// Module.onRuntimeInitialized = function() {
//     console.log(Module._simple_add(1, 2));
//     console.log(Module._simple_add(2, 1));
// };

// // 助手函数，用于动态添加<script>标签
// function importScript(src) {
//     var script = document.createElement("script");
//     script.src = src;
//     script.async = false; // 确保脚本按顺序加载
//     document.head.appendChild(script);
// }
