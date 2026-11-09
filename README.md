# PHANTOM – Ghost Hunting OS for Cardputer ADV

I made a spooky ghost-hunting OS for the M5Stack Cardputer ADV!

I wanted to make something that felt like a real ghost-hunting gadget, with different tools to mess around with. It has a WiFi scanner, Bluetooth radar, digital Ouija board, and a motion detector!

I also gave it a spooky look with green, red, and magenta colors. Here's what I made!

> **Note:** This project was originally called SPOOKY-OS while I was developing it. That was just a placeholder name! SPOOKY-OS and PHANTOM refer to the same project. PHANTOM is the final name.

## Features

- Main menu to navigate between tools
- WiFi scanner with a green signal graph
- Digital Ouija board with random creepy messages
- Spooktooth Bluetooth radar
- Motion detector using the Cardputer's built-in sensor
- Settings menu with screen brightness control

## Quick Start (Precompiled Firmware)

You don't need Arduino IDE to install PHANTOM! You can download the precompiled firmware from the [Releases page](https://github.com/GitDog1203/SPOOKY-OS/releases/tag/v1.0.0).

### Prerequisites (Windows)

**1. Install Python**

- Download Python from https://www.python.org/downloads/
- During installation, check **Add Python to PATH**.
- Open Command Prompt and verify:

```bash
python --version
```

**2. Install esptool**

```bash
python -m pip install esptool
python -m esptool version
```

**3. Connect your Cardputer ADV**

- Connect your Cardputer ADV to your PC using USB.
- Find its COM port in Windows Device Manager under Ports (COM & LPT).

### Installing PHANTOM

1. Go to the [PHANTOM v1.0.0 Release](https://github.com/GitDog1203/SPOOKY-OS/releases/tag/v1.0.0).
2. Download `PHANTOM.merged.bin`.
3. Open Command Prompt.

**4. Check your connection**

Replace `YOUR_COM_PORT` with your actual COM port (for example, COM5).

```bash
python -m esptool --port YOUR_COM_PORT chip-id
```

**5. Flash the firmware**

Replace `YOUR_COM_PORT` and the firmware path with your own values.

```bash
python -m esptool --chip esp32s3 --port YOUR_COM_PORT --baud 460800 write-flash --flash-size 8MB 0x0 "PATH_TO_FIRMWARE.bin"
```

6. Wait for the flashing process to finish. Your Cardputer should restart.
7. Have fun investigating!

**Important:** Flashing replaces the existing firmware on your Cardputer ADV.

## Arduino IDE Setup

1. Install Arduino IDE.
2. Open File → Preferences.
3. Add this to Additional Boards Manager URLs:

`https://m5stack.github.io/arduino-index/package_m5stack_index.json`

4. Open Tools → Board → Boards Manager, search for M5Stack, and install it.
5. Open Library Manager, search for M5Cardputer, and install M5Cardputer by M5Stack along with any requested dependencies.
6. Open the PHANTOM project, select the M5Cardputer board, and compile and upload.

## Demo

Here's a video of PHANTOM running on my Cardputer ADV!

[Watch the project demo](https://youtu.be/va-7R8Tck_o)

## Credits

- Hardware: M5Stack Cardputer ADV
- Arduino libraries: M5Stack
- Project idea and development: Me, with a little help from Gemini.
- README: revised by ChatGPT to make it look nice.

Have fun! (=
