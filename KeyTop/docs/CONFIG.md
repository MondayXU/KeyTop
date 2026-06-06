# KeyTop Configuration Guide

This guide explains how to configure KeyTop, a lightweight Windows system tray tool for keyboard remapping and window management.

## Table of Contents

- [File Structure](#file-structure)
- [Configuration File](#configuration-file)
- [INI Sections](#ini-sections)
- [Hotkey Format](#hotkey-format)
- [Command Line Options](#command-line-options)
- [Environment Variables](#environment-variables)
- [Tips and Troubleshooting](#tips-and-troubleshooting)

## File Structure

KeyTop uses a configuration file named `KeyTop.ini` located in the same directory as the executable. The file structure is:

```
KeyTop/
├── KeyTop.exe              # Main executable
├── KeyTop.ini              # Configuration file (created on first run)
├── KeyTop.ini.example      # Example configuration file
└── docs/                   # Documentation
```

On first run, if `KeyTop.ini` doesn't exist, KeyTop creates it with default values.

## Configuration File

KeyTop uses a standard INI file format with sections enclosed in square brackets `[]`. Each section contains key-value pairs separated by `=`.

### Example Configuration

```ini
[General]
AutoStart=1
LogLevel=2
ShowNotifications=1

[Keyboard]
CapsLock=Escape

[Window]
TopkeyMod=Ctrl
TopkeyVk=Space
TransKeyMod=Ctrl+Shift
TransKeyVk=Space
EnableMidClick=1

[Hotkeys]
OpenNotepad=Ctrl+Shift+N
```

## INI Sections

### [General]

General application settings.

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| AutoStart | Integer (0/1) | 0 | Auto-start with Windows (0=disabled, 1=enabled) |
| LogLevel | Integer (0-3) | 2 | Log level (0=Error, 1=Warning, 2=Info, 3=Debug) |
| ShowNotifications | Integer (0/1) | 1 | Show operation notifications (0=disabled, 1=enabled) |

### [Keyboard]

Keyboard remapping configuration.

| Key | Type | Description |
|-----|------|-------------|
| FromKey | String | Key to remap (e.g., CapsLock) |
| ToKey | String | Target key (e.g., Escape) |

**Supported Keys:**
- Letters: A-Z
- Numbers: 0-9
- Function keys: F1-F12
- Special keys: Escape, Tab, CapsLock, Space, Enter, Backspace, Delete, Home, End, PageUp, PageDown, Insert
- Modifiers: Shift, Ctrl, Alt, Win
- Arrow keys: Up, Down, Left, Right

**Examples:**
```ini
; Single key mapping
CapsLock=Escape

; Multiple mappings (one per line)
CapsLock=Escape
Shift=Ctrl
```

### [Window]

Window management hotkeys and settings.

| Key | Type | Default | Description |
|-----|------|---------|-------------|
| TopkeyMod | String | Ctrl | Modifier for topmost toggle hotkey |
| TopkeyVk | String | Space | Virtual key for topmost toggle hotkey |
| TransKeyMod | String | Ctrl+Shift | Modifier for transparency hotkey |
| TransKeyVk | String | Space | Virtual key for transparency hotkey |
| EnableMidClick | Integer (0/1) | 1 | Enable mouse middle-click for topmost (0=disabled, 1=enabled) |

### [Hotkeys]

Custom hotkeys for launching applications or executing commands.

| Key | Type | Description |
|-----|------|-------------|
| Name | String | Hotkey name (e.g., OpenNotepad) |
| Modifiers+Key | String | Hotkey combination (e.g., Ctrl+Shift+N) |

**Examples:**
```ini
[Hotkeys]
OpenNotepad=Ctrl+Shift+N
OpenCalculator=Alt+C
OpenExplorer=Win+E
```

## Hotkey Format

Hotkeys follow a specific format: `Modifiers+VirtualKey`

### Modifiers

Modifiers can be combined using `+`:
- `Ctrl` - Control key
- `Alt` - Alt key
- `Shift` - Shift key
- `Win` - Windows key

**Examples:**
- `Ctrl` - Just the Control key
- `Ctrl+Shift` - Control and Shift together
- `Ctrl+Alt+Shift` - All three modifiers

### Virtual Keys

Common virtual keys:
- Letters: A-Z
- Numbers: 0-9
- Function: F1-F12
- Space - Spacebar
- Enter - Enter key
- Escape - Escape key
- Tab - Tab key

### Complete Examples

```ini
; Ctrl+Space
TopkeyMod=Ctrl
TopkeyVk=Space

; Ctrl+Shift+Space
TransKeyMod=Ctrl+Shift
TransKeyVk=Space

; Custom hotkey: Alt+F4
MyHotkey=Alt+F4
```

## Command Line Options

KeyTop supports the following command line options:

| Option | Description |
|--------|-------------|
| `--help` | Show help message |
| `--version` | Show version information |
| `--config <path>` | Use custom configuration file |
| `--no-tray` | Run without system tray icon |
| `--debug` | Enable debug logging |

**Examples:**
```batch
; Run with custom config
KeyTop.exe --config "C:\path\to\config.ini"

; Run in debug mode
KeyTop.exe --debug

; Show help
KeyTop.exe --help
```

## Environment Variables

KeyTop recognizes the following environment variables:

| Variable | Description | Default |
|----------|-------------|---------|
| `KEYTOP_CONFIG` | Path to configuration file | Same directory as executable |
| `KEYTOP_LOG_LEVEL` | Override log level (0-3) | From INI file |
| `KEYTOP_LOG_PATH` | Path to log file | Same directory as executable |
| `KEYTOP_AUTO_START` | Override auto-start setting (0/1) | From INI file |

**Examples:**
```batch
; Set custom config path
set KEYTOP_CONFIG=C:\Users\MyUser\KeyTop.ini

; Set debug logging
set KEYTOP_LOG_LEVEL=3

; Run KeyTop
KeyTop.exe
```

## Tips and Troubleshooting

### General Tips

1. **Backup your configuration**: Copy `KeyTop.ini` to a safe location before making changes
2. **Restart after changes**: Close and reopen KeyTop after modifying the configuration
3. **Check logs**: Enable debug logging (`LogLevel=3`) to troubleshoot issues
4. **Use valid keys**: Ensure all key names are correctly spelled (case-insensitive)

### Common Issues

#### KeyTop doesn't start
- Ensure no other application is using the same hotkeys
- Check Windows Defender or antivirus isn't blocking it
- Run with `--debug` flag to see error messages

#### Hotkeys not working
- Verify hotkey format is correct (e.g., `Ctrl+Space`, not `Ctrl + Space`)
- Check for conflicting hotkeys in other applications
- Try running as Administrator

#### Configuration not loading
- Ensure `KeyTop.ini` is in the same directory as `KeyTop.exe`
- Check for syntax errors in the INI file
- Verify section names are correct (e.g., `[General]`, not `[general]`)

#### Auto-start not working
- Ensure `AutoStart=1` is set in `[General]` section
- Check Windows Task Manager > Startup tab
- Verify registry permissions (KeyTop needs to write to `HKCU\Software\Microsoft\Windows\CurrentVersion\Run`)

### Log File Locations

KeyTop creates log files in the same directory as the executable:
- `KeyTop.log` - Main log file
- `KeyTop_error.log` - Error-only log file

### Default Hotkeys

| Action | Hotkey | Description |
|--------|--------|-------------|
| Toggle Topmost | Ctrl+Space | Make/unmake window always on top |
| Toggle Transparency | Ctrl+Shift+Space | Make/unmake window transparent |
| Toggle Topmost (Mouse) | Middle-click | Alternative method, if enabled |

### Performance Tips

- Disable notifications if not needed: `ShowNotifications=0`
- Reduce log level for better performance: `LogLevel=0` or `LogLevel=1`
- Avoid using too many custom hotkeys
- Close unnecessary applications to reduce hotkey conflicts

### Security Notes

- KeyTop runs with user privileges (no admin rights required)
- Configuration is stored locally in the INI file
- No data is sent over the network
- Auto-start uses user-level registry (HKCU)

## Advanced Configuration

### Multiple Keyboard Mappings

You can create multiple keyboard mappings by adding multiple key pairs:

```ini
[CapsLock=Escape
Shift=Ctrl
Alt=Win
```

### Disabled Features

To disable a feature, set its value to 0 or leave it empty:

```ini
; Disable middle-click for topmost
EnableMidClick=0

; Disable notifications
ShowNotifications=0
```

### Custom Logging

For advanced troubleshooting:

```ini
[General]
LogLevel=3  ; Debug level
```

This creates detailed log files in the KeyTop directory.

---

For more information, visit the [KeyTop GitHub repository](https://github.com/yourusername/keytop) or open an issue for support.
