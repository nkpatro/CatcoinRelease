
Debian
====================
This directory contains files used to package catcoind/catcoin-qt
for Debian-based Linux systems. If you compile catcoind/catcoin-qt yourself, there are some useful files here.

## catcoin: URI support ##


catcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install catcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your catcoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/bitcoin128.png` to `/usr/share/pixmaps`

catcoin-qt.protocol (KDE)

