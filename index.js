const addon = require('./build/Release/disable_winkey.node');

class WinKeyManager {
    constructor() {
        this.isDisabled = false;
    }

    /**
     * 禁用Windows键
     * @returns {boolean} 成功返回true，失败返回false
     */
    disable() {
        try {
            const result = addon.disableWinKey();
            if (result) {
                this.isDisabled = true;
                console.log('Windows键已禁用');
            }
            return result;
        } catch (error) {
            console.error('禁用Windows键失败:', error.message);
            return false;
        }
    }

    /**
     * 启用Windows键
     * @returns {boolean} 成功返回true，失败返回false
     */
    enable() {
        try {
            const result = addon.enableWinKey();
            if (result) {
                this.isDisabled = false;
                console.log('Windows键已启用');
            }
            return result;
        } catch (error) {
            console.error('启用Windows键失败:', error.message);
            return false;
        }
    }

    /**
     * 检查Windows键是否被禁用
     * @returns {boolean} 禁用返回true，启用返回false
     */
    isWinKeyDisabled() {
        return addon.isWinKeyDisabled();
    }

    /**
     * 切换Windows键状态
     * @returns {boolean} 当前状态
     */
    toggle() {
        if (this.isWinKeyDisabled()) {
            return this.enable();
        } else {
            return this.disable();
        }
    }
}

module.exports = {
    WinKeyManager,
    // 直接导出函数供简单使用
    disableWinKey: addon.disableWinKey,
    enableWinKey: addon.enableWinKey,
    isWinKeyDisabled: addon.isWinKeyDisabled
};