# 使用说明

## 快速开始

### 1. 编译库

```bash
npm install
npm run build
```

### 2. 集成到Electron项目

#### 方法A：作为npm包（推荐）

```bash
# 在您的Electron项目目录下
npm install /path/to/nodeCadon

# 或者如果发布到npm
npm install node-cadon
```

#### 方法B：直接复制文件

将以下文件复制到您的Electron项目：

```
your-electron-app/
├── lib/node-cadon/           # 创建此目录
│   ├── build/Release/
│   │   └── node_cadon.node   # 必需
│   ├── index.js              # 必需
│   └── package.json          # 可选
```

```js
//main.js

import nodeCadon from '..lib/node-cadon/';
```




### 2. 基本使用

```javascript
const { WinKeyManager } = require("disable-winkey-addon");

const manager = new WinKeyManager();

// 禁用Windows键
manager.disable();

// 启用Windows键
manager.enable();

// 检查状态
console.log(manager.isWinKeyDisabled());
```

### 3. 函数式调用

```javascript
const {
  disableWinKey,
  enableWinKey,
  isWinKeyDisabled,
} = require("disable-winkey-addon");

disableWinKey(); // 禁用
enableWinKey(); // 启用
isWinKeyDisabled(); // 检查状态
```

## 测试

```bash
npm test
```

## 注意事项

- 仅支持 Windows 系统
- 程序退出时会自动恢复 Windows 键
