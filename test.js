const { disableWinKey, enableWinKey, isWinKeyDisabled } = require("./index");

console.log("Windows键禁用测试");
console.log("初始状态:", isWinKeyDisabled() ? "禁用" : "启用");

// 禁用Windows键
console.log("禁用Windows键...");
if (disableWinKey()) {
  console.log("✓ 禁用成功");
  console.log("现在Windows键已被禁用，按 Ctrl+C 退出测试");

  // 保持程序运行，等待用户按Ctrl+C
  const keepAlive = setInterval(() => {
    // 空循环，保持程序活跃
  }, 1000);

  // 处理Ctrl+C退出
  process.on("SIGINT", () => {
    clearInterval(keepAlive);
    console.log("\n正在恢复Windows键...");
    if (enableWinKey()) {
      console.log("✓ Windows键已恢复");
    } else {
      console.log("✗ 恢复失败");
    }
    console.log("测试结束");
    process.exit(0);
  });
} else {
  console.log("✗ 禁用失败");
}

// 确保退出时恢复Windows键
process.on("exit", () => {
  enableWinKey();
});
