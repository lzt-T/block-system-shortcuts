# Windows键禁用 Node.js C++ Addon

这是一个用于禁用Windows键的Node.js C++ Addon库。

## 功能特性

- 禁用/启用左右Windows键
- 提供简单易用的JavaScript API
- 支持类和函数两种调用方式
- 自动资源清理和错误处理

## 系统要求

- Windows操作系统
- Node.js >= 14.0.0
- Visual Studio Build Tools (用于编译)

## 安装

```bash
# 克隆或下载项目
# 安装依赖
npm install

# 编译Addon
npm run build
```

## 使用方法

### 方式一：使用WinKeyManager类

```javascript
const { WinKeyManager } = require('disable-winkey-addon');

const winKeyManager = new WinKeyManager();

// 禁用Windows键
if (winKeyManager.disable()) {
    console.log('Windows键已禁用');
}

// 启用Windows键
if (winKeyManager.enable()) {
    console.log('Windows键已启用');
}

// 检查状态
console.log('Windows键状态:', winKeyManager.isWinKeyDisabled() ? '禁用' : '启用');

// 切换状态
winKeyManager.toggle();
```

### 方式二：直接函数调用

```javascript
const { disableWinKey, enableWinKey, isWinKeyDisabled } = require('disable-winkey-addon');

// 禁用Windows键
disableWinKey();

// 启用Windows键
enableWinKey();

// 检查状态
console.log('Windows键状态:', isWinKeyDisabled() ? '禁用' : '启用');
```

## 运行测试

```bash
npm test
```

## API文档

### WinKeyManager类

#### 构造函数
- `new WinKeyManager()`: 创建Windows键管理器实例

#### 方法
- `disable()`: 禁用Windows键，返回boolean (成功返回true)
- `enable()`: 启用Windows键，返回boolean (成功返回true)
- `isWinKeyDisabled()`: 检查Windows键是否被禁用，返回boolean
- `toggle()`: 切换Windows键状态，返回boolean (当前状态)

### 直接函数

- `disableWinKey()`: 禁用Windows键，返回boolean
- `enableWinKey()`: 启用Windows键，返回boolean
- `isWinKeyDisabled()`: 检查Windows键状态，返回boolean

## 注意事项

1. **系统兼容性**: 此库仅适用于Windows系统
2. **权限要求**: 通常不需要管理员权限
3. **自动清理**: 程序退出时会自动恢复Windows键功能
4. **性能优化**: 使用优化的键盘钩子，最小化输入延迟
5. **资源管理**: 请确保在不需要时调用`enable()`方法释放钩子资源

## 技术实现

- 使用Windows API的`RegisterHotKey`函数注册Windows键为全局热键
- 通过隐藏窗口接收并拦截热键消息
- 通过Node-API (N-API)与JavaScript交互
- 无键盘输入延迟，不影响其他按键性能

## 许可证

MIT License