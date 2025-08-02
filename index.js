const addon = require('./build/Release/disable_winkey.node');

class KeyManager {
    constructor() {
        // 状态由原生插件内部管理
    }

    /**
     * 禁用Windows键
     * @returns {boolean} 成功返回true
     */
    disableWinKey() {
        return addon.disableWinKey();
    }

    /**
     * 启用Windows键
     * @returns {boolean} 成功返回true
     */
    enableWinKey() {
        return addon.enableWinKey();
    }

    /**
     * 检查Windows键是否被禁用
     * @returns {boolean} 禁用返回true，启用返回false
     */
    isWinKeyDisabled() {
        return addon.isWinKeyDisabled();
    }

    /**
     * 禁用Alt+Tab
     * @returns {boolean} 成功返回true
     */
    disableAltTab() {
        return addon.disableAltTab();
    }

    /**
     * 启用Alt+Tab
     * @returns {boolean} 成功返回true
     */
    enableAltTab() {
        return addon.enableAltTab();
    }

    /**
     * 检查Alt+Tab是否被禁用
     * @returns {boolean} 禁用返回true，启用返回false
     */
    isAltTabDisabled() {
        return addon.isAltTabDisabled();
    }

    /**
     * 启用所有功能
     */
    enableAll() {
        this.enableWinKey();
        this.enableAltTab();
    }
}

module.exports = {
    KeyManager,
    // 直接导出函数供简单使用
    disableWinKey: addon.disableWinKey,
    enableWinKey: addon.enableWinKey,
    isWinKeyDisabled: addon.isWinKeyDisabled,
    disableAltTab: addon.disableAltTab,
    enableAltTab: addon.enableAltTab,
    isAltTabDisabled: addon.isAltTabDisabled
};