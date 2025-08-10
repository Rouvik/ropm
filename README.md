# ROPM package manager
# Important Note:
I didnt provide any public key for gpg checks for now, so for the gpg check during setup phase use `n` (NO) to avoid the check or it will crash.

## What is this?
A simple package manager written by me in bare C using libcrypto, curl, gpg as dependencies.
The goal of this package manager is education and not serious hosting of packages, however I cared to implement
all the necessary signing and key checks for my packages to avoid any tampering.

## How does it work?
The package manager has a dedicated list of repositories to download from and interpret the files to get the packages installed
in the system:
- First it downloads the Release and Release.asc file from the repo and checks with a known valid public key shipped with the package manager
to validate the authenticity of Release
- Then from Release it extracts all the important metadata about the repo and SHA256 checksum for the Packages.gz file and then downloads it
- The Packages.gz contains all the listing of packages and their SHA256 checksums for validation
- The ropm then proceeds to find the required package from the list of packages, check it and install it
- All the package tars are expected to contain a Makefile with **all, install and uninstall** targets to work with ropm

## Future plans:
- Better package handling, listing of packages and most importantly **PACKAGE DEPENDENCIES** (currently unsupported, to get dependencies packages must manually download them from their Makefile to install)
- Crossplatform support (currently only supports **Linux/Unix** like systems with **libcrypto, curl and gpg** installed)

## Author:
Rouvik Maji [Gmail](mailto:majirouvik@gmail.com)