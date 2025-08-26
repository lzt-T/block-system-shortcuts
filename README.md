# 按键禁用插件 (Node.js C++ Addon)

这是一个用于禁用特定按键的 Node.js C++ Addon 库，支持禁用 "Super" 键 (Windows 上的 Win 键, macOS 上的 Command 键)、应用切换快捷键 (Win: Alt+Tab, Mac: Cmd+Tab)、Alt键以及F11键。

本项目从一个仅支持 Windows 的实现，重构为了一个高质量、易于维护和扩展的跨平台解决方案。

## 功能特性

- **跨平台支持**: 同时支持 Windows 和 macOS。
- **禁用 Super 键**: 禁用 Windows 键或 macOS 的 Command 键。
- **禁用应用切换**: 禁用 Alt+Tab 或 Command+Tab。
- **禁用 Alt 键**: 禁用 Alt 键 (Windows) 或 Option 键 (macOS)。
- **禁用 F11 键**: 禁用 F11 功能键，防止意外全屏切换。
- **简单易用的 JavaScript API**: 提供统一的 `KeyManager` 类和独立的函数调用方式。
- **高性能**: 使用原生代码实现，对系统性能影响极小。
- **自动资源清理**: 在程序退出时自动恢复所有按键功能，防止意外锁定。

## 系统要求

- **Windows** (x86, x64) 或 **macOS** (x64, arm64)
- **Node.js** >= 14.0.0
- **构建工具**:
  - Windows: Visual Studio Build Tools (包含 C++ 编译器)
  - macOS: Xcode Command Line Tools

## 安装

```bash
# 克隆或下载项目
# 安装依赖并自动编译原生插件
npm install
```
或者，如果需要手动编译：
```bash
npm run build
```

---

## 在 Electron 中使用

在 Electron 中使用原生 Node.js 模块需要一个额外的步骤，因为原生模块必须针对 Electron 内置的 Node.js 版本进行编译，而不是你系统中的 Node.js 版本。

### 方式一：使用 `electron-rebuild` (推荐)

这是最简单、最可靠的方法，能够自动处理编译目标。

**1. 安装 `electron-rebuild`**

在你的 Electron 项目中，将其添加为开发依赖：
```bash
npm install --save-dev electron-rebuild
```

**2. 重新编译原生模块**

每当你的 Electron 项目安装新的原生模块 (如此模块) 或升级 Electron 版本后，都需要运行 `electron-rebuild`。

通常可以将其添加到 `package.json` 的 `scripts` 中以便捷使用：
```json
"scripts": {
  "start": "electron .",
  "rebuild": "electron-rebuild"
}
```
然后运行：
```bash
npm run rebuild
```

### 方式二：手动引入打包后的模块

如果你不想使用 `electron-rebuild`，或者有其他特殊需求，你可以：
1. 在本项目的根目录运行 `npm install` 和 `npm run build`，生成 `build` 目录。
2. 将完整的 `build` 文件夹复制到你的 Electron 项目的某个位置 (例如, `native/build`)。
3. 在你的 Electron 主进程代码中，使用相对路径来引入 `.node` 文件。

```javascript
// 注意：路径需要根据你存放 build 文件夹的位置进行调整
const addon = require('./native/build/Release/disable_winkey.node');

// 然后你就可以使用 addon 对象中的方法了
addon.disableSuperKey();
```
**注意**: 这种方式非常脆弱。手动复制的 `build` 产物是针对你当前系统的 Node.js 版本编译的。如果你的 Electron 项目使用的 Node.js 版本、架构 (x64/arm64) 或操作系统与之不同，模块将无法加载。因此，**强烈推荐使用第一种方式**。

### 在 Electron 主进程中使用

这类系统级的键盘钩子功能**只能在 Electron 的主进程 (Main Process) 中使用**，不能在渲染进程 (Renderer Process) 中使用。

下面是一个在 `main.js` 或 `background.js` 中使用的示例：

```javascript
const { app, BrowserWindow } = require('electron');
// 假设已正确安装和编译，可以直接通过包名引入
const { 
    disableAll, 
    enableAll,
    disableSuperKey,
    disableAltKey,
    disableF11Key
} = require('disable-winkey-addon'); 

function createWindow() {
  const win = new BrowserWindow({
    width: 800,
    height: 600
  });

  win.loadFile('index.html');

  // 在窗口聚焦时禁用按键
  win.on('focus', () => {
    console.log('窗口聚焦，禁用按键。');
    disableAll(); // 禁用所有按键
    
    // 或者单独禁用特定按键
    // disableSuperKey(); // 仅禁用Windows键/Command键
    // disableAltKey();   // 仅禁用Alt键
    // disableF11Key();   // 仅禁用F11键
  });

  // 在窗口失焦时恢复按键
  win.on('blur', () => {
    console.log('窗口失焦，恢复按键。');
    enableAll();
  });
}

app.whenReady().then(createWindow);

// 在应用退出前，确保恢复所有按键功能
app.on('will-quit', () => {
  console.log('应用即将退出，恢复所有按键。');
  enableAll();
});
```

---

## 在 Node.js 中使用 (普通方式)

### 使用 KeyManager 类

```javascript
const { KeyManager } = require('disable-winkey-addon');

const keyManager = new KeyManager();

// 禁用特定按键
keyManager.disableSuperKey();  // 禁用Windows键/Command键
keyManager.disableAppSwitch(); // 禁用Alt+Tab/Cmd+Tab
keyManager.disableAltKey();    // 禁用Alt键/Option键
keyManager.disableF11Key();    // 禁用F11键

// 检查状态
console.log('Super键已禁用:', keyManager.isSuperKeyDisabled());
console.log('Alt键已禁用:', keyManager.isAltKeyDisabled());
console.log('F11键已禁用:', keyManager.isF11KeyDisabled());

// 恢复按键
keyManager.enableAll(); // 恢复所有按键
// ... 更多使用方法见下方 API 文档
```

### 直接函数调用

```javascript
const { 
    disableSuperKey, 
    enableSuperKey,
    disableAltKey,
    enableAltKey,
    disableF11Key,
    enableF11Key,
    disableAll,
    enableAll
} = require('disable-winkey-addon');

// 禁用所有按键
disableAll();

// 或者单独控制
disableSuperKey();
disableAltKey();
disableF11Key();

// 恢复所有按键
enableAll();
// ... 更多使用方法见下方 API 文档
```

## 运行测试

```bash
npm test
```

## API 文档

### `KeyManager` 类

#### 构造函数
- `new KeyManager()`: 创建按键管理器实例。

#### 方法
- `disableSuperKey()`: 禁用 Super 键 (Win/Cmd)。返回 `true`。
- `enableSuperKey()`: 启用 Super 键。返回 `true`。
- `isSuperKeyDisabled()`: 检查 Super 键是否被禁用。返回 `boolean`。
- `disableAppSwitch()`: 禁用应用切换功能 (Alt/Cmd+Tab)。返回 `true`。
- `enableAppSwitch()`: 启用应用切换功能。返回 `true`。
- `isAppSwitchDisabled()`: 检查应用切换功能是否被禁用。返回 `boolean`。
- `disableAltKey()`: 禁用 Alt 键 (Win) / Option 键 (Mac)。返回 `true`。
- `enableAltKey()`: 启用 Alt 键。返回 `true`。
- `isAltKeyDisabled()`: 检查 Alt 键是否被禁用。返回 `boolean`。
- `disableF11Key()`: 禁用 F11 键。返回 `true`。
- `enableF11Key()`: 启用 F11 键。返回 `true`。
- `isF11KeyDisabled()`: 检查 F11 键是否被禁用。返回 `boolean`。
- `disableAll()`: 禁用所有受管功能。
- `enableAll()`: 启用所有受管功能。

### 直接函数
API 与 `KeyManager` 类的方法一一对应。

## 注意事项

1.  **系统兼容性**: 此库同时支持 Windows 和 macOS。
2.  **权限要求**:
    - Windows: 通常不需要管理员权限。
    - macOS: 首次运行时，系统可能会弹出“辅助功能”授权窗口，需要用户授权应用才能监听键盘事件。这是 macOS 的标准安全机制。
3.  **自动清理**: 程序正常退出时会自动调用 `enableAll()` 来恢复按键功能。

## 技术实现
本项目采用平台无关的接口和平台特定的实现相结合的策略。

- **通用接口**: `src/addon.cpp` 使用 N-API 定义了一套统一的 JavaScript 接口。
- **Windows 实现 (`src/win_impl.cpp`)**: 使用 Windows API 的底层键盘钩子 (`SetWindowsHookEx` with `WH_KEYBOARD_LL`)。
- **macOS 实现 (`src/mac_impl.cpp`)**: 使用 Core Graphics 框架的事件分发系统 (`CGEventTapCreate`)。
- **构建系统 (`binding.gyp`)**: 通过条件编译，在不同操作系统上选择对应的实现文件。

## 许可证

MIT License
