const addon = require('./build/Release/disable_winkey.node');

/**
 * A manager for handling keyboard key state.
 */
class KeyManager {
    constructor() {
        // State is managed internally by the native addon.
    }

    /**
     * Disables the "Super" key (Windows key on Windows, Command key on macOS).
     * @returns {boolean} Returns true on success.
     */
    disableSuperKey() {
        return addon.disableSuperKey();
    }

    /**
     * Enables the "Super" key.
     * @returns {boolean} Returns true on success.
     */
    enableSuperKey() {
        return addon.enableSuperKey();
    }

    /**
     * Checks if the "Super" key is disabled.
     * @returns {boolean} Returns true if disabled, false otherwise.
     */
    isSuperKeyDisabled() {
        return addon.isSuperKeyDisabled();
    }

    /**
     * Disables the application switcher (Alt+Tab on Windows, Command+Tab on macOS).
     * @returns {boolean} Returns true on success.
     */
    disableAppSwitch() {
        return addon.disableAppSwitch();
    }

    /**
     * Enables the application switcher.
     * @returns {boolean} Returns true on success.
     */
    enableAppSwitch() {
        return addon.enableAppSwitch();
    }

    /**
     * Checks if the application switcher is disabled.
     * @returns {boolean} Returns true if disabled, false otherwise.
     */
    isAppSwitchDisabled() {
        return addon.isAppSwitchDisabled();
    }

    /**
     * Enables all functionalities.
     */
    enableAll() {
        this.enableSuperKey();
        this.enableAppSwitch();
    }

    /**
     * Disables all functionalities.
     */
    disableAll() {
        this.disableSuperKey();
        this.disableAppSwitch();
    }
}

const keyManager = new KeyManager();

// To solve the `this` context issue cleanly, we export functions
// that are guaranteed to call the methods on the single instance.
module.exports = {
    KeyManager,
    disableSuperKey: () => keyManager.disableSuperKey(),
    enableSuperKey: () => keyManager.enableSuperKey(),
    isSuperKeyDisabled: () => keyManager.isSuperKeyDisabled(),
    disableAppSwitch: () => keyManager.disableAppSwitch(),
    enableAppSwitch: () => keyManager.enableAppSwitch(),
    isAppSwitchDisabled: () => keyManager.isAppSwitchDisabled(),
    enableAll: () => keyManager.enableAll(),
    disableAll: () => keyManager.disableAll(),
};
