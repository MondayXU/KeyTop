# 🔑 KeyTop

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Windows](https://img.shields.io/badge/Platform-Windows-blue.svg)](https://www.microsoft.com/windows)
[![C++](https://img.shields.io/badge/Language-C++-blue.svg)](https://isocpp.org/)
[![Release](https://img.shields.io/badge/Version-v1.0.0-green.svg)](https://github.com/MondayXU/KeyTop/releases)
[![Stars](https://img.shields.io/github/stars/MondayXU/KeyTop?style=social)](https://github.com/MondayXU/KeyTop)

**轻量级 Windows 键盘快捷键重映射和窗口管理工具**

*从 PowerToys 中提取的核心功能，独立运行，极致轻量*

[English](#features) | [功能特性](#features) | [快速开始](#quick-start) | [配置说明](#configuration)

</div>

---

## 📖 目录

- [功能特性](#features)
- [快速开始](#quick-start)
- [编译指南](#building-from-source)
- [配置说明](#configuration)
- [项目结构](#project-structure)
- [架构设计](#architecture)
- [常见问题](#faq)
- [故障排除](#troubleshooting)
- [贡献指南](#contributing)
- [许可证](#license)

---

## ✨ Features

### 🎹 Keyboard Remapping
- **单键映射**: 将一个键映射到另一个键（如 CapsLock → Escape）
- **组合键映射**: 将单个键映射到组合键（如 F1 → Ctrl+C）
- **双向交换**: 交换两个键的功能（如 CapsLock ↔ Escape）

### 🪟 Window Management
- **窗口置顶**: 让任意窗口保持在最前面
- **窗口穿透**: 让窗口透明可点击穿透
- **多种操作方式**: 快捷键、鼠标中键、托盘菜单

### 🎯 System Tray
- **后台运行**: 不占用任务栏空间
- **内存极低**: < 1.5MB 运行内存
- **文件小巧**: < 150KB 可执行文件
- **无需依赖**: 纯 Win32 API，无需安装任何运行时

### 🔧 其他特性
- **开机自启**: 可选配置
- **日志系统**: 内置日志记录
- **Settings GUI**: 图形化设置界面
- **多语言支持**: 中英文

---

## 🚀 Quick Start

### 系统要求
- Windows 10/11 (32位/64位)
- 无需额外依赖

### 安装使用
1. 下载最新版本的 [KeyTop.exe](https://github.com/MondayXU/KeyTop/releases)
2. 双击运行，自动最小化到系统托盘
3. 右键点击托盘图标进行配置

### 默认快捷键
| 功能 | 快捷键 |
|------|--------|
| 窗口置顶切换 | `Ctrl + Space` |
| 窗口穿透切换 | `Ctrl + Shift + Space` |
| 鼠标中键置顶 | 点击窗口标题栏 |

### 托盘菜单
右键点击托盘图标或双击图标，可以访问：
- **Settings** - 打开设置界面
- **Enable Keyboard Hook** - 启用/禁用键盘重映射
- **Enable Mouse Hook** - 启用/禁用鼠标中键功能
- **About** - 显示版本信息
- **Exit** - 退出程序

---

## 🛠️ Building from Source

### 前提条件
- [MinGW-w64](https://www.mingw-w64.org/) 或 [Visual Studio](https://visualstudio.microsoft.com/)
- Windows SDK

### 编译步骤
```bash
# 克隆仓库
git clone https://github.com/MondayXU/KeyTop.git
cd KeyTop

# 使用 MinGW 编译
cd build
make

# 或使用 Visual Studio
# 打开 KeyTop.sln 并构建
```

### 输出文件
- `KeyTop.exe` - 主程序（< 150KB）
- `KeyTop.ini` - 配置文件（自动生成）
- `KeyTop.log` - 日志文件（可选）

### 运行测试
```bash
cd tests
# 编译并运行测试
gcc -I../include ../src/utils.c test_utils.c -o test_utils.exe
./test_utils.exe
```

---

## ⚙️ Configuration

配置文件 `KeyTop.ini` 位于程序同目录下：

```ini
[General]
; 是否开机自启 (0=禁用, 1=启用)
AutoStart=0
; 日志级别 (0=Error, 1=Warning, 2=Info, 3=Debug)
LogLevel=2
; 是否显示操作提示 (0=禁用, 1=启用)
ShowNotifications=1

[Keyboard]
; 格式: 原始键=映射键
CapsLock=Escape
F1=Ctrl+C

[Window]
; 置顶快捷键 (默认: Ctrl+Space)
TopkeyMod=Ctrl
TopkeyVk=Space
; 穿透快捷键 (默认: Ctrl+Shift+Space)
TransKeyMod=Ctrl+Shift
TransKeyVk=Space
; 是否启用鼠标中键置顶 (0=禁用, 1=启用)
EnableMidClick=1

[Hotkeys]
; 自定义热键 (格式: 名称=修饰键+键)
; OpenNotepad=Ctrl+Shift+N
```

### 支持的键名
- **字母**: A-Z
- **数字**: 0-9
- **功能键**: F1-F12
- **特殊键**: Escape, Tab, Enter, Space, Backspace
- **修饰键**: LCtrl, RCtrl, LAlt, RAlt, LShift, RShift, LWin, RWin
- **导航键**: Home, End, PageUp, PageDown, Insert, Delete
- **方向键**: Up, Down, Left, Right
- **小键盘**: Num0-Num9, Num*, Num+, Num-, Num., Num/
- **锁定键**: CapsLock, NumLock, ScrollLock

### 配置备份
KeyTop 会自动创建配置备份（`KeyTop.ini.bak`），如果配置文件损坏，会自动从备份恢复。

---

## 📁 Project Structure

```
KeyTop/
├── src/                    # 源代码
│   ├── main.c             # 程序入口
│   ├── keyboard_hook.c/h  # 键盘钩子模块
│   ├── mouse_hook.c/h     # 鼠标钩子模块
│   ├── window_manager.c/h # 窗口管理模块
│   ├── hotkey.c/h          # 热键模块
│   ├── tray_icon.c/h      # 托盘界面模块
│   ├── config.c/h         # 配置管理模块
│   ├── settings_gui.c/h   # 设置界面模块
│   ├── error_handler.c/h  # 错误处理模块
│   └── utils.c/h          # 工具函数
├── include/
│   └── keytop.h           # 公共头文件
├── res/                   # 资源文件
│   └── settings.rc        # 对话框资源
├── tests/                 # 单元测试
├── docs/                  # 文档
│   ├── CHANGELOG.md       # 更新日志
│   ├── CONTRIBUTING.md    # 贡献指南
│   └── CODE_OF_CONDUCT.md # 行为准则
├── build/                 # 构建脚本
│   ├── Makefile           # MinGW 构建
│   └── release.bat        # 发布脚本
├── .github/               # GitHub 配置
│   └── workflows/ci.yml   # CI/CD
├── KeyTop.ini.example     # 配置示例
├── LICENSE                # MIT 许可证
├── README.md              # 说明文档
└── GITHUB_SETUP.md         # GitHub 设置指南
```

---

## 🏗️ Architecture

KeyTop 采用单线程事件驱动架构：

```
┌─────────────────────────────────────────┐
│              KeyTop                      │
├─────────────────────────────────────────┤
│  主线程 (Windows 消息循环)               │
│  ┌─────────────────────────────┐       │
│  │ 托盘界面 ←→ 配置界面        │       │
│  │     ↓                       │       │
│  │ 键盘钩子回调 (快速处理)      │       │
│  │ 鼠标钩子回调 (快速处理)      │       │
│  │ 窗口操作 (同步执行)          │       │
│  │ 配置读写 (快速I/O)          │       │
│  └─────────────────────────────┘       │
└─────────────────────────────────────────┘
```

**设计原则：**
- 单线程设计，避免线程同步问题
- 事件驱动，空闲时几乎不消耗 CPU
- 钩子回调快速执行，不阻塞
- 内存占用极低（< 1.5MB）
- 模块化设计，易于维护和扩展

**技术栈：**
- 语言: C/C++
- API: Win32 API (user32, kernel32, shell32, gdi32)
- 构建: MinGW-w64 / Visual Studio
- 无第三方依赖

---

## ❓ FAQ

### Q: KeyTop 安全吗？
A: 是的，KeyTop 使用纯 Win32 API 实现，不包含任何网络功能，不会上传任何数据。所有配置都保存在本地。

### Q: KeyTop 会被杀毒软件误报吗？
A: 可能。由于 KeyTop 使用键盘钩子技术，某些杀毒软件可能会误报。你可以将 KeyTop.exe 加入杀毒软件的白名单。

### Q: 如何卸载 KeyTop？
A: KeyTop 是绿色软件，直接删除 `KeyTop.exe` 和 `KeyTop.ini` 即可。如果设置了开机自启，需要先取消。

### Q: 如何重置配置？
A: 删除 `KeyTop.ini` 文件，KeyTop 会自动创建默认配置。

### Q: 可以同时运行多个实例吗？
A: 不可以。KeyTop 使用单实例设计，只能运行一个实例。

### Q: 支持哪些 Windows 版本？
A: Windows 10/11 (32位/64位)。理论上 Windows 7/8 也可能工作，但未测试。

---

## 🔧 Troubleshooting

### KeyTop 无法启动
1. 检查 `KeyTop.log` 日志文件
2. 确保没有其他应用占用相同快捷键
3. 尝试以管理员身份运行
4. 检查杀毒软件是否拦截

### 快捷键不工作
1. 检查键盘钩子是否启用（托盘菜单）
3. 确认快捷键没有与其他应用冲突
4. 尝试更换其他快捷键组合

### 窗口置顶/穿透不生效
1. 某些窗口可能不支持置顶或穿透
2. 尝试以管理员身份运行 KeyTop
3. 检查日志文件获取详细错误信息

### 配置不生效
1. 检查 `KeyTop.ini` 格式是否正确
2. 重启 KeyTop
3. 删除配置文件让 KeyTop 重新创建

---

## 🤝 Contributing

欢迎贡献代码！请阅读 [CONTRIBUTING.md](docs/CONTRIBUTING.md) 了解如何参与。

### 贡献方式
- 🐛 报告 Bug
- 💡 建议新功能
- 🔧 提交 Pull Request
- 📖 改进文档

### 开发流程
1. Fork 仓库
2. 创建功能分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add amazing feature'`)
4. 推送分支 (`git push origin feature/amazing-feature`)
5. 创建 Pull Request

### 行为准则
本项目遵循 [Contributor Covenant](https://www.contributor-covenant.org/) 行为准则。详见 [CODE_OF_CONDUCT.md](docs/CODE_OF_CONDUCT.md)。

---

## 📝 Changelog

### [v1.0.0] - 2026-06-06
- 🎉 初始版本发布
- ✅ 键盘重映射功能
- ✅ 窗口置顶和穿透
- ✅ 系统托盘界面
- ✅ 配置管理
- ✅ 完整文档
- ✅ 单元测试
- ✅ CI/CD 配置

详见 [CHANGELOG.md](docs/CHANGELOG.md)

---

## 📄 License

本项目采用 [MIT License](LICENSE) 开源许可证。

```
MIT License

Copyright (c) 2026 KeyTop Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 🙏 Acknowledgments

- 灵感来源于 [Microsoft PowerToys](https://github.com/microsoft/PowerToys)
- 使用纯 Win32 API 实现，无需第三方依赖
- 感谢所有贡献者和用户

---

## 🔗 Links

- **GitHub**: https://github.com/MondayXU/KeyTop
- **Releases**: https://github.com/MondayXU/KeyTop/releases
- **Issues**: https://github.com/MondayXU/KeyTop/issues
- **Wiki**: https://github.com/MondayXU/KeyTop/wiki

---

<div align="center">

**⭐ 如果这个项目对你有帮助，请给一个 Star！**

[![Star History Chart](https://api.star-history.com/svg?repos=MondayXU/KeyTop&type=Date)](https://star-history.com/#MondayXU/KeyTop&Date)

</div>
