const bindings = require('bindings');

const addon = bindings('disable_winkey');

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
     * Disables the Alt key (Option key on macOS).
     * @returns {boolean} Returns true on success.
     */
    disableAltKey() {
        return addon.disableAltKey();
    }

    /**
     * Enables the Alt key.
     * @returns {boolean} Returns true on success.
     */
    enableAltKey() {
        return addon.enableAltKey();
    }

    /**
     * Checks if the Alt key is disabled.
     * @returns {boolean} Returns true if disabled, false otherwise.
     */
    isAltKeyDisabled() {
        return addon.isAltKeyDisabled();
    }

    /**
     * Disables the F11 key.
     * @returns {boolean} Returns true on success.
     */
    disableF11Key() {
        return addon.disableF11Key();
    }

    /**
     * Enables the F11 key.
     * @returns {boolean} Returns true on success.
     */
    enableF11Key() {
        return addon.enableF11Key();
    }

    /**
     * Checks if the F11 key is disabled.
     * @returns {boolean} Returns true if disabled, false otherwise.
     */
    isF11KeyDisabled() {
        return addon.isF11KeyDisabled();
    }

    /**
     * Enables all functionalities.
     */
    enableAll() {
        this.enableSuperKey();
        this.enableAppSwitch();
        this.enableAltKey();
        this.enableF11Key();
    }

    /**
     * Disables all functionalities.
     */
    disableAll() {
        this.disableSuperKey();
        this.disableAppSwitch();
        this.disableAltKey();
        this.disableF11Key();
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
    disableAltKey: () => keyManager.disableAltKey(),
    enableAltKey: () => keyManager.enableAltKey(),
    isAltKeyDisabled: () => keyManager.isAltKeyDisabled(),
    disableF11Key: () => keyManager.disableF11Key(),
    enableF11Key: () => keyManager.enableF11Key(),
    isF11KeyDisabled: () => keyManager.isF11KeyDisabled(),
    enableAll: () => keyManager.enableAll(),
    disableAll: () => keyManager.disableAll(),
};
