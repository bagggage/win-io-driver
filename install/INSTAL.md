# Installation Guide

This is small open-source driver and it's don't provide a verified certificate.
That's couses a lot of problems while installation. Windows always would say
that it's pretty unsafe, but in fact it doesn't.

## Install

So if you don't have ability to compile driver by itself, you can install it like so:

- Open `ISAIODriver.cer` file and sign it.
- Run `cmd` as **Administrator**.
    - Enter `bcdedit /set testsigning on` to make able to install driver with test certificate.
- Reboot computer.
- Run `install.bat` and confirm installation.

Done!

## Uninstall

Just run `uninstall.bat` script as **Administator**.
Done!