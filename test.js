const { disableAll, enableAll } = require("./index");

console.log("Windows键和Alt+Tab组合键禁用测试");

// 禁用所有
console.log("正在禁用Windows键和Alt+Tab...");
disableAll();
console.log("✓ 禁用成功");
console.log("现在Windows键和Alt+Tab已被禁用，按 Ctrl+C 退出测试");

// 保持程序运行，等待用户按Ctrl+C
const keepAlive = setInterval(() => {
  // 空循环，保持程序活跃
}, 1000);

// 处理Ctrl+C退出
process.on("SIGINT", () => {
  clearInterval(keepAlive);
  console.log("\n正在恢复Windows键和Alt+Tab...");
  enableAll();
  console.log("✓ Windows键和Alt+Tab已恢复");
  console.log("测试结束");
  process.exit(0);
});

// 确保在任何情况下退出时都能恢复
process.on("exit", () => {
  enableAll();
  console.log('程序退出，Windows 键和 Alt+Tab 已恢复。');
});
