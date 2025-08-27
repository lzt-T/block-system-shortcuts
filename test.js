const assert = require('assert');
const {
    disableAll,
    enableAll,
    isSuperKeyDisabled,
    isAppSwitchDisabled,
    isAltKeyDisabled,
    isF11KeyDisabled,
    isCtrlKeyDisabled,
    isF3KeyDisabled,
} = require("./index");

const platform = process.platform;
const superKeyName = platform === 'win32' ? 'Windows键' : 'Command键';
const appSwitchName = platform === 'win32' ? 'Alt+Tab' : 'Command+Tab';
const altKeyName = platform === 'win32' ? 'Alt键' : 'Option键';
const ctrlKeyName = platform === 'win32' ? 'Ctrl键' : 'Control键';

console.log(`[测试] 开始在 ${platform} 平台进行按键禁用测试...`);

try {
    // 1. 初始状态检查
    console.log('[测试] 正在检查初始状态...');
    assert.strictEqual(isSuperKeyDisabled(), false, '初始状态：Super键应为启用状态。');
    assert.strictEqual(isAppSwitchDisabled(), false, '初始状态：应用切换键应为启用状态。');
    assert.strictEqual(isAltKeyDisabled(), false, '初始状态：Alt键应为启用状态。');
    assert.strictEqual(isF11KeyDisabled(), false, '初始状态：F11键应为启用状态。');
    assert.strictEqual(isCtrlKeyDisabled(), false, '初始状态：Ctrl键应为启用状态。');
    assert.strictEqual(isF3KeyDisabled(), false, '初始状态：F3键应为启用状态。');
    console.log('✓ 初始状态正确。');

    // 2. 禁用所有按键
    console.log(`[测试] 正在禁用 ${superKeyName}、${appSwitchName}、${altKeyName}、F11键、${ctrlKeyName} 和 F3键...`);
    disableAll();

    // 3. 禁用后验证
    assert.strictEqual(isSuperKeyDisabled(), true, `禁用后：${superKeyName} 应为禁用状态。`);
    assert.strictEqual(isAppSwitchDisabled(), true, `禁用后：${appSwitchName} 应为禁用状态。`);
    assert.strictEqual(isAltKeyDisabled(), true, `禁用后：${altKeyName} 应为禁用状态。`);
    assert.strictEqual(isF11KeyDisabled(), true, `禁用后：F11键 应为禁用状态。`);
    assert.strictEqual(isCtrlKeyDisabled(), true, `禁用后：${ctrlKeyName} 应为禁用状态。`);
    assert.strictEqual(isF3KeyDisabled(), true, `禁用后：F3键 应为禁用状态。`);
    console.log(`✓ ${superKeyName}、${appSwitchName}、${altKeyName}、F11键、${ctrlKeyName} 和 F3键 已被禁用。`);
    console.log(`现在你可以手动测试按键是否已禁用。按 Ctrl+C 键继续并恢复按键。`);


    // 4. 等待用户手动测试，然后按 Ctrl+C
    const keepAlive = setInterval(() => {
      // 保持进程运行
    }, 1000);

    const cleanup = () => {
        clearInterval(keepAlive);
        console.log(`\n[测试] 正在恢复 ${superKeyName}、${appSwitchName}、${altKeyName}、F11键、${ctrlKeyName} 和 F3键...`);
        enableAll();
        assert.strictEqual(isSuperKeyDisabled(), false, `恢复后：${superKeyName} 应为启用状态。`);
        assert.strictEqual(isAppSwitchDisabled(), false, `恢复后：${appSwitchName} 应为启用状态。`);
        assert.strictEqual(isAltKeyDisabled(), false, `恢复后：${altKeyName} 应为启用状态。`);
        assert.strictEqual(isF11KeyDisabled(), false, `恢复后：F11键 应为启用状态。`);
        assert.strictEqual(isCtrlKeyDisabled(), false, `恢复后：${ctrlKeyName} 应为启用状态。`);
        assert.strictEqual(isF3KeyDisabled(), false, `恢复后：F3键 应为启用状态。`);
        console.log(`✓ ${superKeyName}、${appSwitchName}、${altKeyName}、F11键、${ctrlKeyName} 和 F3键 已成功恢复。`);
        console.log('[测试] 测试成功结束。');
        process.exit(0);
    };

    // 处理 Ctrl+C
    process.on("SIGINT", cleanup);
    
    // 确保在任何情况下退出时都能恢复
    process.on("exit", () => {
        enableAll();
    });

} catch (error) {
    console.error('[测试] 测试失败:', error.message);
    // 确保在失败时也能恢复按键
    enableAll();
    process.exit(1);
}
                                    