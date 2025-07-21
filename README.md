# 🔐 Password Manager

A secure, GUI-based password manager built with C and GTK4 that provides encryption, password generation, and easy management of your credentials.

## 🚀 Technologies Used

### Core Technologies
- **C Programming Language** 🖥️ - Core application logic and system interactions
- **GTK4** 🎨 - Modern graphical user interface framework
- **OpenSSL** 🔒 - Cryptographic functions for secure password storage
- **GLib** 📚 - Cross-platform utility library for GTK applications

### Security Features
- **AES-256-CBC Encryption** 🛡️ - Industry-standard symmetric encryption
- **PBKDF2-HMAC-SHA256** 🔑 - Key derivation function for secure password hashing
- **SHA-256 Hashing** #️⃣ - Secure password verification
- **Random Salt Generation** 🎲 - Enhanced security for password storage

### Build System
- **Make** ⚙️ - Build automation and compilation management
- **GCC** 🔧 - GNU Compiler Collection for C compilation

## 📦 Prerequisites

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential libgtk-4-dev libssl-dev pkg-config
```

### Fedora/RHEL
```bash
sudo dnf install gcc gtk4-devel openssl-devel pkgconfig
```

### Arch Linux
```bash
sudo pacman -S base-devel gtk4 openssl pkgconf
```

## 🛠️ Installation & Setup

1. **Clone the repository** 📥
```bash
git clone <repository-url>
cd passwordManager
```

2. **Build the application** 🔨
```bash
make
```

3. **Initialize the password manager** 🎯
```bash
./passwordManager --init
```
This will:
- Create necessary configuration directories
- Set up your master username and password
- Generate encryption keys

4. **Launch the GUI** 🚀
```bash
./passwordManager
```

## 🎮 How to Use

### 🔐 Initial Login
- Enter your master username and password
- These credentials were set during the `--init` process

### ➕ Adding Passwords
1. Navigate to **"Add Password"**
2. Fill in:
   - **Application** 📱 - Service name (e.g., "Gmail", "GitHub")
   - **Username** 👤 - Your username for that service
   - **Password** 🔑 - Your password for that service
   - **Account Password** 🛡️ - Your master password for encryption
3. Click **"Add To Vault"**

### 📋 Viewing Passwords
1. Navigate to **"List Passwords"**
2. Enter your master password
3. View all stored credentials with **Delete** options

### 🎲 Password Generation
1. Navigate to **"Generate Password"**
2. Adjust the length slider (8-32 characters)
3. Click **"Generate Password"**
4. Copy the generated password for use

### 🗑️ Deleting Passwords
1. Go to **"List Passwords"**
2. Click the **"Delete"** button next to any entry
3. The entry will be removed from both display and storage

## 📁 File Structure

```
passwordManager/
├── 📄 passwordManager.c     # Main application entry point
├── 🎨 gui.c                 # GTK4 GUI implementation
├── 🎨 gui.h                 # GUI function declarations
├── 🔒 encryption.c          # Cryptographic functions
├── 🔒 encryption.h          # Encryption function declarations
├── 💻 cliFunctions.c        # CLI utility functions
├── 💻 cliFunctions.h        # CLI function declarations
├── 📝 terminalUI.c          # Terminal interface (if needed)
├── ⚙️ Makefile              # Build configuration
└── ~/.config/passwordManager/  # User data directory
    ├── 🔑 master.conf       # Hashed credentials
    ├── 🔐 key.enc           # Encrypted AES keys
    └── 💾 storage.db        # Encrypted password database
```

## 🔒 Security Features

- **🛡️ Master Password Protection** - All access requires authentication
- **🔐 AES-256-CBC Encryption** - Military-grade encryption for stored passwords
- **🧂 Salt + PBKDF2** - Prevents rainbow table attacks
- **💾 Secure File Storage** - All data encrypted at rest
- **🔄 Memory Safety** - Proper memory management to prevent leaks
- **⚛️ Atomic File Operations** - Safe file updates prevent corruption

## 🚨 Important Security Notes

- **🔒 Never share your master password**
- **💾 Backup your `~/.config/passwordManager/` directory**
- **🔄 Use a strong master password with mixed characters**
- **🚫 Don't run as root/administrator**

## 🔧 Build Options

### Debug Build
```bash
make debug
```

### Clean Build
```bash
make clean
make
```

### Full Rebuild
```bash
make clean && make
```

## 🐛 Troubleshooting

### Build Issues
- Ensure all dependencies are installed
- Check GTK4 and OpenSSL development packages
- Verify GCC version compatibility

### Runtime Issues
- Run `./passwordManager --init` if seeing warnings
- Check file permissions in `~/.config/passwordManager/`
- Ensure master password is correct

### GUI Issues
- Verify GTK4 is properly installed
- Check display environment variables
- Try running from terminal to see error messages

## 📈 Features

- ✅ Secure password storage with AES-256 encryption
- ✅ Master password authentication
- ✅ Password generation with customizable length
- ✅ GUI-based password management
- ✅ Individual password deletion
- ✅ Automatic list refresh after additions
- ✅ Memory-safe implementation
- ✅ Cross-platform compatibility (Linux)

## 🤝 Contributing

Feel free to submit issues, feature requests, or pull requests to improve this password manager!

## 📜 License

This project is open source. Please check the repository for specific license information.

---

**⚠️ Disclaimer**: While this password manager implements industry-standard security practices, use at your own risk. Always maintain backups of important data.