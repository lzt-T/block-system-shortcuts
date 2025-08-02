const { disableWinKey, enableWinKey, isWinKeyDisabled ,disableAltTab ,enableAltTab} = require("./index");

console.log("Windows键禁用测试 （Alt+Tab也会被禁用）");
// 禁用Windows键
console.log("禁用Windows键和Alt+Tab...");

if (disableWinKey() && disableAltTab()) {
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
    if (enableWinKey() && enableAltTab()) {
      console.log("✓ Windows键和Alt+Tab已恢复");
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
  enableAltTab(); 
});
